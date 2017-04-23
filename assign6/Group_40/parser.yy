%scanner scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc

%union
{
	pair<Data_Type, vector<string> > * vec_decl;
	Symbol_Table * symbol_table;
	vector<Symbol_Table_Entry *> * vec_symbol_entry;
	Procedure * procedure;

	int integer_value;
	float float_value;
	string * string_value;
	Sequence_Ast * sequence_ast;
	Ast * ast;

	vector<string> * vec_string;
	pair<vector<Ast*>, vector<Ast*> > * pair_case_body;

	vector<pair<Data_Type, string> > * vec_pair_type_string;
	vector<Ast*>* vec_ast;
	pair<Data_Type,string>* pair_type_string;
};

//ADD TOKENS HERE
%token <integer_value> INTEGER_NUMBER
%token <float_value> DOUBLE_NUMBER
%token <string_value> NAME
%token <string_value> STRING
%token INTEGER FLOAT
%token ASSIGN VOID
%token IF
%token DO WHILE FOR
%token GOTO BREAK
%token SWITCH CASE DEFAULT
%token RETURN PRINT

%nonassoc THEN
%nonassoc ELSE
%left OR
%left AND
%left EQ NEQ
%left LT GT LEQ GEQ
%left '+' '-'
%left '*' '/'
%right UMINUS
%left NOT
%nonassoc '('


%type <symbol_table> optional_variable_declaration_list
%type <symbol_table> variable_declaration_list
%type <vec_symbol_entry> variable_declaration
%type <vec_decl> declaration

//ADD CODE HERE
%type <sequence_ast> statement_list
%type <ast> assignment_statement
%type <ast> variable
%type <ast> constant
%type <ast> expression_term
%type <ast> arith_expression
%type <ast> a_operand

%type <vec_string> name_list
%type <ast> statement
%type <ast> conditional_statement
%type <ast> loop_statement
%type <ast> do_while_statement
%type <ast> while_statement
%type <ast> bool_expression
%type <ast> rel_expression
%type <ast> b_operand
%type <sequence_ast> initialization_list
%type <sequence_ast> optional_initialization_list
%type <ast> initialization
%type <ast> for_loop_statement
%type <ast> goto_statement
%type <ast> label_statement
%type <ast> break_statement

%type <sequence_ast> optional_dsl
%type <ast> switch_statement
%type <pair_case_body> optional_csl
%type <pair_case_body> csl

%type <ast> function_call
%type <ast> return_statement
%type <ast> print_statement
%type <ast> function_call_statement
%type <vec_ast> optional_expression_list
%type <vec_ast> expression_list
%type <vec_pair_type_string> optional_argument_list
%type <vec_pair_type_string> argument_list
%type <pair_type_string> argument

%start program

%%


// ####################
program: // MOD
	declaration_list mult_procedure_definition
	{
	if (NOT_ONLY_PARSE)
	{
		// cout<<"global_list_in_proc_check"<<endl;
		program_object.global_list_in_proc_check();
		// cout<<"find_and_set_main_proc"<<endl;
		program_object.find_and_set_main_proc();
		// cout<<"check_for_undefined_functions"<<endl;
		program_object.check_for_undefined_functions();
		// cout<<"done"<<endl;
	}
	}
;

declaration_list: // MOD
	mult_procedure_declaration
	{
	if (NOT_ONLY_PARSE)
	{
		Symbol_Table * global_table = new Symbol_Table();
		program_object.set_global_table(*global_table);
	}
	}
|
	variable_declaration_list
	mult_procedure_declaration
	{
	if (NOT_ONLY_PARSE)
	{
		Symbol_Table * global_table = $1;

		CHECK_INVARIANT((global_table != NULL), "Global declarations cannot be null");

		program_object.set_global_table(*global_table);
	}
	}
|
	mult_procedure_declaration
	variable_declaration_list
	{
	if (NOT_ONLY_PARSE)
	{
		Symbol_Table * global_table = $2;

		CHECK_INVARIANT((global_table != NULL), "Global declarations cannot be null");

		program_object.set_global_table(*global_table);
	}
	}
;

mult_procedure_definition:
	procedure_definition
|
	mult_procedure_definition
	procedure_definition
;

mult_procedure_declaration:
	procedure_declaration
|
	mult_procedure_declaration
	procedure_declaration
;

