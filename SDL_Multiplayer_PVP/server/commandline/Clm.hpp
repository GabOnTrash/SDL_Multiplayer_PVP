#include <deque>
#include <sstream>
#include <iostream>
#include <functional>
#include <unordered_map>

struct CliMenu
{
    #define COMMAND(capture) [capture](const std::vector<std::string>& args)

    using Command = std::function<void(std::vector<std::string>&)>;  // function with a vector of parameters

    CliMenu(char charToPrint = '>', size_t historySize = 10)
        : historySize(historySize), charToPrint(charToPrint) 
    {
        AddCommand("help", [this](const auto& args)
        { 
            for (const auto& [name, _] : options) 
                if (name != "help")
                    std::cout << "- " << name << "\n";
        });
    }
    ~CliMenu() = default;

    void AddCommand(const std::string& name, Command func)
    {
        options[name] = std::move(func);
    }
    void Show()
    {
        std::string line;
        
        std::cout << charToPrint << ' ';
        if (!std::getline(std::cin, line)) return;
        if (line.empty()) return;
        
        findCommand(splitInTokens(line));
        line.clear();
    }

private:
    std::vector<std::string> splitInTokens(const std::string& s)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);

        while (tokenStream >> token)
            tokens.push_back(token);

        return tokens;
    }
    void findCommand(std::vector<std::string> commandParts)
    {
        if (commandParts.empty()) return;
        const std::string name = commandParts[0];

        auto it = options.find(name);
        if (it != options.end())
        {
            if (commandsHistory.size() >= historySize)
                commandsHistory.pop_back();
            if (name != "history" && name != "help")
                commandsHistory.emplace_front(commandParts);

            commandParts.erase(commandParts.begin());
            try
            {
                it->second(commandParts);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else 
            std::cerr << "Command not found: type 'help' for suggestions" << std::endl;
    }
    
    std::deque<std::vector<std::string>> commandsHistory;
    std::unordered_map<std::string, Command> options;
    size_t historySize = 0;
    char charToPrint = 0;
};