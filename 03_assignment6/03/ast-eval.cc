#include "icode.hh"

#include<string>
#include<iostream>
#include<iomanip>
#include<typeinfo>
#include<list>
using namespace std ; 
template class Number_Ast<double>;
template class Number_Ast<int>;

// Symbol Table-class
void Symbol_Table::create(Local_Environment & local_global_variables_table)
{ 
	// creates the local environment from the table
	 for (list<Symbol_Table_Entry *>::iterator it=variable_table.begin(); it != variable_table.end(); ++it)
	 {
	 	Data_Type t = (**it).get_data_type() ; 
	 	if (t == int_data_type)
		{
			Eval_Result_Value_Int *  ER = new Eval_Result_Value_Int() ; 
			if( (**it).get_symbol_scope() == global)
			{
				ER -> set_variable_status(true) ; 
				 ER -> set_value(0) ; 
			}
			local_global_variables_table.put_variable_value(*ER , (**it).get_variable_name());  	
		}
		else if (t == double_data_type)
		{
			Eval_Result *  ER = new Eval_Result_Value_Double() ; 
			if( (**it).get_symbol_scope() == global) 
				{	// initializing global variables.
					 ER -> set_variable_status(true) ; 
					 ER -> set_value(0.00) ;
				 }	
			local_global_variables_table.put_variable_value(*ER , (**it).get_variable_name());
		}
	 }
	 
	
}
// Ast class
void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{

}
Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
 
  
}
void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{

}
Eval_Result & Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) 
{


 
}


// Assignment_Ast class

Eval_Result & Assignment_Ast:: evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	// question: Should you evaluate RHS ?? or print it ?? 
	print(file_buffer) ; 
	Symbol_Table_Entry  E = lhs -> get_symbol_entry() ; 
	lhs -> set_value_of_evaluation(eval_env , rhs -> evaluate(eval_env , file_buffer)) ; 
	lhs -> print_value(eval_env , file_buffer) ; // print that guy after evaluation.
	return rhs -> get_value_of_evaluation(eval_env) ; 
}


// Name_Ast class

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
		Symbol_Table_Entry  T = get_symbol_entry() ; 
		if(eval_env.does_variable_exist(T.get_variable_name()) != 1)
		 	{
		 		print_value(interpreter_global_table , file_buffer) ; 
		 		return ; 
		 	} 
	 	file_buffer  << "\n" << AST_SPACE << T.get_variable_name() << " : ";
	 	
		if(eval_env.is_variable_defined(T.get_variable_name()))
		{
			
			if(T.get_data_type() == int_data_type)
				{
					file_buffer << get_value_of_evaluation(eval_env).get_int_value() ; 
				}
			else 
				{
				file_buffer <<	get_value_of_evaluation(eval_env).get_double_value() ; 
				}
		}
		else 
		{
			file_buffer << "undefined" ; 
		}
		file_buffer << "\n\n" ; 
}
Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	// get it's value from the map.
	Symbol_Table_Entry N = get_symbol_entry() ; 
	Table_Scope T = N.get_symbol_scope() ; 
	if(T == local) 
	{
		if(eval_env.get_variable_value(N.get_variable_name()) -> is_variable_defined() == 0 ) 
		{
			printf("cs316: Error variable not defined\n");
			exit(-1) ; 
		} 
		return *(eval_env.get_variable_value(N.get_variable_name())) ;
	}
	else 
	{
		// T is global
		return *(interpreter_global_table.get_variable_value(N.get_variable_name())) ;
	}
}
void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	//set the result in the map.
	Symbol_Table_Entry N = get_symbol_entry() ;
	Table_Scope T = N.get_symbol_scope() ; 
	if(T == local) 
	{
		result.set_variable_status(true) ; 
		eval_env.put_variable_value(result, N.get_variable_name()) ; 
	}
	else if(T == global)
	{
		result.set_variable_status(true) ; 
		interpreter_global_table.put_variable_value(result, N.get_variable_name()) ;
	}
}
Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	// Dont print any...
	return get_value_of_evaluation(eval_env) ; 
}

// Number Ast
template<>
Eval_Result & Number_Ast<int>::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	// make a new Eval_Result and return
	Eval_Result_Value_Int* E = new Eval_Result_Value_Int() ; 
	E -> set_value(constant) ; 
	E -> set_variable_status(true) ; 
	return *E ; 
}

template<>
Eval_Result & Number_Ast<double>::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	// make a new Eval_Result and return
	Eval_Result_Value_Double* E = new Eval_Result_Value_Double() ; 
	E -> set_value(constant) ; 
	E -> set_variable_status(true) ; 
	return *E ; 
}

// Arithmentic_Expr Ast

Eval_Result & Arithmetic_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	
}

// Plus_Ast class

