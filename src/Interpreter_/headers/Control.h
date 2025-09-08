#ifndef CONTROL_H
#define CONTROL_H

#include "Porting.h"

#include <functional>
#include <optional>
#include <string>
#include <variant>

//The project consists of two namespaces: repl and another one with no name
namespace repl{

    enum class ArgType{
        INT,
        STRING,
        FLOAT
    }; //an enum representing any typeof argument name

    using ArgTypes = std::vector<ArgType>;  //vector of argtypes

    using Argument = std::variant<int, std::string, float>; //an object that can hold any type of argument

    //handler 
    using handler = std::function<Argument(std::vector<Argument> const&)>;

    struct CommandFunc{
        std::function<Argument(std::vector<Argument> const&)> f;
        std::optional<ArgTypes> arg_types;
    };

    using Callback = std::function<void(std::string const&)>;
};

#endif