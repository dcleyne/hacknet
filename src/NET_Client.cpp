#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <assert.h>
#include <string.h> 
#include <unistd.h>
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <arpa/inet.h>
#include <netdb.h>
#include "NET_Packet.h"
#include "NET_Client.h"
#include "HN_Display.h"
#include "ENT_Base.h"
#include "HN_Enum.h"

#define HACKNET_PORT 		(9274)
#define MAX_DATA_SIZE		(100)	// how much data can we grab at once

//#define __DEBUGGING_NETWORK__

netClient::netClient(hnDisplay *display, char *serverAddress):
	m_display(display), m_done(false)
{
	m_serverAddress = new sockaddr_in;
	m_display->SetClient(this);
	StartClient(serverAddress);
}

netClient::~netClient()
{
	delete m_serverAddress;
}


void
netClient::StartClient( char * serverAddress )
{
	//int sock_fd;
	hostent 	*he;
	
	//printf("Doing host lookup...\n");
	if ( ( he = gethostbyname(serverAddress) ) == NULL ) // getting host information...
	{
		herror("gethostbyname");
		cleanexit(1);
	}
	
	
	//printf("Opening socket...\n");
	if ( (m_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		perror("socket");
		cleanexit(1);
	}

	m_serverAddress->sin_family = AF_INET;				// host byte order
	m_serverAddress->sin_port = htons(HACKNET_PORT);		// short, network byte order
	m_serverAddress->sin_addr = *((in_addr *)he->h_addr);
	bzero(&(m_serverAddress->sin_zero),8);				// zero out the rest of the struct

	// okay, we're all ready to go now!  (But we haven't yet connected)
}

void
netClient::Go()
{
	mapClientTile 	tile;				// for use in map updates
	netMapTile tileData;
	netMapEntity entityData;
	netMapUpdateBBox bbox;
	netClientLocation clientLoc;
	hnPoint	point;
	char 		buffer[MAX_DATA_SIZE];
	int		byteCount;
	//printf("Trying to connect...\n");
	if ( connect( m_socket, (sockaddr *)m_serverAddress, sizeof(sockaddr) ) == -1 )
	{
		perror("connect");
		cleanexit(1);
	}
	//printf("Connected successfully!\n");
	
	// now that we've connected, we wait for packets from the server or a key from our hnClient...
	
	m_display->Go();
	
	while(!m_done){
		// do stuff here until we quit.
		
		//netServerPacket packet;
		short incomingBytes;

		if ( recv( m_socket, &incomingBytes, sizeof(sint16), MSG_WAITALL ) == -1 )
		{
			perror("recv");
			cleanexit(1);
		}
		incomingBytes = ntohs(incomingBytes);

		//printf("Receiving %d bytes...\n", incomingBytes);
		char buffer[incomingBytes];
		
		if ( recv( m_socket, buffer, incomingBytes, MSG_WAITALL ) == -1 )
		{
			perror("recv");
			cleanexit(1);
		}
#ifdef __DEBUGGING_NETWORK__	
		char *bufferstart = buffer;
		
		for ( int i = 0; i < incomingBytes; i++ )	
		{
			printf("Value: %d\n",*(bufferstart + i));
		}
		while ( 1 ) {}
#endif
		netMetaPacketInput *packet = new netMetaPacketInput(buffer, incomingBytes);
		
		while ( !packet->Done() )
		{
			sint8 type = packet->PeekChar();
			
			switch ( type )
			{
				case SPT_ClientLocation:
					packet->ClientLocation(clientLoc);
					m_display->UpdateLocation( clientLoc.loc );
					//printf("Client location packet\n");
					//printf("loc: %d,%d,%d\n",clientLoc.loc.x,clientLoc.loc.y,clientLoc.loc.z);
					break;
				case SPT_MapTile:
					//printf("Map tile packet\n");
					packet->MapTile(tileData);
					tile.material = (hnMaterialType)tileData.material;
					tile.wall = (hnWallType)tileData.wall;
					m_display->UpdateMapTile(tileData.loc.x, tileData.loc.y, tile);
					break;
				case SPT_MapEntity:
					packet->MapEntity(entityData);
					m_display->UpdateMapCreature(entityData.loc.x, entityData.loc.y, entityData.objectType);
					break;
				case SPT_MapUpdateBBox:
					//printf("Map bbox update packet\n");
					packet->MapUpdateBBox(bbox);
					//mapClientTile tile;

					for ( int i = 0; i < bbox.width; i++ )
						for ( int j = 0; j < bbox.height; j++ )
						{
							tile.material = bbox.material[i+(j*bbox.width)];
							tile.wall = bbox.wall[i+(j*bbox.width)];
							point.Set(bbox.loc.x+i, bbox.loc.y+j, 0);
							tile.entity = bbox.entityType[i+(j*bbox.width)];
							m_display->UpdateMapTile(bbox.loc.x+i, bbox.loc.y+j, tile);
						}

					delete [] bbox.material;
					delete [] bbox.wall;
					delete [] bbox.entityType;
					break;
				/*case SPT_Refresh:
					//printf("Refresh packet\n");
					packet->Char(type);
					m_display->Refresh();
					break;*/
				case SPT_BadPacketNotice:
					delete m_display;
					packet->Char(type);
					printf("Server killed us for sending an unknown packet.  Version mismatch?\n");
					m_done = true;
					//exit(1);
					break;
				case SPT_QuitConfirm:
					packet->Char(type);
					m_done = true;
					break;
				default:
					printf("Unknown packet type %d!!  :(\n", type);
					break;
			}
		}
		m_display->Refresh();	// do a screen refresh if we need it.
	}
	close( m_socket );
}

void
netClient::SendQuit( bool save )
{
	netClientPacket packet;
	packet.type = (save) ? CPT_Save : CPT_Quit;

	char packetSize = sizeof( netClientPacket );

	if ( send( m_socket, &packetSize, 1, 0 ) == -1 )
	{
		perror("send");
		cleanexit(1);
	}
	if ( send( m_socket, &packet, packetSize, 0 ) == -1 )
	{
		perror("send");
		cleanexit(1);
	}
}

void
netClient::SendMove(hnDirection dir)
{
	//printf("Sending move packet...\n");
	netClientPacket packet;
	packet.type = CPT_Move;
	packet.data.move.direction = dir;

	char packetSize = sizeof( packet.data.move ) + sizeof( packet.type );
	
	if ( send( m_socket, &packetSize, 1, 0 ) == -1 )
	{
		perror("send");
		cleanexit(1);
	}
	if ( send( m_socket, &packet, packetSize, 0 ) == -1 )
	{
		perror("send");
		cleanexit(1);
	}
}

void
netClient::Disconnect()
{
	//printf("Disconnecting...\n");
	m_done = true;
}

void netClient::cleanexit(int id)
{
	delete m_display;
	exit(id);
}
