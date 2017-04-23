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
	if(lhs_condition->get_data_type() == int_data_type) {
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
	} else {
		CHECK_INVARIANT((lhs_condition != NULL), "Lhs cannot be null in two op expr");
		CHECK_INVARIANT((rhs_condition != NULL), "Rhs cannot be null in two op expr");

		Code_For_Ast & lhs_stmt = lhs_condition->compile();
		Register_Descriptor * lhs_reg = lhs_stmt.get_reg();
		CHECK_INVARIANT(lhs_reg, "Lhs register cannot be null in two op expr");

		Code_For_Ast & rhs_stmt = rhs_condition->compile();
		Register_Descriptor * rhs_reg = rhs_stmt.get_reg();
		CHECK_INVARIANT(rhs_reg, "Rhs register cannot be null in two op expr");

		Register_Addr_Opd * lhs_opd = new Register_Addr_Opd(lhs_reg);
		Register_Addr_Opd * rhs_opd = new Register_Addr_Opd(rhs_reg);

		list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

		if (lhs_stmt.get_icode_list().empty() == false)
			ic_list = lhs_stmt.get_icode_list();

		if (rhs_stmt.get_icode_list().empty() == false)
			ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

		Register_Descriptor * new_reg =  machine_desc_object.get_new_register<gp_data>();
		Register_Addr_Opd * new_opd = new Register_Addr_Opd(new_reg);

		Const_Opd<int> * p0_opd = new Const_Opd<int>(0);
		Icode_Stmt * curr_stmt = new Move_IC_Stmt(imm_load, p0_opd, new_opd);
		ic_list.push_back(curr_stmt);
		switch(rel_op) {
			case less_than:
			case greater_equalto:
				curr_stmt = new Compute_IC_Stmt(slt_d, new_opd, rhs_opd, lhs_opd);
			case less_equalto:
			case greater_than:
				curr_stmt = new Compute_IC_Stmt(sle_d, new_opd, rhs_opd, lhs_opd);
			case equalto:
			case not_equalto:
				curr_stmt = new Compute_IC_Stmt(seq_d, new_opd, rhs_opd, lhs_opd);
		}
		ic_list.push_back(curr_stmt);

		lhs_reg->reset_use_for_expr_result();
		rhs_reg->reset_use_for_expr_result();

		string label0 = get_new_label();
		curr_stmt = new Control_Flow_IC_Stmt(bc1f, NULL, NULL, label0);
		ic_list.push_back(curr_stmt);

		Const_Opd<int> * p1_opd = new Const_Opd<int>(1);
		curr_stmt = new Move_IC_Stmt(imm_load, p1_opd, new_opd);
		ic_list.push_back(curr_stmt);

		Label_IC_Stmt * label_stmt = new Label_IC_Stmt(label, NULL, label0);
		ic_list.push_back(label_stmt);

		curr_stmt = new Compute_IC_Stmt(not_t, new_opd, new_opd, p1_opd);
		ic_list.push_back(curr_stmt);

		Code_For_Ast * rel_op_float;
		rel_op_float = new Code_For_Ast(ic_list, new_reg);
		return *rel_op_float;
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

Code_For_Ast & Switch_Statement_Ast::compile()
{
	CHECK_INVARIANT((expr != NULL), "expr cannot be null in switch stmt");

	break_label.push_back("");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	Code_For_Ast & expr_stmt = expr->compile();
	Register_Descriptor * expr_reg = expr_stmt.get_reg();
	if (expr_stmt.get_icode_list().empty() == false)
		ic_list = expr_stmt.get_icode_list();
	CHECK_INVARIANT(expr_reg, "expr register cannot be null in switch stmt");
	Register_Addr_Opd * expr_opd = new Register_Addr_Opd(expr_reg);

	// Assigning a label to each case
	vector<string> labels;
	for(auto it : body_v) {
		labels.push_back(get_new_label());
	}

	// Building JumpTable
	for(int i = 0; i< const_v.size(); ++i) {
		Code_For_Ast & const_stmt = const_v[i]->compile();
		Register_Descriptor * const_reg = const_stmt.get_reg();
		if (const_stmt.get_icode_list().empty() == false)
			ic_list.splice(ic_list.end(), const_stmt.get_icode_list());
		CHECK_INVARIANT(const_reg, "const register cannot be null in switch stmt");
		Register_Addr_Opd * const_opd = new Register_Addr_Opd(const_reg);
		Control_Flow_IC_Stmt * control_stmt = new Control_Flow_IC_Stmt(beq, expr_opd, const_opd, labels[i]);
		ic_list.push_back(control_stmt);
		const_reg->reset_use_for_expr_result();
	}

	// Default
	Control_Flow_IC_Stmt * dcontrol_stmt = new Control_Flow_IC_Stmt(j, NULL, NULL, labels.back());
	ic_list.push_back(dcontrol_stmt);

	// Adding code at labels
	for(int i = 0; i< body_v.size(); ++i) {
		Label_IC_Stmt * label_stmt = new Label_IC_Stmt(label, NULL, labels[i]);
		ic_list.push_back(label_stmt);
		Code_For_Ast & temp_stmt = body_v[i]->compile();
		if (temp_stmt.get_icode_list().empty() == false)
			ic_list.splice(ic_list.end(), temp_stmt.get_icode_list());
	}

	expr_reg->reset_use_for_expr_result();

	// Handling labels for break statements
	string iter_label = break_label.back();
	break_label.pop_back();
	if(iter_label != "") {
		Label_IC_Stmt * iter_label_stmt = new Label_IC_Stmt(label, NULL, iter_label);
		ic_list.push_back(iter_label_stmt);
	}
	Code_For_Ast * switch_stmt;
	if (ic_list.empty() == false)
		switch_stmt = new Code_For_Ast(ic_list, NULL);

	return *switch_stmt;
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

	Code_For_Ast *goto_stmt = new Code_For_Ast(ic_list, NULL);

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

	Move_IC_Stmt* store_reg_lhs;
	Move_IC_Stmt* store_reg_rhs;
	if(lhs->get_data_type() == int_data_type) {
		store_reg_lhs = new Move_IC_Stmt(mov, lhs_opd, res_opd);
		store_reg_rhs = new Move_IC_Stmt(mov, rhs_opd, res_opd);	
	} else {
		store_reg_lhs = new Move_IC_Stmt(move_d, lhs_opd, res_opd);
		store_reg_rhs = new Move_IC_Stmt(move_d, rhs_opd, res_opd);	
	}
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

Code_For_Ast & Print_Ast::compile()
{
	CHECK_INVARIANT((to_print != NULL), "expression cannot be null in print statement");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	Register_Addr_Opd * sp_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[sp]);
	Register_Addr_Opd * v0_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[v0]);
	Register_Addr_Opd * a0_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[a0]);
	Register_Addr_Opd * f12_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[f12]);

	Const_Opd<int> * p4_opd = new Const_Opd<int>(4);
	Const_Opd<int> * m4_opd = new Const_Opd<int>(-4);
	Const_Opd<int> * p8_opd = new Const_Opd<int>(8);
	Const_Opd<int> * m8_opd = new Const_Opd<int>(-8);
	Const_Opd<int> * p1_opd = new Const_Opd<int>(1);
	Const_Opd<int> * p3_opd = new Const_Opd<int>(3);
	
	Compute_IC_Stmt* compute_stmt;
	Move_IC_Stmt* move_stmt, *move_before_syscall_stmt, *load_before_syscall_stmt;
	Label_IC_Stmt* label_stmt;

	Code_For_Ast & expr_ast = to_print->compile();
	if(get_data_type() == string_data_type) {
		Const_Opd<string> * str = new Const_Opd<string>(global_str_map[((String_Ast*)to_print)->get_string_label()]);
		move_before_syscall_stmt = new Move_IC_Stmt(la, str, a0_opd);
		load_before_syscall_stmt = new Move_IC_Stmt(imm_load, p4_opd, v0_opd);
	} else {
	 	Register_Descriptor * expr_reg = expr_ast.get_reg();
		CHECK_INVARIANT(expr_reg, "expr register cannot be null in print statement");
		Register_Addr_Opd * expr_opd = new Register_Addr_Opd(expr_reg);
	
		if (expr_ast.get_icode_list().empty() == false)
			ic_list = expr_ast.get_icode_list();

		if(get_data_type() == int_data_type) {
			move_before_syscall_stmt = new Move_IC_Stmt(mov, expr_opd, a0_opd);
			load_before_syscall_stmt = new Move_IC_Stmt(imm_load, p1_opd, v0_opd);
		} else {
			move_before_syscall_stmt = new Move_IC_Stmt(move_d, expr_opd, f12_opd);
			load_before_syscall_stmt = new Move_IC_Stmt(imm_load, p3_opd, v0_opd);
		}
		expr_reg->reset_use_for_expr_result();
	}
	
	compute_stmt = new Compute_IC_Stmt(imm_add, sp_opd, sp_opd, m4_opd);
	ic_list.push_back(compute_stmt);
	move_stmt = new Move_IC_Stmt(store, v0_opd, sp_opd);
	ic_list.push_back(move_stmt);
	compute_stmt = new Compute_IC_Stmt(imm_add, sp_opd, sp_opd, m4_opd);
	ic_list.push_back(compute_stmt);
	move_stmt = new Move_IC_Stmt(store, a0_opd, sp_opd);
	ic_list.push_back(move_stmt);
	compute_stmt = new Compute_IC_Stmt(imm_add, sp_opd, sp_opd, m8_opd);
	ic_list.push_back(compute_stmt);
	move_stmt = new Move_IC_Stmt(store_d, f12_opd, sp_opd);
	ic_list.push_back(move_stmt);

	ic_list.push_back(move_before_syscall_stmt);
	ic_list.push_back(load_before_syscall_stmt);
	label_stmt = new Label_IC_Stmt(syscall, NULL, "");
	ic_list.push_back(label_stmt);

	move_stmt = new Move_IC_Stmt(load_d, sp_opd, f12_opd);
	ic_list.push_back(move_stmt);
	compute_stmt = new Compute_IC_Stmt(imm_add, sp_opd, sp_opd, p8_opd);
	ic_list.push_back(compute_stmt);
	move_stmt = new Move_IC_Stmt(load, sp_opd, a0_opd);
	ic_list.push_back(move_stmt);
	compute_stmt = new Compute_IC_Stmt(imm_add, sp_opd, sp_opd, p4_opd);
	ic_list.push_back(compute_stmt);
	move_stmt = new Move_IC_Stmt(load, sp_opd, v0_opd);
	ic_list.push_back(move_stmt);
	compute_stmt = new Compute_IC_Stmt(imm_add, sp_opd, sp_opd, p4_opd);
	ic_list.push_back(compute_stmt);

	Code_For_Ast * print_stmt = new Code_For_Ast(ic_list, NULL);
	return *print_stmt;
}

