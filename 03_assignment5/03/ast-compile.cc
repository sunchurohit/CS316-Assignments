// #include "icode.hh"
// #include "reg-alloc.hh"
#include<string>
#include<iostream>
#include<iomanip>
#include<typeinfo>
#include<list>
using namespace std ; 
template class Number_Ast<double>;
template class Number_Ast<int>;

//// Ast  class
Code_For_Ast & Ast::compile()
{

}

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register)
{

}

//// Assignment Class

Code_For_Ast & Assignment_Ast::compile()
{
	// printf("enter Assignment_Ast\n");
	if(get_data_type() == int_data_type)
	{
		Code_For_Ast R = rhs -> compile() ; 
		Register_Descriptor* Reg = R.get_reg() ; 
		Code_For_Ast S = lhs -> create_store_stmt(Reg) ;  // after which the register can be freed.
		Reg -> reset_use_for_expr_result() ; 
		list<Icode_Stmt *> L = S.get_icode_list() ; 
		list<Icode_Stmt *> Rh = R.get_icode_list() ;
		Rh.merge(L) ; 
		for(list<Icode_Stmt *>:: iterator i = L.begin() ; i != L.end() ; i++)
		{
			Rh.push_back(*i) ; 
		}
		Code_For_Ast* C = new Code_For_Ast( Rh, Reg ) ;
		return *C ; 
	}
	else
	{
		Code_For_Ast R = rhs -> compile() ; 
		Mem_Addr_Opd* Mem = new Mem_Addr_Opd(lhs->get_symbol_entry()) ;
		Register_Addr_Opd* Reg = new Register_Addr_Opd(R.get_reg()) ; 
		Move_IC_Stmt * M = new Move_IC_Stmt(store_d , Reg , Mem) ;// register , address 
		R.get_reg() -> reset_use_for_expr_result()  ; // free the register
		Code_For_Ast* C = new Code_For_Ast(R.get_icode_list(), R.get_reg()) ; 
		C -> set_reg(R.get_reg());
		C -> append_ics(*M) ; 
		return *C ; 
	}

}
Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
}

//// Name_Ast Class

Code_For_Ast & Name_Ast::compile()
{
	Code_For_Ast* C = new Code_For_Ast() ;
	if(get_data_type() == int_data_type)
	{
		Register_Descriptor* R = machine_desc_object.get_new_register<int_reg>() ; // get new register
		Register_Addr_Opd* Reg = new Register_Addr_Opd(R) ; 
		Mem_Addr_Opd* Mem = new Mem_Addr_Opd(*variable_symbol_entry) ; 
		Move_IC_Stmt* Mov = new Move_IC_Stmt(load, Mem, Reg) ;	/// register , address
		C -> set_reg(R) ; 
		C -> append_ics(*Mov) ; 
		return *C ; 
	}
	else
	{
		Register_Descriptor* R = machine_desc_object.get_new_register<float_reg>() ; // get new register
		Register_Addr_Opd* Reg = new Register_Addr_Opd(R) ; 
		Mem_Addr_Opd* Mem = new Mem_Addr_Opd(*variable_symbol_entry) ; 
		Move_IC_Stmt* Mov = new Move_IC_Stmt(load_d, Mem, Reg) ;	/// register , address	
		C -> set_reg(R) ; 
		C -> append_ics(*Mov) ; 
		return *C ; 
	}

}
Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
}
Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register)
{
	Code_For_Ast* C = new Code_For_Ast() ;
	if(get_data_type() == int_data_type)
	{
		Register_Addr_Opd* Reg = new Register_Addr_Opd(store_register) ; 
		Mem_Addr_Opd* Mem = new Mem_Addr_Opd(*variable_symbol_entry) ; 
		Move_IC_Stmt* Mov = new Move_IC_Stmt(store, Reg, Mem) ;	/// the way, opd1, result
		C -> set_reg(store_register) ; 
		C -> append_ics(*Mov) ; 
			
	}
	else
	{
		Register_Addr_Opd* Reg = new Register_Addr_Opd(store_register) ; 
		Mem_Addr_Opd* Mem = new Mem_Addr_Opd(*variable_symbol_entry) ; 
		Move_IC_Stmt* Mov = new Move_IC_Stmt(store_d, Reg, Mem) ;	/// register , address	
		C -> set_reg(store_register) ; 
		C -> append_ics(*Mov) ; 
	}
	return *C ; 
}
// Number_Ast Class

