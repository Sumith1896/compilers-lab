#include <set>
#include <vector>
#include <map>
#include "icode.hh"
#include "reg-alloc.hh"

using namespace std;

// #define DEBUG


struct classcomp {
  bool operator() (Ics_Opd* const &a, Ics_Opd* const &b) const;
};
typedef set<Ics_Opd*,classcomp> ELEMENT_SET;

void print_set(ELEMENT_SET s, ostream & file_buffer);

class Basic_Block
{
public:
	list<Icode_Stmt *> bb_icode_list;
	int block_number;

	Basic_Block *l, *r;
	ELEMENT_SET gen, killl, in, out;
	Basic_Block();
	~Basic_Block();

	static int total_blocks;
	int get_block_number();
	void push_stmt(Icode_Stmt *curr_stmt);
	void set_children(Basic_Block *left, Basic_Block *right);
	void print(ostream & file_buffer);
	void insert_gen(Ics_Opd * opd);
	void insert_kill(Ics_Opd * opd);
	void recompute_gen_kill();
	bool eliminate_dead_code();
};

class CFG
{
	Basic_Block* root;
	map <string, Basic_Block* > l_blocks;
	vector <Basic_Block*> all_blocks;
	ELEMENT_SET global_elem;

public:
	CFG(Basic_Block* r);
	~CFG();

	Basic_Block* get_block(string index);
	Basic_Block* get_final_block();
	void save_block(Basic_Block* b);
	void print(ostream & file_buffer);
	void compute_in_out();
	bool eliminate_dead_code();
	list<Icode_Stmt *> get_icode_list();
	void set_global_set(ELEMENT_SET &s);
	void reset_global_set();
	ELEMENT_SET get_global_set();
};
