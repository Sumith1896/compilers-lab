#include <iostream>
#include <fstream>
#include <typeinfo>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "procedure.hh"
#include "program.hh"

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register)
{
	stringstream msg;
	msg << "No create_store_stmt() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::print_assembly()
{
	stringstream msg;
	msg << "No print_assembly() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::print_icode()
{
	stringstream msg;
	msg << "No print_icode() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

////////////////////////////////////////////////////////////////

Code_For_Ast & Assignment_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Assignment_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Assignment_Ast");

	Code_For_Ast & load_stmt = rhs->compile();

	Register_Descriptor * load_register = load_stmt.get_reg();
	CHECK_INVARIANT(load_register, "Load register cannot be null in Assignment_Ast");
	load_register->set_use_for_expr_result();

	Code_For_Ast store_stmt = lhs->create_store_stmt(load_register);

	CHECK_INVARIANT((load_register != NULL), "Load register cannot be null in Assignment_Ast");
	load_register->reset_use_for_expr_result();

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (load_stmt.get_icode_list().empty() == false)
		ic_list = load_stmt.get_icode_list();

	if (store_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, load_register);

	return *assign_stmt;
}


/////////////////////////////////////////////////////////////////

Code_For_Ast & Name_Ast::compile()
{
	Mem_Addr_Opd * mem_opd = new Mem_Addr_Opd(*variable_symbol_entry);
	Register_Descriptor * new_reg;
	Icode_Stmt * curr_stmt;

	if(get_data_type() == int_data_type) {
		new_reg =  machine_desc_object.get_new_register<gp_data>();
		Register_Addr_Opd * reg_opd = new Register_Addr_Opd(new_reg);
		curr_stmt = new Move_IC_Stmt(load, mem_opd, reg_opd);
	} else {
		new_reg =  machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd * reg_opd = new Register_Addr_Opd(new_reg);
		curr_stmt = new Move_IC_Stmt(load_d, mem_opd, reg_opd);
	}

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(curr_stmt);
	Code_For_Ast * op_stmt;
	if (ic_list.empty() == false)
		op_stmt = new Code_For_Ast(ic_list, new_reg);

	return *op_stmt;
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register)
{
	Mem_Addr_Opd * mem_opd = new Mem_Addr_Opd(*variable_symbol_entry);
	Register_Addr_Opd * reg_opd = new Register_Addr_Opd(store_register);
	Icode_Stmt * curr_stmt;

	if(get_data_type() == int_data_type) {
		curr_stmt = new Move_IC_Stmt(store, reg_opd, mem_opd);
	} else {
		curr_stmt = new Move_IC_Stmt(store_d, reg_opd, mem_opd);
	}

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(curr_stmt);
	Code_For_Ast * op_stmt;
	if (ic_list.empty() == false)
		op_stmt = new Code_For_Ast(ic_list, store_register);

	return *op_stmt;
}


///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile()
{
	Const_Opd<DATA_TYPE> * con_opd = new Const_Opd<DATA_TYPE>(constant);
	Register_Descriptor * new_reg;
	Icode_Stmt * curr_stmt;

	if(node_data_type == int_data_type) {
		new_reg =  machine_desc_object.get_new_register<gp_data>();
		Register_Addr_Opd * reg_opd = new Register_Addr_Opd(new_reg);
		curr_stmt = new Move_IC_Stmt(imm_load, con_opd, reg_opd);
	} else {
		new_reg =  machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd * reg_opd = new Register_Addr_Opd(new_reg);
		curr_stmt = new Move_IC_Stmt(imm_load_d, con_opd, reg_opd);
	}

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(curr_stmt);
	Code_For_Ast * op_stmt;
	if (ic_list.empty() == false)
		op_stmt = new Code_For_Ast(ic_list, new_reg);

	return *op_stmt;

}

///////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast & CodeForOneOpAst(Ast *lhs, Tgt_Op intop, Tgt_Op dubop, Data_Type type) {
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in one op expr");

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_reg = lhs_stmt.get_reg();
	CHECK_INVARIANT(lhs_reg, "Lhs register cannot be null in one op expr");

	Register_Addr_Opd * lhs_opd = new Register_Addr_Opd(lhs_reg);

	Register_Descriptor * new_reg;
	Icode_Stmt * curr_stmt;
	if(type == int_data_type) {
		new_reg =  machine_desc_object.get_new_register<gp_data>();
		Register_Addr_Opd * new_opd = new Register_Addr_Opd(new_reg);
		curr_stmt = new Compute_IC_Stmt(intop, new_opd, lhs_opd, NULL);
	} else {
		new_reg =  machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd * new_opd = new Register_Addr_Opd(new_reg);
		curr_stmt = new Compute_IC_Stmt(dubop, new_opd, lhs_opd, NULL);
	}

	lhs_reg->reset_use_for_expr_result();

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	ic_list.push_back(curr_stmt);

	Code_For_Ast * op_stmt;
	if (ic_list.empty() == false)
		op_stmt = new Code_For_Ast(ic_list, new_reg);

	return *op_stmt;
}

///////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast & CodeForTwoOpAst(Ast *lhs, Ast *rhs, Tgt_Op intop, Tgt_Op dubop, Data_Type type) {
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in two op expr");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in two op expr");

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_reg = lhs_stmt.get_reg();
	CHECK_INVARIANT(lhs_reg, "Lhs register cannot be null in two op expr");

	Code_For_Ast & rhs_stmt = rhs->compile();
	Register_Descriptor * rhs_reg = rhs_stmt.get_reg();
	CHECK_INVARIANT(rhs_reg, "Rhs register cannot be null in two op expr");

	Register_Addr_Opd * lhs_opd = new Register_Addr_Opd(lhs_reg);
	Register_Addr_Opd * rhs_opd = new Register_Addr_Opd(rhs_reg);

	Register_Descriptor * new_reg;
	Icode_Stmt * curr_stmt;
	if(type == int_data_type) {
		new_reg =  machine_desc_object.get_new_register<gp_data>();
		Register_Addr_Opd * new_opd = new Register_Addr_Opd(new_reg);
		if(intop != not_t) {
			curr_stmt = new Compute_IC_Stmt(intop, new_opd, lhs_opd, rhs_opd);
		} else {
			curr_stmt = new Compute_IC_Stmt(intop, new_opd, rhs_opd, lhs_opd);
		}
	} else {
		new_reg =  machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd * new_opd = new Register_Addr_Opd(new_reg);
		if(dubop != not_t) {
			curr_stmt = new Compute_IC_Stmt(dubop, new_opd, lhs_opd, rhs_opd);
		} else {
			curr_stmt = new Compute_IC_Stmt(intop, new_opd, rhs_opd, lhs_opd);
		}
	}

	lhs_reg->reset_use_for_expr_result();
	rhs_reg->reset_use_for_expr_result();

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	ic_list.push_back(curr_stmt);

	Code_For_Ast * arith_stmt;
	if (ic_list.empty() == false)
		arith_stmt = new Code_For_Ast(ic_list, new_reg);

	return *arith_stmt;
}

