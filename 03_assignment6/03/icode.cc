#include "icode.hh"

#include<string>
#include<iostream>
#include<iomanip>
#include<typeinfo>
#include<list>
using namespace std ; 
template class Number_Ast<double>;
template class Number_Ast<int>;
template class Const_Opd<double>;
template class Const_Opd<int>;

// // code for instruction_descriptor
Instruction_Descriptor::Instruction_Descriptor(Tgt_Op op, string name, string mnn, string ics, Icode_Format icf, Assembly_Format af)
{
	this -> inst_op = op ;
	this -> name = name ;
	this -> mnemonic = mnn ;
	this -> ic_symbol = ics; 
	this -> ic_format = icf ;
	this -> assem_format = af ;
}

Instruction_Descriptor::Instruction_Descriptor()
{

}

Tgt_Op Instruction_Descriptor::get_op()
{
	return this -> inst_op ;
}

string Instruction_Descriptor::get_name()
{
	return this -> name ;
}

string Instruction_Descriptor::get_mnemonic()
{
	return this -> mnemonic ;
}

string Instruction_Descriptor::get_ic_symbol()
{
	return this -> ic_symbol ;
}

Icode_Format Instruction_Descriptor::get_ic_format()
{
	return this -> ic_format ;
}

Assembly_Format Instruction_Descriptor::get_assembly_format()
{
	return this -> assem_format ;
}

void Instruction_Descriptor::print_instruction_descriptor(ostream & file_buffer)
{
	/// afterwards
	// printf("printing Instruction_Descriptor\n");
	file_buffer << name << ":" ; //<< " " << mnemonic << " "<< ic_symbol<< " " << ic_format << " " << assem_format << endl; 
}

/// Operands classes

/// Ics_Opd class
Register_Descriptor * Ics_Opd::get_reg()
{

}
// Mem_Addr_opnd

Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se)
{
	symbol_entry = new Symbol_Table_Entry() ; 
	*symbol_entry = se ;
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer)
{
	// file_buffer << "hey printing" ; 
	string name = symbol_entry -> get_variable_name();
	file_buffer << name  ;

}

void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer)
{
	// file_buffer << "This is a memory address" ; 
	if( symbol_entry -> get_symbol_scope() == global)
	{
		file_buffer << symbol_entry -> get_variable_name();
	}
	else
	{
		file_buffer << symbol_entry -> get_start_offset() ; 
		if(symbol_entry -> get_ref_offset() == sp_ref)
		{
			file_buffer  << "($sp)" ;
		}
		else
		{
			file_buffer << "($fp)" ;
		}
	}
	

}

Mem_Addr_Opd & Mem_Addr_Opd::operator= (const Mem_Addr_Opd & rhs)
{
	this -> symbol_entry = rhs.symbol_entry ;
}

///Register_Addr

Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * rd)
{
	this -> register_description = rd ;
}

Register_Descriptor * Register_Addr_Opd::get_reg()
{
	return this -> register_description ;
}

void Register_Addr_Opd::print_ics_opd(ostream & file_buffer)
{
	string name = register_description -> get_name() ; 
	file_buffer << name; 
}

void Register_Addr_Opd::print_asm_opd(ostream & file_buffer)
{
	// file_buffer << "hey printing" ; 
	file_buffer << "$" << register_description -> get_name() ;
}

Register_Addr_Opd & Register_Addr_Opd::operator=(const Register_Addr_Opd & rhs)
{
	this -> register_description = rhs.register_description ;
}

//// Const_opd  class 
template <class T>
Const_Opd<T>::Const_Opd(T num)
{
	this -> num = num ;
}

template <class T>
void Const_Opd<T>::print_ics_opd(ostream & file_buffer)
{
	// file_buffer << "hey printing" ; 
	file_buffer << num; 
}

template <class T>
void Const_Opd<T>::print_asm_opd(ostream & file_buffer)
{
	file_buffer << num ;
}

template <class T>
Const_Opd<T> & Const_Opd<T>::operator=(const Const_Opd<T> & rhs)
{
	this -> num = rhs.num ;
}

/// Ics classes

Instruction_Descriptor & Icode_Stmt::get_op()
{
	return op_desc ;
}
Ics_Opd * Icode_Stmt::get_opd1()
{

}
Ics_Opd * Icode_Stmt::get_opd2()
{

}
Ics_Opd * Icode_Stmt::get_result()
{

}

