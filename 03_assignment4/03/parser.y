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
         Relational_Op relop ;
         Logical_Op lop ;  
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


// assignment 4 
%token <string_value> WHILE 
%token <string_value> IF
%token <string_value> ELSE 
%token <string_value> DO  
%token <relop > RELOP1
%token <relop > RELOP2 
%token <lop> AND 
%token <lop> OR 
%token <lop> NOT 


%nterm <symbol_table>  declarations
%nterm <symbol_table> variable_list 
%nterm <symbol_table> declaration 
%nterm <symbol_table> declaration_list
%nterm <symbol_table> variable_declaration
%nterm <procedure> main_procedure
%nterm <data_type> type
%nterm <ast> variable
%nterm <ast> constant
%nterm <ast> term
%nterm <ast> expression 
%nterm <l_ast> statement_list
%nterm <ast> statement
%nterm <ast> assignment_statement
%nterm <ast> while_statement
%nterm <ast> iterative_statement
%nterm <ast> if_statement
%nterm <ast> condition_expression
%nterm <ast> question_expression
%right '='
%left '?' ':'
%left OR 
%left AND 
%left RELOP1
%left RELOP2
%left '+' '-'
%left '*' '/'
%left UMINUS
%right NOT
%nonassoc IFX
%nonassoc ELSE


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
                        |       statement_list 
                               iterative_statement
                                {

                                    $$ = $1 ; 
                                	$$ -> push_back($2) ; 
                            	}
                        // |       statement_list
                        //         question_expression


                        // |       statement_list
                        //         dowhile_statement       
        ;   


statement 				: 	 
							 assignment_statement 
							{
								$$ = $1 ; 
							}
							| while_statement
							{
								$$ = $1 ; 
							}
							| if_statement
							{
                                $$ = $1 ; 
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

expression              : expression '+' expression 
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
                        | expression '-' expression 
                        {
                               if( $3 -> get_data_type() == $1 -> get_data_type() )
                             {
                                 $$ = new Minus_Ast( $1 , $3 , yylineno ) ; 
                                 $$ -> set_data_type( $1 -> get_data_type()) ;                           
                             }
                              else { yyerror("cs316: Error: Heterogeneous data types.") ; exit(-1) ; }
                  
                        }
                        | expression '*' expression 
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
                        | expression '/' expression  
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
                        | question_expression 
                        {
                            $$ = $1 ;
                        } 
                        | '-' expression %prec UMINUS
                        {
                             $$ = new UMinus_Ast($2,NULL,yylineno);
                             $$ -> set_data_type( $2 -> get_data_type()) ; 
                        }
                        | term 
                        {   $$ = $1 ; }
                        ;



term                  : variable 
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


condition_expression    :  expression RELOP1 expression 
							{
                                // lhs, relop, rhs, line
							     $$ = new Relational_Expr_Ast( $1 , $2 , $3 , yylineno);  
                                 if($$ -> check_ast() == 0)
                                 {
                                    yyerror("cs316: Error: Relational expression data types didn't match") ; 
                                    exit(-1) ; 
                                 }
                                  $$ -> set_data_type(int_data_type) ; 
                                 // printf("Done1\n");    		
							}
                        |  expression RELOP2 expression
                            {
                                 $$ = new Relational_Expr_Ast( $1 , $2 , $3, yylineno) ; 
                                 if($$ -> check_ast() == 0)
                                 {
                                    yyerror("cs316: Error: Relational expression data types didn't match") ; 
                                    exit(-1) ; 
                                 }
                                $$ -> set_data_type(int_data_type) ; 
                                  // printf("Done2\n");            
                            }
                        |  condition_expression AND condition_expression
                            {
                                // lhs,  bool_op, rhs, line no.
                                $$ = new Logical_Expr_Ast($1 , $2 , $3 , yylineno) ; 

                                 if($$ -> check_ast() == 0)
                                 {
                                    yyerror("cs316: Error: Logical_Op expression data types didn't match") ; 
                                    exit(-1) ; 
                                 }
                                 $$ -> set_data_type(int_data_type) ; 
                            }
                        |  condition_expression OR condition_expression
                        {
                              $$ = new Logical_Expr_Ast($1 , $2 , $3 , yylineno) ;
                                 if($$ -> check_ast() == 0)
                                 {
                                    yyerror("cs316: Error: Logical_Op expression data types didn't match") ; 
                                    exit(-1) ; 
                                 }
                             $$ -> set_data_type(int_data_type) ;  
                        }
                        |  NOT condition_expression
                        {
                            $$ = new Logical_Expr_Ast(NULL , $1 , $2 , yylineno) ; 
                             if($2 == NULL)
                             {
                                yyerror("cs316: Error: Logical_Op expression error") ; 
                                exit(-1) ; 
                             }
                           $$ -> set_data_type(int_data_type) ; 
                        }
                        | '(' condition_expression ')'
                        {
                            $$ = $2 ; 
                        }

                        ; 

question_expression      : condition_expression '?' expression ':' expression 
                          {
                            $$ = new Conditional_Expression_Ast($1,$3,$5,yylineno) ;
                            if($$ -> check_ast() == 0) 
                            {
                                yyerror("cs316: Error: Heterogeneous data types.") ;
                                exit(-1) ; 
                            } 
                            // set data type of condition expressions
                            $$ -> set_data_type($3->get_data_type()) ;
                            //printf("%d\n", $$ -> get_data_type() ); 
                          }
                        ; 

if_statement            : IF '(' condition_expression ')'  iterative_statement   ELSE
                            iterative_statement 
                             {
                                // cond, then, else, line no
                                $$ = new Selection_Statement_Ast($3 , $5 , $7 ,yylineno) ; 
                             }
                          | IF '(' condition_expression ')'  iterative_statement    %prec IFX  
                            {
                                $$ = new Selection_Statement_Ast($3 , $5 , NULL ,yylineno) ; 
                            }
                            ;  

iterative_statement     : statement 
                        {
                            $$ = $1  ; 
                        }
                        |   '{'  statement_list '}'
                        {
                            Sequence_Ast * S = new Sequence_Ast(yylineno) ;
                            list<Ast*> X = *$2 ; 
                            if(X.size() == 0) 
                            {
                            	yyerror("cs316: Error: Empty Block error") ; 
                            	exit(-1) ; 
                            }
                            for(list<Ast*>::iterator i = X.begin() ; i != X.end() ; i++)
                                {
                                    S -> ast_push_back(*i); 
                                }

                            $$ = S ;  
                        }
                        ; 


while_statement			: WHILE '(' condition_expression ')' iterative_statement 
							{
                                // what about the types ...
                                // cond, body, line, bool
								$$ = new Iteration_Statement_Ast($3 , $5 , yylineno, false) ; 
							}
                          |
                          DO iterative_statement WHILE '(' condition_expression ')' ';'
                          {
                                // Sequence_Ast * S = new Sequence_Ast(yylineno) ;
                                // list<Ast*> X = *$2 ; 
                                // for(list<Ast*>::iterator i = X.begin() ; i != X.end() ; i++)
                                // {
                                //     S -> ast_push_back(*i); 
                                // } 
                             $$ = new Iteration_Statement_Ast($5 ,$2 , yylineno, true) ; 
                          }
							; 

%%

void yyerror (char*  s)
{
  printf("%s" , s) ; 
}

extern YYSTYPE yylval ; 
