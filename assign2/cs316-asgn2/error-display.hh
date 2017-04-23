
/*********************************************************************************************

                                cfglp : A CFG Language Processor
                                --------------------------------

           About:

           Implemented by Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs306: Language  Processors (theory and lab)  at IIT
           Bombay.

           Release  date Jan  15, 2013.  Copyrights reserved  by Uday
           Khedker. This implemenation has been made available purely
           for academic purposes without any warranty of any kind.

           A  doxygen   generated  documentation  can  be   found  at
           http://www.cse.iitb.ac.in/~uday/cfglp


***********************************************************************************************/

#ifndef ERROR_DISPLAY_HH
#define ERROR_DISPLAY_HH

#include <sstream>
#include <string>

#define NO_FILE_LINE -1

#define CONTROL_SHOULD_NOT_REACH false

using namespace std;

bool error_status();
void report_violation_of_condition(bool condition, string s, int lineno);
void report_violation_and_abort(bool condition, string s, int lineno);
void check_invariant_underlying_function(bool condition, string error_message);

#define CHECK_INVARIANT(x,y) \
     {  stringstream ___new___string___; \
        ___new___string___ << y << " (Invariant at line " << __LINE__ <<  ", file " << __FILE__ << ").\n"; \
        check_invariant_underlying_function(x, ___new___string___.str()); \
     }

#define CHECK_INPUT(condition, error_message, lineno)							\
	report_violation_of_condition(condition, error_message, lineno);

#define CHECK_INPUT_AND_ABORT(condition, error_message, lineno)							\
	report_violation_and_abort(condition, error_message, lineno);

#endif