template<>
Code_For_Ast & Number_Ast<int>::compile()
{
	// cout << this -> constant << endl ; 
	Register_Descriptor* R = machine_desc_object.get_new_register<int_reg>() ; // get new register
	Const_Opd<int>*  Val = new Const_Opd<int>(this -> constant) ;  
	Register_Addr_Opd* Reg = new Register_Addr_Opd(R) ; 
	Move_IC_Stmt* M = new Move_IC_Stmt(imm_load, Val, Reg) ; // here, result is Reg.
	Code_For_Ast* C = new Code_For_Ast() ;
	C -> set_reg(R) ; 
	C -> append_ics(*M) ; 
	return *C ;  
	
}
template<>
Code_For_Ast & Number_Ast<double>::compile()
{
	Register_Descriptor* R = machine_desc_object.get_new_register<float_reg>() ; // get new register
	Const_Opd<double>*  Val = new Const_Opd<double>(this -> constant) ;  
	Register_Addr_Opd* Reg = new Register_Addr_Opd(R) ; 
	Move_IC_Stmt* M = new Move_IC_Stmt(imm_load_d, Val,Reg) ; // result is val.
	Code_For_Ast* C = new Code_For_Ast() ;
	C -> set_reg(R) ; 
	C -> append_ics(*M) ; 
	return *C ;  
}

template<class T>
Code_For_Ast & Number_Ast<T>::compile_and_optimize_ast(Lra_Outcome & lra)
{

}
//// Arithmetic_Ast Class

Code_For_Ast & Arithmetic_Expr_Ast::compile() 
{

}


///// Plus_Ast
Code_For_Ast & Plus_Ast::compile()
{
	// get both of them.
	Code_For_Ast L = lhs -> compile() ; 
	Code_For_Ast R = rhs -> compile() ;
	list<Icode_Stmt *> S = L.get_icode_list() ;
	list<Icode_Stmt *> Rh = R.get_icode_list() ; 
	for(list<Icode_Stmt *>:: iterator i = Rh.begin() ; i != Rh.end() ; i++)
		{
			// printf("%d\n", 1 );
			S.push_back(*i) ; 
		}
	// add the addition ast statement.
	if(get_data_type() == int_data_type)
	{
		Register_Descriptor* Reg = machine_desc_object.get_new_register<int_reg>() ; // get new register		
		Register_Addr_Opd* Reg1 = new Register_Addr_Opd(L.get_reg()) ; 
		Register_Addr_Opd* Reg2 = new Register_Addr_Opd(R.get_reg()) ; 
		Register_Addr_Opd* result = new Register_Addr_Opd(Reg) ; 		
		Compute_IC_Stmt * Add_statment = new Compute_IC_Stmt(add, Reg1, Reg2, result );// this is the statement
		S.push_back(Add_statment) ; // add the statment.
		// clear the children's registers
		L.get_reg() -> reset_use_for_expr_result() ; 
		R.get_reg() -> reset_use_for_expr_result() ; 
	
		Code_For_Ast* C = new Code_For_Ast(S, Reg) ; 
		return *C ;  
	}
	else
	{
		Register_Descriptor* Reg = machine_desc_object.get_new_register<float_reg>() ; // get new register		
		Register_Addr_Opd* Reg1 = new Register_Addr_Opd(L.get_reg()) ; 
		Register_Addr_Opd* Reg2 = new Register_Addr_Opd(R.get_reg()) ; 
		Register_Addr_Opd* result = new Register_Addr_Opd(Reg) ; 			
		Compute_IC_Stmt * Add_statment = new Compute_IC_Stmt(add_d, Reg1, Reg2, result);// this is the statement
		S.push_back(Add_statment) ; // add the statment..
		// clear the children's registers
		L.get_reg() -> reset_use_for_expr_result() ; 
		R.get_reg() -> reset_use_for_expr_result() ; 

		Code_For_Ast* C = new Code_For_Ast(S , Reg) ; 
		return *C ;	
	}


}

Code_For_Ast & Plus_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{

}


/////// Minus_Ast

Code_For_Ast & Minus_Ast::compile()
{
	// get both of them.
	Code_For_Ast L = lhs -> compile() ; 
	Code_For_Ast R = rhs -> compile() ;
	list<Icode_Stmt *> S = L.get_icode_list() ;
	list<Icode_Stmt *> Rh = R.get_icode_list() ; 
	for(list<Icode_Stmt *>:: iterator i = Rh.begin() ; i != Rh.end() ; i++)
		{
			// printf("%d\n", 1 );
			S.push_back(*i) ; 
		}
	// add the addition ast statement.
	if(get_data_type() == int_data_type)
	{
		Register_Descriptor* Reg = machine_desc_object.get_new_register<int_reg>() ; // get new register		
		Register_Addr_Opd* Reg1 = new Register_Addr_Opd(L.get_reg()) ; 
		Register_Addr_Opd* Reg2 = new Register_Addr_Opd(R.get_reg()) ; 
		Register_Addr_Opd* result = new Register_Addr_Opd(Reg) ; 		
		Compute_IC_Stmt * Add_statment = new Compute_IC_Stmt(sub, Reg1, Reg2, result );// this is the statement
		S.push_back(Add_statment) ; // add the statment.
		// clear the children's registers
		L.get_reg() -> reset_use_for_expr_result() ; 
		R.get_reg() -> reset_use_for_expr_result() ; 
		
		Code_For_Ast* C = new Code_For_Ast(S, Reg) ; 
		return *C ;  
	}
	else
	{
		Register_Descriptor* Reg = machine_desc_object.get_new_register<float_reg>() ; // get new register		
		Register_Addr_Opd* Reg1 = new Register_Addr_Opd(L.get_reg()) ; 
		Register_Addr_Opd* Reg2 = new Register_Addr_Opd(R.get_reg()) ; 
		Register_Addr_Opd* result = new Register_Addr_Opd(Reg) ; 			
		Compute_IC_Stmt * Add_statment = new Compute_IC_Stmt(sub_d, Reg1, Reg2, result);// this is the statement
		S.push_back(Add_statment) ; // add the statment..
		// clear the children's registers
		L.get_reg() -> reset_use_for_expr_result() ; 
		R.get_reg() -> reset_use_for_expr_result() ; 
		
		Code_For_Ast* C = new Code_For_Ast(S , Reg) ; 
		return *C ;	
	}

}