void Icode_Stmt::set_opd1(Ics_Opd * io)
{

}
void Icode_Stmt::set_opd2(Ics_Opd * io)
{

}
void Icode_Stmt::set_result(Ics_Opd * io)
{

}

/// Move_IC_Stmt

Move_IC_Stmt::Move_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * result)
{
	this -> opd1 = opd1 ;
	this -> result = result ;
	this -> op_desc = *machine_desc_object.spim_instruction_table[inst_op] ; 
}

Move_IC_Stmt & Move_IC_Stmt::operator=(const Move_IC_Stmt & rhs)
{
	this -> opd1 = rhs.opd1 ;
	this -> result = rhs.result ;
	this -> op_desc = rhs.op_desc ;
	return *this ;
}

Instruction_Descriptor & Move_IC_Stmt::get_inst_op_of_ics()
{
	return this -> op_desc ;
}

Ics_Opd * Move_IC_Stmt::get_opd1()
{
	return this -> opd1 ;
}

void Move_IC_Stmt::set_opd1(Ics_Opd * io)
{
	this -> opd1 = io ;
}

Ics_Opd * Move_IC_Stmt::get_result()
{
	return this -> result ;
}

void Move_IC_Stmt::set_result(Ics_Opd * io)
{
	this -> result = io ;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer)
{
	// printf("called\n");	
	file_buffer << "\t" ; 
	op_desc.print_instruction_descriptor(file_buffer) ; 
	file_buffer << "    \t" ; 
	result -> print_ics_opd(file_buffer) ; 
	file_buffer << " <- " ;  
	opd1 -> print_ics_opd(file_buffer) ; 
	file_buffer << "\n" ; 


}
void Move_IC_Stmt::print_assembly(ostream & file_buffer)
{
	file_buffer << "\t" ; 
	file_buffer << op_desc.get_mnemonic() << " ";
	if(op_desc.get_assembly_format() == a_op_r_o1)
	{
		result -> print_asm_opd(file_buffer) ;
	 	file_buffer << ", " ;
	 	opd1 -> print_asm_opd(file_buffer) ; 
	 	file_buffer << "\n" ;
	}
	else if(op_desc.get_assembly_format() == a_op_o1_r)
	{
	 	opd1 -> print_asm_opd(file_buffer) ; 
	 	file_buffer << ", " ;
		result -> print_asm_opd(file_buffer) ;
	 	file_buffer << "\n" ;
	}	 
}

/// Label_IC_Stmt

Label_IC_Stmt::Label_IC_Stmt(Tgt_Op inst_op, string label)
{
	
	this -> label = label ;
	if (inst_op == nop)  
	{
		this -> op_desc = *machine_desc_object.spim_instruction_table[mov] ; 	
	}
	else 
	{
		this -> op_desc = *machine_desc_object.spim_instruction_table[inst_op] ; 
	}
}

Label_IC_Stmt & Label_IC_Stmt::operator=(const Label_IC_Stmt & rhs)
{
	this -> label = rhs.label ;
	this -> op_desc = rhs.op_desc ;
}

Instruction_Descriptor & Label_IC_Stmt::get_inst_op_of_ics()
{
	return this -> op_desc ;
}

string Label_IC_Stmt::get_label()
{
	return this -> label ;
}

void Label_IC_Stmt::set_label(string label)
{
	this -> label = label ;
}

void Label_IC_Stmt::print_icode(ostream & file_buffer)
{
  // print the label
	if(op_desc.get_op() == mov)
	{
		file_buffer << endl << label << ":" << endl ; // newline label, newline
	}
	else
	{
		file_buffer << "\t" ;
		file_buffer << "goto " << label << endl ; 
	}
	
}
void Label_IC_Stmt::print_assembly(ostream & file_buffer)
{
	if(op_desc.get_op() == mov)
	{
		file_buffer << endl << label << ":" << endl ; // newline label, newline
		
	}
	else
	{
		file_buffer << "\t" ;
		file_buffer << op_desc.get_mnemonic() << " " << label << endl ; 
	}
}


// Compute_IC_Stmt class

Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * opd2, Ics_Opd * result)
{
	op_desc = *machine_desc_object.spim_instruction_table[inst_op] ; 
	this -> opd1 = opd1;   
	this -> opd2 = opd2;   
	this -> result = result; 
}

