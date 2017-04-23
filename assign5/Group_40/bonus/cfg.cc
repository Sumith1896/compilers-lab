#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <typeinfo>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "cfg.hh"
#include "icode.hh"

void check_insert_set(ELEMENT_SET&set, Ics_Opd* el) {
	if (el != NULL and
		(typeid(*el) == typeid(Mem_Addr_Opd) or
		typeid(*el) == typeid(Register_Addr_Opd)))
			set.insert(el);
}

void check_delete_set(ELEMENT_SET&set, Ics_Opd* el) {
	if (el != NULL and
		(typeid(*el) == typeid(Mem_Addr_Opd) or
		typeid(*el) == typeid(Register_Addr_Opd)))
			set.erase(el);
}

void print_set(ELEMENT_SET s, ostream & file_buffer) {
	for(auto opd: s) {
		if(typeid(*opd) == typeid(Mem_Addr_Opd))
			file_buffer << opd->get_symbol_table_entry()->get_variable_name() << ",";
		else
			file_buffer << opd->get_reg()->get_name() << ",";
	}
	file_buffer << endl;
}

bool classcomp::operator()(Ics_Opd* const &a, Ics_Opd* const &b) const {
	// Order:
	// NULL < Mem_Addr_Opd(Global) < Mem_Addr_Opd(Local) < Register_Addr_Opd
	// Mem_Addr_Opd      : sorted on get_symbol_table_entry()->get_variable_name()
	// Register_Addr_Opd : sorted on get_reg()->get_name()

	if(a==NULL and b==NULL)
		return false;
	if(a == NULL and b!=NULL)
		return true;
	if(a != NULL and b==NULL)
		return false;

	if(typeid(*a) == typeid(Mem_Addr_Opd) and
		typeid(*b) == typeid(Register_Addr_Opd))
		return true;

	if( typeid(*a) == typeid(Register_Addr_Opd) and
		typeid(*b) == typeid(Mem_Addr_Opd))
		return false;

	if(typeid(*a) == typeid(Mem_Addr_Opd) and
		typeid(*b) == typeid(Mem_Addr_Opd))
	{
		if(a->get_symbol_table_entry()->get_symbol_scope() == 
			b->get_symbol_table_entry()->get_symbol_scope())
		{
			return (a->get_symbol_table_entry()->get_variable_name() < 
					b->get_symbol_table_entry()->get_variable_name());
		}
		else if(a->get_symbol_table_entry()->get_symbol_scope() == global)
			return true;
		else 
			return false;
	}

	if(typeid(*a) == typeid(Register_Addr_Opd) and
		typeid(*b) == typeid(Register_Addr_Opd))
		return a->get_reg()->get_name() < b->get_reg()->get_name();

	return false;
}

Basic_Block::Basic_Block() {
	block_number = total_blocks++;
	l = r = NULL;
}

Basic_Block::~Basic_Block() {
}

int Basic_Block::total_blocks = 0;

int Basic_Block::get_block_number() {
	return block_number;
}

void Basic_Block::push_stmt(Icode_Stmt *curr_stmt) {
	bb_icode_list.push_back(curr_stmt);
}

void Basic_Block::set_children(Basic_Block *left, Basic_Block *right) {
	l = left;
	r = right;
}

void Basic_Block::print(ostream & file_buffer) {
	file_buffer << "=======================" << endl;
	file_buffer << "Block #  : " << block_number << endl;
	file_buffer << "Children : " << ((l==NULL)?"NULL": to_string(l->block_number)) << ","
	                             << ((r==NULL)?"NULL": to_string(r->block_number)) << endl;
	file_buffer << "GEN      : "; print_set(gen,  file_buffer);
	file_buffer << "KILL     : "; print_set(killl,file_buffer);
	file_buffer << "IN       : "; print_set(in,   file_buffer);
	file_buffer << "OUT      : "; print_set(out,  file_buffer);
	file_buffer << "icode_stmt_list:" << endl;
	for(auto stmt : bb_icode_list)
		stmt->print_icode(file_buffer);
	file_buffer << "=======================" << endl;
}

void Basic_Block::insert_gen(Ics_Opd * opd) {
	if (opd != NULL and
		(typeid(*opd) == typeid(Mem_Addr_Opd) or
		typeid(*opd) == typeid(Register_Addr_Opd)))
		if(killl.find(opd)==killl.end())
			gen.insert(opd);
}

void Basic_Block::insert_kill(Ics_Opd * opd) {
	if (opd != NULL and
		(typeid(*opd) == typeid(Mem_Addr_Opd) or
		typeid(*opd) == typeid(Register_Addr_Opd)))
		killl.insert(opd);
}

void Basic_Block::recompute_gen_kill() {
	gen.clear();
	killl.clear();
	for(auto curr_stmt : bb_icode_list) {
		Tgt_Op op = curr_stmt->get_op().get_op();

		switch(op) {
			case label:
			case j:
				break;

			case beq:
			case bne:
			case bgtz:
			case bgez:
			case bltz:
			case blez:
				insert_gen(curr_stmt->get_opd1());
				insert_gen(curr_stmt->get_opd2());
				break;

			default:
				insert_gen(curr_stmt->get_opd1());
				insert_gen(curr_stmt->get_opd2());
				insert_kill(curr_stmt->get_result());
		}
	}
}