Code_For_Ast & Minus_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{

}

///// Divide_Ast

Code_For_Ast & Divide_Ast::compile()
{
	// get both of them.
	Code_For_Ast L = lhs -> compile() ; 
	Code_For_Ast R = rhs -> compile() ;
	list<Icode_Stmt *> S = L.get_icode_list() ;
	list<Icode_Stmt *> Rh = R.get_icode_list() ; 
	for(list<Icode_Stmt *>:: iterator i = Rh.begin() ; i != Rh.end() ; i++)
		{
			// printf("%d\n", 1 );
			S.push_back(*i) ; 
		}
	// add the addition ast statement.
	if(get_data_type() == int_data_type)
	{
		Register_Descriptor* Reg = machine_desc_object.get_new_register<int_reg>() ; // get new register		
		Register_Addr_Opd* Reg1 = new Register_Addr_Opd(L.get_reg()) ; 
		Register_Addr_Opd* Reg2 = new Register_Addr_Opd(R.get_reg()) ; 
		Register_Addr_Opd* result = new Register_Addr_Opd(Reg) ; 		
		Compute_IC_Stmt * Add_statment = new Compute_IC_Stmt(divd, Reg1, Reg2, result );// this is the statement
		S.push_back(Add_statment) ; // add the statment.
		// clear the children's registers
		L.get_reg() -> reset_use_for_expr_result() ; 
		R.get_reg() -> reset_use_for_expr_result() ; 

		Code_For_Ast* C = new Code_For_Ast(S, Reg) ; 
		return *C ;  
	}
	else
	{
		Register_Descriptor* Reg = machine_desc_object.get_new_register<float_reg>() ; // get new register		
		Register_Addr_Opd* Reg1 = new Register_Addr_Opd(L.get_reg()) ; 
		Register_Addr_Opd* Reg2 = new Register_Addr_Opd(R.get_reg()) ; 
		Register_Addr_Opd* result = new Register_Addr_Opd(Reg) ; 			
		Compute_IC_Stmt * Add_statment = new Compute_IC_Stmt(div_d, Reg1, Reg2, result);// this is the statement
		S.push_back(Add_statment) ; // add the statment..
		// clear the children's registers
		L.get_reg() -> reset_use_for_expr_result() ; 
		R.get_reg() -> reset_use_for_expr_result() ; 

		Code_For_Ast* C = new Code_For_Ast(S , Reg) ; 
		return *C ;	
	}

}

Code_For_Ast & Divide_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{

}

//// Multiply Ast

Code_For_Ast & Mult_Ast::compile()
{
	// get both of them.
	Code_For_Ast L = lhs -> compile() ; 
	Code_For_Ast R = rhs -> compile() ;
	list<Icode_Stmt *> S = L.get_icode_list() ;
	list<Icode_Stmt *> Rh = R.get_icode_list() ; 
	for(list<Icode_Stmt *>:: iterator i = Rh.begin() ; i != Rh.end() ; i++)
		{
			// printf("%d\n", 1 );
			S.push_back(*i) ; 
		}
	// add the addition ast statement.
	if(get_data_type() == int_data_type)
	{
		Register_Descriptor* Reg = machine_desc_object.get_new_register<int_reg>() ; // get new register		
		Register_Addr_Opd* Reg1 = new Register_Addr_Opd(L.get_reg()) ; 
		Register_Addr_Opd* Reg2 = new Register_Addr_Opd(R.get_reg()) ; 
		Register_Addr_Opd* result = new Register_Addr_Opd(Reg) ; 		
		Compute_IC_Stmt * Add_statment = new Compute_IC_Stmt(mult, Reg1, Reg2, result );// this is the statement
		S.push_back(Add_statment) ; // add the statment.
		// clear the children's registers
		L.get_reg() -> reset_use_for_expr_result() ; 
		R.get_reg() -> reset_use_for_expr_result() ; 

		Code_For_Ast* C = new Code_For_Ast(S, Reg) ; 
		return *C ;  
	}
	else
	{
		Register_Descriptor* Reg = machine_desc_object.get_new_register<float_reg>() ; // get new register		
		Register_Addr_Opd* Reg1 = new Register_Addr_Opd(L.get_reg()) ; 
		Register_Addr_Opd* Reg2 = new Register_Addr_Opd(R.get_reg()) ; 
		Register_Addr_Opd* result = new Register_Addr_Opd(Reg) ; 			
		Compute_IC_Stmt * Add_statment = new Compute_IC_Stmt(mult_d, Reg1, Reg2, result);// this is the statement
		S.push_back(Add_statment) ; // add the statment..
		// clear the children's registers
		L.get_reg() -> reset_use_for_expr_result() ; 
		R.get_reg() -> reset_use_for_expr_result() ; 

		Code_For_Ast* C = new Code_For_Ast(S , Reg) ; 
		return *C ;	
	}

}

