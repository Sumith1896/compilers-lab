
#include <iostream>
#include <fstream>
#include <ostream>
#include <istream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>

using namespace std;

#include "common-classes.hh"

#include "user-options.hh"
#include "error-display.hh"

User_Options command_options;

User_Options::User_Options() 
{
	not_only_parse = true;
	show_tokens_selected = false;
	show_ast_selected = false;
	demo_mode_selected = false;

	show_symtab_selected = false;
	show_ic_selected = false;
	do_compile_selected = true;
} 

User_Options::~User_Options()
{
	if (demo_mode_selected == false)
	{
		if (show_tokens_selected)
			tokens_file_buffer.close(); 

		if (show_ast_selected)
			ast_file_buffer.close(); 

		output_file_buffer.close(); 
	}
}

bool User_Options::is_show_tokens_selected() {	return show_tokens_selected; }
bool User_Options::is_show_ast_selected() { return show_ast_selected; }
bool User_Options::is_demo_mode_selected() { return demo_mode_selected; }
bool User_Options::is_show_symtab_selected() { return show_symtab_selected; }
bool User_Options::is_show_ic_selected() { return show_ic_selected; }
bool User_Options::is_do_compile_selected() { return do_compile_selected; }

void User_Options::create_tokens_buffer()
{
	CHECK_INVARIANT(show_tokens_selected, "The show_tokens option should be set");

	if (demo_mode_selected)
		tokens_buffer = &cout;

	else
		tokens_buffer = &tokens_file_buffer; 
}

void User_Options::create_ast_buffer()
{
	CHECK_INVARIANT(show_ast_selected, "The show_ast option should be set");

	if (demo_mode_selected)
		ast_buffer = &cout;

	else
		ast_buffer = &ast_file_buffer; 
}

void User_Options::create_output_buffer()
{ 
	if (demo_mode_selected)
		output_buffer = &cout;
	else
		output_buffer = &output_file_buffer; 
}

void User_Options::create_symtab_buffer()
{
	CHECK_INVARIANT(show_symtab_selected, "The show_symtab options should be set");

	if (demo_mode_selected)
		symtab_buffer = &cout;
	else
		symtab_buffer = &symtab_file_buffer;
}

void User_Options::create_ic_buffer()
{
	CHECK_INVARIANT(show_ic_selected, "The show_ic option should be set");

	if (demo_mode_selected)
		ic_buffer = &cout;
	else
		ic_buffer = &ic_file_buffer;
}

ostream & User_Options::get_tokens_buffer()
{
	if (tokens_buffer == NULL)
		tokens_buffer = &cout;

	*tokens_buffer << std::fixed;
	*tokens_buffer << std::setprecision(2);

	return *tokens_buffer;
}

ostream & User_Options::get_ast_buffer()
{
	if (ast_buffer == NULL)
		ast_buffer = &cout;

	*ast_buffer << std::fixed;
	*ast_buffer << std::setprecision(2);

	return *ast_buffer;
}

ostream & User_Options::get_output_buffer()
{
	if (output_buffer == NULL)
		output_buffer = &cout;

	*output_buffer << std::fixed;
	*output_buffer << std::setprecision(2);

	return *output_buffer;
}

ostream & User_Options::get_symtab_buffer()
{
	if (symtab_buffer == NULL)
		symtab_buffer = &cout;

	*symtab_buffer << std::fixed;
	*symtab_buffer << std::setprecision(2);

	return *symtab_buffer;
}

ostream & User_Options::get_ic_buffer()
{
	if (ic_buffer == NULL)
		ic_buffer = &cout;

	*ic_buffer << std::fixed;
	*ic_buffer << std::setprecision(2);

	return *ic_buffer;
}

string User_Options::get_file_name()
{
	return source_file;
}

void User_Options::remove_files()
{
	remove(tokens_file_name.c_str());
	remove(ast_file_name.c_str());
	remove(output_file_name.c_str());
	
	remove(symtab_file_name.c_str());
	remove(ic_file_name.c_str());
}

