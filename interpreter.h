#pragma once

#include <stdio.h>
#include <vector>
#include <string>
#include <variant>

enum interpreter_result_type {
	interpreter_result_type_none ,
	interpreter_result_type_integer ,
	interpreter_result_type_string ,
};

struct InterpreterResult {
	enum interpreter_result_type type = interpreter_result_type_none;
	std::string string;
	int integer;
};



InterpreterResult
interpret_line_stdin(void) {
	InterpreterResult result;

	return result;
}