//////////////////////////////////////////////////////////////////////////////

Code_For_Ast & String_Ast::compile()
{
	// update global map
	if(global_str_map.find(lbl) == global_str_map.end()){
	  string string_id = get_new_string_label();
	  global_str_map[lbl] = string_id;
	} 
}

//////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Return_Ast::compile()
{
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	string label = "epilogue_" + get_func_name();

	if(get_data_type() == void_data_type) {
		Control_Flow_IC_Stmt * control_stmt = new Control_Flow_IC_Stmt(j, NULL, NULL, label);
		ic_list.push_back(control_stmt);
		Code_For_Ast * ret_stmt = new Code_For_Ast(ic_list, NULL);
		return *ret_stmt;
	} 

	Code_For_Ast & expr_ast = ret_val->compile();
	Register_Descriptor * expr_reg = expr_ast.get_reg();
	CHECK_INVARIANT(expr_reg, "expr register cannot be null in return statement");
	Register_Addr_Opd * expr_opd = new Register_Addr_Opd(expr_reg);
	expr_reg->reset_use_for_expr_result();

	Icode_Stmt * curr_stmt;
	if(get_data_type() == int_data_type) {
		Register_Addr_Opd * v1_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[v1]);
		curr_stmt = new Move_IC_Stmt(mov, expr_opd, v1_opd);
	} else if(get_data_type() == double_data_type) {
		Register_Addr_Opd * f0_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[f0]);
		curr_stmt = new Move_IC_Stmt(move_d, expr_opd, f0_opd);
	}

	if (expr_ast.get_icode_list().empty() == false)
		ic_list = expr_ast.get_icode_list();
	ic_list.push_back(curr_stmt);
	Control_Flow_IC_Stmt * control_stmt = new Control_Flow_IC_Stmt(j, NULL, NULL, label);
	ic_list.push_back(control_stmt);
	Code_For_Ast * ret_stmt = new Code_For_Ast(ic_list, NULL);
	return *ret_stmt;
}

