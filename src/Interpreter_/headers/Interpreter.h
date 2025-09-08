#ifndef INTERPRETER_HEADER
#define INTERPRETER_HEADER

#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <string>

#include "porting.h"
#include "CommandTypesHelper.h"

namespace repl
{
    struct Command{
        CommandFunc command;
        std::vector<Argument> arguments;
    };


    REPL_INTERPRETER_EXPORT class Interpreter{
        public:
            bool queue(std::string const& command);
            bool registerCommand(std::string const& command_name, CommandFunc const& handler);
            void poll();
            void setCallback(Callback const& callback);
        private:
            std::map<std::string, CommandFunc> _commands;
            std::queue<Command> _command_queue;
            std::optional<Callback> _callback;
    };
    REPL_INTERPRETER_EXPORT std::unique_ptr<Interpreter> make_interpreter();
}

#endif 