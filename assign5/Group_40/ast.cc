#include <iostream>
#include <fstream>
#include <typeinfo>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
// #include "local-environment.hh"
#include "symbol-table.hh"
#include "ast.hh"
// #include "sequence-ast.hh"
#include "procedure.hh"
#include "program.hh"

int Ast::labelCounter = 0;
vector<string> Ast::break_label = vector<string>();

Ast::Ast()
{
}

Ast::~Ast()
{}

bool Ast::check_ast()
{
	stringstream msg;
	msg << "No check_ast() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Data_Type Ast::get_data_type()
{
	stringstream msg;
	msg << "No get_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Symbol_Table_Entry & Ast::get_symbol_entry()
{
	stringstream msg;
	msg << "No get_symbol_entry() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

bool Ast::is_value_zero()
{
	stringstream msg;
	msg << "No is_value_zero() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::set_data_type(Data_Type dt)
{
	stringstream msg;
	msg << "No set_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

////////////////////////////////////////////////////////////////

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line)
{
	CHECK_INVARIANT(temp_lhs!=NULL, "LHS cannot be null")
	CHECK_INVARIANT(temp_rhs!=NULL, "RHS cannot be null")
	lhs = temp_lhs;
	rhs = temp_rhs;
	this->node_data_type = lhs->get_data_type();
	this->ast_num_child = binary_arity;
	this->lineno = line;
	this->check_ast();
}

Assignment_Ast::~Assignment_Ast()
{
	if(lhs != NULL)
		delete lhs;

	if(rhs != NULL)
		delete rhs;
}

bool Assignment_Ast::check_ast()
{
	CHECK_INVARIANT(typeid(*lhs)==typeid(Name_Ast),
				"Cannot assign to "+string(typeid(*lhs).name()));

	CHECK_INPUT(rhs->get_data_type() == lhs->get_data_type(),
				"Assignment statement data type not compatible",
				this->lineno);

	return true;
}

void Assignment_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_SPACE << "Asgn:\n";
	file_buffer << AST_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

/////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line)
{

	CHECK_INVARIANT((var_entry.get_variable_name() == name),
		"Variable's symbol entry is not matching its name");
	variable_symbol_entry = &var_entry;
	this->node_data_type = variable_symbol_entry->get_data_type();
	this->ast_num_child = zero_arity;
	this->lineno = line;
}

Name_Ast::~Name_Ast()
{}

Data_Type Name_Ast::get_data_type()
{
	return this->node_data_type;
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry()
{
	return (*variable_symbol_entry);
}

void Name_Ast::set_data_type(Data_Type dt)
{
	this->node_data_type = dt;
}

void Name_Ast::print(ostream & file_buffer)
{
	file_buffer << "Name : ";
	file_buffer << variable_symbol_entry->get_variable_name();
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Data_Type constant_data_type, int line)
{
	constant = number;
	this->node_data_type = constant_data_type;
	this->ast_num_child = zero_arity;
	this->lineno = line;
}

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::~Number_Ast()
{}

template <class DATA_TYPE>
Data_Type Number_Ast<DATA_TYPE>::get_data_type()
{
	return this->node_data_type;
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::set_data_type(Data_Type dt)
{
	this->node_data_type = dt;
}

template <class DATA_TYPE>
bool Number_Ast<DATA_TYPE>::is_value_zero()
{
	return (this->constant==0);
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::print(ostream & file_buffer)
{
	file_buffer << "Num : ";
	file_buffer << this->constant;
}

///////////////////////////////////////////////////////////////////////////////

Arithmetic_Expr_Ast::~Arithmetic_Expr_Ast()
{
	if(lhs != NULL)
		delete lhs;

	if(rhs != NULL)
		delete rhs;
}

Data_Type Arithmetic_Expr_Ast::get_data_type()
{
	CHECK_INVARIANT(this->ast_num_child != zero_arity,
		"Arithmetic_Expr_Ast cannot have zero_arity");

	return this->node_data_type;
}

void Arithmetic_Expr_Ast::set_data_type(Data_Type dt)
{
	this->node_data_type = dt;
}

bool Arithmetic_Expr_Ast::check_ast()
{
	if(this->ast_num_child == binary_arity || this->ast_num_child == ternary_arity) {
		CHECK_INPUT(lhs->get_data_type()==rhs->get_data_type(),
					"Arithmetic statement data type not compatible",
					this->lineno);
	}

	return true;
}

/////////////////////////////////////////////////////////////////////

Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line)
{
	CHECK_INVARIANT(l!=NULL, "LHS cannot be null")
	CHECK_INVARIANT(r!=NULL, "RHS cannot be null")
	this->lhs = l;
	this->rhs = r;
	this->node_data_type = l->get_data_type();
	this->ast_num_child = binary_arity;
	this->lineno = line;
	this->check_ast();
}

void Plus_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "Arith: PLUS\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

/////////////////////////////////////////////////////////////////

Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line)
{
	CHECK_INVARIANT(l!=NULL, "LHS cannot be null")
	CHECK_INVARIANT(r!=NULL, "RHS cannot be null")
	this->lhs = l;
	this->rhs = r;
	this->node_data_type = l->get_data_type();
	this->ast_num_child = binary_arity;
	this->lineno = line;
	this->check_ast();
}

void Minus_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "Arith: MINUS\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////

Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line)
{
	CHECK_INVARIANT(l!=NULL, "LHS cannot be null")
	CHECK_INVARIANT(r!=NULL, "RHS cannot be null")
	this->lhs = l;
	this->rhs = r;
	this->node_data_type = l->get_data_type();
	this->ast_num_child = binary_arity;
	this->lineno = line;
	this->check_ast();
}

void Mult_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "Arith: MULT\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

////////////////////////////////////////////////////////////////////

Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line)
{
	CHECK_INVARIANT(l!=NULL, "LHS cannot be null")
	CHECK_INVARIANT(r!=NULL, "RHS cannot be null")
	this->lhs = l;
	this->rhs = r;
	this->node_data_type = l->get_data_type();
	this->ast_num_child = binary_arity;
	this->lineno = line;
	this->check_ast();
}

void Divide_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "Arith: DIV\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////////

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line)
{
	CHECK_INVARIANT(l!=NULL, "LHS cannot be null")
	this->lhs = l;
	this->rhs = NULL;
	this->node_data_type = l->get_data_type();
	this->ast_num_child = unary_arity;
	this->lineno = line;
	this->check_ast();
}

void UMinus_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "Arith: UMINUS\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////////

Conditional_Operator_Ast::Conditional_Operator_Ast(Ast* cond, Ast * l, Ast * r, int line)
{
	CHECK_INVARIANT(cond!=NULL, "Condition cannot be null")
	CHECK_INVARIANT(l!=NULL, "LHS cannot be null")
	CHECK_INVARIANT(r!=NULL, "RHS cannot be null")
	this->cond = cond;
	this->lhs = l;
	this->rhs = r;
	// if(l->get_data_type() != r->get_data_type())
	// 	this->node_data_type = void_data_type;
	// else
		this->node_data_type = l->get_data_type();
	this->ast_num_child = ternary_arity;
	this->lineno = line;
	this->check_ast();
}

Conditional_Operator_Ast::~Conditional_Operator_Ast()
{
	if(lhs != NULL)
		delete lhs;

	if(rhs != NULL)
		delete rhs;

	if(cond != NULL)
		delete cond;
}

void Conditional_Operator_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "Arith: Conditional\n";
	file_buffer << AST_SUB_NODE_SPACE << "COND (";
	cond->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////////

Relational_Expr_Ast::Relational_Expr_Ast(Ast * lhs, Relational_Op rop, Ast * rhs, int line)
{
	CHECK_INVARIANT(lhs!=NULL, "LHS cannot be null")
	CHECK_INVARIANT(rhs!=NULL, "RHS cannot be null")
	this->rel_op = rop;
	this->lhs_condition = lhs;
	this->rhs_condition = rhs;
	this->node_data_type = int_data_type;
	this->ast_num_child = binary_arity;
	this->lineno = line;
	this->check_ast();
}

Relational_Expr_Ast::~Relational_Expr_Ast()
{
	if(lhs_condition != NULL)
		delete lhs_condition;

	if(rhs_condition != NULL)
		delete rhs_condition;
}

Data_Type Relational_Expr_Ast::get_data_type()
{
	return this->node_data_type;
}

void Relational_Expr_Ast::set_data_type(Data_Type dt)
{
	this->node_data_type = dt;
}

bool Relational_Expr_Ast::check_ast()
{
	if(this->ast_num_child == binary_arity) {
		CHECK_INPUT(lhs_condition->get_data_type()==rhs_condition->get_data_type(),
					"Relational statement data type not compatible",
					this->lineno);
	}

	return true;
}

void Relational_Expr_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "Condition: ";
	switch(this->rel_op) {
		case less_equalto:
			file_buffer << "LE";
			break;
		case less_than:
			file_buffer << "LT";
			break;
		case greater_than:
			file_buffer << "GT";
			break;
		case greater_equalto:
			file_buffer << "GE";
			break;
		case equalto:
			file_buffer << "EQ";
			break;
		case not_equalto:
			file_buffer << "NE";
			break;
	}
	file_buffer << "\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	lhs_condition->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs_condition->print(file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////////

Boolean_Expr_Ast::Boolean_Expr_Ast(Ast * lhs, Boolean_Op bop, Ast * rhs, int line)
{
	CHECK_INVARIANT(rhs!=NULL, "RHS cannot be null")
	this->bool_op = bop;
	this->lhs_op = lhs;
	this->rhs_op = rhs;
	this->node_data_type = rhs->get_data_type();
	this->ast_num_child = binary_arity;
	this->lineno = line;
	this->check_ast();
}

Boolean_Expr_Ast::~Boolean_Expr_Ast()
{
	if(lhs_op != NULL)
		delete lhs_op;

	if(rhs_op != NULL)
		delete rhs_op;
}

Data_Type Boolean_Expr_Ast::get_data_type()
{
	return this->node_data_type;
}

void Boolean_Expr_Ast::set_data_type(Data_Type dt)
{
	this->node_data_type = dt;
}

bool Boolean_Expr_Ast::check_ast()
{
	return true;
}

void Boolean_Expr_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "Condition: ";
	switch(this->bool_op) {
		case boolean_or:
			file_buffer << "OR";
			break;
		case boolean_and:
			file_buffer << "AND";
			break;
		case boolean_not:
			file_buffer << "NOT";
			break;
	}
	file_buffer << "\n";
	if(lhs_op != NULL) {
		file_buffer << AST_SUB_NODE_SPACE << "LHS (";
		lhs_op->print(file_buffer);
		file_buffer << ")\n";
	}
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs_op->print(file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////////

Selection_Statement_Ast::Selection_Statement_Ast(Ast * cond, Ast* then_part, Ast* else_part, int line)
{
	CHECK_INVARIANT(cond!=NULL, "Condition cannot be null")
	CHECK_INVARIANT(then_part!=NULL, "then_part cannot be null")
	CHECK_INVARIANT(else_part!=NULL, "then_part cannot be null")
	this->cond = cond;
	this->then_part = then_part;
	this->else_part = else_part;
	this->node_data_type = void_data_type;
	this->ast_num_child = ternary_arity;
	this->lineno = line;
	this->check_ast();
}

Selection_Statement_Ast::~Selection_Statement_Ast()
{
	if(cond != NULL)
		delete cond;

	if(then_part != NULL)
		delete then_part;

	if(else_part != NULL)
		delete else_part;
}

Data_Type Selection_Statement_Ast::get_data_type()
{
	return this->node_data_type;
}

void Selection_Statement_Ast::set_data_type(Data_Type dt)
{
	this->node_data_type = dt;
}

bool Selection_Statement_Ast::check_ast()
{
	return true;
}

void Selection_Statement_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_SPACE << "IF :\n";
	file_buffer << AST_SPACE << "CONDITION (";
	cond->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SPACE << "THEN (";
	then_part->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SPACE << "ELSE (";
	else_part->print(file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////////

Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * cond, Ast* body, int line, bool do_form)
{
	CHECK_INVARIANT(cond!=NULL, "Condition cannot be null")
	CHECK_INVARIANT(body!=NULL, "Body cannot be null")
	this->cond = cond;
	this->body = body;
	this->is_do_form = do_form;
	this->node_data_type = void_data_type;
	this->ast_num_child = binary_arity;
	this->lineno = line;
	this->check_ast();
}

Iteration_Statement_Ast::~Iteration_Statement_Ast()
{
	if(cond != NULL)
		delete cond;

	if(body != NULL)
		delete body;
}

Data_Type Iteration_Statement_Ast::get_data_type()
{
	return this->node_data_type;
}

void Iteration_Statement_Ast::set_data_type(Data_Type dt)
{
	this->node_data_type = dt;
}

bool Iteration_Statement_Ast::check_ast()
{
	return true;
}

void Iteration_Statement_Ast::print(ostream & file_buffer)
{
	if(this->is_do_form) {
		file_buffer << "\n";
		file_buffer << AST_SPACE << "DO (";
		body->print(file_buffer);
		file_buffer << ")\n";
		file_buffer << AST_SPACE << "WHILE CONDITION (";
		cond->print(file_buffer);
		file_buffer << ")";
	} else {
		file_buffer << "\n";
		file_buffer << AST_SPACE << "WHILE :\n";
		file_buffer << AST_SPACE << "CONDITION (";
		cond->print(file_buffer);
		file_buffer << ")\n";
		file_buffer << AST_SPACE << "BODY (";
		body->print(file_buffer);
		file_buffer << ")";
	}
}

//////////////////////////////////////////////////////////////////////

Label_Ast::Label_Ast(string label, int line)
{
	CHECK_INVARIANT(label!="", "label cannot be null")
	this->lbl = label;
	this->node_data_type = void_data_type;
	this->ast_num_child = zero_arity;
	this->lineno = line;
	this->check_ast();
}

bool Label_Ast::check_ast()
{
	return true;
}

void Label_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_SPACE << "Label (" << this->lbl << "):";
}

//////////////////////////////////////////////////////////////////////

Goto_Label_Ast::Goto_Label_Ast(string label, int line)
{
	CHECK_INVARIANT(label!="", "label cannot be null")
	this->lbl = label;
	this->node_data_type = void_data_type;
	this->ast_num_child = zero_arity;
	this->lineno = line;
	this->check_ast();
}

bool Goto_Label_Ast::check_ast()
{
	return true;
}

void Goto_Label_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_SPACE << "GOTO (" << this->lbl << ")";
}

//////////////////////////////////////////////////////////////////////

Break_Ast::Break_Ast(int line)
{
	this->node_data_type = void_data_type;
	this->ast_num_child = zero_arity;
	this->lineno = line;
	this->check_ast();
}

bool Break_Ast::check_ast()
{
	return true;
}

void Break_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_SPACE << "BREAK";
}

