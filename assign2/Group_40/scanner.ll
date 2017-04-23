%filenames="scanner"
%lex-source="scanner.cc"

digit [0-9]
op [-+*/]
char [a-zA-Z_]
meta [{}();]
integer [0-9]+
double ([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+)

%%

int 	{
		store_token_name("INTEGER");
		return Parser::INTEGER;
	}

float 	{
		store_token_name("FLOAT");
		return Parser::FLOAT;
	}

void 	{
		store_token_name("VOID");
		return Parser::VOID;
	}

= 	{
		store_token_name("ASSIGN_OP");
		return Parser::ASSIGN;
	}

{integer} 	{
		ParserBase::STYPE__ *val = getSval();
        val->integer_value = atoi(matched().c_str());
		store_token_name("NUM");
		return Parser::INTEGER_NUMBER;
	}

({double}|{integer})([Ee][+-]?{digit}+)?	{
		ParserBase::STYPE__ *val = getSval();
        val->float_value = atof(matched().c_str());
		store_token_name("FNUM");
		return Parser::DOUBLE_NUMBER;
	}

{char}({digit}|{char}|_)* 	{
		ParserBase::STYPE__ *val = getSval();
		val->string_value = new string(matched());
		store_token_name("NAME");
		return Parser::NAME;
	}

{meta} 	{
		store_token_name("META CHAR");
		return matched()[0];
	}

{op}	{
		store_token_name("ARITHOP");
		return matched()[0];
	}

\n    			|
";;".*  		|
[ \t]*";;".*	|
[ \t]*"//".*	|
[ \t] 	{
		if (command_options.is_show_tokens_selected())
			ignore_token();
	}

. 	{
		string error_message;
		error_message =  "Illegal character `" + matched();
		error_message += "' on line " + lineNr();

		CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, error_message, lineNr());
	}