Eval_Result & Plus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	// get the ERs of lhs and rhs and obtain the overall ER.
	if(get_data_type() == int_data_type)
	{
		int res = (lhs -> evaluate(eval_env , file_buffer)).get_int_value()
					+ (rhs -> evaluate(eval_env , file_buffer)).get_int_value() ; 
		Eval_Result_Value_Int * E = new Eval_Result_Value_Int() ; 
		E -> set_value(res) ; 
		return *E ;  
	}
	else
	{
		double res = (lhs -> evaluate(eval_env , file_buffer)).get_double_value()
					+ (rhs -> evaluate(eval_env , file_buffer)).get_double_value() ; 
		Eval_Result_Value_Double * E = new Eval_Result_Value_Double() ; 
		E -> set_value(res) ; 
		return *E ; 	
	}

}

// Minus_Ast class

Eval_Result & Minus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if(get_data_type() == int_data_type)
	{
		int res = (lhs -> evaluate(eval_env , file_buffer)).get_int_value()
					- (rhs -> evaluate(eval_env , file_buffer)).get_int_value() ; 
		Eval_Result_Value_Int * E = new Eval_Result_Value_Int() ; 
		E -> set_value(res) ; 
		return *E ;  
	}
	else
	{
		double res = (lhs -> evaluate(eval_env , file_buffer)).get_double_value()
					- (rhs -> evaluate(eval_env , file_buffer)).get_double_value() ; 
		Eval_Result_Value_Double * E = new Eval_Result_Value_Double() ; 
		E -> set_value(res) ; 
		return *E ; 	
	}
	
}

// Mult_Ast class

Eval_Result & Mult_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if(get_data_type() == int_data_type)
	{
		int res = (lhs -> evaluate(eval_env , file_buffer)).get_int_value()
					* (rhs -> evaluate(eval_env , file_buffer)).get_int_value() ; 
		Eval_Result_Value_Int * E = new Eval_Result_Value_Int() ; 
		E -> set_value(res) ; 
		return *E ;  
	}
	else
	{
		double res = (lhs -> evaluate(eval_env , file_buffer)).get_double_value()
					* (rhs -> evaluate(eval_env , file_buffer)).get_double_value() ; 
		Eval_Result_Value_Double * E = new Eval_Result_Value_Double() ; 
		E -> set_value(res) ; 
		return *E ; 	
	}
}

// Divide_Ast class
Eval_Result & Divide_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if(get_data_type() == int_data_type)
	{
		if(rhs -> evaluate(eval_env , file_buffer).get_int_value() == 0)
		{
			printf("cs316: Error : Division by zero\n") ; exit(-1) ;
		}
		int res = (lhs -> evaluate(eval_env , file_buffer)).get_int_value()
					/ (rhs -> evaluate(eval_env , file_buffer)).get_int_value() ; 
		Eval_Result_Value_Int * E = new Eval_Result_Value_Int() ; 
		E -> set_value(res) ; 
		return *E ;  
	}
	else
	{
		if(rhs -> evaluate(eval_env , file_buffer).get_double_value() == 0.0)
		{
			printf("cs316: Error : Division by zero\n") ; exit(-1) ;
		}

		double res = (lhs -> evaluate(eval_env , file_buffer)).get_double_value()
					/ (rhs -> evaluate(eval_env , file_buffer)).get_double_value() ; 
		Eval_Result_Value_Double * E = new Eval_Result_Value_Double() ; 
		E -> set_value(res) ; 
		return *E ; 	
	}
	
}

// UMinus_Ast class
Eval_Result & UMinus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if(get_data_type() == int_data_type)
	{
		int res = -1 * (lhs -> evaluate(eval_env , file_buffer)).get_int_value()
					; 
		Eval_Result_Value_Int * E = new Eval_Result_Value_Int() ; 
		E -> set_value(res) ; 
		return *E ;  
	}
	else
	{
		double res = -1  * (lhs -> evaluate(eval_env , file_buffer)).get_double_value()
					 ; 
		Eval_Result_Value_Double * E = new Eval_Result_Value_Double() ; 
		E -> set_value(res) ; 
		return *E ; 	
	}
}
// Contional Expression Ast
Eval_Result & Conditional_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if((cond -> evaluate(eval_env , file_buffer)).get_int_value() == 1)
	{
		return lhs -> evaluate(eval_env , file_buffer) ; 
	}
	else
	{
		return rhs -> evaluate(eval_env , file_buffer) ; 
	}

}

// Return_Ast class

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{

}

// Relational_Expr_Ast Class

