#include <chrono>
#include <thread>
#include "commandline/Clm.hpp"
#include "../common/network/Logger.hpp"
#include "serverimpl/ServerImpl.hpp"

void updateCli(CliMenu& menu)
{
	while (true)
		menu.Show();
}

int main(int argc, char* argv[])
{
    Logger::Get().Init("Logs.txt");

    size_t port = 0; 
	if (argc != 2)
	{
        std::cout << "Usage: " << argv[0] << " <port>, Defaulting to port 40000" << std::endl;
        port = 40000;
	}
	else
	{
		try
		{
            port = std::stoi(argv[1]);
		}
        catch (const std::exception& e)
		{
			std::cerr << "Error: " << e.what() << std::endl;
			return 1;
        }
	}

	ServerImpl server(port);
	CliMenu interface;

	interface.AddCommand("List_players", [&server](const auto& args)
	{
		if (!server.ListPlayers())
			LOG_INFO_CONSOLE("ListPlayers: No players found");
	});
	interface.AddCommand("Show_Map_Layout", [&server](const auto& args)
	{
		server.KickPlayer();
	});
	
	std::thread cli(updateCli, std::ref(interface));
	cli.detach();

	auto start = std::chrono::high_resolution_clock::now();
	while (true)
	{
		auto end = std::chrono::high_resolution_clock::now();
		float ts = (start - end).count();
		start = end;

		server.UpdatePhysics(ts);
		server.Update();
	}
}