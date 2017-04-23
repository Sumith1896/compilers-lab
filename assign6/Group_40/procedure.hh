
#ifndef PROCEDURE_HH
#define PROCEDURE_HH

#include <string.h>
#include <map>
#include <list>

#define PROC_SPACE "   "
#define LOC_VAR_SPACE "      "

using namespace std;

class Procedure;

class Procedure
{
	Data_Type return_type;
	string name;
	Symbol_Table local_symbol_table;
	Symbol_Table formal_symbol_table;
	Sequence_Ast * sequence_ast;
	vector<pair<Data_Type,string> > args;
	int called;
	bool defined;

	int lineno;

public:
	Procedure(Data_Type proc_return_type, string proc_name, int line);
	~Procedure();

	string get_proc_name();
	int get_lineno();
  	void set_sequence_ast(Sequence_Ast & sa);
	void set_local_list(Symbol_Table & new_list);
	void set_formal_list(Symbol_Table & new_list);
	Data_Type get_return_type();
	Symbol_Table_Entry & get_symbol_table_entry(string variable_name);

	void print(ostream & file_buffer); 
	void print_sym(ostream & file_buffer);

	bool variable_in_symbol_list_check(string variable);

	// compile
	void compile();
	void print_icode(ostream & file_buffer);
	void print_assembly(ostream & file_buffer);
	void set_args(vector<pair<Data_Type,string> > & args);
	vector<pair<Data_Type,string> > get_args();
	void set_called(int l);
	void set_defined();
	int get_called();
	bool get_defined();
private:
	void print_prologue(ostream & file_buffer);
	void print_epilogue(ostream & file_buffer);
};

#endif