Compute_IC_Stmt & Compute_IC_Stmt::operator=(const Compute_IC_Stmt & rhs)
{
	this -> op_desc = rhs.op_desc ;
	this -> opd1 = rhs.opd1 ;
	this -> opd2 = rhs.opd2 ;
	this -> result = rhs.result ;
	return *this ;
}

Instruction_Descriptor & Compute_IC_Stmt::get_inst_op_of_ics()
{
	return op_desc ; 
}

Ics_Opd * Compute_IC_Stmt::get_opd1()
{
	return this -> opd1 ; 
}
void Compute_IC_Stmt::set_opd1(Ics_Opd * io)
{
	this -> opd1 = io  ; 
}
Ics_Opd * Compute_IC_Stmt::get_opd2()
{
	return this -> opd2 ; 
}
void Compute_IC_Stmt::set_opd2(Ics_Opd * io)
{
	this -> opd2 = io  ; 
}

Ics_Opd * Compute_IC_Stmt::get_result()
{
		return this -> result ; 
}
void Compute_IC_Stmt::set_result(Ics_Opd * io)
{
	this -> result = io ; 
}
void Compute_IC_Stmt::print_icode(ostream & file_buffer)
{
	// print the corresponding statemnt.
	// printf("called\n");	
	// a seperate case of uminus, uminus_d
	if(op_desc.get_ic_format() == i_r_op_o1 )
	{
		file_buffer << "\t" ; 
		op_desc.print_instruction_descriptor(file_buffer) ;// print that. 
		file_buffer << "    " <<"\t" ; 
		result -> print_ics_opd(file_buffer) ; 
		file_buffer << " <- " ;  
		opd1 -> print_ics_opd(file_buffer) ;
		file_buffer << endl ; 
	}
	else if(op_desc.get_ic_format() == i_o1_op_o2)
	{
		file_buffer << "\t" ; 
		op_desc.print_instruction_descriptor(file_buffer) ;// print that. 
		file_buffer << "    " <<"\t" ; 
		opd1 -> print_ics_opd(file_buffer) ; 
		file_buffer << " , " ;  
		opd2 -> print_ics_opd(file_buffer) ;
		file_buffer << endl ;
	}
	else 
	{
		file_buffer << "\t" ; 
		op_desc.print_instruction_descriptor(file_buffer) ;// print that. 
		file_buffer << "    " <<"\t" ; 
		result -> print_ics_opd(file_buffer) ; 
		file_buffer << " <- " ;  
		opd1 -> print_ics_opd(file_buffer) ; 
		file_buffer << " , " ; 
		opd2 -> print_ics_opd(file_buffer) ; 
		file_buffer << "\n" ;
	}

}
void Compute_IC_Stmt::print_assembly(ostream & file_buffer)
{
	file_buffer << "\t" ; 
	file_buffer << op_desc.get_mnemonic() << " ";
	if(op_desc.get_assembly_format() == a_op_r_o1_o2)
	{
		result -> print_asm_opd(file_buffer) ;
	 	file_buffer << ", " ;
	 	opd1 -> print_asm_opd(file_buffer) ; 
	 	file_buffer << ", " ;
	 	opd2 -> print_asm_opd(file_buffer) ;
	 	file_buffer << "\n" ;
	}
	else if(op_desc.get_assembly_format() == a_op_o1_o2_r)
	{
	 	opd1 -> print_asm_opd(file_buffer) ; 
	 	file_buffer << ", " ;
	 	opd2 -> print_asm_opd(file_buffer) ;
	 	file_buffer << ", " ;
		result -> print_asm_opd(file_buffer) ;
	 	file_buffer << "\n" ;
	}
	else if(op_desc.get_assembly_format() == a_op_o1_o2)
	{
		opd1 -> print_asm_opd(file_buffer) ;
		file_buffer << ", " ;
		opd2 -> print_asm_opd(file_buffer) ;
		file_buffer << "\n" ;
	}
	else if(op_desc.get_assembly_format() == a_op_r_o1)
	{
		result -> print_asm_opd(file_buffer) ;
	 	file_buffer << ", " ;
		opd1 -> print_asm_opd(file_buffer) ; 
	 	file_buffer << "\n" ;	
	}
	else
	{
		file_buffer << "\n" ;
	}

}

