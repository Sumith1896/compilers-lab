
#include <string>
#include <iostream>

using namespace std;

#include "common-classes.hh"
#include "user-options.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "procedure.hh"
#include "program.hh"

Symbol_Table::Symbol_Table()
{
	start_offset_of_first_symbol = 0;
	size_in_bytes = 0;
}

Symbol_Table::~Symbol_Table()
{}

void Symbol_Table::operator==(Symbol_Table & formal_list)
{
	list<Symbol_Table_Entry *> & f_list = formal_list.get_table();

	list<Symbol_Table_Entry *>::iterator i;
	list<Symbol_Table_Entry *>::iterator j;

	for (i = variable_table.begin(), j = f_list.begin(); i != variable_table.end() && j != f_list.end(); i++, j++)
	{
		Data_Type type1 = (*i)->get_data_type();
		Data_Type type2 = (*j)->get_data_type();

		CHECK_INPUT((type1 == type2), "Return type of one of the parameters of the procedure and its prototype doesn't match", NO_FILE_LINE);

		string name1 = (*i)->get_variable_name();
		string name2 = (*j)->get_variable_name();

		CHECK_INPUT((name1 == name2), "Variable name of one of the parameters of the procedre and its prototypes doesn't match", NO_FILE_LINE);
	}

	CHECK_INPUT((i == variable_table.end() && j == f_list.end()),
			"Procedure and its prototype parameter f_list length doens't match", NO_FILE_LINE);
}

void Symbol_Table::set_table_scope(Table_Scope list_scope)
{
	scope = list_scope;

	list<Symbol_Table_Entry *>::iterator i;
	for(i = variable_table.begin(); i != variable_table.end(); i++)
		(*i)->set_symbol_scope(list_scope);
}

Table_Scope Symbol_Table::get_table_scope()
{
	return scope;
}

bool Symbol_Table::is_empty()
{
	return variable_table.empty();
}

void Symbol_Table::push_symbol(Symbol_Table_Entry * variable)
{
	variable_table.push_back(variable);
}

list<Symbol_Table_Entry *> & Symbol_Table::get_table()
{
	return variable_table;
}

void Symbol_Table::global_list_in_proc_map_check()
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		string name = (*i)->get_variable_name();
		program_object.variable_in_proc_map_check(name);
	}
}

bool Symbol_Table::variable_in_symbol_list_check(string variable)
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		if ((*i)->get_variable_name() == variable)
			return true;
	}

	return false;
}

bool Symbol_Table::variable_in_formal_list_check(string variable)
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		if ((*i)->get_variable_name() == variable)
			return true;
	}

	return false;
}

Symbol_Table_Entry & Symbol_Table::get_symbol_table_entry(string variable_name)
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		if ((*i)->get_variable_name() == variable_name)
			return **i;
	}

	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "The variable symbol entry doesn't exist");
}

string Symbol_Table::get_variable_in_formal_list(int position)
{
	int count = 1;

	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++, count++)
	{
		if (count == position)
			return (*i)->get_variable_name();
	}

	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Actual and formal parameter should match");
}

/////////////////////////////////////////////////////////////

Symbol_Table_Entry::Symbol_Table_Entry()
{
	variable_data_type = void_data_type;
	start_offset = end_offset = 0;
	register_description = NULL;
	ref_off = fp_ref;
}

Symbol_Table_Entry::Symbol_Table_Entry(string & name, Data_Type new_data_type, int line, Offset_With_Ref_To ro)
{
	variable_name = name;
	variable_data_type = new_data_type;

	start_offset = end_offset = 0;
	register_description = NULL;
	ref_off = ro;

	lineno = line;
}

Symbol_Table_Entry::~Symbol_Table_Entry()
{}

Offset_With_Ref_To Symbol_Table_Entry::get_ref_offset()
{
	return ref_off;
}

bool Symbol_Table_Entry::operator==(Symbol_Table_Entry & entry)
{
	if (variable_name != entry.get_variable_name())
		return false;
	else if (variable_data_type != entry.get_data_type())
		return false;
	else if (scope != entry.get_symbol_scope())
		return false;

	return true;
}

void Symbol_Table_Entry::set_symbol_scope(Table_Scope sp)
{
	scope = sp;
}

Table_Scope Symbol_Table_Entry::get_symbol_scope()
{
	return scope;
}

int Symbol_Table_Entry::get_lineno()
{
	return lineno;
}

Data_Type Symbol_Table_Entry::get_data_type()
{
	return variable_data_type;
}

void Symbol_Table_Entry::set_data_type(Data_Type dt)
{
	variable_data_type = dt;
}

string Symbol_Table_Entry::get_variable_name()
{
	return variable_name;
}
