#include "local-environment.hh"

#include<string>
#include<iostream>
#include<iomanip>
#include<typeinfo>
#include<list>
using namespace std ; 

// Eval_Result class
void  Eval_Result::set_value(int number) 
{
	// do nothing
}
void  Eval_Result::set_value(double number) 
{
	// do nothing
}
int Eval_Result::get_int_value()
{
	 return 0 ;
}
double Eval_Result::get_double_value()
{
	return 0 ; 
}

bool Eval_Result::is_variable_defined()
{
	return false ;
}
void Eval_Result::set_variable_status(bool def)
{
	// do nothing
}

void Eval_Result::set_result_enum(Result_Enum res) 
{
	result_type = res ; 
}
Result_Enum Eval_Result::get_result_enum() 
{
	return result_type ; 
}

// Eval_Result_Value
void  Eval_Result_Value::set_value(int number) 
{
	// 
}
void  Eval_Result_Value::set_value(double number) 
{

}
int Eval_Result_Value::get_int_value()
{
	 return 0 ;
}
double Eval_Result_Value::get_double_value()
{
	return 0 ; 
}

bool Eval_Result_Value::is_variable_defined()
{
	return false ;
}
void Eval_Result_Value::set_variable_status(bool def)
{

}

void Eval_Result_Value::set_result_enum(Result_Enum res) 
{

}
Result_Enum Eval_Result_Value::get_result_enum() 
{
	return result_type ; 
}


// Eval_Result_Value_Int class
Eval_Result_Value_Int:: Eval_Result_Value_Int()
{
	// variable is set to not defined 
	defined = false ; 
	result_type = int_result ; 
	// do we need to do it, really ?
}

void  Eval_Result_Value_Int::set_value(int number) 
{
	// set value of variable that is to be taken care by you buddy.
	defined = true ; 
	value = number ; 
}
void  Eval_Result_Value_Int::set_value(double number) 
{
	 // do nothing
}
int Eval_Result_Value_Int::get_int_value()
{
	if(defined) return value ; 
}
bool Eval_Result_Value_Int::is_variable_defined()
{
	return defined ;
}
void Eval_Result_Value_Int::set_variable_status(bool def)
{
	defined = def ; 
}

void Eval_Result_Value_Int::set_result_enum(Result_Enum res) 
{
	result_type = int_result ; 
}
Result_Enum Eval_Result_Value_Int::get_result_enum() 
{
	// return result type
	return result_type ; 
}

// Eval_Result_Value_Double
Eval_Result_Value_Double:: Eval_Result_Value_Double()
{
	// variable is set to not defined 
	defined = false ; 
	result_type = double_result ; 
	// do we need to do it, really ?
}

void  Eval_Result_Value_Double::set_value(int number) 
{
	 // do nothing
}
void  Eval_Result_Value_Double::set_value(double number) 
{
	// set value of variable that is to be taken care by you buddy.
	defined = true ; 
	value = number ; 
}
double Eval_Result_Value_Double::get_double_value()
{
	if(defined) return value ; 
}
bool Eval_Result_Value_Double::is_variable_defined()
{
	return defined ;
}
void Eval_Result_Value_Double::set_variable_status(bool def)
{
	defined = def ; 
}

void Eval_Result_Value_Double::set_result_enum(Result_Enum res) 
{
	result_type = res ; 
}
Result_Enum Eval_Result_Value_Double::get_result_enum() 
{
	// return result type
	return result_type ; 
}

// Local Environment 

Local_Environment::Local_Environment()
{
   
}
Local_Environment::~Local_Environment()
{

}

void Local_Environment::print(ostream & file_buffer)
{
	// print the environment.
	for(map<string , Eval_Result*> :: iterator i = variable_table.begin() ; 
	i != variable_table.end() ; i++ )
	{
		
		file_buffer << AST_SPACE << i -> first  <<  " : "; 
		if(is_variable_defined(i -> first))
		{
			Eval_Result* E = i -> second ; 
			if(E -> get_result_enum() == int_result)
				file_buffer << E -> get_int_value()  ; 
			else file_buffer << E -> get_double_value()  ; 
		}
		else
		{
			file_buffer << "undefined" ; 
		}
		file_buffer << "\n" ; //  print a new line and continue ; 
	}

}
bool Local_Environment::is_variable_defined(string name)
{  
	// if exits , return the definition, else no.
	if( !(does_variable_exist(name))) return false ; 

	return variable_table[name] ->is_variable_defined() ; 

}
Eval_Result * Local_Environment::get_variable_value(string name)
{
    if( does_variable_exist(name)) return variable_table[name] ; 

    // else return NULL
    return NULL ;  
}
void Local_Environment::put_variable_value(Eval_Result & value, string name)
{
	// put that value in this.
   variable_table[name] = &value ; 
}
bool Local_Environment::does_variable_exist(string name)
{
 // check in the map
	return variable_table.find(name) != variable_table.end() ; 
}













