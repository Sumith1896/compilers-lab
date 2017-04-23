#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
// #include "local-environment.hh"
#include "symbol-table.hh"
#include "ast.hh"
// #include "sequence-ast.hh"
#include "procedure.hh"

#include <string.h>

Procedure::Procedure(Data_Type proc_return_type, string proc_name, int line) {
	this->return_type = proc_return_type;
	this->name = proc_name;
	this->lineno = line;
	this->called = -1;
	this->defined = false;
}

Procedure::~Procedure() {
	if (defined)
		delete this->sequence_ast;
}

string Procedure::get_proc_name() {
	return this->name;
}

int Procedure::get_lineno() {
	return this->lineno;
}

void Procedure::set_sequence_ast(Sequence_Ast & sa) {
	this->sequence_ast = &sa;
}

void Procedure::set_local_list(Symbol_Table & new_list) {
	this->local_symbol_table = new_list;
	this->local_symbol_table.set_table_scope(local);
	this->local_symbol_table.assign_offsets();
}

void Procedure::set_formal_list(Symbol_Table & new_list) {
	this->formal_symbol_table = new_list;
	this->formal_symbol_table.set_table_scope(formal);
	this->formal_symbol_table.set_start_offset_of_first_symbol(8);
	this->formal_symbol_table.assign_offsets();
}

void Procedure::set_args(vector<pair<Data_Type,string> > & args) {
	this->args = args;
	Symbol_Table s;
	for(auto dt_s: args) {
		CHECK_INPUT ((dt_s.second != this->get_proc_name()),
				"Argument name cannot be same as the procedure name", this->lineno);

		CHECK_INPUT((s.variable_in_symbol_list_check(dt_s.second) == false),
				"Argument with same name already exists", this->lineno);

		Symbol_Table_Entry * entry = new Symbol_Table_Entry(dt_s.second, dt_s.first, this->lineno);
		s.push_symbol(entry);
	}
	this->set_formal_list(s);
}

Data_Type Procedure::get_return_type() {
	return return_type;
}

Symbol_Table_Entry & Procedure::get_symbol_table_entry(string variable_name) {
	if(this->local_symbol_table.variable_in_symbol_list_check(variable_name))
		return this->local_symbol_table.get_symbol_table_entry(variable_name);
	else if(this->formal_symbol_table.variable_in_formal_list_check(variable_name))
		return this->formal_symbol_table.get_symbol_table_entry(variable_name);
	else
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
			"variable_name not present in symbol table");
}

void Procedure::print(ostream & file_buffer) {
	if (!defined)
		return;
	file_buffer << PROC_SPACE <<"Procedure: " << this->name 
	            << ", Return Type: ";

	switch(this->return_type) {
		case void_data_type:	file_buffer<<"VOID";break;
		case int_data_type:		file_buffer<<"INT";break;
		case double_data_type:	file_buffer<<"FLOAT";break;
		case string_data_type:	file_buffer<<"STRING";break;
		default:
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Invalid Data type");
	}
	file_buffer<<endl;
	sequence_ast->print(file_buffer);
}

void Procedure::print_sym(ostream & file_buffer) {

}

bool Procedure::variable_in_symbol_list_check(string variable) {
	return this->local_symbol_table.variable_in_symbol_list_check(variable)
		or this->formal_symbol_table.variable_in_formal_list_check(variable);
}

void Procedure::compile() {
	if (!defined)
		return;
	this->sequence_ast->compile();
}

void Procedure::print_icode(ostream & file_buffer) {
	if (!defined)
		return;

}

void Procedure::set_called(int l) {
	this->called = l;
}

void Procedure::set_defined() {
	this->defined = true;
}

int Procedure::get_called() {
	return this->called;
}

bool Procedure::get_defined() {
	return this->defined;
}

void Procedure::print_assembly(ostream & file_buffer) {
	if (!defined)
		return;
	file_buffer << endl;
	file_buffer << "\t.text \t\t\t# The .text assembler directive indicates" << endl;
	file_buffer << "\t.globl " << this->name << "\t\t# The following is the code (as oppose to data)" << endl;
	file_buffer << this->name << ":\t\t\t\t# .globl makes main know to the " << endl;
	file_buffer << "\t\t\t\t# outside of the program." << endl;
	print_prologue(file_buffer);
	file_buffer << endl;
	sequence_ast->print_assembly(file_buffer);
	file_buffer << endl;
	print_epilogue(file_buffer);
	file_buffer << endl;
}

void Procedure::print_prologue(ostream & file_buffer) {
	if (!defined)
		return;
	file_buffer << "# Prologue begins " << endl;
	file_buffer << "\tsw $ra, -4($sp)\t\t# Save the return address" << endl;
	file_buffer << "\tsw $fp, -8($sp)\t\t# Save the frame pointer" << endl;
	file_buffer << "\tsub $fp, $sp, 8\t\t# Update the frame pointer" << endl;
	file_buffer << endl;
	file_buffer << "\tsub $sp, $sp, " << (-1*this->local_symbol_table.get_size()+8) << "\t\t# Make space for the locals" << endl;
	file_buffer << "# Prologue ends" << endl;
}

void Procedure::print_epilogue(ostream & file_buffer) {
	if (!defined)
		return;
	file_buffer << "# Epilogue Begins" << endl;
	file_buffer << "epilogue_" << this->name << ":" << endl;
	file_buffer << "\tadd $sp, $sp, " << (-1*this->local_symbol_table.get_size()+8) << endl;
	file_buffer << "\tlw $fp, -8($sp)  \n\tlw $ra, -4($sp)   \n\tjr        $31\t\t# Jump back to the called procedure" << endl;
	file_buffer << "# Epilogue Ends" << endl;
}	

vector<pair<Data_Type,string> > Procedure::get_args() {
	return this->args;
}
