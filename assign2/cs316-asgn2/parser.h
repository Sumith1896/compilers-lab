

#ifndef Parser_h_included
#define Parser_h_included

#include "parserbase.h"
#include "scanner.h"

//////////////////////////// Parser Initial Code ///////////////////////////

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "local-environment.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "sequence-ast.hh"
#include "procedure.hh"
#include "program.hh"

////////////////////////////////////////////////////////////////////////////

#undef Parser
class Parser: public ParserBase
{
  Scanner d_scanner;

  public:
    Parser(string input_file_name)
    {
      d_scanner.switchStreams(input_file_name, "");
      d_scanner.setSval(&d_val__);

      NOT_ONLY_PARSE = command_options.not_only_parse;
    }

    int parse();
    void print();

    int get_line_number();          // Used for errors

    bool NOT_ONLY_PARSE;

  private:
    void error(char const *msg);
    int lex();

    void executeAction(int ruleNr);
    void errorRecovery();
    int lookup(bool recovery);
    void nextToken();
    void print__();
    void exceptionHandler__(std::exception const &exc)
    {
      std::cout << exc.what() << '\n';
      ++d_nErrors__;
    }
};


#endif
