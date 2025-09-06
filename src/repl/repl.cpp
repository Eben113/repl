#include <headers/porting.h> 
#include <headers/Interpreter.h> 

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

namespace{
    void print_callback(std::string const& result){
        std::cout << ">>>" << result << std::endl;
    }

    static bool quit = false;

    repl::Argument quit_callback(std::vector<repl::Argument> const& args){
        quit = true;
        return 0;
    }

    repl::Argument add_handler(std::vector<repl::Argument> const& args)
    {
        int sum = 0;
        for(auto const& arg : args){
            auto const str_val = repl::convert_argument<std::string>(arg).value_or("0");

            auto const maybe_val = repl::TypeConverter<repl::ArgType::INT>::Convert(str_val);
            if(!maybe_val){
                return 0;
            }
            sum += repl::convert_argument<int>(*maybe_val).value_or("0");
        }
        return sum;
    }

    repl::Argument echo_handler(std::vector<repl::Argument> const& args){
        std::ostringstream output;
        for(auto const arg : args){
            auto const maybe_val = repl::convert_argument<std::string>(arg);
            if(!maybe_val){
                return std::string("echo: invalid argument");
            }
            output << *maybe_val << " ";
        }

        return output.str();
    }
}

int main(){

    auto interpreter = repl::make_interpreter();


    interpreter -> registerCommand("echo", {echo_handler, std::nullopt});
    interpreter -> registerCommand("add", {add_handler, std::nullopt});
    interpreter -> registerCommand("quit", {quit_callback, std::nullopt});
    interpreter->setCallback(print_callback);


    std::string query;
    while(!quit){
        std::cout << ">> ";
        std::getline(std::cin, query);

        interpreter->queue(query);
        interpreter->poll();
    }
}