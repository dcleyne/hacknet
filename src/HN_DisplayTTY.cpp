#if HAVE_LIBNCURSES
  #include <ncurses.h>
#else
  #if HAVE_LIBCURSES
    #include <curses.h>
  #else
    #error "No curses library found!"
  #endif
#endif
#include <pthread.h>
#include <string.h>
#include "HN_DisplayTTY.h"
#include "NET_Client.h"
#include "HN_Enum.h"

//#define __DEBUGGING_NETWORK__

#if HAS_COLOR_SET
#else
	// if we don't have a 'color_set' function in curses, provide something for the source code's calls
	// to bind to.
	int color_set(short,void * ){}
#endif


hnDisplayTTY::hnDisplayTTY( char * name ):
	hnDisplay(name),
	m_mode(MODE_Normal),
	m_talkLength(0),
	m_messageLines(0),
	m_needsRefresh(false),
	m_done(false)
{

	m_talkBuffer[0] = '\0';

	for ( int i = 0; i < MAX_MESSAGE_LINES; i++ )
		m_messageBuffer[i][0] = '\0';

#ifndef __DEBUGGING_NETWORK__
	initscr();
	keypad(stdscr,true); // enable keyboard mapping
	nonl();
	cbreak();
	noecho();

	if ( has_colors() )
	{
		start_color();
		init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
		init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
		init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
		init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
		init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
		init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
		init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
	}
	
	refresh();	
#endif
	
}

hnDisplayTTY::~hnDisplayTTY()
{
#ifndef __DEBUGGING_NETWORK__
	refresh();
	endwin();
#endif

	// put any debugging output here, if needed -- curses has been shut down and printf is safe again.
}

bool
hnDisplayTTY::Go()
{
	pthread_t ioThread;
	
	// create a thread to run the event loop...
	
	pthread_create( &ioThread, NULL, StartEventLoop, (void *)this );
	
	return 0;
}

void * StartEventLoop(void *arg)
{
	hnDisplayTTY *display = (hnDisplayTTY *)arg;
	
	display->EventLoop();

	return 0;
}

void
hnDisplayTTY::EventLoop()
{
	
#ifdef __DEBUGGING_NETWORK__
	m_client->SendMove(DIR_West);
	//m_client->SendTalk("Hoi!");
#endif	

	while ( !m_done )
	{
		int commandkey = getch();

		switch( m_mode )
		{
			case MODE_Normal:
				HandleKeypressNormal( commandkey );
				break;
			case MODE_Talking:
				HandleKeypressTalking( commandkey );
				break;
		}
		Refresh();
	}

	m_client->Disconnect();
}

void
hnDisplayTTY::HandleKeypressNormal(int commandkey)
{
	switch( commandkey )
	{
		case 'q':
			m_client->SendQuit(false);
			m_done = true;
			break;
		case '"':
			m_mode = MODE_Talking;
			m_needsRefresh = true;		// need to print prompt on screen
			break;
		case 'h':
		case '4':
			//printf("Got move west.\n");
			m_client->SendMove(DIR_West);
			break;
		case 'j':
		case '2':
			//printf("Got move south.\n");
			m_client->SendMove(DIR_South);
			break;
		case 'k':
		case '8':
			//printf("Got move north.\n");
			m_client->SendMove(DIR_North);
			break;
		case '6':
		case 'l':
			//printf("Got move east.\n");
			m_client->SendMove(DIR_East);
			break;
		case '9':
		case 'u':
			m_client->SendMove(DIR_NorthEast);
			break;
		case '7':
		case 'y':
			m_client->SendMove(DIR_NorthWest);
			break;
		case '3':
		case 'n':
			m_client->SendMove(DIR_SouthEast);
			break;
		case '1':
		case 'b':
			m_client->SendMove(DIR_SouthWest);
			break;
		case '>':
			m_client->SendMove(DIR_Down);
			break;
		case '<':
			m_client->SendMove(DIR_Up);
			break;
		default:
			//printf("Got unknown keypress.\n");
			break;
	}
}

void
hnDisplayTTY::HandleKeypressTalking( int commandKey )
{
	switch ( commandKey )
	{
		case '\r':				// other characters I should be checking for here?
		case KEY_ENTER:
			m_mode = MODE_Normal;		 
			m_client->SendTalk( m_talkBuffer );
			m_talkLength = 0;
			m_talkBuffer[0]='\0';
			m_needsRefresh = true;		// get rid of prompts
			// transmit the string now!
			break;
		case KEY_BACKSPACE:
			if ( m_talkLength > 0 )
			{
				m_talkLength--;
				m_talkBuffer[m_talkLength] = '\0';
			}
			m_needsRefresh = true;
			break;
		default:
			// TODO:  Check commandkey is a valid alphanumeric character!
			
			if ( m_talkLength < MAX_TALK_BYTES-1 )
			{
				m_talkBuffer[m_talkLength] = commandKey;
				m_talkBuffer[m_talkLength+1] = '\0';
				m_talkLength++;
				m_needsRefresh = true;
			}
			break;
	}
}

void
hnDisplayTTY::UpdateLocation( const hnPoint &point )
{
	hnDisplay::UpdateLocation(point);

#ifndef __DEBUGGING_NETWORK__
	//color_set( COLOR_WHITE,NULL);
	//mvaddch(m_position.y,m_position.x,'@');
	//move(m_position.y,m_position.x);
	// now we need to update our display appropriately...
//	refresh();
#endif
}