//////////////////////////////////////////////////////////////////////

Sequence_Ast::Sequence_Ast(int line)
{
	this->node_data_type = void_data_type;
	this->ast_num_child = zero_arity;
	this->lineno = line;
}

Sequence_Ast::~Sequence_Ast()
{
	for (auto it : statement_list) {
		if(it != NULL)
			delete it;
	}
}

void Sequence_Ast::ast_push_back(Ast * ast)
{
	statement_list.push_back(ast);
}

void Sequence_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << SA_SPACE << "Sequence Ast:\n";
	for(auto it : statement_list) {
		it->print(file_buffer);
	}
}

// check correctness
void Sequence_Ast::eliminate_dead_code()
{
	if (!sa_icode_list.empty()) {
		
		CFG* cfg = generate_cfg();
		cfg -> compute_in_out();

		#ifdef DEBUG
		// Print cfg
		cout << "INITIAL CFG:-----" << endl;
		cfg -> print(cout);
		int iter=0;
		cout << "Elimination iteration " << iter++ << endl;
		#endif

		while(cfg->eliminate_dead_code()) {
			cfg -> compute_in_out();
			
			#ifdef DEBUG
			cout << "Elimination iteration " << iter++ << endl;
			#endif
		}

		// Update icode list of sequence-ast
		sa_icode_list = cfg->get_icode_list();

		delete cfg;
	}
}