bool Basic_Block::eliminate_dead_code() {
	ELEMENT_SET live = out;
	Ics_Opd	*opd1 = NULL, *opd2=NULL, *res=NULL;
	bool flag = false, mark_delete = false;

	#ifdef DEBUG
	cout << "Eliminating dead code in block:" << endl;
	print(cout);
	#endif


	for(auto it = bb_icode_list.rbegin(); it != bb_icode_list.rend();) {
		auto curr_stmt = *it;
		Tgt_Op op = curr_stmt->get_op().get_op();

		#ifdef DEBUG
		cout<<"Live Set: "; print_set(live,cout);
		cout<<"stmt:\t"; curr_stmt->print_icode(cout);
		#endif

		switch(op) {
			case label:
			case j:
				break;

			case beq:
			case bne:
			case bgtz:
			case bgez:
			case bltz:
			case blez:
				opd1 = curr_stmt->get_opd1();
				opd2 = curr_stmt->get_opd2();
				check_insert_set(live,opd1);
				check_insert_set(live,opd2);
				break;

			default:
				res  = curr_stmt->get_result();
				opd1 = curr_stmt->get_opd1();
				opd2 = curr_stmt->get_opd2();
				if (live.find(res) == live.end()) {

					#ifdef DEBUG
					// Printing deletion
					cout <<"Marked for elimination!" << endl;
					#endif

					// mark curr_stmt for deletion from icode_list
					mark_delete = true;

					flag = true;
				}
				else {
					// Update live set
					check_delete_set(live,res);
					check_insert_set(live,opd1);
					check_insert_set(live,opd2);
				}
		}

		if(mark_delete) {
			auto del_it = it.base();
			del_it--;
			curr_stmt = *del_it;

			#ifdef DEBUG
			cout <<" Deleting statement." << endl;
			#endif

			bb_icode_list.erase(del_it);
			delete curr_stmt;
			mark_delete = false;
		}
		else{
			it++;
		}
	}

	return flag;
}

CFG::CFG(Basic_Block *r) {
	root = r;
}

CFG::~CFG() {
	for(auto block:all_blocks)
		delete block;
	l_blocks.clear();
	all_blocks.clear();
	Basic_Block::total_blocks = 0;
}

Basic_Block* CFG::get_block(string index) {
	// Returns Basic_Block* to block starting with label 'index'
	// creates such a block if it doesn't exist
	if(l_blocks.find(index) != l_blocks.end()) {
		return l_blocks[index];
	} else {
		return l_blocks[index] = new Basic_Block();
	}
}

Basic_Block* CFG::get_final_block() {
	return all_blocks.back();
}

void CFG::save_block(Basic_Block* b) {
	// Add block to all_blocks vector
	all_blocks.push_back(b);
}

void CFG::print(ostream & file_buffer) {
	file_buffer << "Root:  " << root->get_block_number() << endl;
	file_buffer << "Total: " << Basic_Block::total_blocks << endl;
	for(auto block : all_blocks)
		block->print(file_buffer);
}

void CFG::compute_in_out() {
	bool changed = true;
	ELEMENT_SET ti, to;

	for(auto block:all_blocks) {
		block->in.clear();
		block->out.clear();
	}

	reset_global_set();

	for(auto block:all_blocks)
		for(auto elem:block->gen)
			block->in.insert(elem);

	int iter = 0;
	while(changed) {
		changed = false;
		for(auto block:all_blocks) {
			ti = block->in;
			to = block->out;

			if(block->l != NULL)
				for(auto elem:block->l->in)
					to.insert(elem);
			if(block->r != NULL)
				for(auto elem:block->r->in)
					to.insert(elem);
			if(block->out.size() < to.size()) {
				block->out = to;
				changed = true;
			}

			for(auto elem:block->killl)
				to.erase(elem);
			ti = block->in;
			for(auto elem:to)
				ti.insert(elem);
			if(block->in.size() < ti.size()) {
				block->in = ti;
				changed = true;
			}
		}
	}
}

bool CFG::eliminate_dead_code() {
	#ifdef DEBUG
	cout << "Starting Dead Code Elimination" << endl;
	#endif

	bool flag = false;
	for(auto block : all_blocks)
		if(block->eliminate_dead_code()) {
			block->recompute_gen_kill();
			flag = true;
		}

	#ifdef DEBUG
	cout << "Finished Dead Code Elimination" << endl;
	#endif

	return flag;
}

list<Icode_Stmt *> CFG::get_icode_list() {
	list<Icode_Stmt *> code;
	for(auto block:all_blocks)
		code.splice(code.end(),block->bb_icode_list);
	return code;
}

void CFG::set_global_set(ELEMENT_SET &s) {
	global_elem = s;
	all_blocks.back()->out = global_elem;
}

void CFG::reset_global_set() {
	all_blocks.back()->out = global_elem;
}

ELEMENT_SET CFG::get_global_set() {
	return global_elem;
}