procedure_declaration: // MOD
	VOID NAME '(' optional_argument_list ')' ';'
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");
		CHECK_INVARIANT(($4 != NULL), "Formal symbol table cannot be null");

		string proc_name = *$2;
		vector<pair<Data_Type,string> >* args_vec = $4;

		Procedure* proc = new Procedure(void_data_type, proc_name, get_line_number());
		proc->set_args(*args_vec);

		// CHECK_INPUT ((program_object.proc_global_variable_check(proc_name) == false),
		// 			"A global variable with the same name has already been declared", get_line_number());
		CHECK_INPUT ((program_object.proc_proc_check(proc) == false),
					"Procedure with same name has already been declared before", get_line_number());

		for(auto arg: *args_vec)
			CHECK_INPUT (arg.second != proc_name,
				"Formal parameter name cannot be same as Procedure name", get_line_number());

		for(int i=0; i<(*args_vec).size();i++)
			for(int j = i+1; j<(*args_vec).size();j++ )
			CHECK_INPUT ((*args_vec)[i].second != (*args_vec)[j].second,
				"Formal parameters of the same procedure should have distinct names", get_line_number());

		program_object.add_procedure(proc);

		delete $2;
		delete $4;
	}
	}
|
	INTEGER NAME '(' optional_argument_list ')' ';'
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");
		CHECK_INVARIANT(($4 != NULL), "Formal symbol table cannot be null");

		string proc_name = *$2;
		vector<pair<Data_Type,string> >* args_vec = $4;

		Procedure* proc = new Procedure(int_data_type, proc_name, get_line_number());
		proc->set_args(*args_vec);

		// CHECK_INPUT ((program_object.proc_global_variable_check(proc_name) == false),
		// 			"A global variable with the same name has already been declared", get_line_number());
		CHECK_INPUT ((program_object.proc_proc_check(proc) == false),
					"Procedure has already been declared before", get_line_number());

		program_object.add_procedure(proc);

		delete $2;
		delete $4;
	}
	}
|
	FLOAT NAME '(' optional_argument_list ')' ';'
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");
		CHECK_INVARIANT(($4 != NULL), "Formal symbol table cannot be null");

		string proc_name = *$2;
		vector<pair<Data_Type,string> >* args_vec = $4;

		Procedure* proc = new Procedure(double_data_type, proc_name, get_line_number());
		proc->set_args(*args_vec);

		// CHECK_INPUT ((program_object.proc_global_variable_check(proc_name) == false),
		// 			"A global variable with the same name has already been declared", get_line_number());
		CHECK_INPUT ((program_object.proc_proc_check(proc) == false),
					"Procedure has already been declared before", get_line_number());

		program_object.add_procedure(proc);

		delete $2;
		delete $4;
	}
	}
;


procedure_definition: // MOD
	NAME '(' optional_argument_list ')'
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Procedure name cannot be null");
		CHECK_INVARIANT(($3 != NULL), "Formal symbol table cannot be null");

		string proc_name = *$1;
		vector<pair<Data_Type,string> >* args_vec = $3;

		current_procedure = program_object.find_proc(proc_name, *args_vec);
		CHECK_INPUT_AND_ABORT(current_procedure != NULL,
			"Procedure prototype not found", get_line_number());
		current_procedure->set_defined();
	}
	}
	'{' optional_variable_declaration_list
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

		Symbol_Table * local_table = $7;

		if (local_table == NULL)
			local_table = new Symbol_Table();

		current_procedure->set_local_list(*local_table);
	}
	}

	statement_list '}'
	{
	if (NOT_ONLY_PARSE)
	{
		Sequence_Ast* seq = $9;

		CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");
		CHECK_INVARIANT((seq != NULL), "statement list cannot be null");

		seq->check_last_statement_is_return(get_line_number());

		current_procedure->set_sequence_ast(*seq);
		current_procedure = NULL;
	}
	}
;

// ####################

optional_variable_declaration_list:
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = NULL;
	}
	}
|
	variable_declaration_list
	{
	if (NOT_ONLY_PARSE)
	{
		// cout << "optional_variable_declaration_list" <<endl;
		CHECK_INVARIANT(($1 != NULL), "Declaration statement list cannot be null here");

		$$ = $1;
	}
	}
;