Code_For_Ast & Mult_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{

}

// U Minus Ast

Code_For_Ast & UMinus_Ast::compile()
{
	// get both of them.
	Code_For_Ast L = lhs -> compile() ; 
	list<Icode_Stmt *> S = L.get_icode_list() ;
	// add the addition ast statement.
	if(get_data_type() == int_data_type)
	{
		Register_Descriptor* Reg = machine_desc_object.get_new_register<int_reg>() ; // get new register		
		Register_Addr_Opd* Reg1 = new Register_Addr_Opd(L.get_reg()) ;
		Register_Addr_Opd* Reg2 = new Register_Addr_Opd(L.get_reg()) ;  
		Register_Addr_Opd* result = new Register_Addr_Opd(Reg) ; 		
		Compute_IC_Stmt * Add_statment = new Compute_IC_Stmt(uminus, Reg1, Reg2,result );// this is the statement
		S.push_back(Add_statment) ; // add the statment.
		// clear the children's registers
		L.get_reg() -> reset_use_for_expr_result() ; 

		Code_For_Ast* C = new Code_For_Ast(S, Reg) ; 
		return *C ;  
	}
	else
	{
		Register_Descriptor* Reg = machine_desc_object.get_new_register<float_reg>() ; // get new register		
		Register_Addr_Opd* Reg1 = new Register_Addr_Opd(L.get_reg()) ; 
		Register_Addr_Opd* Reg2 = new Register_Addr_Opd(L.get_reg()) ; 
		Register_Addr_Opd* result = new Register_Addr_Opd(Reg) ; 			
		Compute_IC_Stmt * Add_statment = new Compute_IC_Stmt(uminus_d, Reg1, Reg2 ,result);// this is the statement
		S.push_back(Add_statment) ; // add the statment..
		// clear the children's registers
		L.get_reg() -> reset_use_for_expr_result() ; 

		Code_For_Ast* C = new Code_For_Ast(S , Reg) ; 
		return *C ;	
	}

}

Code_For_Ast & UMinus_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{

}

//// Return_Ast class

Code_For_Ast & Return_Ast::compile()
{
}

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
}


//// Relational_Expr_Ast class