void
hnDisplayTTY::PlotSquare(sint8 x, sint8 y)
{
	hnMaterialType floorType = m_map[m_position.z]->MaterialAt(x,y);
	hnWallType wallType = m_map[m_position.z]->WallAt(x,y);
	
	
	/*const char floorTileChar[] ={
		'?',
		'.',
		' ',
		'#',
		'.',
		'{',
		'^',
		' '
	};*/

	const char floorTileColor[] ={
		COLOR_WHITE,
		COLOR_WHITE,
		COLOR_WHITE,
		COLOR_WHITE,
		COLOR_BLUE,
		COLOR_BLUE,
		COLOR_RED,
		COLOR_WHITE
	};

	// todo:  Adjust display for walls
#ifndef __DEBUGGING_NETWORK__
	
	char theChar = ' ';

	switch (wallType)
	{
		case WALL_Vertical:
			theChar = '|';
			break;
		case WALL_Horizontal:
			theChar = '-';
			break;
		case WALL_Cross:
			theChar = '-';
			break;
		case WALL_Corridor:
			theChar = '#';
			break;
		case WALL_Room:
		case WALL_Doorway:
			theChar = '.';
			break;
		case WALL_StairsUp:
			theChar = '<';
			break;
		case WALL_StairsDown:
			theChar = '>';
			break;
		default:
			theChar = wallType;
			break;
	}
	
	if ( x >= 0 && x < m_map[m_position.z]->GetWidth() && y >= 0 && y < m_map[m_position.z]->GetHeight() )
	{
		mapClientTile & tile = m_map[m_position.z]->MapTile(x,y);
		color_set( floorTileColor[floorType],NULL);
		
		if ( tile.entity == ENTITY_Human )	// if someone is standing here...
		{
			color_set( COLOR_WHITE, NULL );
			theChar = '@';			// draw '@' instead of ground.  This is a HACK!
		}
		if ( tile.entity == ENTITY_GridBug )
		{
			color_set( COLOR_MAGENTA, NULL );
			theChar = 'x';
		}
		mvaddch(y + MAX_MESSAGE_LINES+1,x,theChar);
		move(m_position.y + MAX_MESSAGE_LINES+1,m_position.x);

		m_needsRefresh = true;
	}
#endif
}

void
hnDisplayTTY::UpdateMapTile(const hnPoint &point, const mapClientTile &tile)
{
	hnDisplay::UpdateMapTile(point,tile);
	m_needsRefresh = true;
	//PlotSquare(x,y);
}


void
hnDisplayTTY::UpdateMapCreature( const hnPoint &point, entType type )
{
	hnDisplay::UpdateMapCreature(point,type);
	m_needsRefresh = true;
	//PlotSquare(x,y);
}

void
hnDisplayTTY::TextMessage( char * message )
{
	if ( m_messageLines < MAX_MESSAGE_LINES )
	{
		// we haven't filled up our set of lines yet, so just add us...
		strncpy( m_messageBuffer[m_messageLines], message, MAX_MESSAGE_BYTES );
		m_messageLines++;
	}
	else
	{
		// we're full, so scroll the lines up, then add us at the bottom.
		for ( int i = 0; i < MAX_MESSAGE_LINES-1; i++ )
			strncpy( m_messageBuffer[i], m_messageBuffer[i+1], MAX_MESSAGE_BYTES );
		strncpy( m_messageBuffer[MAX_MESSAGE_LINES-1], message, MAX_MESSAGE_BYTES );	
	}
	
	m_needsRefresh = true;
}

void
hnDisplayTTY::UpdateGroupData( int groupMemberCount, int groupMemberTurnCount, bool submittedTurn )
{
	hnDisplay::UpdateGroupData( groupMemberCount, groupMemberTurnCount, submittedTurn );

	if ( (groupMemberCount != m_groupMemberCount) || (m_groupMemberCount > 1) )
		m_needsRefresh = true;
}

void
hnDisplayTTY::Refresh()
{
	if ( m_needsRefresh )
	{
#ifndef __DEBUGGING_NETWORK__
		// redraw screen -- this is hackish.. I ought to make a single function that does this,
		// instead of repeatedly calling a single function for every point on the screen.
		for ( int j = 0; j < m_map[m_position.z]->GetHeight(); j++ )
			for ( int i = 0; i < m_map[m_position.z]->GetWidth(); i++ )
			{
				PlotSquare(i,j);
			}
		
		// clear upper prompt area.
		int maxy, maxx;
		getmaxyx(stdscr, maxy, maxx);
		for ( int j = 0; j < MAX_MESSAGE_LINES+1; j++ )
		{
			for ( int i = 0; i < maxx; i++ )
			{
				mvaddch(j,i,' ');
			}
		}
		// clear lower prompt area
		for ( int j = 23; j < 24; j++ )
		{
			for ( int i = 0; i < maxx; i++ )
			{
				mvaddch(j,i,' ');
			}
		}
		
		// do status bar
		
		if ( m_groupMemberCount > 1 )
		{
			move( 23, 0 );
			printw("%d/%d group turns.  ", m_groupMemberTurnCount, m_groupMemberCount);
			if ( !m_submittedTurn && m_groupMemberTurnCount > 0 )	// if somebody's entered a turn and I haven't..
				printw("You haven't submitted a turn.");
		}
		// do upper prompts.
		
		for ( int i = 0; i < MAX_MESSAGE_LINES; i++ )
		{
			move( i, 0 );
			printw("%s", m_messageBuffer[i]);
		}
		
		if ( m_mode == MODE_Talking )
		{
			// draw our string in the top few lines..
			
			move( MAX_MESSAGE_LINES, 2 );
			printw("Say: %s", m_talkBuffer);
		}
		else
		{
			// now put our cursor back onto us, when in normal mode.
			move(m_position.y + MAX_MESSAGE_LINES+1,m_position.x);
		}
	
		refresh();
#endif
		m_needsRefresh = false;
	}
}
