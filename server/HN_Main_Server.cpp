#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string>
#include <iostream>
#include "HN_Game.h"
#include "HN_Random.h"
#include "NET_Server.h"
#include "HN_Group.h"
#include "HN_Logger.h"

#include <boost/filesystem.hpp>

int main(int argc, char *argv[])
{
	// parse arguments

	bool legalArgs = true;
	bool useGroups = false;

	std::string basePath;
	int port;

	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			if (strncmp(argv[i], "--with-groups", 13) == 0
					|| strncmp(argv[i], "--withgroups", 12) == 0
					|| strncmp(argv[i], "--wg", 4) == 0)
			{
				useGroups = true;
			}
			else
			{
				// Have a look to see if it is the base directory
				std::string fullArg(argv[i]);
				size_t iIndex = fullArg.rfind("=");
				if (iIndex != std::string::npos && fullArg.length() >= 2)
				{
					std::string arg = fullArg.substr(0, iIndex);
					std::string val = fullArg.substr(iIndex + 1,
							fullArg.length() - iIndex - 1);

					if (arg.compare("--path") == 0)
					{
						basePath = val;
					}
					if (arg.compare("--port") == 0)
					{
						port = atoi(val.c_str());
					}
				}
			}
		}
	}

	// If any of the required information is missing
	if (basePath.compare("") == 0 || port == 0)
	{
		legalArgs = false;
	}

	if (!legalArgs)
	{
		printf("HackNet Server version %s\n", VERSION);
		printf(
				"Usage: hacknetd --path=<dir for files> --port=<port number for connections> [--with-groups]\n");
		exit(1);
	}

	std::string LogPath = basePath + "/log";
	std::string SavePath = basePath + "/save";

	try
	{
		boost::filesystem::create_directories(LogPath);
		boost::filesystem::create_directories(SavePath);
	} catch (...)
	{
		printf("!!! Failed to initialise server storage !!!");
		exit(1);
	}

	HN_Logger::SetLogFile(LogPath + "/hacknetd.log");
	HN_Logger::SetLogLevel(HN_Logger::DEBUG);
	if (!useGroups)
		hnGroupManager::SetMaxGroupDistance(0);

	HN_Logger::LogInfo("HackNet version %s starting up...", VERSION);
	HN_Logger::LogInfo("BasePath(%s), Port(%i)", basePath.c_str(), port);

	HN_Logger::LogInfo("Initialising random number system...");
	hnRandom::Startup(time(NULL));

	HN_Logger::LogInfo("Initialising game...");
	hnGame::Startup();
	hnGame *game = hnGame::GetInstance();

	if (game)
	{
		HN_Logger::LogInfo("Starting server...");
		netServer::Startup(port);
		netServer *server = netServer::GetInstance();

		server->Go();

		HN_Logger::LogInfo("Server exiting.");
		netServer::Shutdown();
	}

	HN_Logger::LogInfo("Game ending...");
	hnGame::Shutdown();
}
