
#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
// #include "local-environment.hh"
#include "symbol-table.hh"
#include "ast.hh"
// #include "sequence-ast.hh"
#include "procedure.hh"
#include "program.hh"
#include "user-options.hh"
#include "icode.hh"
#include "reg-alloc.hh"

Program program_object;
map<string, string> global_str_map;

Program::Program() {
	machine_desc_object.initialize_register_table();
	machine_desc_object.initialize_instruction_table();
	machine_desc_object.clear_local_register_mappings();
	global_str_map = map<string,string>();
}

Program::~Program() {

}

void Program::delete_all() {
	for(auto it: proc_set) {
		delete it;
	}
}

// void Program::set_procedure(Procedure * proc, int line) {
// 	this->main_procedure = proc;
// }

void Program::set_global_table(Symbol_Table & new_global_table) {
	this->global_symbol_table = new_global_table;
	this->global_symbol_table.set_table_scope(global);
	this->global_symbol_table.assign_offsets();
}

Symbol_Table_Entry & Program::get_symbol_table_entry(string variable) {
	return this->global_symbol_table.get_symbol_table_entry(variable);
}

void Program::find_and_set_main_proc() {
	for(auto it: proc_set) {
		if(it->get_proc_name() == "main"){
			main_procedure = it;
			break;
		}
	}
}

void Program::check_for_undefined_functions() {
	for(auto it: proc_set) {
		CHECK_INPUT_AND_ABORT((it->get_defined() or (it->get_called()==-1)),
			"Function was called but not defined",it->get_called())
	}
}

void Program::print_sym() {

}

void Program::print() {
	command_options.create_ast_buffer();
	command_options.get_ast_buffer()<<"Program:"<<endl;
	for(auto pr:proc_set) {
		pr->print(command_options.get_ast_buffer());
	}
}

bool Program::variable_proc_name_check(string symbol) {
	return false;
}

bool Program::variable_in_symbol_list_check(string variable) {
	return global_symbol_table.variable_in_symbol_list_check(variable);
}

void Program::compile() {
	for(auto it: proc_set) {
			it->compile();
	}
	this->print_assembly();
}

void Program::print_assembly() {
	command_options.create_output_buffer();
	if(!global_symbol_table.is_empty() || global_str_map.size() != 0)
		this->print_header(command_options.get_output_buffer());
	for(auto it: proc_set) {
		it->print_assembly(command_options.get_output_buffer());
	}
}

void Program::print_header(ostream & file_buffer) {
	file_buffer << endl;
	file_buffer << "\t.data\n";
	for(auto it: global_symbol_table.get_table()) {
		if(it->get_data_type() == int_data_type) {
			file_buffer << it->get_variable_name() << ":\t.word 0\n";
		}
		if(it->get_data_type() == double_data_type) {
			file_buffer << it->get_variable_name() << ":\t.space 8\n";
		}
	}
	for(auto it: global_str_map) {
		file_buffer << it.second << ":\t .asciiz \t" << it.first << endl;
	}
}

void Program::global_list_in_proc_check() {
	for(auto it: proc_set) {
		CHECK_INPUT(this->global_symbol_table.variable_in_symbol_list_check(it->get_proc_name()) == false,
			"Global variable with same name exists", it->get_lineno());
	}
}

void Program::variable_in_proc_map_check(string name) {
	for(auto it: proc_set) {
		CHECK_INPUT(it->get_proc_name() != name,
			"Procedure with same name exists", it->get_lineno());
	}
}

bool Program::proc_proc_check(Procedure* proc) {
	for(auto it: proc_set) {
		CHECK_INPUT(it->get_proc_name() != proc->get_proc_name(),
			"Procedure with same name exists", it->get_lineno());
	}
}

void Program::add_procedure(Procedure* proc) {
	this->proc_set.insert(proc);
}

Procedure* Program::find_proc(string proc_name, vector<pair<Data_Type,string> > & args_vec) {
	for(auto it: proc_set) {
		if(it->get_proc_name() == proc_name
			and args_vec==it->get_args())
			return it;
	}
	return NULL;
}

Procedure* Program::find_proc(string proc_name, vector<Ast*> &arguments) {
	for(auto it: proc_set) {
		if(it->get_proc_name() == proc_name)
			return it;
	}
	return NULL;
}

bool classcomp::operator()(Procedure* const &a, Procedure* const &b) const {
	// Order:
	// NULL < Proc (Sort lexicographically)

	if(a==NULL and b==NULL)
		return false;
	if(a == NULL and b!=NULL)
		return true;
	if(a != NULL and b==NULL)
		return false;

	return a->get_proc_name() < b->get_proc_name();

}