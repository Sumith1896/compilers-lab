%filenames="scanner"
%lex-source="scanner.cc"

digit [0-9]
op [-+*/]
char [a-zA-Z_]
meta [{}();,?:\[\]]
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

if 	{
		store_token_name("IF");
		return Parser::IF;
	}

else 	{
		store_token_name("ELSE");
		return Parser::ELSE;
	}

do 	{
		store_token_name("DO");
		return Parser::DO;
	}

while 	{
		store_token_name("WHILE");
		return Parser::WHILE;
	}

for 	{
		store_token_name("FOR");
		return Parser::FOR;
	}

goto 	{
		store_token_name("GOTO");
		return Parser::GOTO;
	}

break 	{
		store_token_name("BREAK");
		return Parser::BREAK;
	}

switch 	{
		store_token_name("SWITCH");
		return Parser::SWITCH;
	}

case 	{
		store_token_name("CASE");
		return Parser::CASE;
	}

default	{
		store_token_name("DEFAULT");
		return Parser::DEFAULT;
	}

return	{
		store_token_name("RETURN");
		return Parser::RETURN;
	}

print	{
		store_token_name("PRINT");
		return Parser::PRINT;
	}

"<="	{
		store_token_name("LEQ");
		return Parser::LEQ;	
}

">="	{
		store_token_name("GEQ");
		return Parser::GEQ;	
}

"<"		{
		store_token_name("LT");
		return Parser::LT;	
}

">"		{
		store_token_name("GT");
		return Parser::GT;	
}

"!="	{
		store_token_name("NEQ");
		return Parser::NEQ;	
}

"=="	{
		store_token_name("EQ");
		return Parser::EQ;	
}

"="		{
		store_token_name("ASSIGN_OP");
		return Parser::ASSIGN;
}

"&&"	{
		store_token_name("AND");
		return Parser::AND;	
}

"||"	{
		store_token_name("OR");
		return Parser::OR;	
}

"!"		{
		store_token_name("NOT");
		return Parser::NOT;	
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

\"[^\"]+\"	{
		ParserBase::STYPE__ *val = getSval();
		val->string_value = new string(matched());
		store_token_name("STRING");
		return Parser::STRING;
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