Code_For_Ast & Relational_Expr_Ast::compile()
{
	Code_For_Ast L = lhs_condition -> compile() ; 
	Code_For_Ast R = rhs_condition -> compile() ;
	list<Icode_Stmt *> S = L.get_icode_list() ;
	list<Icode_Stmt *> Rh = R.get_icode_list() ; 
	bool int_t = (lhs_condition -> get_data_type() == int_data_type) ; 
	for(list<Icode_Stmt *>:: iterator i = Rh.begin() ; i != Rh.end() ; i++)
	{
			// printf("%d\n", 1 );
		S.push_back(*i) ; 
	}
	Register_Descriptor* Reg ; // get new register
	if (int_t)
	{
		Reg = machine_desc_object.get_new_register<int_reg>()  ; 
	}
	else
	{
		Reg = machine_desc_object.get_new_register<float_reg>()  ; 
	}
	//Reg = machine_desc_object.get_new_register<float_reg>()  ;  
	Register_Addr_Opd* Reg1 = new Register_Addr_Opd(L.get_reg()) ; 
	Register_Addr_Opd* Reg2 = new Register_Addr_Opd(R.get_reg()) ; 
	Register_Addr_Opd* result = new Register_Addr_Opd(Reg) ; 
	Compute_IC_Stmt * Relational_statment ; 
	if(rel_op == less_equalto)
	{
			// Relational_statment = new Compute_IC_Stmt(sle, Reg1, Reg2, result );// this is the statement		
		if(int_t)
			Relational_statment = new Compute_IC_Stmt(sle, Reg1, Reg2, result );// this is the statement 
		else
			Relational_statment = new Compute_IC_Stmt(sle_d, Reg1, Reg2, result );// this is the statement	
	}
	else if(rel_op == less_than)
	{
			// Relational_statment = new Compute_IC_Stmt(slt, Reg1, Reg2, result );// this is the statement		
		if(int_t)
			Relational_statment = new Compute_IC_Stmt(slt, Reg1, Reg2, result );// this is the statement
		else 
			Relational_statment = new Compute_IC_Stmt(slt_d, Reg1, Reg2, result );// this is the statement
	}
	else if(rel_op == greater_than)
	{
			// Relational_statment = new Compute_IC_Stmt(sgt, Reg1, Reg2, result );// this is the statement
		if(int_t)
			Relational_statment = new Compute_IC_Stmt(sgt, Reg1, Reg2, result );// this is the statement
		else
			Relational_statment = new Compute_IC_Stmt(sgt_d, Reg1, Reg2, result );// this is the statement
	}
	else if(rel_op == greater_equalto)
	{
			// Relational_statment = new Compute_IC_Stmt(sge, Reg1, Reg2, result );// this is the statement	
		if(int_t)
			Relational_statment = new Compute_IC_Stmt(sge, Reg1, Reg2, result );// this is the statement
		else
			Relational_statment = new Compute_IC_Stmt(sge_d, Reg1, Reg2, result );// this is the statement
	}
	else if(rel_op == equalto)
	{
			// Relational_statment = new Compute_IC_Stmt(seq, Reg1, Reg2, result );// this is the statement
		if(int_t)
			Relational_statment = new Compute_IC_Stmt(seq, Reg1, Reg2, result );// this is the statement
		else
			Relational_statment = new Compute_IC_Stmt(seq_d, Reg1, Reg2, result );// this is the statement
	}
	else if(rel_op == not_equalto)
	{
		// Relational_statment = new Compute_IC_Stmt(sne, Reg1, Reg2, result );// this is the statement
		if(int_t)
			Relational_statment = new Compute_IC_Stmt(sne, Reg1, Reg2, result );// this is the statement
		else
			Relational_statment = new Compute_IC_Stmt(sne_d, Reg1, Reg2, result );// this is the statement
	}
	S.push_back(Relational_statment) ; // add the statment.
	// release the registers
	L.get_reg() -> reset_use_for_expr_result() ; 
	R.get_reg() -> reset_use_for_expr_result() ; 	
	Code_For_Ast* C = new Code_For_Ast(S, Reg) ; 
	return *C ; 
}



//// Logical_Expr_Ast class

Code_For_Ast & Logical_Expr_Ast::compile()
{
	// lhs_op can be null when type is not 
	if(bool_op == _logical_not)
	{
	   list<Icode_Stmt*> Comb ;
	   Const_Opd<int>*  Val = new Const_Opd<int>(1) ;
	   Register_Descriptor* New_Reg = machine_desc_object.get_new_register<int_reg>() ;
	   Register_Addr_Opd* new_result = new Register_Addr_Opd(New_Reg) ; 		
	   Move_IC_Stmt* M = new Move_IC_Stmt(imm_load, Val, new_result) ;
	   Comb.push_back(M) ;


	   Code_For_Ast R = rhs_op -> compile() ;
	   list<Icode_Stmt *> Rh = R.get_icode_list() ; 

	   for(list<Icode_Stmt *>:: iterator i = Rh.begin() ; i != Rh.end() ; i++)
	   {	
			// printf("%d\n", 1 );
			Comb.push_back(*i) ; 
	   }

	   Register_Descriptor* Reg = machine_desc_object.get_new_register<int_reg>() ; // get new register
	   Register_Addr_Opd* Reg1 = new Register_Addr_Opd(R.get_reg()) ; 

	   Register_Addr_Opd* Reg2 = new Register_Addr_Opd(R.get_reg()) ; 
	   Register_Addr_Opd* result = new Register_Addr_Opd(Reg) ; 
	   Compute_IC_Stmt * Add_statment = new Compute_IC_Stmt(not_t, Reg1, new_result, result );// this is the statement
	   Comb.push_back(Add_statment) ; // add the statment.
	   // release the register
	   R.get_reg() -> reset_use_for_expr_result() ; 
	   Code_For_Ast* C = new Code_For_Ast(Comb, Reg) ; 
	   return *C ;
	}
	else if(bool_op == _logical_or)
	{
	   Code_For_Ast L = lhs_op -> compile() ; 
	   Code_For_Ast R = rhs_op -> compile() ;
	   list<Icode_Stmt *> S = L.get_icode_list() ;
	   list<Icode_Stmt *> Rh = R.get_icode_list() ; 
	   for(list<Icode_Stmt *>:: iterator i = Rh.begin() ; i != Rh.end() ; i++)
	   {
				// printf("%d\n", 1 );
	    	S.push_back(*i) ; 
	   }
	   Register_Descriptor* Reg = machine_desc_object.get_new_register<int_reg>() ; // get new register
	   Register_Addr_Opd* Reg1 = new Register_Addr_Opd(L.get_reg()) ; 
	   Register_Addr_Opd* Reg2 = new Register_Addr_Opd(R.get_reg()) ; 
	   Register_Addr_Opd* result = new Register_Addr_Opd(Reg) ; 
	   Compute_IC_Stmt * Add_statment = new Compute_IC_Stmt(or_t, Reg1, Reg2, result );// this is the statement
	   S.push_back(Add_statment) ; // add the statment.
	   // release the registers
	   L.get_reg() -> reset_use_for_expr_result() ; 
	   R.get_reg() -> reset_use_for_expr_result() ; 
	   Code_For_Ast* C = new Code_For_Ast(S, Reg) ; 
	   return *C ;
	}
	else if(bool_op == _logical_and)
	{	
	   Code_For_Ast L = lhs_op -> compile() ; 
	   Code_For_Ast R = rhs_op -> compile() ;
	   list<Icode_Stmt *> S = L.get_icode_list() ;
	   list<Icode_Stmt *> Rh = R.get_icode_list() ; 
	   for(list<Icode_Stmt *>:: iterator i = Rh.begin() ; i != Rh.end() ; i++)
	   {
	    	S.push_back(*i) ; 
	   }
	   Register_Descriptor* Reg = machine_desc_object.get_new_register<int_reg>() ; // get new register
	   Register_Addr_Opd* Reg1 = new Register_Addr_Opd(L.get_reg()) ; 
	   Register_Addr_Opd* Reg2 = new Register_Addr_Opd(R.get_reg()) ; 
	   Register_Addr_Opd* result = new Register_Addr_Opd(Reg) ; 
	   Compute_IC_Stmt * Add_statment = new Compute_IC_Stmt(and_t, Reg1, Reg2, result );// this is the statement
	   S.push_back(Add_statment) ; // add the statment.
	    // release the registers
	   L.get_reg() -> reset_use_for_expr_result() ; 
	   R.get_reg() -> reset_use_for_expr_result() ; 
	   Code_For_Ast* C = new Code_For_Ast(S, Reg) ; 
	   return *C ;	
	}
}

