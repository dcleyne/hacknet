#ifndef __NET_PACKET_H__
#define __NET_PACKET_H__

#include "HN_Types.h"
#include "HN_Point.h"
#include "HN_Enum.h"


enum{
	SPT_Message,		// message from the server -- THIS MUST ALWAYS BE TYPE 0, SO WE CAN SEND VERSION ERROR MESSAGES TO CLIENT
	SPT_BadPacketNotice,	// ALWAYS ID 1.  -- You just sent me a bad packet, and are being disconnected.  Version mismatch?
	SPT_JoinOk,		// you've been accepted.
	SPT_ClientLocation,
	SPT_ClientStatistics,	// send client strength, dex, etc.
	SPT_ClientHitPoints,	// send client hit point info
	SPT_ClientSpellPoints,	// send client spell point info
	SPT_ClientExperience,	// send client experience point info
	SPT_MapTile,
	SPT_DungeonReset,
	SPT_MapReset,
	SPT_MapUpdateBBox,
	SPT_MapObjectList,
	SPT_MapEntity,
	SPT_GroupData,		// sending information on the player's group.
	SPT_QuitConfirm,	// yes, you're out of the game
	SPT_SaveConfirm,	// yes, you've been saved and are out of the game
	SPT_Refresh,		// we just finished sending a set of events.  Go ahead and refresh the screen now.
	SPT_MAX
};

struct netClientLocation
{
	hnPoint loc;
};

struct netClientStatistics
{
	sint8 strength;
	sint8 dexterity;
	sint8 constitution;
	sint8 intelligence;
	sint8 wisdom;
	sint8 charisma;
};

struct netClientHitPoints
{
	sint16 maxHitPoints;
	sint16 hitPoints;
};

struct netClientSpellPoints
{
	sint16 maxSpellPoints;
	sint16 spellPoints;
};
	
struct netClientExperience
{
	sint16 experiencePoints;
	sint8 level;
};

struct netMapTile
{
	hnPoint loc;
	sint16 material;
	sint16 wall;
};

struct netGroupData
{
	sint8 memberCount;
	sint8 memberTurns;
	sint8 haveTurnFromClient;
};

struct netMapReset		// we've moved to a new map.. init a map with this width/height.
{
	sint8 width;
	sint8 height;
	sint8 depth;
};

struct netMapUpdateBBox
{
	hnPoint loc;
	sint8 width;
	sint8 height;

	sint16 *material;  // width-first material array (top row, then next row, etc)
	sint16 *wall;
	sint8 *entityType;
};

struct netMapObjectList
{
	hnPoint loc;
	sint8 numObjects;
	sint8 *objectType;
};

struct netMapEntity
{
	hnPoint loc;
	sint8 objectType;
};


enum 
{
	CPT_Version,	// must always be ID zero so we can communicate with server regardless of version number!
	CPT_Move,
	CPT_Wait,
	CPT_Attack,	// an attack is actually different from a move, so we don't attack people by accident.
	CPT_Name,
	CPT_Talk,
	CPT_RequestRefresh,	// request refreshed information on this level
	CPT_Save,	// Save and quit
	CPT_Quit,	// Quit without saving.
	CPT_MAX
};

class netMetaPacket
{
protected:
	char			*m_buffer;
	char			*m_bufferPoint;			// we're we're currently looking in the buffer

	unsigned long		m_bufferLength;			// how long the buffer actually is.
	unsigned long   	m_bufferDistance;		// how much we've read/written to the buffer already

	bool			m_error;			// set true if we have a read/write error.

public:
				netMetaPacket(char *buffer, uint32 bufferLength);
	virtual			~netMetaPacket();

	char *			GetBuffer() { return m_buffer; }
	virtual long		GetBufferLength() { return m_bufferLength; }

	bool			Done() { return m_bufferDistance >= m_bufferLength; }
	
	/*****************   Server Packets  ******************/
	bool			ClientLocation( netClientLocation &packet );
	bool			ClientStatistics( netClientStatistics &packet );
	bool			ClientExperience( netClientExperience &packet );
	bool			ClientHitPoints( netClientHitPoints &packet );
	bool			ClientSpellPoints( netClientSpellPoints &packet );
	bool			TextMessage( char * messagebuffer, sint16 & bufferlength );
	bool			MapTile( netMapTile &packet );
	bool			GroupData( netGroupData &packet );
	bool			DungeonReset( sint8 & levelCount );
	bool			MapReset( netMapReset &packet );
	bool			MapUpdateBBox( netMapUpdateBBox &packet );
	bool			MapEntity( netMapEntity &packet );
	bool			MapObjectList( netMapObjectList &packet );
	bool			JoinOK();
	bool			QuitConfirm();
	bool			SaveConfirm();
	bool			Refresh();
	bool			BadPacketNotice();
	
	/*****************   Client Packets  ******************/
	bool			ClientMove( sint8 & direction );
	bool			ClientAttack( sint8 & direction );
	bool			ClientWait();
	bool			ClientName( char * namebuffer, sint16 & bufferLength );
	bool			ClientTalk( char * talkbuffer, sint16 & bufferLength );
	bool			ClientRequestRefresh( sint8 & level );
	bool			ClientSave();
	bool			ClientQuit();

	
	virtual bool 		Sint8( sint8 & ) = 0;
	virtual bool 		Sint16( sint16 & ) = 0;
	virtual bool 		Sint32( sint32 & ) = 0;
	virtual bool 		Uint8( uint8 & ) = 0;
	virtual bool 		Uint16( uint16 & ) = 0;
	virtual bool 		Uint32( uint32 & ) = 0;
	virtual bool		String( char *, sint16 & ) = 0;

	virtual	bool		Input() { return false; }
	virtual bool		Output() { return false; }
};

class netMetaPacketInput: public netMetaPacket		// we've received this and are parsing out of it
{
public:
				netMetaPacketInput(char *packet, uint32 bufferLength);
	virtual			~netMetaPacketInput();

	virtual bool 		Sint8( sint8 & );
	virtual bool 		Sint16( sint16 & );
	virtual bool 		Sint32( sint32 & );
	virtual bool 		Uint8( uint8 & );
	virtual bool 		Uint16( uint16 & );
	virtual bool 		Uint32( uint32 & );
	virtual bool		String( char *, sint16 & );
	
	virtual sint8 		PeekSint8();
	virtual sint16 		PeekSint16();
	virtual sint32		PeekSint32();
	
	virtual	bool		Input() { return true; }
};

class netMetaPacketOutput: public netMetaPacket		// we're writing into this
{
public:
				netMetaPacketOutput(char *packet, uint32 bufferLength);
	virtual			~netMetaPacketOutput();
	
	virtual long		GetBufferLength() { return m_bufferDistance; }	// tell how many bytes we actually wrote, not how big our buffer was

	virtual bool 		Sint8( sint8 & );
	virtual bool 		Sint16( sint16 & );
	virtual bool 		Sint32( sint32 & );
	virtual bool 		Uint8( uint8 & );
	virtual bool 		Uint16( uint16 & );
	virtual bool 		Uint32( uint32 & );
	virtual bool		String( char *, sint16 & );
	
	virtual	bool		Output() { return true; }
};





#endif //__NET_PACKET_H__