Eval_Result & Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result* E = new Eval_Result_Value_Int() ;
	E -> set_variable_status(true) ;
 	Eval_Result * L = &(lhs_condition -> evaluate(eval_env , file_buffer)) ;
 	Eval_Result * R = &(rhs_condition -> evaluate(eval_env , file_buffer)) ;
 	int il , ir ;
 	double dl , dr ; 
 	bool c = true ; 
 	if(L -> get_result_enum() == int_result)
 	{
 		il  = L -> get_int_value() ;
 		ir = R -> get_int_value() ; 
 	} 
 	else 
 	{
 		c = false ; 
 		dl = L -> get_double_value() ; 
 		dr = R -> get_double_value() ; 
 	}
	 switch(rel_op) {
      case less_equalto :
         {
   				if(c)
   				{
   					if(il <= ir) E -> set_value(1) ; 
   					else E -> set_value(0) ; 
   				}
   				else  
   				{
   					if(dl <= dr) E -> set_value(1) ; 
   					else E -> set_value(0) ; 	
   				}
   				return *E ; 
         }
      case less_than :
      {
      		if(c)
   				{
   					if(il < ir) E -> set_value(1) ; 
   					else E -> set_value(0) ; 
   				}
   				else  
   				{
   					if(dl < dr) E -> set_value(1) ; 
   					else E -> set_value(0) ; 	
   				}
   				return *E ;
      }
      case greater_than :
         {
			if(c)
   				{
   					if(il > ir) E -> set_value(1) ; 
   					else E -> set_value(0) ; 
   				}
   				else  
   				{
   					if(dl > dr) E -> set_value(1) ; 
   					else E -> set_value(0) ; 	
   				}
   				return *E ;
         }
      case greater_equalto :
        {
        	if(c)
   				{
   					if(il >= ir) E -> set_value(1) ; 
   					else E -> set_value(0) ; 
   				}
   				else  
   				{
   					if(dl >= dr) E -> set_value(1) ; 
   					else E -> set_value(0) ; 	
   				}
   				return *E ;
        }
      case equalto :
         {
         	if(c)
   				{
   					if(il == ir) E -> set_value(1) ; 
   					else E -> set_value(0) ; 
   				}
   				else  
   				{
   					if(dl == dr) E -> set_value(1) ; 
   					else E -> set_value(0) ; 	
   				}
   				return *E ;
         }
       case not_equalto:
       {
       		if(c)
   				{
   					if(il != ir) E -> set_value(1) ; 
   					else E -> set_value(0) ; 
   				}
   				else  
   				{
   					if(dl != dr) E -> set_value(1) ; 
   					else E -> set_value(0) ; 	
   				}
   				return *E ;
       }
      default :
         {
        	
   		}
   }
}

// Logical_Expr_Ast Class
Eval_Result & Logical_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result* E = new Eval_Result_Value_Int() ;
	E -> set_variable_status(true) ;
 	Eval_Result * R = &(rhs_op -> evaluate(eval_env , file_buffer)) ;
 	int il , ir ;
 	ir = R -> get_int_value() ; 
	 switch(bool_op) {
      case _logical_not :
         {		
         		if(ir == 0) E -> set_value(1);
         		else E -> set_value(0) ;
   				
   				return *E ; 
         }
      case _logical_or :
      {
  			 	Eval_Result * L = &(lhs_op -> evaluate(eval_env , file_buffer)) ;
      			il  = L -> get_int_value();

      			if((il == 1) || (ir == 1)) E -> set_value(1) ;
      			else E -> set_value(0) ;

   				return *E ;
      }
      case _logical_and :
         {
         		Eval_Result * L = &(lhs_op -> evaluate(eval_env , file_buffer)) ;
      			il  = L -> get_int_value();

      			if((il == 1) && (ir == 1)) E -> set_value(1) ;
      			else E -> set_value(0) ;

   				return *E ;
         }
   }
}

// Iteration_Statement
Eval_Result & Iteration_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if(is_do_form) { body -> evaluate(eval_env , file_buffer) ; }
	while( (cond -> evaluate(eval_env , file_buffer)).get_int_value() == 1 )
	{
		body -> evaluate(eval_env , file_buffer) ; 
	}
	Eval_Result* E = new Eval_Result_Value_Int() ; 
	E -> set_variable_status(true) ; 
	E -> set_value(1) ;
	return  *E; 
}
// Selection_Statement_Ast class
Eval_Result & Selection_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if((cond -> evaluate(eval_env , file_buffer)).get_int_value() == 1)
	{
		then_part -> evaluate(eval_env , file_buffer) ; 
	}
	else
	{
		if(else_part != NULL)
			else_part -> evaluate(eval_env , file_buffer) ; 	
	}
	Eval_Result* E = new Eval_Result_Value_Int() ; 
	E -> set_variable_status(true) ; 
	E -> set_value(1) ;	
	return  *E;
}

// Sequence_Statement_Stament

Eval_Result & Sequence_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
		for(list<Ast *>::iterator i =  statement_list.begin() ; 
			i != statement_list.end() ; i++)
		{
			(**i).evaluate(eval_env , file_buffer); 
		}
		Eval_Result* E = new Eval_Result_Value_Int() ; 
		E -> set_variable_status(true) ; 
		E -> set_value(1) ;	
		return  *E; 
}

Eval_Result & Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	
}