////  Conditional_Expression_Ast Class

Code_For_Ast & Conditional_Expression_Ast::compile()
{
	// If statement
	// compile the cond expression
	list<Icode_Stmt*> Comb ; 
	Register_Descriptor* new_Reg ; 
	bool int_t = (rhs -> get_data_type() == int_data_type) ; 
	if(int_t)
	{
		new_Reg = machine_desc_object.get_new_register<int_reg>() ; // get new register
	}
	else
	{
		new_Reg = machine_desc_object.get_new_register<float_reg>() ; // get new register	
	}
	// add condition
	Code_For_Ast Cond =  cond -> compile() ;
	list<Icode_Stmt*> Cond_List = Cond.get_icode_list() ; 
	for(list<Icode_Stmt *>:: iterator i = Cond_List.begin() ; i != Cond_List.end() ; i++)
		{
			Comb.push_back(*i) ; 
		}

	Tgt_Op last_op = (Comb.back() -> get_op()).get_op() ;
	
	Tgt_Op required_op = beq ;
	if(last_op == slt_d || last_op == sle_d || last_op == seq_d )
	{
		required_op = bc1f ; 
	}
	else if(last_op == sge_d || last_op == sgt_d || last_op == sne_d)
	{
		required_op = bc1t ; 
	}



	string right_side = get_new_label() ; 
	string escape_label = get_new_label() ; // there is always the else part 

	Register_Addr_Opd* Reg = new Register_Addr_Opd(Cond.get_reg()) ;
	// add check



	Control_Flow_IC_Stmt* Jump = new Control_Flow_IC_Stmt(required_op , Reg , right_side ) ; // what label name
	Comb.push_back(Jump) ;  
	
	
	// then part
	Code_For_Ast Then =  lhs -> compile() ;
	list<Icode_Stmt*> Then_list = Then.get_icode_list() ; 
	for(list<Icode_Stmt *>:: iterator i = Then_list.begin() ; i != Then_list.end() ; i++)
		{
			Comb.push_back(*i) ; 
		}
	 // add the or statement.
	// if(rhs -> get_data_type() == int_data_type)
	// {
	Register_Addr_Opd* R1 = new Register_Addr_Opd(new_Reg) ; 
	Register_Addr_Opd* R2 = new Register_Addr_Opd(Then.get_reg()) ; 
	Move_IC_Stmt* M1 ; 
	if(int_t)
		M1 = new Move_IC_Stmt(mov , R2 , R1); 
	else
		M1 = new Move_IC_Stmt(move_d , R2 , R1); 
	Comb.push_back(M1) ; 
	// }

	// jump to out.
		Label_IC_Stmt* j_out = new Label_IC_Stmt(j , escape_label); // for if statement
		Label_IC_Stmt* Else_label = new Label_IC_Stmt(nop, right_side); 
		Comb.push_back(j_out) ; 
		Comb.push_back(Else_label) ; 
		Code_For_Ast Else =  rhs -> compile() ;
		list<Icode_Stmt*> Else_list = Else.get_icode_list() ; 
		// add else part
		for(list<Icode_Stmt *>:: iterator i = Else_list.begin() ; i != Else_list.end() ; i++)
		{
			Comb.push_back(*i) ; 
		} 
	Register_Addr_Opd* R3 = new Register_Addr_Opd(Else.get_reg()) ; 	 
	Move_IC_Stmt* M2; 
	if(int_t)
		M2 = new Move_IC_Stmt(mov , R3 , R1); 
	else
		M2 = new Move_IC_Stmt(move_d , R3 , R1); 
	Comb.push_back(M2) ;	
	// final out
	Label_IC_Stmt* Escape_label = new Label_IC_Stmt(nop, escape_label);
	Comb.push_back(Escape_label) ; 
	// push all of them
  // release the registers
	   Cond.get_reg() -> reset_use_for_expr_result() ; 
	   // Then.get_reg() -> reset_use_for_expr_result() ; 
	   // Else.get_reg() -> reset_use_for_expr_result() ; 
	// Register_Descriptor* R ; 
	Code_For_Ast* C = new Code_For_Ast(Comb, new_Reg) ; 
	return *C ; 



}