variable_declaration_list:
	variable_declaration
	{
	if (NOT_ONLY_PARSE)
	{
		vector<Symbol_Table_Entry *> * vec_decl_stmt = $1;

		CHECK_INVARIANT((vec_decl_stmt != NULL), "Non-terminal declaration statement cannot be null");
		CHECK_INVARIANT((vec_decl_stmt->size() > 0), "Non-terminal declaration statement cannot be null");

		Symbol_Table * decl_list = new Symbol_Table();

		for(int i=0; i<vec_decl_stmt->size(); i++) {

			Symbol_Table_Entry * decl_stmt = (*vec_decl_stmt)[i];
			CHECK_INVARIANT((decl_stmt != NULL), "Declaration statement cannot be null");

			string decl_name = decl_stmt->get_variable_name();
			CHECK_INPUT ((program_object.variable_proc_name_check(decl_name) == false),
					"Variable name cannot be same as the procedure name", get_line_number());

			if(current_procedure != NULL) {
				CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
					"Variable name cannot be same as procedure name", get_line_number());
				CHECK_INPUT((current_procedure->variable_in_symbol_list_check(decl_name) == false ),
					"Variable name is already used", get_line_number());
			}

			CHECK_INPUT((decl_list->variable_in_symbol_list_check(decl_name) == false),
				"Variable is declared twice", get_line_number());

			decl_list->push_symbol(decl_stmt);
		}

		$$ = decl_list;
	}
	}
|
	variable_declaration_list variable_declaration
	{
	if (NOT_ONLY_PARSE)
	{
		// if declaration is local then no need to check in global list
		// if declaration is global then this list is global list

		// cout << "variable_declaration_list ext.." <<endl;
		Symbol_Table * decl_list = $1;
		vector<Symbol_Table_Entry *> * vec_decl_stmt = $2;

		CHECK_INVARIANT((decl_list != NULL), "The declaration statement list cannot be null");
		CHECK_INVARIANT((vec_decl_stmt != NULL), "Declaration statement cannot be null");
		CHECK_INVARIANT((vec_decl_stmt->size() > 0), "Declaration statement cannot be null");

		for(int i=0; i<vec_decl_stmt->size(); i++) {

			Symbol_Table_Entry * decl_stmt = (*vec_decl_stmt)[i];
			CHECK_INVARIANT((decl_stmt != NULL), "Declaration statement cannot be null");

			string decl_name = decl_stmt->get_variable_name();
			CHECK_INPUT ((program_object.variable_proc_name_check(decl_name) == false),
					"Variable name cannot be same as the procedure name", get_line_number());

			if(current_procedure != NULL) {
				CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
					"Variable name cannot be same as procedure name", get_line_number());
				CHECK_INPUT((current_procedure->variable_in_symbol_list_check(decl_name) == false ),
					"Variable name is already used", get_line_number());
			}

			CHECK_INPUT((decl_list->variable_in_symbol_list_check(decl_name) == false),
				"Variable is declared twice", get_line_number());

			decl_list->push_symbol(decl_stmt);
		}

		$$ = decl_list;
	}
	}
;

variable_declaration:
	declaration ';'
	{
	if (NOT_ONLY_PARSE)
	{
		// cout << "variable_declaration:" <<endl;
		pair<Data_Type, vector<string> >* decl = $1;

		CHECK_INVARIANT((decl != NULL), "Declaration cannot be null");
		CHECK_INVARIANT(((decl->second).size() != 0), "Declaration cannot be empty");

		Data_Type type = decl->first;
		vector<string> decl_name_list = decl->second;
		vector<Symbol_Table_Entry *> * decl_entry_vec = new vector<Symbol_Table_Entry *>;

		for (int i = 0; i < decl_name_list.size(); ++i)
		{
			string decl_name = decl_name_list[i];
			Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(decl_name, type, get_line_number());
			decl_entry_vec->push_back(decl_entry);
		}

		// cout << "variable_declaration_ended" <<endl;
		$$ = decl_entry_vec;
	}
	}
;

declaration:
	INTEGER name_list
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($2 != NULL, "Name cannot be NULL");
		auto decl = new pair<Data_Type, vector<string> >;
		vector<string>* nl = $2;

		decl->first = int_data_type;
		decl->second = *nl;
		$$ = decl;
	}
	}
|
	FLOAT name_list
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($2 != NULL, "Name cannot be NULL");
		auto decl = new pair<Data_Type, vector<string> >;
		vector<string>* nl = $2;

		decl->first = double_data_type;
		decl->second = *nl;
		$$ = decl;
	}
	}
;

name_list:
	NAME
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT($1 != NULL, "Name cannot be NULL");
		vector<string>* nl = new vector<string>(1,*($1));
		$$ = nl;
	}
	}
|
	name_list ',' NAME
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT($1 != NULL, "Name List cannot be NULL");
		CHECK_INVARIANT($3 != NULL, "Name cannot be NULL");
		vector<string>* nl = $1;
		nl->push_back(*($3));
		$$ = nl;
	}
	}
;

statement_list:
	{
	if (NOT_ONLY_PARSE)
	{
		Sequence_Ast* stmt_list = new Sequence_Ast(get_line_number());
		$$ = stmt_list;
	}
	}
