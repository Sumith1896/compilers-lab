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

void Symbol_Table::set_start_offset_of_first_symbol(int n)
{
	start_offset_of_first_symbol = n;
}

int Symbol_Table::get_start_offset_of_first_symbol()
{
	return start_offset_of_first_symbol;
}

int Symbol_Table::get_size()
{
	return size_in_bytes;
}

void Symbol_Table::assign_offsets()
{
	size_in_bytes = start_offset_of_first_symbol;
	int offset;

	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		int size;
		if (((*i)->get_symbol_scope() == local) || ((*i)->get_symbol_scope() == global))
		{
			size = -(get_size_of_value_type((*i)->get_data_type()));

			(*i)->set_start_offset(size_in_bytes);
			size_in_bytes += size;
			(*i)->set_end_offset(size_in_bytes);
		}

		else if ((*i)->get_symbol_scope() == formal)
		{
			size = get_size_of_value_type((*i)->get_data_type());

			(*i)->set_end_offset(size_in_bytes);
			size_in_bytes += size;
			(*i)->set_start_offset(size_in_bytes);
		}

		else
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");


	}
}

int Symbol_Table::get_size_of_value_type(Data_Type dt)
{
	switch(dt)
	{
	case int_data_type: return 4; break;
	case double_data_type: return 8; break;
	case void_data_type: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Attempt to seek size of type void");
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Data type not supperted");
	}
}

void Symbol_Table::print_assembly(ostream & file_buffer)
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		if (scope == global){
			if((*i)->get_data_type() == 1){
				file_buffer << (*i)->get_variable_name() << ":\t.word 0\n";
			}
			else if((*i)->get_data_type() == 2){
				file_buffer << (*i)->get_variable_name() << ":\t.space 8\n";
			}
		}
	}
}

void Symbol_Table::print(ostream & file_buffer)
{
	list<Symbol_Table_Entry *>::iterator i;

	for(i = variable_table.begin(); i != variable_table.end(); i++)
	{
		string name = (*i)->get_variable_name();
		Data_Type dt = (*i)->get_data_type();
		int start_off = (*i)->get_start_offset();
		int end_off = (*i)->get_end_offset();

		file_buffer << "   Name: " << name;

		switch(dt)
		{
		case int_data_type: file_buffer << " Type: INT"; break;
		defualt: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Variable data type can only be int");
		} 

		file_buffer << " Entity Type: VAR";

		if (start_off == end_off)
			file_buffer << " (No offset assigned yet)\n";
		else
			file_buffer << " Start Offset: " << -start_off << " End Offset: " << -end_off << "\n";
	}
}



/////////////////////////////////////////////////////////////

int Symbol_Table_Entry::get_start_offset()	{ return start_offset; }
int Symbol_Table_Entry::get_end_offset()	{ return end_offset; }
void Symbol_Table_Entry::set_start_offset(int n) { start_offset = n; }
void Symbol_Table_Entry::set_end_offset(int n)	{ end_offset = n; }

void Symbol_Table_Entry::set_register(Register_Descriptor * reg)
{
	register_description = reg;
}

Register_Descriptor * Symbol_Table_Entry::get_register()
{
	return register_description;
}

void Symbol_Table_Entry::free_register(Register_Descriptor * destination_reg_descr)
{
	CHECK_INVARIANT((destination_reg_descr != NULL), 
			"The register descriptor of the destination should not be NULL while freeing a register");

	/* Remove the destination from its register descriptor */
	destination_reg_descr->reset_use_for_expr_result();
	destination_reg_descr->remove_symbol_entry_from_list(*this);

	/* Remove the register descriptor from the destination */
	set_register(NULL);
}

void Symbol_Table_Entry::update_register(Register_Descriptor * result_reg_descr)
{
	CHECK_INVARIANT((result_reg_descr != NULL), 
			"The register descriptor of the result should not be NULL while updating register information");

	Register_Descriptor * destination_reg_descr = get_register();

	if(destination_reg_descr != NULL)
		free_register(destination_reg_descr);

	/* set the register of the destination as the new register */
	set_register(result_reg_descr);

	/* add the destination to the list of sym of the register */
	result_reg_descr->set_use_for_expr_result();
	result_reg_descr->update_symbol_information(*this);
}