CFG* Sequence_Ast::generate_cfg()
{
	string lbl;
	Basic_Block *label_block, *next_block;

	Basic_Block *curr_block = new Basic_Block();
	CFG *cfg = new CFG(curr_block);

	ELEMENT_SET global_elems;

	for(auto curr_stmt : sa_icode_list) {
		Tgt_Op op = curr_stmt->get_op().get_op();

		switch(op) {
			case label:
				lbl = ((Label_IC_Stmt*)curr_stmt)->get_offset();
				label_block = cfg->get_block(lbl);
				curr_block -> set_children(NULL, label_block);

				// Save block currblock
				cfg -> save_block(curr_block);

				// Push statement to label_block
				curr_block = label_block;
				curr_block->push_stmt(curr_stmt);
				break;

			case j:
				curr_block->push_stmt(curr_stmt);

				lbl = ((Control_Flow_IC_Stmt*)curr_stmt)->get_Offset();
				label_block = cfg->get_block(lbl);
				curr_block->set_children(NULL, label_block);

				// Save block currblock
				cfg -> save_block(curr_block);
				curr_block = new Basic_Block();
				break;

			case beq:
			case bne:
			case bgtz:
			case bgez:
			case bltz:
			case blez:
				curr_block->insert_gen(curr_stmt->get_opd1());
				curr_block->insert_gen(curr_stmt->get_opd2());
				curr_block->push_stmt(curr_stmt);

				lbl = ((Control_Flow_IC_Stmt*)curr_stmt)->get_Offset();
				label_block = cfg->get_block(lbl);
				next_block = new Basic_Block();
				curr_block->set_children(next_block, label_block);

				// Save block currblock
				cfg -> save_block(curr_block);

				curr_block = next_block;
				break;

			default:
				curr_block->insert_gen(curr_stmt->get_opd1());
				curr_block->insert_gen(curr_stmt->get_opd2());
				curr_block->insert_kill(curr_stmt->get_result());
				curr_block->push_stmt(curr_stmt);

				Ics_Opd* res = curr_stmt->get_result();
				if(res!=NULL and 
					typeid(*res) == typeid(Mem_Addr_Opd) and 
					res->get_symbol_table_entry()->get_symbol_scope() == global)
					global_elems.insert(res);
		}
	}
	// Save last block
	cfg -> save_block(curr_block);
	cfg -> set_global_set(global_elems);

	#ifdef DEBUG
	cout << "Computed Global Set: "; print_set(cfg -> get_global_set(), cout);
	#endif

	return cfg;
}

template class Number_Ast<double>;
template class Number_Ast<int>;