|
	statement_list statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "statement_list cannot be NULL");
		CHECK_INVARIANT($2 != NULL, "statement cannot be NULL");
		Sequence_Ast* stmt_list = $1;
		Ast* stmt = $2;

		if(stmt != NULL)
			stmt_list->ast_push_back(stmt);

		$$ = stmt_list;
	}
	}
;

statement:
	assignment_statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "statement cannot be NULL");
		// ($1)->check_ast();
		$$ = (Ast*)$1;
	}
	}
|
	conditional_statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "statement cannot be NULL");
		// ($1)->check_ast();
		$$ = (Ast*)$1;
	}
	}
|
	loop_statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "statement cannot be NULL");
		// ($1)->check_ast();
		$$ = (Ast*)$1;
	}
	}
|
	'{' statement_list '}'
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($2 != NULL, "statement cannot be NULL");
		$$ = (Ast*)$2;
	}
	}
|
	goto_statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "statement cannot be NULL");
		// ($1)->check_ast();
		$$ = (Ast*)$1;
	}
	}
|
	label_statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "statement cannot be NULL");
		// ($1)->check_ast();
		$$ = (Ast*)$1;
	}
	}
|
	break_statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "statement cannot be NULL");
		// ($1)->check_ast();
		$$ = (Ast*)$1;
	}
	}
|
	switch_statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "statement cannot be NULL");
		// ($1)->check_ast();
		$$ = (Ast*)$1;
	}
	}
|
	function_call_statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "statement cannot be NULL");
		// ($1)->check_ast();
		$$ = (Ast*)$1;
	}
	}
|
	return_statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "statement cannot be NULL");
		// ($1)->check_ast();
		$$ = (Ast*)$1;
	}
	}
|
	print_statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "statement cannot be NULL");
		// ($1)->check_ast();
		$$ = (Ast*)$1;
	}
	}
;

function_call_statement:
	function_call ';'
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "function_call cannot be NULL");
		// ($1)->check_ast();
		$$ = (Ast*)$1;
	}
	}
;

print_statement:
	PRINT '(' arith_expression ')' ';'
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($3 != NULL, "arith_expression cannot be NULL");
		// ($1)->check_ast();
		Ast* arith = $3;
		Ast* print_ast = new Print_Ast(arith,get_line_number());
		$$ = print_ast;
	}
	}
|
	PRINT '(' STRING')' ';'
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($3 != NULL, "string cannot be NULL");
		// ($1)->check_ast();
		string s = *$3;
		Ast* str_ast = new String_Ast(s,get_line_number());
		Ast* print_ast = new Print_Ast(str_ast,get_line_number());
		$$ = print_ast;
	}
	}
;

return_statement:
	RETURN ';'
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INPUT_AND_ABORT(current_procedure != NULL,
			"Return statement cannot be outside procedue", get_line_number());

		Ast* return_ast = new Return_Ast(NULL,current_procedure->get_proc_name(),get_line_number());
		$$ = return_ast;
	}
	}
|
	RETURN arith_expression';'
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($2 != NULL, "arith_expression cannot be NULL");

		CHECK_INPUT_AND_ABORT(current_procedure != NULL,
			"Return statement cannout be outside procedue", get_line_number());
		CHECK_INPUT(current_procedure->get_return_type() == $2->get_data_type(),
			"Return statement doesn't match with function type", get_line_number());

		// ($1)->check_ast();
		Ast* arith = $2;
		Ast* return_ast = new Return_Ast(arith,current_procedure->get_proc_name(),get_line_number());
		$$ = return_ast;
	}
	}
;

conditional_statement:
	IF '(' bool_expression ')' statement 		%prec THEN
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		Ast* cond = $3;
		Ast* then_part = $5;
		Ast* else_part = new Sequence_Ast(get_line_number());

		CHECK_INVARIANT(cond != NULL, "condition cannot be NULL");
		CHECK_INVARIANT(then_part != NULL, "then_part cannot be NULL");
		CHECK_INVARIANT(else_part != NULL, "else_part cannot be NULL");

		Ast* sel_stmt = new Selection_Statement_Ast(cond,then_part,else_part, get_line_number());
		$$ = sel_stmt;
	}
	}
|
	IF '(' bool_expression ')' statement ELSE statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		Ast* cond = $3;
		Ast* then_part = $5;
		Ast* else_part = $7;


		CHECK_INVARIANT(cond != NULL, "condition cannot be NULL");
		CHECK_INVARIANT(then_part != NULL, "then_part cannot be NULL");
		CHECK_INVARIANT(else_part != NULL, "else_part cannot be NULL");

		Ast* sel_stmt = new Selection_Statement_Ast(cond,then_part,else_part, get_line_number());
		$$ = sel_stmt;
	}
	}
