We had added support for FOR loops, GOTO statements and BREAK statements previously.

SWITCH statement
================
We added support for SWITCH statements in this assignment.
- We have defined a new Ast class called Switch_Ast. The grammar allowed is 

switch_statement:
	SWITCH '(' arith_expression ')' '{'
	optional_csl
	optional_dsl
	'}'

where optional_dsl is a optional default statement and optional_csl is an optional list of case statements.
This implementation uses a jump table to implement different cases and handle fall-throughs elegantly.
The implementation of compile is present in ast-compile.cc and is self explanatory (very similar to the BREAK statement explained in the last assignment). The testcases are present in switch_testcases/

Additionally we have also used strong liveness instead of simple liveness which saves on the number of iterations and is more efficient.
