#ifndef REG_ALLOC_HH
#define REG_ALLOC_HH

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

/////////////////////////////// Register_Descriptor ////////////////////

typedef enum
{
	none,	/* dummy to indicate no register */
	zero,	/* constant register */
	v0,	/* expression result register */
	v1,	/* function result register */
	a0,	/* argument register */
	a1, a2, a3,
	t0,      /* temporary caller-save registers */
	t1, t2, t3, t4, t5, t6, t7, t8, t9, 
	s0,	/* temporary callee-save registers */ 
	s1, s2, s3, s4, s5, s6, s7,
	mfc,	/* float register to int register */
	mtc,	/* int register to float register */
	f0, 	/* floating point registers */
	f2, f4, f6, f8,
	f10, f12, f14, f16, f18,
	f20, f22, f24, f26, f28, f30,
	gp,	/* global data pointer register */
	sp,	/* stack pointer register */
	fp,	/* frame pointer register */
	ra	/* return address register */
} Spim_Register;

typedef enum 
{ 
	int_num,
	float_num
} Register_Val_Type;

typedef enum 
{ 
	fixed_reg, 
	gp_data, 
	fn_result, 
	argument,
	pointer, 
	ret_address,
	float_reg
} Register_Use_Category;

class Register_Descriptor
{
    Spim_Register reg_id;
    string reg_name;
    Register_Val_Type value_type;
    Register_Use_Category reg_use; 

    list<Symbol_Table_Entry *> lra_symbol_list;
    bool used_for_expr_result;
    bool reg_occupied;
    bool used_for_fn_return;

  public:
    Register_Descriptor (Spim_Register reg, string nam, Register_Val_Type vt, Register_Use_Category uc);
    Register_Descriptor() {}
    ~Register_Descriptor() {}

    bool is_symbol_list_empty();
    void update_symbol_information(Symbol_Table_Entry & symbol_entry);

    int count_symbol_entry_in_list();

    bool find_symbol_entry_in_list(Symbol_Table_Entry & symbol_entry);
    void remove_symbol_entry_from_list(Symbol_Table_Entry & symbol_entry);

    Register_Use_Category get_use_category(); 
    Spim_Register get_register();

    string get_name();
    void clear_lra_symbol_list();

    bool is_register_occupied();
    void set_register_occupied();
    void reset_register_occupied();

    bool is_used_for_fn_return();
    void set_used_for_fn_return();
    void reset_used_for_fn_return();

    bool is_used_for_expr_result();
    void set_use_for_expr_result();
    void reset_use_for_expr_result();

    template <Register_Use_Category dt>
    bool is_free();
};

///////////////////////////// Machine Description ////////////////////////////////

class Machine_Description
{
public:
	map<Tgt_Op, Instruction_Descriptor *> spim_instruction_table;
	map<Spim_Register, Register_Descriptor *> spim_register_table;

	void initialize_instruction_table();
	void initialize_register_table();

	void validate_init_local_register_mapping_before_fn_call();
	void validate_init_local_register_mapping();
	void clear_local_register_mappings();

	void clear_reg_not_used_for_expr_result();

	template <Register_Use_Category dt>
	int count_free_register();

	template <Register_Use_Category dt>
	Register_Descriptor * get_new_register();
};

extern Machine_Description machine_desc_object;

#endif
