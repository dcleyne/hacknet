SET(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "-g -Wall")
include_directories(. ../common ../client)

########### next target ###############

set(SOURCES 
    ../common/HN_Point.cpp	
    ../common/HN_Random.cpp
    ../common/HN_Status.cpp
    ../common/MAP_Client.cpp
    ../common/NET_Packet.cpp
    ../common/OBJ_Types.cpp

    ../client/HN_Display.cpp
    ../client/NET_Client.cpp
    ../client/OBJ_Registry.cpp

    HN_DisplayTTY.cpp 
    HN_DisplayTTY.h 
    HN_Main_Client.cpp)

add_executable(hacknet.tty ${SOURCES})
add_definitions("-D VERSION=\\\"0.0.7\\\"")
add_definitions("-D HAVE_LIBNCURSES" "-D HAS_COLOR_SET")
target_link_libraries(hacknet.tty ${CURSES_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

########### install files ###############

