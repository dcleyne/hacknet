#ifndef __HN_DISPLAY_H__
#define __HN_DISPLAY_H__

#include "HN_Enum.h"
#include "HN_Point.h"
#include "MAP_Client.h"
#include "ENT_Types.h"

#define MAX_NAME_BYTES (128)

class netClient;

class hnDisplay
{
protected:
	char		m_name[MAX_NAME_BYTES];
	netClient 	*m_client;
	hnPoint		m_position;
	mapClient	*m_map;		// this is our map of the current level...
public:
			hnDisplay( char * name );
	virtual		~hnDisplay();

	void		SetClient( netClient *client ) { m_client = client; }
	virtual bool 	Go() = 0;  	// main event loop goes in here.  Return
					// true if we're alive and saving, false
					// if otherwise.
	
	virtual void	Refresh() {}
	
	virtual void	MapReset( sint8 width, sint8 height );
	virtual void	UpdateLocation( const hnPoint &point ) { m_position = point; }
	virtual void	UpdateMapTile( sint8 x, sint8 y, const mapClientTile & tile );
	virtual void	UpdateMapCreature( sint8 x, sint8 y, entType type );
	virtual void	TextMessage( char * message ) {}

	char *		GetName() { return m_name; }
};

#endif