;

loop_statement:
	do_while_statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "do_while_statement cannot be NULL");
		$$ = (Ast*)$1;
	}
	}
|
	while_statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "while_statement cannot be NULL");
		$$ = (Ast*)$1;
	}
	}
|
	for_loop_statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "for_loop_statement cannot be NULL");
		$$ = (Ast*)$1;
	}
	}
;

do_while_statement:
	DO statement WHILE '(' bool_expression ')' ';'
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		Ast* cond = $5;
		Ast* body = $2;

		CHECK_INVARIANT(cond != NULL, "condition cannot be NULL");
		CHECK_INVARIANT(body != NULL, "body cannot be NULL");

		Ast* iter_stmt = new Iteration_Statement_Ast(cond,body,get_line_number(),true);
		$$ = iter_stmt;
	}
	}
;

while_statement:
	WHILE '(' bool_expression ')' statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		Ast* cond = $3;
		Ast* body = $5;

		CHECK_INVARIANT(cond != NULL, "condition cannot be NULL");
		CHECK_INVARIANT(body != NULL, "body cannot be NULL");

		Ast* iter_stmt = new Iteration_Statement_Ast(cond,body,get_line_number(),false);
		$$ = iter_stmt;
	}
	}
;

// Make sure to call check_ast in assignment_statement and arith_expression
// Refer to error_display.hh for displaying semantic errors if any
assignment_statement:
	variable ASSIGN arith_expression ';'
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "variable cannot be NULL");
		CHECK_INVARIANT($3 != NULL, "arith_expression cannot be NULL");
		Ast* var = $1;
		Ast* exp = $3;

		Ast* ass = new Assignment_Ast(var, exp, get_line_number());

		$$ = ass;
	}
	}
|
	variable ASSIGN function_call ';'
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "variable cannot be NULL");
		CHECK_INVARIANT($3 != NULL, "function_call cannot be NULL");
		Ast* var = $1;
		Ast* exp = $3;

		Ast* ass = new Assignment_Ast(var, exp, get_line_number());

		$$ = ass;
	}
	}

;

arith_expression:
		//ADD RELEVANT CODE ALONG WITH GRAMMAR RULES HERE
                // SUPPORT binary +, -, *, / operations, unary -, and allow parenthesization
                // i.e. E -> (E)
                // Connect the rules with the remaining rules given below
	a_operand '+' a_operand
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Left Operand cannot be null");
		CHECK_INVARIANT(($3 != NULL), "Right Operand cannot be null");
		Ast* l = $1;
		Ast* r = $3;
		Ast* ast = new Plus_Ast(l,r,get_line_number());
		// ast->check_ast();
		$$ = ast;
	}
	}
|
	a_operand '-' a_operand
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Left Operand cannot be null");
		CHECK_INVARIANT(($3 != NULL), "Right Operand cannot be null");
		Ast* l = $1;
		Ast* r = $3;
		Ast* ast = new Minus_Ast(l,r,get_line_number());
		// ast->check_ast();
		$$ = ast;
	}
	}
|
	a_operand '*' a_operand
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Left Operand cannot be null");
		CHECK_INVARIANT(($3 != NULL), "Right Operand cannot be null");
		Ast* l = $1;
		Ast* r = $3;
		Ast* ast = new Mult_Ast(l,r,get_line_number());
		// ast->check_ast();
		$$ = ast;
	}
	}
|
	a_operand '/' a_operand
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Left Operand cannot be null");
		CHECK_INVARIANT(($3 != NULL), "Right Operand cannot be null");
		Ast* l = $1;
		Ast* r = $3;
		Ast* ast = new Divide_Ast(l,r,get_line_number());
		// ast->check_ast();
		$$ = ast;
	}
	}
|
	'-' a_operand %prec UMINUS
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($2 != NULL), "Operand cannot be null");
		Ast* l = $2;
		Ast* ast = new UMinus_Ast(l,NULL,get_line_number());
		$$ = ast;
	}
	}
|
	'(' a_operand ')'
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($2 != NULL), "Operand cannot be null");
		$$ = $2;
	}
	}
|
	bool_expression '?' a_operand ':' a_operand
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Condition cannot be null");
		CHECK_INVARIANT(($3 != NULL), "Left Operand cannot be null");
		CHECK_INVARIANT(($5 != NULL), "Right Operand cannot be null");
		Ast* cond = $1;
		Ast* l = $3;
		Ast* r = $5;
		Ast* ast = new Conditional_Operator_Ast(cond,l,r,get_line_number());
		// ast->check_ast();
		$$ = ast;
	}
	}
