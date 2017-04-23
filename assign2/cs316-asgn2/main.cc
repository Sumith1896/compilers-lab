#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>

using namespace std;

#include"common-classes.hh"
#include"local-environment.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"sequence-ast.hh"
#include"procedure.hh"
#include"program.hh"
#include"parser.h"

#include"dirent.h"

int main(int argc, char * argv[]) 
{
	string input_file_name = command_options.process_user_command_options(argc, argv);

	Parser cfglp_parser(input_file_name);

	CHECK_INPUT((!cfglp_parser.parse()), "Cannot parse the input program", NO_FILE_LINE);

	if (command_options.not_only_parse)
	{
		if ((error_status() == false) && (command_options.is_show_ast_selected()))
			program_object.print();

		if ((error_status() == false) && (command_options.is_show_symtab_selected()))
		{
		#ifdef COMPILE
			program_object.print_sym();
		#else
			CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, 
			"CFGLP is currently in interpretation mode. Select another option or compile CFGLP in compilation mode", -1);
		#endif
		}

		// if ((error_status() == false) && (command_options.is_do_eval_selected()))
		// 	program_object.evaluate();

		if ((error_status() == false) && (command_options.is_do_eval_selected() == false))
		{
		#ifdef COMPILE
			program_object.compile();

			if (command_options.is_show_symtab_selected())
				program_object.print_sym();
		#else
			CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, 
			"CFGLP is currently in interpretation mode. Select another option or compile CFGLP in compilation mode", -1);
		#endif
		}

		program_object.delete_all();
	}

	return 0;
}
