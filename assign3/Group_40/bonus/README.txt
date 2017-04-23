We have added support for FOR LOOPS with the following grammar:


	optional_initialization_list:
		\*empty*\
	|
		initialization_list
	;

	initialization_list:
		initialization
	|
		initialization ',' initialization_list
	;

	initialization:
		variable ASSIGN arith_expression
	;

	for_loop:
		FOR '(' optional_initialization_list ';' 
		        bool_expression ';'
		        optional_initialization_list ')'
		statement

The testcases are given in the for_testcases folder