|
	expression_term
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "expression_term cannot be null");
		$$ = $1;
	}
	}
;

bool_expression:
	b_operand AND b_operand
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Left Operand cannot be null");
		CHECK_INVARIANT(($3 != NULL), "Right Operand cannot be null");
		Ast* l = $1;
		Ast* r = $3;
		Boolean_Op op = boolean_and;
		Ast* ast = new Boolean_Expr_Ast(l,op,r,get_line_number());
		// ast->check_ast();
		$$ = ast;
	}
	}
|
	b_operand OR b_operand
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Left Operand cannot be null");
		CHECK_INVARIANT(($3 != NULL), "Right Operand cannot be null");
		Ast* l = $1;
		Ast* r = $3;
		Boolean_Op op = boolean_or;
		Ast* ast = new Boolean_Expr_Ast(l,op,r,get_line_number());
		// ast->check_ast();
		$$ = ast;
	}
	}
|
	NOT b_operand
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($2 != NULL), "Left Operand cannot be null");
		Ast* l = NULL;
		Ast* r = $2;
		Boolean_Op op = boolean_not;
		Ast* ast = new Boolean_Expr_Ast(l,op,r,get_line_number());
		// ast->check_ast();
		$$ = ast;
	}
	}
|
	'(' b_operand ')'
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($2 != NULL), "Bool Operand cannot be null");
		$$ = $2;
	}
	}
|
	rel_expression
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Bool Operand cannot be null");
		$$ = $1;
	}
	}
;


rel_expression:
	a_operand LT a_operand
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Left Operand cannot be null");
		CHECK_INVARIANT(($3 != NULL), "Right Operand cannot be null");
		Ast* l = $1;
		Ast* r = $3;
		Relational_Op op = less_than;
		Ast* ast = new Relational_Expr_Ast(l,op,r,get_line_number());
		// ast->check_ast();
		$$ = ast;
	}
	}
|
	a_operand GT a_operand
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Left Operand cannot be null");
		CHECK_INVARIANT(($3 != NULL), "Right Operand cannot be null");
		Ast* l = $1;
		Ast* r = $3;
		Relational_Op op = greater_than;
		Ast* ast = new Relational_Expr_Ast(l,op,r,get_line_number());
		// ast->check_ast();
		$$ = ast;
	}
	}
|
	a_operand LEQ a_operand
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Left Operand cannot be null");
		CHECK_INVARIANT(($3 != NULL), "Right Operand cannot be null");
		Ast* l = $1;
		Ast* r = $3;
		Relational_Op op = less_equalto;
		Ast* ast = new Relational_Expr_Ast(l,op,r,get_line_number());
		// ast->check_ast();
		$$ = ast;
	}
	}
|
	a_operand GEQ a_operand
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Left Operand cannot be null");
		CHECK_INVARIANT(($3 != NULL), "Right Operand cannot be null");
		Ast* l = $1;
		Ast* r = $3;
		Relational_Op op = greater_equalto;
		Ast* ast = new Relational_Expr_Ast(l,op,r,get_line_number());
		// ast->check_ast();
		$$ = ast;
	}
	}
|
	a_operand EQ a_operand
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Left Operand cannot be null");
		CHECK_INVARIANT(($3 != NULL), "Right Operand cannot be null");
		Ast* l = $1;
		Ast* r = $3;
		Relational_Op op = equalto;
		Ast* ast = new Relational_Expr_Ast(l,op,r,get_line_number());
		// ast->check_ast();
		$$ = ast;
	}
	}
|
	a_operand NEQ  a_operand
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Left Operand cannot be null");
		CHECK_INVARIANT(($3 != NULL), "Right Operand cannot be null");
		Ast* l = $1;
		Ast* r = $3;
		Relational_Op op = not_equalto;
		Ast* ast = new Relational_Expr_Ast(l,op,r,get_line_number());
		// ast->check_ast();
		$$ = ast;
	}
	}
;

a_operand:
	arith_expression
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Operand cannot be null");
		$$ = $1;
	}
	}
;

b_operand:
	bool_expression
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Operand cannot be null");
		$$ = $1;
	}
	}
;

// ############

expression_term: // MOD
	variable
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Variable cannot be null");
		$$ = $1;
	}
	}
|
	constant
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Constant cannot be null");
		$$ = $1;
	}
	}
;