///////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Relational_Expr_Ast::compile()
{
	switch(rel_op) {
		case less_equalto:
			return CodeForTwoOpAst(lhs_condition, rhs_condition, sle, sle, get_data_type());
		case less_than:
			return CodeForTwoOpAst(lhs_condition, rhs_condition, slt, slt, get_data_type());
		case greater_than:
			return CodeForTwoOpAst(lhs_condition, rhs_condition, sgt, sgt, get_data_type());
		case greater_equalto:
			return CodeForTwoOpAst(lhs_condition, rhs_condition, sge, sge, get_data_type());
		case equalto:
			return CodeForTwoOpAst(lhs_condition, rhs_condition, seq, seq, get_data_type());
		case not_equalto:
			return CodeForTwoOpAst(lhs_condition, rhs_condition, sne, sne, get_data_type());
	}
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Boolean_Expr_Ast::compile()
{
	switch(bool_op) {
		case boolean_or: 
			return CodeForTwoOpAst(lhs_op, rhs_op, or_t, or_t, get_data_type());
		case boolean_and: 
			return CodeForTwoOpAst(lhs_op, rhs_op, and_t, and_t, get_data_type());
		case boolean_not: 
			Ast * one =  new Number_Ast<int>(1, int_data_type, -1);
			return CodeForTwoOpAst(one, rhs_op, not_t, not_t, get_data_type());
	}
}

///////////////////////////////////////////////////////////////////////

Code_For_Ast & Selection_Statement_Ast::compile()
{
	CHECK_INVARIANT((cond != NULL), "cond cannot be null in conditional_op stmt");
	CHECK_INVARIANT((then_part != NULL), "then_part cannot be null in conditional_op stmt");
	CHECK_INVARIANT((else_part != NULL), "then_part cannot be null in conditional_op stmt");

	Code_For_Ast & cond_stmt = cond->compile();
	Code_For_Ast & then_stmt = then_part->compile();
	Code_For_Ast & else_stmt = else_part->compile();

	string label0 = get_new_label();
	string label1 = get_new_label();

	Register_Descriptor * cond_reg = cond_stmt.get_reg();
	CHECK_INVARIANT(cond_reg, "cond register cannot be null in conditional_op stmt");
	Register_Addr_Opd * cond_opd = new Register_Addr_Opd(cond_reg);
	Register_Addr_Opd * zero_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]);
	Control_Flow_IC_Stmt * control_stmt = new Control_Flow_IC_Stmt(beq, cond_opd, zero_opd, label0);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (cond_stmt.get_icode_list().empty() == false)
		ic_list = cond_stmt.get_icode_list();

	ic_list.push_back(control_stmt);

	if (then_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), then_stmt.get_icode_list());


	Control_Flow_IC_Stmt * control_stmt2 = new Control_Flow_IC_Stmt(j, NULL, NULL, label1);
	ic_list.push_back(control_stmt2);

	Label_IC_Stmt * label0_stmt = new Label_IC_Stmt(label, NULL, label0);
	ic_list.push_back(label0_stmt);

	if (else_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), else_stmt.get_icode_list());	

	cond_reg->reset_use_for_expr_result();

	Label_IC_Stmt * label1_stmt = new Label_IC_Stmt(label, NULL, label1);
	ic_list.push_back(label1_stmt);

	Code_For_Ast * sel_stmt;
	if (ic_list.empty() == false)
		sel_stmt = new Code_For_Ast(ic_list, NULL);

	return *sel_stmt;	
}

