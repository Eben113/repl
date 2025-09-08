#include "headers/Interpreter.h"

#include <iterator>
#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <algorithm>

namespace
{
    std::vector<std::string_view> split(std::string_view str){
        constexpr std::string_view delim(" ");
        std::vector<std::string_view>  output;

        for(auto first = str.data(),
                second = str.data(),
                last = first + str.size();
            first != last || second != last;
            first = second + 1){

                second = std::find_first_of(first, last, std::cbegin(delim), std::cend(delim));

                if(first != second){
                    output.emplace_back(first, second-first);
                }

                if(second == last) break;
            }
        return output;
    }



    std::optional<std::vector<repl::Argument>> ParseArgs(
        std::vector<std::string_view> const& args,
        repl::ArgTypes const& types){

            if(args.size() != types.size()){
                return std::nullopt;
            }

        std::vector<repl::Argument> ParsedArgs;

        for(std::size_t i = 0; i < args.size(); ++i){

            std::optional<repl::Argument> current = std::nullopt;
            switch(types[i]){
                case repl::ArgType::INT:
                    current = repl::TypeConverter<repl::ArgType::INT>::Convert(args[i]);
                    break;
                case repl::ArgType::STRING:
                    current = repl::TypeConverter<repl::ArgType::STRING>::Convert(args[i]);
                    break;
                case repl::ArgType::FLOAT:
                    current = repl::TypeConverter<repl::ArgType::FLOAT>::Convert(args[i]);
                    break;
            }

            if(!current){
                return std::nullopt;
            }

            ParsedArgs.push_back(*current);
        }
        return ParsedArgs;

        }


    std::vector<repl::Argument> makeArgs(std::vector<std::string_view> const& strings){

        std::vector<repl::Argument> Args;
        std::transform(begin(strings), end(strings), back_inserter(Args),
        [](auto  const& args){
            return std::string{begin(args), end(args)};
        });

        return Args;   
    }
}

bool repl::Interpreter::queue(std::string const& command_str){
    auto const split_tokens = split(command_str);
    if(split_tokens.size() == 0){
        return false;
    }

    auto const command_name = std::string{begin(split_tokens[0]), end(split_tokens[0])};
    auto const found = _commands.find(command_name);
    if(found == end(_commands)){
        if(_callback){
            (*_callback)("Unrecnogised command: " + command_name);
        }

        return false;
    }

    std::vector<std::string_view> const str_args{next(begin(split_tokens)), end(split_tokens)};

    auto const command = found->second;
    if(command.arg_types){
        auto const checkedArgs = ParseArgs(str_args, *command.arg_types);
        if(!checkedArgs){
            return false;
        }
        _command_queue.push({command, *checkedArgs});
        return true;
    }

    _command_queue.push({command, makeArgs(str_args)});
    return true;
}

bool repl::Interpreter::registerCommand(std::string const& command_name, CommandFunc const& handler){
    auto const split_commands = split(command_name);
    if(split_commands.size() != 1){
        return false;
    }

    auto const found = _commands.find(command_name);
    if(found != end(_commands)){
        return false;
    }
    _commands[command_name] = handler;
    return true;
}

void repl::Interpreter::poll(){
    for(std::size_t i = 0; i < _command_queue.size(); ++i){
        auto const command_req = _command_queue.front();
        auto const result = command_req.command.f(command_req.arguments);

        if(_callback){
            auto const out_str = std::visit(overloaded{
                [](std::string const& arg) -> std::optional<std::string> {return arg;},
                [](int const& arg) -> std::optional<std::string> {return std::to_string(arg);},
                [](float const& arg) -> std::optional<std::string> {return std::to_string(arg);},
                [](auto const& arg) -> std::optional<std::string> {return std::string(arg);}
            }, result);

            if(out_str){
                (*_callback)(*out_str);
            }
        }
        _command_queue.pop();
    }
}

void repl::Interpreter::setCallback(Callback const& callback){
    _callback = callback;
}

std::unique_ptr<repl::Interpreter> repl::make_interpreter(){
    return std::make_unique<repl::Interpreter>();
}