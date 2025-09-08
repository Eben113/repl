cmake_minimum_required(VERSION 3.21)

# Must go before the project() command:
if(NOT DEFINED PROJECT_NAME)
	set(IS_ROOT_PROJECT "YES")
else()
	set(IS_ROOT_PROJECT "NO")
endif()

project(CommandInterpreter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED "On")
set(CMAKE_CXX_EXTENSIONS "Off")



add_subdirectory(src/repl)
add_subdirectory(src/Interpreter_)


if (WIN32)
    # I stayed away from exceptions in this implementation, however,
	# the MSVC standard assumes that all extern functions will throw unless
	# the following compiler flag is defined. If not added, the extern definitions
	# coming from <iostream> will be assumed to throw, even when the standard 
	# guarantees they dont. I.e. leave the definition below for the use of
	# std::cout and it's << operator, as it doesn't throw anyway.
    target_compile_options(repl_interpreter PRIVATE /EHsc)
    #target_compile_options(SanityTests PRIVATE /EHsc)
endif()