///////////////////////////////////////////////////////////////////////

Code_For_Ast & Goto_Label_Ast::compile()
{
	CHECK_INVARIANT((lbl != ""), "label cannot be null in a Label stmt");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	Control_Flow_IC_Stmt * control_stmt = new Control_Flow_IC_Stmt(j, NULL, NULL, lbl);
	ic_list.push_back(control_stmt);

	Code_For_Ast * goto_stmt;
	if (ic_list.empty() == false)
		goto_stmt = new Code_For_Ast(ic_list, NULL);

	return *goto_stmt;	
}

///////////////////////////////////////////////////////////////////////

Code_For_Ast & Label_Ast::compile()
{
	CHECK_INVARIANT((lbl != ""), "label cannot be null in a Goto label stmt");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	Label_IC_Stmt * label_stmt = new Label_IC_Stmt(label, NULL, lbl);
	ic_list.push_back(label_stmt);

	Code_For_Ast * lab_stmt;
	if (ic_list.empty() == false)
		lab_stmt = new Code_For_Ast(ic_list, NULL);

	return *lab_stmt;
}

///////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Break_Ast::compile()
{
	CHECK_INPUT(!break_label.empty(),	"Break stmt not inside a iteration stmt", lineno);
	string iter_label = break_label.back();
	if(iter_label == "") {
		break_label.back() = get_new_label();
	}
	iter_label = break_label.back();

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	Control_Flow_IC_Stmt * control_stmt = new Control_Flow_IC_Stmt(j, NULL, NULL, iter_label);
	ic_list.push_back(control_stmt);

	Code_For_Ast * goto_stmt;
	if (ic_list.empty() == false)
		goto_stmt = new Code_For_Ast(ic_list, NULL);

	return *goto_stmt;	
}

