#include "ast.hh"

#include<string>
#include<iostream>
#include<iomanip>
#include<typeinfo>
#include<list>
using namespace std ; 
template class Number_Ast<double>;
template class Number_Ast<int>;

Ast::Ast() {

}

Ast::~Ast() {

}

Data_Type Ast::get_data_type()
{
	return node_data_type ;  
}

void Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt ; 
}

bool Ast::is_value_zero() // doubt
{
	return false ; 
}

bool Ast::check_ast()
{
	return true ; 
}

Symbol_Table_Entry& Ast::get_symbol_entry(){
	Symbol_Table_Entry * s = new Symbol_Table_Entry() ; 
	return *s ; 
}

void Ast::print(ostream & file_buffer){
	// file_buffer << "Program:\n" ;
	// file_buffer << "   Procedure: main, Return Type: voidsss1\n\n" ; no need to print anything over here
}


Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line)
{
	lineno = line ; 
	lhs = temp_lhs ;
	rhs = temp_rhs ;	 

	// also add the data_type
	if(temp_lhs !=NULL && temp_rhs != NULL && 
		temp_lhs -> get_data_type() == temp_lhs -> get_data_type())
		node_data_type	= temp_rhs -> get_data_type() ;  

	//arity
	ast_num_child = binary_arity ; 

}
bool Assignment_Ast::check_ast()
{
	if(lhs == NULL || rhs == NULL) return 0 ; 
    return lhs -> get_data_type() == rhs -> get_data_type() ; 
}

void Assignment_Ast ::print(ostream & file_buffer){

	file_buffer << "\n" ;
	file_buffer << AST_SPACE << "Asgn:\n" ;
	file_buffer << AST_NODE_SPACE << "LHS (" ;
	lhs -> print(file_buffer) ;
	file_buffer << ")\n" ;
	file_buffer << AST_NODE_SPACE << "RHS (" ;
	rhs -> print(file_buffer) ;
	file_buffer << ")" ;
	// file_buffer << "\n\t\tAsgn:\n"  ;  
	// file_buffer << "\t\t\tLHS (" ; 
	// lhs -> print(file_buffer) ;
	// file_buffer << ")\n" ; 
	// file_buffer << "\t\t\tRHS (" ; 
	// rhs -> print(file_buffer) ;
	// file_buffer << ")" ; 

}

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line)
{
	// variable_symbol_entry = new Symbol_Table_Entry() ; 
	// *variable_symbol_entry = var_entry ; // copy data_type, name, lineno.
	variable_symbol_entry =  &var_entry ; 
	lineno = line ; 
	node_data_type = var_entry.get_data_type() ; 
	// print whether ast is local or global
}

Data_Type Name_Ast:: get_data_type()
{
	return node_data_type ; 
}
Symbol_Table_Entry & Name_Ast::get_symbol_entry()
{
	return *(variable_symbol_entry) ; 
}
void Name_Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt; 
	variable_symbol_entry -> set_data_type(dt) ; 
}
void Name_Ast::print(ostream & file_buffer)
{
	file_buffer << "Name : " ;
	file_buffer << variable_symbol_entry -> get_variable_name(); // print name

}

/// number class
template <class T>
Number_Ast<T>::Number_Ast(T number, Data_Type constant_data_type, int line)
{
	// set line number, data_type , value
	lineno = line ;
	node_data_type = constant_data_type ;
	constant = number ;
}
template<class T>
Data_Type Number_Ast<T>::get_data_type()
{
	return node_data_type ;
}

template <class T>
void Number_Ast<T>::set_data_type(Data_Type dt)
{
	node_data_type = dt ;
}

template<class T>
bool Number_Ast<T>::is_value_zero(){
	return constant == 0 ; 
}

template<class T>
void Number_Ast<T>::print(ostream & file_buffer){
	file_buffer << "Num : " << constant ; 
}



///Arith_ast
Data_Type Arithmetic_Expr_Ast::get_data_type()
{
	return node_data_type ;
}

void Arithmetic_Expr_Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

bool Arithmetic_Expr_Ast::check_ast()
{
	if(lhs == NULL || rhs == NULL)
	{
		return false;
	}
	else
	{
		return lhs -> get_data_type() == rhs -> get_data_type() ;	
	}
}

void Arithmetic_Expr_Ast::print(ostream & file_buffer)
{
	// file_buffer << "Arith: " ; no need
}


Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line)
{
	// set line_number, asts.
	lineno = line ;
	lhs = l;
	rhs = r;
	// arity
	ast_num_child = binary_arity ; 

}


void Plus_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: PLUS\n" ;
	file_buffer << AST_SUB_NODE_SPACE << "LHS (" ;
	lhs->print(file_buffer);
	file_buffer << ")\n" ;
	file_buffer << AST_SUB_NODE_SPACE << "RHS (" ;
	rhs->print(file_buffer) ;
	file_buffer << ")" ;
	// file_buffer << "\n\t\t\tArith: " <<"PLUS\n\t" ;
	// lhs->print(file_buffer);
	// file_buffer << "\n\t" ;
	// rhs->print(file_buffer);
}

Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line)
{
	lineno = line ;
	lhs = l;
	rhs = r;
	ast_num_child = binary_arity ; 
}

void Minus_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: MINUS\n" ;
	file_buffer << AST_SUB_NODE_SPACE << "LHS (" ;
	lhs->print(file_buffer);
	file_buffer << ")\n" ;
	file_buffer << AST_SUB_NODE_SPACE << "RHS (" ;
	rhs->print(file_buffer) ;
	file_buffer << ")" ;
	// file_buffer << "\n\t\t\tArith: " << "MINUS\n\t" ;
	// lhs->print(file_buffer);
	// file_buffer << "\n\t" ;
	// rhs->print(file_buffer);
}

Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line)
{
	lineno = line ;
	lhs = l;
	rhs = r;
	ast_num_child = binary_arity ; 
}

void Divide_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: DIV\n" ;
	file_buffer << AST_SUB_NODE_SPACE << "LHS (" ;
	lhs->print(file_buffer);
	file_buffer << ")\n" ;
	file_buffer << AST_SUB_NODE_SPACE << "RHS (" ;
	rhs->print(file_buffer) ;
	file_buffer << ")" ;
	// file_buffer << "\n\t\t\tArith: " << "DIV\n\t" ;
	// lhs->print(file_buffer);
	// file_buffer << "\n\t" ;
	// rhs->print(file_buffer);
}

Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line)
{
	lineno = line ;
	lhs = l;
	rhs = r;
	ast_num_child = binary_arity ; 
}

void Mult_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: MULT\n" ;
	file_buffer << AST_SUB_NODE_SPACE << "LHS (" ;
	lhs->print(file_buffer);
	file_buffer << ")\n" ;
	file_buffer << AST_SUB_NODE_SPACE << "RHS (" ;
	rhs->print(file_buffer) ;
	file_buffer << ")" ;

	// file_buffer << "\n\t\t\tArith: " << "MULT\n\t" ;
	// lhs->print(file_buffer);
	// file_buffer << "\n\t" ;
	// rhs->print(file_buffer);
}

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line)
{
	lineno = line ; 
	lhs = l ; 
	rhs = r ; 
	ast_num_child = binary_arity ; 
}

void UMinus_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: "<< "UMINUS\n" ;
	file_buffer << AST_SUB_NODE_SPACE << "LHS (" ; 
	lhs->print(file_buffer);
	file_buffer << ")" ; 
}


Return_Ast::Return_Ast(int line)
{
	lineno = line ; 
}

void Return_Ast::print(ostream & file_buffer)
{
	// what ??
}














