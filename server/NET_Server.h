#ifndef __NET_SERVER_H__
#define __NET_SERVER_H__

// Network class for HackNet.  Our network is set up this way:
//
//  (Game) -> (netServer) -> [internet] -> (netClient) -> (Client)

#include <string>
#include "HN_Enum.h"
#include "HN_Types.h"
#include "HN_Config_Server.h"
#include "NET_Packet.h"
#include "ENT_Base.h"
#include "OBJ_Types.h"

class mapTile;
class hnGame;
class mapBase;
class hnPoint;
struct sockaddr_in;

#define MAX_META_PACKET_SIZE (65536)
#define MAX_CLIENT_PACKET_SIZE (128)

struct clientData
{

	int socket;
	char packet[MAX_CLIENT_PACKET_SIZE]; // construction area for incoming packets
	char * packetRecv; // pointer into 'packet' member above, where incoming packet data should be appended.
	sint16 incomingPacketSize; // how many more bytes of packet data to read before packet is finished
	sint16 packetFullSize; // how many total bytes in packet data when finished.

	sint16 incomingPacketSizeBuffer; // assemble packet length here
	sint16 incomingPacketSizeSize; // how many more bytes of packet data to read before we know how big our incoming packet will be.
};

class netSocketThread;

class netServer
{
	friend class netSocketThread;

	static netServer* s_instance;

	hnGame* m_game;

	int m_socket;
	sockaddr_in* m_localAddress;

	clientData m_client[MAX_CLIENTS]; // socket fds for all our clients
	int m_clientCount; // how many clients have we got?

	netMetaPacketOutput* m_metaPacket;
	char m_buffer[MAX_META_PACKET_SIZE];
	int m_packetClientID; // client we're currently making a metapacket for

	int m_Port; // Port number to connect to

protected:
	netServer(int port); // constructs and starts listening on our port..
	virtual ~netServer();
	void StartServer();
	bool ProcessClientPacket(int clientID, char *buffer, short bufferLength); // false == invalid packet, so kill the connection.
public:
	static void Startup(int port);
	static void Shutdown();
	static netServer * GetInstance();

	void Go(); // wait for connections and send data to our game.

	void DisconnectClientID(int id); // disconnect the client with the given ID number
	void ProcessMove(); // process an incoming packet of data

	bool StartMetaPacket(int clientID); // who will eventually receive this packet?
	bool TransmitMetaPacket(); // send the metapacket we were constructing

	void SendClientLocation(const hnPoint &loc); // tell the client where they are.
	void SendStatus(hnStatus *status);
	void SendGroupData(int groupMemberCount, int groupTurnsSubmitted,
			bool clientSubmittedTurn);
//	bool		SendMapUpdate(int clientID, mapBase *map);	// server sends move update to client
	void SendMapTile(const hnPoint & loc, const mapTile & floorType); // new floortype
	void SendMapUpdateBBox(netMapUpdateBBox & bbox);
	void SendInventory(netInventory &inven);
	void SendInventoryItem(const objDescription &desc, int id);
	void SendMapObjectList(const hnPoint & loc, int numObjects,
			entType floorType); // change to list of objects on this square.  # of objects in pile, and topmost object type are sent to client.
	void SendDungeonReset(sint8 levelCount);
	void SendMapReset(int width, int height, int depth);
	void SendMessage(char * message);

	void SendObjectStats(uint16 objectClasses);
	void SendObjectName(uint16 objectID, uint16 type, const char *name);

	void SendDroppedItem(const objDescription & object);
	void SendTakenItem(const objDescription & object, int inventorySlot);
	void SendWieldedItem(const objDescription & object, int inventorySlot);

	void SendQuitConfirm(int clientID); // doesn't require a metapacket
	void SendSaveConfirm(int clientID); // doesn't require a metapacket
	void SendBadPacketNotice(int clientID); // doesn't require a metapacket
};

#endif	//__HN_NETSERVER_H__