///////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Iteration_Statement_Ast::compile()
{
	CHECK_INVARIANT((cond != NULL), "cond cannot be null in conditional_op stmt");
	CHECK_INVARIANT((body != NULL), "body cannot be null in conditional_op stmt");

	Code_For_Ast & cond_stmt = cond->compile();

	string label0 = get_new_label();
	string label1 = get_new_label();

	break_label.push_back("");

	Code_For_Ast & body_stmt = body->compile();

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if(!is_do_form) { 
		Control_Flow_IC_Stmt * control_stmt = new Control_Flow_IC_Stmt(j, NULL, NULL, label1);
		ic_list.push_back(control_stmt);
	}

	Label_IC_Stmt * label0_stmt = new Label_IC_Stmt(label, NULL, label0);
	ic_list.push_back(label0_stmt);

	if (body_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), body_stmt.get_icode_list());

	Label_IC_Stmt * label1_stmt = new Label_IC_Stmt(label, NULL, label1);
	ic_list.push_back(label1_stmt);

	if (cond_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), cond_stmt.get_icode_list());

	Register_Descriptor * cond_reg = cond_stmt.get_reg();
	CHECK_INVARIANT(cond_reg, "cond register cannot be null in conditional_op stmt");
	Register_Addr_Opd * cond_opd = new Register_Addr_Opd(cond_reg);
	Register_Addr_Opd * zero_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]);
	Control_Flow_IC_Stmt * control_stmt2 = new Control_Flow_IC_Stmt(bne, cond_opd, zero_opd, label0);
	cond_reg->reset_use_for_expr_result();
	ic_list.push_back(control_stmt2);

	string iter_label = break_label.back();
	break_label.pop_back();

	if(iter_label != "") {
		Label_IC_Stmt * iter_label_stmt = new Label_IC_Stmt(label, NULL, iter_label);
		ic_list.push_back(iter_label_stmt);
	}

	Code_For_Ast * iter_stmt;
	if (ic_list.empty() == false)
		iter_stmt = new Code_For_Ast(ic_list, NULL);

	return *iter_stmt;
}

/////////////////////////////////////////////////////////////////

Code_For_Ast & Plus_Ast::compile()
{
	return CodeForTwoOpAst(lhs, rhs, add, add_d, get_data_type());
}

/////////////////////////////////////////////////////////////////

Code_For_Ast & Minus_Ast::compile()
{
	return CodeForTwoOpAst(lhs, rhs, sub, sub_d, get_data_type());
}

//////////////////////////////////////////////////////////////////

Code_For_Ast & Mult_Ast::compile()
{
	return CodeForTwoOpAst(lhs, rhs, mult, mult_d, get_data_type());	
}

////////////////////////////////////////////////////////////////////

