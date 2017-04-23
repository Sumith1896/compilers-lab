

#ifndef SEQUENCE_AST_HH
#define SEQUENCE_AST_HH

#include <string>
#include <map>
#include <list>

#define SA_SPACE "      "

using namespace std;

class Sequence_Ast;

class Sequence_Ast
{
	list<Ast *> statement_list;

	list<Icode_Stmt *> sa_icode_list;

	int lineno;

public:
	Sequence_Ast(int line);
	~Sequence_Ast();

	void ast_push_back(Ast * ast);

	void print_sequence_ast(ostream & file_buffer);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	// compile
	void compile();
	void print_assembly(ostream & file_buffer);
	void print_icode(ostream & file_buffer);
};

#endif
