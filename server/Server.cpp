#include <iostream>
#include <thread>

#include "../../common/network/Logger.hpp"
#include "commandline/Clm.hpp"

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

	// create server
	CliMenu interface;

	interface.AddCommand(
        "start", [port](const auto& args)
        { Logger::Get().Log(Logger::Level::INFO, "Starting server on port " + std::to_string(port)); });
	
	std::thread cli(updateCli, std::ref(interface));
	cli.detach();

	while (true)
        continue;
	
	return 0;
}