Code_For_Ast & Conditional_Operator_Ast::compile()
{
	CHECK_INVARIANT((cond != NULL), "cond cannot be null in selection statement");
	CHECK_INVARIANT((lhs != NULL), "lhs cannot be null in selection statement");
	CHECK_INVARIANT((rhs != NULL), "rhs cannot be null in selection statement");

	Code_For_Ast & cond_stmt = cond->compile();
	Register_Descriptor * cond_reg = cond_stmt.get_reg();
	CHECK_INVARIANT(cond_reg, "cond register cannot be null in selection statement");
	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_reg = lhs_stmt.get_reg();
	CHECK_INVARIANT(lhs_reg, "lhs register cannot be null in selection statement");
	Code_For_Ast & rhs_stmt = rhs->compile();
	Register_Descriptor * rhs_reg = rhs_stmt.get_reg();
	CHECK_INVARIANT(rhs_reg, "rhs rerhgister cannot be null in selection statement");

	string label0 = get_new_label();
	string label1 = get_new_label();

	Register_Addr_Opd * cond_opd = new Register_Addr_Opd(cond_reg);
	Register_Addr_Opd * zero_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]);
	Control_Flow_IC_Stmt * control_stmt = new Control_Flow_IC_Stmt(beq, cond_opd, zero_opd, label0);

	Register_Addr_Opd * lhs_opd = new Register_Addr_Opd(lhs_reg);
	Register_Addr_Opd * rhs_opd = new Register_Addr_Opd(rhs_reg);
	Register_Descriptor * res_reg;
	if(get_data_type() == int_data_type) {
		res_reg =  machine_desc_object.get_new_register<gp_data>();
	} else {
		res_reg =  machine_desc_object.get_new_register<float_reg>();
	}
	Register_Addr_Opd * res_opd = new Register_Addr_Opd(res_reg);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (cond_stmt.get_icode_list().empty() == false)
		ic_list = cond_stmt.get_icode_list();

	ic_list.push_back(control_stmt);

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), lhs_stmt.get_icode_list());

	Compute_IC_Stmt * store_reg_lhs = new Compute_IC_Stmt(or_t, res_opd, lhs_opd, zero_opd);
	Compute_IC_Stmt * store_reg_rhs = new Compute_IC_Stmt(or_t, res_opd, rhs_opd, zero_opd);
	cond_reg->reset_use_for_expr_result();
	lhs_reg->reset_use_for_expr_result();
	rhs_reg->reset_use_for_expr_result();

	ic_list.push_back(store_reg_lhs);

	Control_Flow_IC_Stmt * control_stmt2 = new Control_Flow_IC_Stmt(j, NULL, NULL, label1);
	ic_list.push_back(control_stmt2);
	
	Label_IC_Stmt * label0_stmt = new Label_IC_Stmt(label, NULL, label0);
	ic_list.push_back(label0_stmt);

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	ic_list.push_back(store_reg_rhs);

	Label_IC_Stmt * label1_stmt = new Label_IC_Stmt(label, NULL, label1);
	ic_list.push_back(label1_stmt);

	Code_For_Ast * cond_op_stmt;
	if (ic_list.empty() == false)
		cond_op_stmt = new Code_For_Ast(ic_list, res_reg);

	return *cond_op_stmt;
}

////////////////////////////////////////////////////////////////////

Code_For_Ast & Divide_Ast::compile()
{
	return CodeForTwoOpAst(lhs, rhs, divd, div_d, get_data_type());	
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & UMinus_Ast::compile()
{
	return CodeForOneOpAst(lhs, uminus, uminus_d, get_data_type());	
}

//////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Sequence_Ast::compile()
{
	sa_icode_list = list<Icode_Stmt *>();

	for(list<Ast *>::iterator it = statement_list.begin(); 
			it != statement_list.end(); it++) {
		Code_For_Ast & temp_stmt = (*it)->compile();
		sa_icode_list.splice(sa_icode_list.end(), temp_stmt.get_icode_list());
	}

	Code_For_Ast * seq_stmt;
	seq_stmt = new Code_For_Ast(sa_icode_list, NULL);

	return *seq_stmt;

}

void Sequence_Ast::print_assembly(ostream & file_buffer)
{
	for(list<Icode_Stmt *>::iterator it = sa_icode_list.begin(); 
			it != sa_icode_list.end(); it++) {
		(*it)->print_assembly(file_buffer);
	}
	
}

void Sequence_Ast::print_icode(ostream & file_buffer)
{
	for(list<Icode_Stmt *>::iterator it = sa_icode_list.begin(); 
			it != sa_icode_list.end(); it++) {
		(*it)->print_icode(file_buffer);
	}
}

//////////////////////////////////////////////////////////////////////////////

template class Number_Ast<double>;
template class Number_Ast<int>;