function_call:
	NAME '(' optional_expression_list ')'
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "function_call name cannot be null");
		CHECK_INVARIANT(($3 != NULL), "optional_expression_list cannot be null");

		string proc_name = *$1;
		vector<Ast*> arg_vec = *$3;

		Procedure* p = program_object.find_proc(proc_name, arg_vec);

		CHECK_INPUT_AND_ABORT((current_procedure != NULL),
			"Cannot make procedure call outside procedure",get_line_number());
		CHECK_INPUT((current_procedure->variable_in_symbol_list_check(proc_name) == false),
			"Cannot call function " + proc_name + " here,"+
			+" a variable with the same name exists.", get_line_number());
		CHECK_INPUT_AND_ABORT((p != NULL),
			"Procedure corresponding to the name is not found", get_line_number());

		p->set_called(get_line_number());
		Ast* f_ast = new Function_Call_Ast(proc_name,arg_vec,p,get_line_number());
		$$ = f_ast;
	}
	}
;

optional_expression_list:
	// empty
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = new vector<Ast*>;
	}
	}
|
	expression_list
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "expression_list cannot be null");
		$$ = $1;
	}
	}
;

expression_list:
	arith_expression
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "arith_expression cannot be null");
		vector<Ast*>* v = new vector<Ast*>;
		v->push_back($1);
		$$ = v;
	}
	}
|
	expression_list ',' arith_expression
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "expression_list cannot be null");
		CHECK_INVARIANT(($3 != NULL), "arith_expression cannot be null");
		vector<Ast*>* v = $1;
		v->push_back($3);
		$$ = v;
	}
	}
;

// ############

variable:
	NAME
	{
	if (NOT_ONLY_PARSE)
	{
		Symbol_Table_Entry * var_table_entry;

		CHECK_INVARIANT(($1 != NULL), "Variable name cannot be null");

		if (current_procedure->variable_in_symbol_list_check(*$1))
			 var_table_entry = &(current_procedure->get_symbol_table_entry(*$1));

		else if (program_object.variable_in_symbol_list_check(*$1))
			var_table_entry = &(program_object.get_symbol_table_entry(*$1));

		else
			CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Variable has not been declared", get_line_number());


		$$ = new Name_Ast(*$1, *var_table_entry, get_line_number());

		delete $1;
	}
	}
;

constant:
	INTEGER_NUMBER
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		int numb = $1;
		Ast * numb_ast = new Number_Ast<int>(numb, int_data_type, get_line_number());
		$$ = numb_ast;
	}
	}
|
	DOUBLE_NUMBER
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		double numb = $1;
		Ast * numb_ast = new Number_Ast<double>(numb, double_data_type, get_line_number());
		$$ = numb_ast;
	}
	}
;

optional_initialization_list:
	// empty
	{
	if (NOT_ONLY_PARSE)
	{
		Sequence_Ast* init_list = new Sequence_Ast(get_line_number());
		$$ = init_list;
	}
	}
|
	initialization_list
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "initialization_list cannot be null");
		Sequence_Ast* init_list = $1;
		$$ = init_list;
	}
	}
;

initialization_list:
	initialization
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "initialization cannot be null");
		Sequence_Ast* init_list = new Sequence_Ast(get_line_number());
		Ast* entry = $1;
		init_list->ast_push_back(entry);
		$$ = init_list;
	}
	}
|
	initialization ',' initialization_list
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "initialization cannot be null");
		CHECK_INVARIANT(($3 != NULL), "initialization_list cannot be null");
		Sequence_Ast* init_list = $3;
		Ast* entry = $1;
		init_list->ast_push_back(entry);
		$$ = init_list;
	}
	}
;

initialization:
	variable ASSIGN arith_expression
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($1 != NULL, "variable cannot be NULL");
		CHECK_INVARIANT($3 != NULL, "arith_expression cannot be NULL");
		Ast* var = $1;
		Ast* exp = $3;

		Ast* ass = new Assignment_Ast(var, exp, get_line_number());

		$$ = ass;
	}
	}
;

for_loop_statement:
	FOR '(' optional_initialization_list ';'
	        bool_expression ';'
	        optional_initialization_list ')'
		statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT($3 != NULL, "optional_initialization_list cannot be NULL");
		CHECK_INVARIANT($5 != NULL, "bool_expression cannot be NULL");
		CHECK_INVARIANT($7 != NULL, "optional_initialization_list cannot be NULL");
		CHECK_INVARIANT($9 != NULL, "statement cannot be NULL");
		Ast* init = $3;
		Ast* cond = $5;
		Ast* incr = $7;
		Ast* body = $9;

		Sequence_Ast* ret = new Sequence_Ast(get_line_number());
		Sequence_Ast* new_body = new  Sequence_Ast(get_line_number());
		new_body->ast_push_back(body);
		new_body->ast_push_back(incr);
		Ast* loop = new Iteration_Statement_Ast(cond,new_body,get_line_number(),false);

		ret->ast_push_back(init);
		ret->ast_push_back(loop);

		$$ = (Ast*)ret;
	}
	}
