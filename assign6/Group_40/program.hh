
#ifndef PROGRAM_HH
#define PROGRAM_HH

#include <string>
#include <set>

#define GLOB_SPACE "   "

using namespace std;

class Program;

extern Program program_object;
extern map<string,string> global_str_map;

struct classcomp {
  bool operator() (Procedure* const &a, Procedure* const &b) const;
};
typedef set<Procedure*,classcomp> PROC_SET;

class Program
{
	Symbol_Table global_symbol_table;
	Procedure * main_procedure;
	// set<Procedure* > proc_set;
	PROC_SET proc_set;

public:
	Program();
	~Program();
	void delete_all();

	// void set_procedure(Procedure * proc, int line);
	void set_global_table(Symbol_Table & new_global_table);
	void find_and_set_main_proc();
	void check_for_undefined_functions();


	Symbol_Table_Entry & get_symbol_table_entry(string variable);

	void print_sym();
	void print();

	bool variable_proc_name_check(string symbol);
	bool variable_in_symbol_list_check(string variable);

	// compile
	void compile();
	void print_assembly();
	void print_header(ostream & file_buffer);

	// new
	void global_list_in_proc_check(); 
	void variable_in_proc_map_check(string name);
	bool proc_proc_check(Procedure* proc); // Is there another procedure like proc?
	void add_procedure(Procedure* proc); // Add proc to list/set of procedures
	Procedure* find_proc(string proc_name, vector<pair<Data_Type,string> > & args_vec); //return an existing procedure with these specs, NULL if doesn't exist
	Procedure* find_proc(string proc_name, vector<Ast*> &arguments);
};

#endif
