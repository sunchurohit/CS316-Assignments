%{
#include <stdio.h>
#include <stdlib.h>
  extern int yylex(void);	
  void yyerror (char *s);
  extern "C" void yyerror(const char * s) ; 
  extern int yylineno ; 
  Data_Type line_data_type ; 
  Table_Scope scope = global ;  
  Symbol_Table* current_table ;
  Symbol_Table* global_table ;

%}


%union { int integer_value;
         double double_value;
         std::string * string_value;
         list<Ast *> * ast_list;
         Ast * ast;
         Symbol_Table * symbol_table;
         Symbol_Table_Entry * symbol_entry;
         Basic_Block * basic_block;
         Procedure * procedure;
         Data_Type data_type ;
    	 Name_Ast * name_ast ; 
    	 Number_Ast <int> * int_ast ;
    	 Number_Ast <float> * float_ast ;
         Arithmetic_Expr_Ast * arith_ast ; 
    	 list<Ast*> * l_ast ; 
     }
%start program
%token <string_value> NAME 
%token <integer_value> INTEGER_NUMBER
%token <string_value> INTEGER
%token <string_value> FLOAT
%token <string_value> VOID
%token <string_value> MAIN 
%token <string_value> ASSIGN
%token <double_value> DOUBLE_NUMBER

%nterm <symbol_table>  declarations
%nterm <symbol_table> variable_list 
%nterm <symbol_table> declaration 
%nterm <symbol_table> declaration_list
%nterm <symbol_table> variable_declaration
%nterm <procedure> main_procedure
%nterm <data_type> type
%nterm <ast> variable
%nterm <ast> constant
%nterm <ast> factor
%nterm <ast> term
%nterm <ast> assignment_statement
%nterm <ast> expression 
%nterm <l_ast> statement_list
%nterm <ast> ufactor
%right '='
%left '+' '-'
%left '*' '/'
%%
program                 : declarations main_procedure
                            { 
                        		program_object.set_global_table(*$1);
	                                program_object.set_procedure( $2 , yylineno) ; 
                            }
        ; 

declarations            : /* empty */ 
                            { if(scope == global) 
                            { 
                                $$ = global_table = new Symbol_Table() ;  scope = local ;
                            }
                            else {
                                $$ = new Symbol_Table() ; current_table = $$ ;
                                } 
                            } | 
                             declaration_list 
                            { 
                                $$ = $1 ;
                                if(scope == global)
                                {   global_table = $$ ;  scope = local ;}
                                else current_table = $$ ; 
                            }
        ;  

declaration_list       :    variable_declaration 
			                 {  
                                $$ = $1 ;
                            }
			            |
                             declaration_list variable_declaration 
			              { 
                            $1 -> append_list(*$2,yylineno); 
                            $$ = $1 ;
                            }
       		        ; 

variable_declaration    :       declaration ';' 
		                     {
                                $$ = $1 ;
                            }
        
        ;

declaration             :       type variable_list 
                             {
		                    		$$ = $2 ; 
                             }
        ;

type                    :       INTEGER
                            {
                                line_data_type = int_data_type ;
                            }
                        |       FLOAT
                            {
                                line_data_type = double_data_type ; 
	            		    }
        
        ; 

variable_list           :       NAME 
                                {
                                    Symbol_Table_Entry* S = new Symbol_Table_Entry( *$1 , line_data_type , yylineno ) ; 
                                    S -> set_symbol_scope(scope) ;
                                    $$ = new Symbol_Table() ;
                                    $$ -> set_table_scope(scope) ; 
                                    $$ -> push_symbol(S) ;
                                }
                        |      variable_list ','  NAME 
                                {
                                     Symbol_Table_Entry* S = new Symbol_Table_Entry( *$3 , line_data_type , yylineno ) ; 
                                     S -> set_symbol_scope(scope) ;
                                     Symbol_Table* SS = new Symbol_Table() ; 
                                     SS -> push_symbol(S) ;
                                     $1 -> append_list( *SS , yylineno) ; 
                                     $$ = $1 ; 
                                }
        
        ;

main_procedure          :  VOID NAME '(' ')' '{' declarations statement_list '}'
        			     {
			            		$$ = new Procedure(void_data_type, *$2, yylineno) ;
			            		$$ -> set_local_list(*$6); 
			        	    	current_table = $6 ;
       		                   		 $$ -> set_ast_list(*$7) ; 
			              }
                        
        ; 