string User_Options::process_user_command_options(int argc, char * argv[])
{
	string input_file_name;

	bool user_input_file_entered = false;
	string user_input_file_name;
	char * user_input_file_c_string = NULL;

	const string usage = "\n     Usage: sclp [options] [file]\n\
			Options:\n\
			-help     Show this help\n\
			-parse	  Stop processing with parsing \n\
			-tokens   Show the tokens in file.toks (or out.toks)\n\
			-ast      Show abstract syntax trees in file.ast (or out.ast)\n\
			-symtab   Show the symbol table of delcarations in file.sy, (or out.sym)\n\
			-compile  Compile the program and generate spim code in file.s (or out.s)\n\
			-icode	  Compile the program and show the intermediate code in file.ic (or out.ic)\n\
			-d        Demo version. Use stdout for the outputs instead of files\n\n";

	for (int i = 1; i < argc; i++)
	{    
		char * option = strdup(argv[i]);
		if (!strncmp(option,"-",1))
		{
			if (!strcmp(option,"-d"))
				demo_mode_selected = true;

			else if (!strcmp(option, "-parse"))
				not_only_parse = false;

			else if (!strcmp(option, "-tokens"))
				show_tokens_selected = true;

			else if (!strcmp(option, "-ast")) 
				show_ast_selected = true;
		
			else if (!strcmp(option, "-symtab"))
				show_symtab_selected = true;

			else if (!strcmp(option, "-icode"))
			{
				show_ic_selected = true;
			}

			else if (!strcmp(option, "-compile"))
			{
				do_compile_selected = true;
			}
			
			else if (!(strcmp(option,"--help")) || !(strcmp(option,"-help")))
			{        
				cerr << usage;
				exit(0);
			}
			else 
			{
				string error_message = "Unknown option `" + string(option) + "'" + usage;
				CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, error_message, NO_FILE_LINE);
			}
		}
		else if (user_input_file_entered)
		{
			string error_message = "Only one input file name can be provided " + usage;
			CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, error_message, NO_FILE_LINE);
		}
		else    
		{    
			user_input_file_entered = true;
			user_input_file_c_string = strdup(option);
		}
	}

	if (user_input_file_entered == true)
	{
		CHECK_INVARIANT (user_input_file_c_string, "Input file string cannot be NULL");

		user_input_file_name = string (user_input_file_c_string);
		source_file = user_input_file_name;
		input_file_name = user_input_file_name;
	}
	else
		user_input_file_name = "out";

	CHECK_INVARIANT(user_input_file_name.c_str(), "Input file name cannot be NULL");

	tokens_file_name = user_input_file_name + ".toks"; 
	ast_file_name = user_input_file_name + ".ast"; 
	symtab_file_name = user_input_file_name + ".sym";
	ic_file_name = user_input_file_name + ".ic";

	output_file_name = user_input_file_name + ".s";

	remove (tokens_file_name.c_str()); 
	remove (ast_file_name.c_str()); 
	remove (output_file_name.c_str()); 
	remove (symtab_file_name.c_str());
	remove (ic_file_name.c_str());
	remove ("out.toks");
	remove ("out.ast");
	remove ("out.sym");
	remove ("out.ic");

	if (demo_mode_selected == false)
	{
		if (show_tokens_selected)
			tokens_file_buffer.open(tokens_file_name.c_str()); 

		if (show_ast_selected)
			ast_file_buffer.open(ast_file_name.c_str()); 

		if (show_symtab_selected)
			symtab_file_buffer.open(symtab_file_name.c_str());

		if (show_ic_selected)
			ic_file_buffer.open(ic_file_name.c_str());

		output_file_buffer.open(output_file_name.c_str()); 

		if (show_tokens_selected && !tokens_file_buffer)
		        CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Unable to open output file for tokens");

		if (show_ast_selected && !ast_file_buffer)
		        CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Unable to open output file for AST");

		if (show_symtab_selected && !symtab_file_buffer)
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Unable to open output file for symtab");

		if (show_ic_selected && !ic_file_buffer)
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Unable to open output file for icode");
	}

	return input_file_name;
}
