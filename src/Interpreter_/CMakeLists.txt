add_library(repl_interpreter Interpreter.cpp)
target_include_directories(repl_interpreter PUBLIC include)
target_include_directories(repl_interpreter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)


include(GenerateExportHeader)
set(INTERPRETER_EXPORT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/headers")
generate_export_header(repl_interpreter EXPORT_FILE_NAME "${INTERPRETER_EXPORT_DIRECTORY}/porting.h")