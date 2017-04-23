#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <string>

using namespace std;

#include "common-classes.hh"

#include "user-options.hh"
#include "error-display.hh"

void check_invariant_underlying_function(bool condition, string error_message)
{
	if (!condition)
	{
		cerr << "\nsclp internal error: " << error_message << "\n";

		command_options.remove_files();
		exit(1);
	}
}

bool global_error_status = false;

void report_violation_of_condition(bool condition, string error_message, int lineno)
{
	string file_name = command_options.get_file_name();
	if (!condition)
	{
		cerr <<  "sclp error: File: " << file_name << ", Line: " << lineno << ": " << error_message << "\n";
		global_error_status = true;
	}
} 

void report_violation_and_abort(bool condition, string error_message, int lineno)
{
	string file_name = command_options.get_file_name();
	if (!condition)
	{
		cerr <<  "sclp error: File: " << file_name << ", Line: " << lineno << ": " << error_message << "\n";
		exit(1);
	}
} 

bool error_status()
{
	return global_error_status;
}
