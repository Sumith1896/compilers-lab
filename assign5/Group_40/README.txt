Building the executable
=======================
Run command `make` to compile and generate executable `sclp`

Generating intermediate code
============================
./sclp -d <input-file>
Prints out the unoptimized and optimized intermediate code as well as the assemby code.

./sclp -icode <input-file>
In addition to the previously generated files also generates a file with '.dce' extension which contains the icode with dead code elimination.

Explanations:
We have created struct CFG and Basic_Block in files 'cfg.hh' and 'cfg.cc'
The control flow graph is constructed from the icode_list (sa_icode_list in Sequence_Ast)
On calling eliminate_dead_code function:
	We do repeated iteration of the following process till convergence
	- Generate Gen and Kill Set for each node
	- Generate In and Out Set for each node using Gen and Kill, and the set of global variables
	- Eliminate dead code lines from each block. To do this start from the bottom, move to the top while maintaining a live variables set
Update the sa_icode_list in Sequence_Ast

Points to note
==============
Strong liveness: Please note that we are uing strong liveness over simple liveness, in our basic submission also. This is because dead code elimination, using both simple and strong liveness gives the same result. The only difference being strong liveness is more efficient and requires lesser number of iterations.

Global Variables: We have handled global variables also. We assume that as we are doing a local analysis of the procedure, the global variables are live whenever the prcedure returns. This is taken care of by adding all global variable to the out set of the final block (which happens to be the only block which returns).