//// Control_Flow_IC_Stmt

Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1,Ics_Opd * opd2,  string label, int size )
{
	op_desc = *machine_desc_object.spim_instruction_table[inst_op] ; 
	this -> opd1 = opd1; 
	this -> opd2 = opd2;   
	this -> offset = label ; 
	this -> actual_param_size = size ; 
} 
// Control_Flow_IC_Stmt::~Control_Flow_IC_Stmt() {}

Control_Flow_IC_Stmt & Control_Flow_IC_Stmt::operator=(const Control_Flow_IC_Stmt & rhs)
{
	this -> op_desc = rhs.op_desc ;
	this -> opd1 = rhs.opd1 ;
	this -> opd2 = rhs.opd2 ;

	this -> offset = rhs.offset ;
	return *this ;
}
Instruction_Descriptor & Control_Flow_IC_Stmt::get_inst_op_of_ics()
{
	return  op_desc ; 
}
Ics_Opd * Control_Flow_IC_Stmt::get_opd1()
{
	return this -> opd1 ; 
}
void Control_Flow_IC_Stmt::set_opd1(Ics_Opd * io)
{
	this -> opd1 = io  ; 
}


Ics_Opd * Control_Flow_IC_Stmt::get_opd2()
{
	return this -> opd2 ; 
}
void Control_Flow_IC_Stmt::set_opd2(Ics_Opd * io)
{
	this -> opd2 = io  ; 
}

void Control_Flow_IC_Stmt::set_Offset(string label)
{
	this -> offset = label ; 
}
string Control_Flow_IC_Stmt::get_Offset()
{
	return this -> offset ;
}

void Control_Flow_IC_Stmt::print_icode(ostream & file_buffer)
{
	if(op_desc.get_ic_format() == i_op)
	{
		file_buffer << "\t";
		file_buffer << op_desc.get_name() << endl ;
	}
	else if(op_desc.get_ic_format() == i_op_st)
	{
		file_buffer << "\t" ;
		file_buffer << op_desc.get_name() << " " << label << endl ;
	}
	else
	{
		file_buffer << "\t" ; 
		op_desc.print_instruction_descriptor(file_buffer) ;// print that. 
		file_buffer << "    " <<"\t" ;  
		opd1 -> print_ics_opd(file_buffer) ; 
		file_buffer << " , " ; 
		// printing zero
		file_buffer << "zero : goto " << label << endl ; 	
	}
	

}
void Control_Flow_IC_Stmt::print_assembly(ostream & file_buffer)
{
	// could include bge files...
	if(op_desc.get_assembly_format() == a_op)
	{
		file_buffer << "\t" << op_desc.get_mnemonic() << endl;
	}
	else if(op_desc.get_assembly_format() == a_op_st)
	{
		file_buffer << "\t" ; 
		file_buffer << op_desc.get_mnemonic() ; 
		file_buffer << " " << offset << endl ;
	}
	else
	{
		file_buffer << "\t" ; 
		file_buffer << op_desc.get_mnemonic() << " ";
		opd1 -> print_asm_opd(file_buffer) ;  
		file_buffer << ", " ; // print first ; 
		opd2 -> print_asm_opd(file_buffer) ;
		file_buffer  << ", " << offset << endl;
		// file_buffer << ", $zero, " << label << endl ; 	
	}
}

//// Code_For_ Ast
Code_For_Ast::Code_For_Ast()
{
	// list<Icode_Stmt *> L ; 
	// this -> ics_list = L ; 
}
Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg)
{
	this -> ics_list = ic_l ;
	this -> result_register = reg ; // by reference
}

void Code_For_Ast::append_ics(Icode_Stmt & ics)
{
	ics_list.push_back(&ics) ;
}
list<Icode_Stmt *> & Code_For_Ast::get_icode_list()
{
	return this -> ics_list ;
}
Register_Descriptor * Code_For_Ast::get_reg()
{
	return  result_register;
}
void Code_For_Ast::set_reg(Register_Descriptor * reg)
{
	result_register = reg ;
}
Code_For_Ast & Code_For_Ast::operator=(const Code_For_Ast & rhs)
{
	/// May need to copy by creating elements.
	this -> ics_list = rhs.ics_list ;
	this -> result_register = rhs.result_register ;
	return *this ;
}