//////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Function_Call_Ast::compile()
{
	CHECK_INVARIANT((proc != NULL), "procedure cannot be null in function call statement");
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	Register_Addr_Opd * sp_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[sp]);

	int offset = 0;

	for(auto it = arguments.end(); it != arguments.begin(); ) {
		--it;
		Code_For_Ast & expr_ast = (*it)->compile();
		if (expr_ast.get_icode_list().empty() == false)
			ic_list.splice(ic_list.end(), expr_ast.get_icode_list());
		Register_Descriptor * expr_reg = expr_ast.get_reg();
		CHECK_INVARIANT(expr_reg, "expr register cannot be null in return statement");
		Register_Addr_Opd * expr_opd = new Register_Addr_Opd(expr_reg);

		if((*it)->get_data_type() == int_data_type) {
			offset -= 4;
			Icode_Stmt * curr_stmt = new Parameter_Store_Stmt(store, expr_opd, sp_opd, offset);
			ic_list.push_back(curr_stmt);
		} else {
			offset -= 8;
			Icode_Stmt * curr_stmt = new Parameter_Store_Stmt(store_d, expr_opd, sp_opd, offset);
			ic_list.push_back(curr_stmt);
		}

		expr_reg->reset_use_for_expr_result();
	}

	Const_Opd<int> * offset_opd = new Const_Opd<int>(-1*offset);
	if(offset != 0) {
		Icode_Stmt* compute_stmt = new Compute_IC_Stmt(sub, sp_opd, sp_opd, offset_opd);
		ic_list.push_back(compute_stmt);
	}
	Control_Flow_IC_Stmt * control_stmt = new Control_Flow_IC_Stmt(jal, NULL, NULL, proc->get_proc_label());
	ic_list.push_back(control_stmt);
	if(offset != 0) {
		Icode_Stmt* compute_stmt = new Compute_IC_Stmt(add, sp_opd, sp_opd, offset_opd);
		ic_list.push_back(compute_stmt);
	}

	Register_Descriptor* reg;

	if(get_data_type() == void_data_type) {

	} else if(get_data_type() == int_data_type) {
		reg = machine_desc_object.get_new_register<gp_data>();
		Register_Addr_Opd * v1_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[v1]);
		Register_Addr_Opd * reg_opd = new Register_Addr_Opd(reg);
		Icode_Stmt * curr_stmt = new Move_IC_Stmt(mov, v1_opd, reg_opd);
		ic_list.push_back(curr_stmt);
	} else {
		reg = machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd * f0_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[f0]);
		Register_Addr_Opd * reg_opd = new Register_Addr_Opd(reg);
		Icode_Stmt * curr_stmt = new Move_IC_Stmt(move_d, f0_opd, reg_opd);
		ic_list.push_back(curr_stmt);
	}

	Code_For_Ast * fn_stmt = new Code_For_Ast(ic_list, reg);
	machine_desc_object.clear_local_register_mappings();
	return *fn_stmt;
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
