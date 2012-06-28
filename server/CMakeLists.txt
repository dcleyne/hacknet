set(CMAKE_CXX_FLAGS "-g -Wall")
SET(CMAKE_MODULE_PATH "../cmake")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Threads REQUIRED)
find_package(Boost 1.48.0 COMPONENTS system filesystem)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

FIND_PACKAGE(log4cpp REQUIRED)
if (LOG4CPP_FOUND)
	INCLUDE_DIRECTORIES(${LOG4CPP_INCLUDE_DIR})
endif()

include_directories(. ../common)

########### next target ###############

set(SOURCES
    ../common/HN_Point.cpp	
    ../common/HN_Random.cpp
    ../common/HN_Status.cpp
    ../common/MAP_Client.cpp
    ../common/NET_Packet.cpp
    ../common/OBJ_Types.cpp
    HN_Main_Server.cpp
    HN_Group.cpp
    HN_Group.h
    HN_Game.cpp
    HN_Game.h
    HN_Player.cpp
    HN_Player.h
    HN_Dungeon.cpp
    HN_Dungeon.h
    HN_Config_Server.h
    HN_Properties.h
    HN_Logger.h
    HN_Logger.cpp
    HN_Convert.h
    NET_Server.cpp
    NET_Server.h
    MAP_Base.cpp
    MAP_Base.h
    MAP_Hack.cpp
    MAP_Hack.h
    MAP_Cavern.cpp
    MAP_Cavern.h
    ENT_Base.cpp
    ENT_Base.h
    ENT_Definition.cpp
    ENT_Definition.h
    ENT_GridBug.cpp
    ENT_GridBug.h
    ENT_Human.cpp
    ENT_Human.h
    OBJ_Convert.h
    OBJ_Base.cpp
    OBJ_Base.h
    OBJ_Definition.cpp
    OBJ_Definition.h
    OBJ_Amulet.cpp
    OBJ_Amulet.h
    OBJ_Armour.cpp
    OBJ_Armour.h
    OBJ_Manager.cpp
    OBJ_Manager.h
    OBJ_Potion.cpp
    OBJ_Potion.h
    OBJ_Ring.cpp
    OBJ_Ring.h
    OBJ_Food.cpp
    OBJ_Food.h
    OBJ_Weapon.cpp
    OBJ_Weapon.h)

add_executable(hacknetd ${SOURCES})
add_definitions("-D VERSION=\\\"0.0.7\\\"")

if(Boost_FOUND AND LOG4CPP_FOUND)
    target_link_libraries(hacknetd ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${LOG4CPP_LIBRARIES})                                                                                                                                                                                                                            
endif()

########### install files ###############

