#ifndef COMMAND_TYPES_H
#define COMMAND_TYPES_H


#include "Control.h"

#include <charconv>
#include <optional>
#include <string_view>

namespace repl{

    template <ArgType T>
    struct TypeConverter{
        static std::optional<Argument> Convert(std::string_view const& argumrnt) = delete;
    };

    template<>
    struct TypeConverter <ArgType::STRING> {
        static std::optional<Argument> Convert(std::string_view const& argument){
            return std::string{begin(argument), end(argument)};
        };
    };


    template<>
    struct TypeConverter <ArgType::INT> {
        static std::optional<Argument> Convert(std::string_view const& argument){
            int result;
            auto const err = std::from_chars(argument.data(), argument.data() + argument.size(), result);

            if(err.ptr != argument.data() + argument.size()){
                return std::nullopt;
            }

            if((err.ec == std::errc::invalid_argument) || (err.ec == std::errc::result_out_of_range)){
                return std::nullopt;
            }
            return result;
        };
    };

    template<>
    struct TypeConverter <ArgType::FLOAT> {
        static std::optional<Argument> Convert(std::string_view const& argument){
            char* endptr = nullptr;
            std::string null_terminated_string = std::string{begin(argument), end(argument)};
            auto const result = std::strtof(null_terminated_string.c_str(), &endptr);

            if(*endptr != '\0'){
                return std::nullopt;
            }
            return result;
        }
    };

    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
    template  <typename T>
    std::optional<T> convert_argument(Argument const& argument){
        return std::visit(overloaded{
            [](T const& arg) -> std::optional<T>{return arg;},
            [](auto const&)  -> std::optional<T> { return std::nullopt; }
        }, argument);
    };
}
#endif