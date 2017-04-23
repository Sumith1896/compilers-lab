

#ifndef Scanner_H_INCLUDED_
#define Scanner_H_INCLUDED_

/////////////////// Header files of cfglp ///////////////////////////////

#include<cstdlib>
#include<iostream>
#include<string>
#include<fstream>
#include<string.h>

using namespace std;

#include"common-classes.hh"
#include"local-environment.hh"
#include"user-options.hh"
#include"error-display.hh"
#include"icode.hh"
#include"reg-alloc.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"sequence-ast.hh"
#include"procedure.hh"
#include"program.hh"

///////////////////////////////////////////////////////////////////////

#include "parserbase.h"
#include "scannerbase.h"

class Scanner: public ScannerBase
{
    public:
        explicit Scanner(std::istream &in = std::cin,
                                std::ostream &out = std::cout);

        Scanner(std::string const &infile, std::string const &outfile);
        
        int lex();

    private:
        int lex__();
        int executeAction__(size_t ruleNr);
        void postCode(PostEnum__ type);
        void print();
        void preCode(); 

/////////////////// Functions for scanner.ll //////////////////////////////

private:
	string token_name;
	ParserBase::STYPE__ * dval;

public:
	void store_token_name(string name);
	void display_token(int token_code);
	void ignore_token();

	int get_scan_line_number();						// Used for errors

	void setSval(ParserBase::STYPE__ * val);
	ParserBase::STYPE__ * getSval();

//////////////////////////////////////////////////////////////////////////
};

inline Scanner::Scanner(std::istream &in, std::ostream &out)
:
    ScannerBase(in, out)
{
}

inline Scanner::Scanner(std::string const &infile, std::string const &outfile)
:
    ScannerBase(infile, outfile)
{
}

inline int Scanner::lex()
{
    return lex__();
}

inline void Scanner::preCode() 
{
////////////////////////// Command options for cfglp ////////////////////////
#ifndef TOKENS
#define TOKENS

	if (command_options.is_show_tokens_selected())
		command_options.create_tokens_buffer();

#endif
////////////////////////////////////////////////////////////////////////////
}

inline void Scanner::postCode(PostEnum__ type)
{
	
}

inline void Scanner::print() 
{
    print__();
}

/////////////////////////// Functions for scanner.ll ///////////////////////

inline void Scanner::store_token_name(string name)
{
	token_name = name;
}

inline void Scanner::display_token(int token_code)
{
	ostream & file_buffer = command_options.get_tokens_buffer();
	
	file_buffer << "Line: " << lineNr() << " ";
	file_buffer << "\tToken Name: " << token_name;
	file_buffer << " \tToken: " << matched();
	file_buffer << " \tToken Code: " << token_code << "\n";
}

inline void Scanner::ignore_token()
{
	if (command_options.is_show_tokens_selected())
	{
		if (matched()[0] == '\n')
		{
			ostream & file_buffer = command_options.get_tokens_buffer();
			file_buffer << "Line: "<< lineNr() << " ";
			file_buffer << "\tIgnored NEWLINE character\n";
		}
		else
		{
			ostream & buffer = command_options.get_tokens_buffer();

			buffer << "Line: " << lineNr() << " ";
			buffer << "\tIgnored lexeme: '" << matched() << "'\n";
		}
	}
}

inline void Scanner::setSval(ParserBase::STYPE__ * val)
{
	dval = val;
}

inline int Scanner::get_scan_line_number()
{
	return lineNr();
}

inline ParserBase::STYPE__ * Scanner::getSval()
{
	return dval;
}

///////////////////////////////////////////////////////////////////////////

#endif

