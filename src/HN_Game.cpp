#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "HN_Game.h"
#include "MAP_Hack.h"
#include "NET_Server.h"
#include "HN_Point.h"
#include "HN_Consts.h"
#include "ENT_Player.h"

static hnPoint offsetVector[10];
hnGame * hnGame::s_instance = NULL;


void
hnGame::Startup()
{
	assert(s_instance == NULL);
	s_instance = new hnGame;
}

void
hnGame::Shutdown()
{
	assert(s_instance);
	delete s_instance;
	s_instance = NULL;
}

hnGame *
hnGame::GetInstance()
{
	assert(s_instance);
	return s_instance;
}


hnGame::hnGame()
{
	offsetVector[0].Set(0,-1,0);	// north
	offsetVector[1].Set(1,-1,0);	// northeast
	offsetVector[2].Set(1,0,0);	// east
	offsetVector[3].Set(1,1,0);	// southeast
	offsetVector[4].Set(0,1,0);	// south
	offsetVector[5].Set(-1,1,0);	// southwest
	offsetVector[6].Set(-1,0,0);	// west
	offsetVector[7].Set(-1,-1,0);	// northwest
	offsetVector[8].Set(0,0,-1);	// up
	offsetVector[9].Set(0,0,1);	// down
	
	printf("Generating maps\n");
	for ( int i = 0; i < MAX_LEVELS; i++ )
	{
		printf(".");
		m_levelMap[i] = NULL;
		m_levelMap[i] = new mapHack(LEVEL_WIDTH, LEVEL_HEIGHT);
		m_levelMap[i]->Generate();
	}
	printf("\n");
}

hnGame::~hnGame()
{
	for ( int i = 0; i < MAX_LEVELS; i++ )
	{
		delete m_levelMap[i];
	}
}

void
hnGame::ClientJoined(int playerID)
{
	// a client has joined
	
	int x = 0;	
	int y = 0;	
	int z = 0;

	int count = 0;

	do {
		x = (rand() % LEVEL_WIDTH);
		y = (rand() % LEVEL_HEIGHT);

		if ( count++ > 500 )
			break;			// AIEE!!!

	}while ( m_levelMap[z]->WallAt(x,y) & WALL_Any );

	printf("Setting playerID %d initial position to: (%d,%d,%d)\n", playerID, x, y, z);
	m_player[playerID].pos.x = x;
	m_player[playerID].pos.y = y;
	m_player[playerID].pos.z = z;
	m_player[playerID].entity = new entPlayer(hnPoint(x,y,z));
	m_levelMap[z]->PutEntityAt( m_player[playerID].entity, x, y );
	
	netServer::GetInstance()->StartMetaPacket( playerID );
	netServer::GetInstance()->SendClientLocation( &m_player[playerID].pos );
	
	hnPoint pos = m_player[playerID].pos;
	
	pos += offsetVector[DIR_NorthWest];
	netMapUpdateBBox update;
	update.loc = pos;
	update.width = 3;
	update.height = 3;
	update.material = new sint16[update.width * update.height];
	update.wall = new sint16[update.width * update.height];
	update.entityType = new sint8[update.width * update.height];
	
	for ( int j = 0; j < update.height; j++ )
		for ( int i = 0; i < update.width; i++)
		{
			update.material[i+(j*update.width)] = m_levelMap[pos.z]->MaterialAt(pos.x+i,pos.y+j);
			update.wall[i+(j*update.width)] = m_levelMap[pos.z]->WallAt(pos.x+i,pos.y+j);
			update.entityType[i+(j*update.width)] = (m_levelMap[pos.z]->MapTile(pos.x+i,pos.y+j).entity) ? ENTITY_Player : ENTITY_None;
		}
	netServer::GetInstance()->SendMapUpdateBBox( &update );

	delete [] update.material;
	delete [] update.wall;
	delete [] update.entityType;		

	netServer::GetInstance()->TransmitMetaPacket();	// all done!  Send it!
			
}

void
hnGame::ClientQuit(int playerID)
{
	// when a client quits, we need to remove his entity.
	
	m_levelMap[m_player[playerID].pos.z]->RemoveEntity( m_player[playerID].entity );
	delete m_player[playerID].entity;
}


