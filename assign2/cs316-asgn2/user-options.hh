
/*********************************************************************************************

                                cfglp : A CFG Language Processor
                                --------------------------------

           About:

           Implemented by Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs306: Language  Processors (theory and lab)  at IIT
           Bombay.

           Release  date Jan  15, 2013.  Copyrights reserved  by Uday
           Khedker. This implemenation has been made available purely
           for academic purposes without any warranty of any kind.

           A  doxygen   generated  documentation  can  be   found  at
           http://www.cse.iitb.ac.in/~uday/cfglp


***********************************************************************************************/

#ifndef USER_OPTIONS_HH
#define USER_OPTIONS_HH

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <ostream>

using namespace std;

class User_Options;

class User_Options
{
	string source_file;
	string tokens_file_name;
	string ast_file_name;
	string output_file_name;
	string symtab_file_name;
	string ic_file_name;

	bool show_tokens_selected;
	bool show_ast_selected;
	bool do_eval_selected;
	bool demo_mode_selected;
	bool show_symtab_selected;
	bool show_ic_selected;
	bool do_lra_selected;
	bool do_compile_selected;

	ostream * tokens_buffer;
	ostream * ast_buffer;
	ostream * output_buffer;
	ostream * symtab_buffer;
	ostream * ic_buffer;

	ofstream tokens_file_buffer;
	ofstream ast_file_buffer;
	ofstream output_file_buffer;
	ofstream symtab_file_buffer;
	ofstream ic_file_buffer;

	FILE * file_buffer;

public:
	User_Options(); 
	~User_Options();

	bool not_only_parse;

	bool is_show_tokens_selected();
	bool is_show_ast_selected();
	bool is_do_eval_selected();
	bool is_demo_mode_selected();
	bool is_show_symtab_selected();
	bool is_show_ic_selected();
	bool is_do_lra_selected();
	bool is_do_compile_selected();

	string process_user_command_options(int argc, char * argv[]);
	string get_file_name();

	void create_tokens_buffer();
	void create_ast_buffer();
	void create_output_buffer();
	void create_symtab_buffer();
	void create_ic_buffer();

	ostream & get_tokens_buffer();
	ostream & get_ast_buffer();
	ostream & get_output_buffer();
	ostream & get_symtab_buffer();
	ostream & get_ic_buffer();

	void remove_files();
};

extern User_Options command_options;

#endif