statement_list          :   /* empty */ 
		                { 
				        	 $$ = new list<Ast*> ;
					    }
                        |        statement_list 
                              assignment_statement
 			                     { 
		                            $$ = $1 ; 
                                	$$ -> push_back($2) ; 
                            		}        
        ;

assignment_statement    :   variable ASSIGN expression ';'
                            {
                                if( $1 -> get_data_type() == $3 -> get_data_type()) 
                                { 
                                   $$  = new Assignment_Ast($1 , $3 , yylineno);                          
                                }
		            		else 
				            {
				              yyerror("cs316: Error: Heterogeneous data types.") ; exit(-1) ;				
			            	}
                            }        
        ;

variable                :   NAME
		                  { 
				          	if(current_table->variable_in_symbol_list_check(*$1)) 
				        	{  
						    	$$ = new Name_Ast(*$1, current_table-> get_symbol_table_entry(*$1), yylineno);
							$$ -> set_data_type((current_table -> get_symbol_table_entry(*$1)).get_data_type())  ;  
					        }
						else
						{
						   if(global_table->variable_in_symbol_list_check(*$1))
						   {
						       $$ = new Name_Ast(*$1, global_table-> get_symbol_table_entry(*$1), yylineno);
							$$ -> set_data_type((global_table -> get_symbol_table_entry(*$1)).get_data_type())  ;
						   }
					           else { 
                                         yyerror("cs316: Error: No variable declared!!") ; exit(-1) ;
                                    }
						}
                         }
        
        ;

constant                :   INTEGER_NUMBER 
			             { 
			                	$$ = new Number_Ast <int>($1, int_data_type, yylineno);
		                    }
                        |   DOUBLE_NUMBER
			             {
    	            			$$ = new Number_Ast <double>($1, double_data_type, yylineno);
			             }
			
                        ;




expression              : expression '+' term 
                         { 
                             if( $3 -> get_data_type() == $1 -> get_data_type() )
                              {
                                  $$ = new Plus_Ast( $1 , $3 , yylineno ) ; 
				                  $$ -> set_data_type( $1 -> get_data_type()) ; 
                              }
			                  else {
                              yyerror("cs316: Error: Heterogeneous data types.") ; exit(-1) ;
                              }
                        } 
                        | expression '-' term 
		            	{
	                           if( $3 -> get_data_type() == $1 -> get_data_type() )
                             {
                            	 $$ = new Minus_Ast( $1 , $3 , yylineno ) ; 
		                		 $$ -> set_data_type( $1 -> get_data_type()) ;                           
				             }
		                      else { yyerror("cs316: Error: Heterogeneous data types.") ; exit(-1) ; }
			      
		            	} 
                        | term 
	            		{   $$ = $1 ; }
                        ;

term                    : term '*' ufactor
		                 {
                             if( $3 -> get_data_type() == $1 -> get_data_type() )
                             {
                                $$ = new Mult_Ast( $1 , $3 , yylineno ) ; 
				                $$ -> set_data_type( $1 -> get_data_type()) ;                         
     			             }
			                  else
			                  {
                                yyerror("cs316: Error: Heterogeneous data types.") ; exit(-1) ;
                            }

			            }
                        | term '/' ufactor 
	            		{
			                  if( $3 -> get_data_type() == $1 -> get_data_type() )
                             {
                                $$ = new Divide_Ast( $1 , $3 , yylineno ) ; 
			                	$$ -> set_data_type( $1 -> get_data_type()) ;                           
			                  }
			                  else
			                 {
                                yyerror("cs316: Error: Heterogeneous data types.") ; exit(-1) ; 
                            }

			            }
                        | ufactor 
                          {
                            $$ = $1 ; 
                          }
                        ;
ufactor                 :  '-' factor 
                        {
                             $$ = new UMinus_Ast($2,NULL,yylineno);
                             $$ -> set_data_type( $2 -> get_data_type()) ; 
                        }
                        
                        | factor 
                         {
                                 $$ = $1 ; 
                         }
                         ; 

factor                  : variable 
	    	             {
		        	         $$ = $1 ;
    			         }
                        | constant 
		                 {
			                  $$ = $1 ;
			             }
                         | '(' expression ')'
                         {
                            $$ = $2 ; 
                         }
 		        ;


%%

void yyerror (char*  s)
{
  printf("%s" , s) ; 
}

extern YYSTYPE yylval ; 
