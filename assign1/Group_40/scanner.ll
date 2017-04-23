%filenames="scanner"
%lex-source="scanner.cc"

digit [0-9]
op [-+*/]
char [a-zA-Z_]
braces [{}\(\);]

%%

int 	{
		return Parser::INTEGER;
	}

void 	{
		return Parser::VOID;
	}

return 	{
		return Parser::RETURN;
	}

= 	{
		return Parser::ASSIGN;
	}

-?{digit}+ 	{
		ParserBase::STYPE__ *val = getSval();
        val->integer_value = atoi(matched().c_str());
		return Parser::INTEGER_NUMBER;
	}

{char}[0-9a-zA-Z_]* 	{
		ParserBase::STYPE__ *val = getSval();
		val->string_value = new string(matched());
		return Parser::NAME;
	}

{braces} 	{
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