//// Selection_Statement_Ast class

Code_For_Ast & Selection_Statement_Ast::compile()
{
	// If statement
	// compile the cond expression
	list<Icode_Stmt*> Comb ; 
	// add condition
	Code_For_Ast Cond =  cond -> compile() ;
	list<Icode_Stmt*> Cond_List = Cond.get_icode_list() ; 
	for(list<Icode_Stmt *>:: iterator i = Cond_List.begin() ; i != Cond_List.end() ; i++)
		{
			Comb.push_back(*i) ; 
		}


	int l = Comb.size() ; 
	
	Tgt_Op last_op = (Comb.back() -> get_op()).get_op() ;
	
	Tgt_Op required_op = beq ;
	if(last_op == slt_d || last_op == sle_d ||  last_op == seq_d  )
	{
		required_op = bc1f ; 
	}
	else if(last_op == sge_d || last_op == sgt_d ||  last_op == sne_d)
	{
		required_op = bc1t ; 
	}


	string escape_label = get_new_label() ; 
	string else_label = escape_label ; 
	if (else_part != NULL) escape_label = get_new_label() ; 
	// cout << else_label << " " << escape_label << endl ; 
	Register_Descriptor* RR = Cond.get_reg() ;
	Register_Addr_Opd* Reg = new Register_Addr_Opd(RR) ;
	// add check
	
	Control_Flow_IC_Stmt* Jump = new Control_Flow_IC_Stmt(required_op , Reg , else_label ) ; // what label name
	
	Comb.push_back(Jump) ;  
	
	
	Code_For_Ast Then =  then_part -> compile() ;
	 
	list<Icode_Stmt*> Then_list = Then.get_icode_list() ; 
	for(list<Icode_Stmt *>:: iterator i = Then_list.begin() ; i != Then_list.end() ; i++)
		{
			Comb.push_back(*i) ; 
		}
	 
	// jump to out.
	if (else_part != NULL)	
	{
		Label_IC_Stmt* j_out = new Label_IC_Stmt(j , escape_label); // for if statement
		Label_IC_Stmt* Else_label = new Label_IC_Stmt(nop, else_label); 
		Comb.push_back(j_out) ; 
		Comb.push_back(Else_label) ; 
		Code_For_Ast Else =  else_part -> compile() ;
		list<Icode_Stmt*> Else_list = Else.get_icode_list() ; 
		// add else part
		for(list<Icode_Stmt *>:: iterator i = Else_list.begin() ; i != Else_list.end() ; i++)
		{
			Comb.push_back(*i) ; 
		}
		// release the register
		//Else.get_reg() -> reset_use_for_expr_result() ;
	}

	// final out
	Label_IC_Stmt* Escape_label = new Label_IC_Stmt(nop, escape_label);
	Comb.push_back(Escape_label) ; 
	// push all of them
	 // release the registers
	   RR -> reset_use_for_expr_result() ; 
	   // Then.get_reg() -> reset_use_for_expr_result() ; 
 

	Register_Descriptor* R ; 
	Code_For_Ast* C = new Code_For_Ast(Comb, R) ; 
	return *C ; 

}

//// Iteration_Statement_Ast class