void
hnGame::ClientMove(int playerID, hnDirection dir)
{
	playerData & data = m_player[playerID];
	if ( dir >= 0 && dir < 10 )	// final sanity check
	{
		bool legalMove = false;
		//bool legalMove = true;
		
		if ( dir >= 0 && dir < 8 )
		{
			// north->northwest
			hnPoint potentialPos = m_player[playerID].pos + offsetVector[dir];
			
			if ( m_levelMap[potentialPos.z]->WallAt(potentialPos.x,potentialPos.y) & WALL_Passable )
				if ( m_levelMap[potentialPos.z]->MapTile(potentialPos.x,potentialPos.y).entity == NULL )
					legalMove = true;
		}
		else
		{
			// up or down -- check for appropriate stairways here.
			legalMove = false;
		}

		if ( legalMove )
		{
			// TODO:
			// I'd really like to move most (all?) of this code into hnPlayer::Move() and/or MoveTo().
			
			netServer::GetInstance()->StartMetaPacket(playerID);
			
			// first, player moves out of his current square.
			m_levelMap[m_player[playerID].pos.z]->RemoveEntity(m_player[playerID].entity);
			// now move the player
			hnPoint iniPos = m_player[playerID].pos;
			m_player[playerID].pos += offsetVector[dir];
			hnPoint endPos = m_player[playerID].pos;
			hnPoint pos = m_player[playerID].pos;
			// and put the player object into his new position on the map
			m_player[playerID].entity->SetPosition(pos);
			m_levelMap[pos.z]->PutEntityAt(m_player[playerID].entity, pos.x, pos.y);
			netServer::GetInstance()->SendClientLocation( &m_player[playerID].pos );
			
			// now tell the player about what they can see from this new location.
			// just show him all the ground around himself, at present.
			pos += offsetVector[DIR_NorthWest];
			netMapUpdateBBox update;
			update.loc = pos;
			update.width = 3;
			update.height = 3;
			update.material = new sint16[update.width * update.height];
			update.wall = new sint16[update.width * update.height];
			update.entityType = new sint8[update.width * update.height];
			
			for ( int j = 0; j < update.height; j++ )
				for ( int i = 0; i < update.width; i++)
				{
					update.material[i+(j*update.width)] = m_levelMap[pos.z]->MaterialAt(pos.x+i,pos.y+j);
					update.wall[i+(j*update.width)] = m_levelMap[pos.z]->WallAt(pos.x+i,pos.y+j);
					update.entityType[i+(j*update.width)] = (m_levelMap[pos.z]->MapTile(pos.x+i,pos.y+j).entity) ? ENTITY_Player : ENTITY_None;
					//if ( update.entityType[i+(j*update.width)] == ENTITY_Player )
					//	printf("Sending player location.\n");
				}
			netServer::GetInstance()->SendMapUpdateBBox( &update );
		
			delete [] update.material;
			delete [] update.wall;
			delete [] update.entityType;			

			netServer::GetInstance()->TransmitMetaPacket();	// all done!  Send it!
			
			
			for ( int i = 0; i < MAX_CLIENTS; i++ )
			{
				if ( i != playerID && m_player[i].entity != NULL )
				{
					if ( abs(m_player[i].pos.x - iniPos.x) <= 1 && 
						abs(m_player[i].pos.y - iniPos.y) <= 1 &&
						m_player[i].pos.z == iniPos.z )
					{
						// if we have a connected nearby player who isn't the one we've just sent an update to...
						netServer::GetInstance()->StartMetaPacket(i);
						update.loc = iniPos;
						update.width = 1;
						update.height = 1;
						update.material = new sint16;
						update.wall = new sint16;
						update.entityType = new sint8;
						update.material[0] = m_levelMap[iniPos.z]->MaterialAt(iniPos.x,iniPos.y);
						update.wall[0] = m_levelMap[iniPos.z]->WallAt(iniPos.x,iniPos.y);
						update.entityType[0] = (m_levelMap[iniPos.z]->MapTile(iniPos.x,iniPos.y).entity) ? ENTITY_Player : ENTITY_None;
						
						netServer::GetInstance()->SendMapUpdateBBox( &update );
						netServer::GetInstance()->TransmitMetaPacket();

						delete update.material;
						delete update.wall;
						delete update.entityType;
						// send this client info on the moving entity.
					}
					if ( abs(m_player[i].pos.x - endPos.x) <= 1 && 
						abs(m_player[i].pos.y - endPos.y) <= 1 &&
						m_player[i].pos.z == endPos.z )
					{
						// if we have a connected nearby player who isn't the one we've just sent an update to...
						netServer::GetInstance()->StartMetaPacket(i);
						update.loc = endPos;
						update.width = 1;
						update.height = 1;
						update.material = new sint16;
						update.wall = new sint16;
						update.entityType = new sint8;
						update.material[0] = m_levelMap[endPos.z]->MaterialAt(endPos.x,endPos.y);
						update.wall[0] = m_levelMap[endPos.z]->WallAt(endPos.x,endPos.y);
						update.entityType[0] = (m_levelMap[endPos.z]->MapTile(endPos.x,endPos.y).entity) ? ENTITY_Player : ENTITY_None;
						
						netServer::GetInstance()->SendMapUpdateBBox( &update );
						netServer::GetInstance()->TransmitMetaPacket();
						
						delete update.material;
						delete update.wall;
						delete update.entityType;
						// send this client info on the moving entity.
					}
				}
			}
		}
	}else{
		printf("Tried to move in an illegal direction.\n");
	}

}