;

goto_statement:
	GOTO NAME ';'
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT($2 != NULL, "Label cannot be NULL");
		Ast* stmt = new Goto_Label_Ast("usr_"+*$2,get_line_number());
		$$ = stmt;
		delete $2;
	}
	}
;

label_statement:
	NAME ':'
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT($1 != NULL, "Label cannot be NULL");
		Ast* stmt = new Label_Ast("usr_"+*$1,get_line_number());
		$$ = stmt;
		delete $1;
	}
	}
;

break_statement:
	BREAK ';'
	{
	if (NOT_ONLY_PARSE)
	{
		Ast* stmt = new Break_Ast(get_line_number());
		$$ = stmt;
	}
	}
;

switch_statement:
	SWITCH '(' arith_expression ')' '{'
	optional_csl
	optional_dsl
	'}'
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT($3 != NULL, "arith_expression cannot be NULL");
		CHECK_INVARIANT($6 != NULL, "optional_csl cannot be NULL");
		CHECK_INVARIANT($7 != NULL, "optional_dsl cannot be NULL");
		Ast* expr = $3;
		pair< vector<Ast*>, vector<Ast*> >* cas = $6;
		Sequence_Ast* def = $7;
		cas->second.push_back((Ast*)def);
		Ast* stmt = new Switch_Statement_Ast(expr,cas->first,cas->second,get_line_number());
		$$ = stmt;
	}
	}
;

optional_csl:
	csl
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT($1 != NULL, "csl cannot be NULL");
		$$ = $1;
	}
	}
|
	{
	if (NOT_ONLY_PARSE)
	{
		pair< vector<Ast*>, vector<Ast*> >* p = new pair<vector<Ast*>, vector<Ast*> >();
		$$ = p;
	}
	}
;

csl:
	CASE constant ':' statement_list
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT($2 != NULL, "constant cannot be NULL");
		CHECK_INVARIANT($4 != NULL, "statement_list cannot be NULL");
		pair< vector<Ast*>, vector<Ast*> >* p = new pair<vector<Ast*>, vector<Ast*> >();
		p->first.push_back((Ast*)$2);
		p->second.push_back((Ast*)$4);
		$$ = p;
	}
	}
|
	csl
	CASE constant ':' statement_list
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT($1 != NULL, "csl cannot be NULL");
		CHECK_INVARIANT($3 != NULL, "constant cannot be NULL");
		CHECK_INVARIANT($5 != NULL, "statement_list cannot be NULL");
		pair< vector<Ast*>, vector<Ast*> >* p = $1;
		p->first.push_back((Ast*)$3);
		p->second.push_back((Ast*)$5);
		$$ = p;
	}
	}
;

optional_dsl:
	DEFAULT ':' statement_list
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT($3 != NULL, "statement_list cannot be NULL");
		$$ = $3;
	}
	}
|
	{
	if (NOT_ONLY_PARSE)
	{
		Sequence_Ast* stmt = new Sequence_Ast(get_line_number());
		$$ = stmt;
	}
	}
;

// #######################

optional_argument_list:
	// empty
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = new vector<pair<Data_Type,string> >;
	}
	}
|
	argument_list
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT($1 != NULL, "argument_list cannot be NULL");
		$$ = $1;
	}
	}
;

argument_list:
	argument
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT($1 != NULL, "argument cannot be NULL");
		vector<pair<Data_Type,string> >* v = new vector<pair<Data_Type,string> >;
		v->push_back(*$1);
		delete $1;
		$$ = v;
	}
	}
|
	argument_list ',' argument
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT($1 != NULL, "argument_list cannot be NULL");
		CHECK_INVARIANT($3 != NULL, "argument cannot be NULL");
		vector<pair<Data_Type,string> >* v = $1;
		v->push_back(*$3);
		delete $3;
		$$ = v;
	}
	}
;

argument:
	INTEGER	NAME
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT($2 != NULL, "name cannot be NULL");
		pair<Data_Type,string>* p = new pair<Data_Type,string>;
		p->first = int_data_type;
		p->second = *$2;
		delete $2;
		$$ = p;
	}
	}
|
	FLOAT	NAME
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT($2 != NULL, "name cannot be NULL");
		pair<Data_Type,string>* p = new pair<Data_Type,string>;
		p->first = double_data_type;
		p->second = *$2;
		delete $2;
		$$ = p;
	}
	}
;