Code_For_Ast & Iteration_Statement_Ast::compile()
{
	list<Icode_Stmt*> Comb ;
	
	// generate label first -> check sclp
	string body_label = get_new_label() ;
	string main_label = get_new_label() ;
	Label_IC_Stmt* first_label = new Label_IC_Stmt(j , main_label);

	if(!is_do_form)
	{
		Comb.push_back(first_label) ; // goto added to the list	
	}
		// compile body first

	Label_IC_Stmt* Label1 = new Label_IC_Stmt(mov, body_label);
	Comb.push_back(Label1) ;

	Code_For_Ast body_part = body -> compile() ;
	list<Icode_Stmt*> body_part_list = body_part.get_icode_list();
	for(list<Icode_Stmt *>:: iterator i = body_part_list.begin() ; i != body_part_list.end() ; i++)
	{
		Comb.push_back(*i) ; 
	}
	
	// compile condition part
	Label_IC_Stmt* Label2 = new Label_IC_Stmt(mov, main_label);
	Comb.push_back(Label2) ;

	Code_For_Ast cond_part = cond -> compile() ;

	Tgt_Op last_op = (cond_part.get_icode_list().back() -> get_op()).get_op() ;
	
	Tgt_Op required_op = bne ;
	if( last_op == sge_d || last_op == sgt_d || last_op == sne_d)
	{
		required_op = bc1f ; 
	}
	else if(last_op == slt_d || last_op == sle_d  || last_op == seq_d )
	{
		required_op = bc1t ; 
	}

	list<Icode_Stmt*> cond_part_list = cond_part.get_icode_list();

	for(list<Icode_Stmt *>:: iterator i = cond_part_list.begin() ; i != cond_part_list.end() ; i++)
	{
		Comb.push_back(*i) ; 
	}

	Register_Addr_Opd* Reg = new Register_Addr_Opd(cond_part.get_reg()) ;
	Control_Flow_IC_Stmt* Jump = new Control_Flow_IC_Stmt(required_op , Reg , body_label ) ;
	Comb.push_back(Jump) ;
	// release the registers
	cond_part.get_reg() -> reset_use_for_expr_result() ; 
	//body_part.get_reg() -> reset_use_for_expr_result() ; 

	Register_Descriptor* R ; 
	Code_For_Ast* C = new Code_For_Ast(Comb, R) ; 
	return *C ; 
	
}


//// Sequence_Ast class

Code_For_Ast & Sequence_Ast::compile()
{
	// need to write this for sequence ast.
	list<Icode_Stmt*> Comb ; 
	 for (std::list<Ast*>::iterator it = statement_list.begin(); it != statement_list.end(); it++)
	 {
	 	Code_For_Ast M = (*it)->compile();
	 	list<Icode_Stmt*> L = M.get_icode_list() ; 
	 	for(list<Icode_Stmt *>:: iterator i = L.begin() ; i != L.end() ; i++)
		{
			Comb.push_back(*i) ; 
		}
	 }
	 Register_Descriptor* R ; 
	 Code_For_Ast* C = new Code_For_Ast(Comb, R);
	 return *C ;  
}

////////// Print_Ast class compile
Code_For_Ast & Print_Ast::compile()
{
	list<Icode_Stmt*> Comb  ;
	bool int_t = (var -> get_data_type() == int_data_type) ; 
	Const_Opd<int>*  Val;
	if(int_t)
		Val = new Const_Opd<int>(1)  ; 
	else
		Val = new Const_Opd<int>(3) ;  
	Register_Descriptor *Reg = machine_desc_object.spim_register_table[v0] ;
	Register_Addr_Opd* result = new Register_Addr_Opd(Reg) ; 		

	Move_IC_Stmt* M = new Move_IC_Stmt(imm_load, Val, result) ;
	Comb.push_back(M) ; 
	if(int_t)
	{	
		Register_Descriptor *Reg1 = machine_desc_object.spim_register_table[a0] ;
		Register_Addr_Opd* result1 = new Register_Addr_Opd(Reg1) ; 		
		Mem_Addr_Opd* Mem = new Mem_Addr_Opd(var -> get_symbol_entry()) ; 
		Move_IC_Stmt* N = new Move_IC_Stmt(load, Mem, result1) ;
		Comb.push_back(N) ;
	}
	else
	{
		Register_Descriptor *Reg1 = machine_desc_object.spim_register_table[f12] ;
		Register_Addr_Opd* result1 = new Register_Addr_Opd(Reg1) ; 		
		Mem_Addr_Opd* Mem = new Mem_Addr_Opd(var -> get_symbol_entry()) ; 
		Move_IC_Stmt* N = new Move_IC_Stmt(load_d, Mem, result1) ;
		Comb.push_back(N) ;	
	}
	Control_Flow_IC_Stmt* Print = new Control_Flow_IC_Stmt(Tgt_Op::print, NULL, "") ;
	Comb.push_back(Print) ;

	Register_Descriptor* R ; 
	Code_For_Ast* C = new Code_For_Ast(Comb, R);
	return *C ; 

}
