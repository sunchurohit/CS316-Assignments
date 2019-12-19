%{
#include "parser.tab.h"
%}

INTEGER int
FLOAT float
VOID void
PRINT print
RETURN return
letter  [a-zA-Z_]
digit [0-9]
arithoperator [-+*/]
metachar [,;(){}:\?]
assignment = 
fnum {digit}+\.{digit}+
num {digit}+
name {letter}({letter}|{digit})*
relop "<"|"<="|">"|">="|"=="|"!="
boolop "&&"|"||"|"!"

WHILE while
IF if
ELSE else
DO do
comment \/\/.*
%%


{INTEGER} {
	//printf("int keyword found %s , lineno: %d\n" , yytext ,yylineno) ; 
	store_token_name("INTEGER",yytext,yylineno);
	//yylval.string_value =  new std::string(yytext) ;
	return INTEGER ; 
} 
{FLOAT} {
	//printf("float keyword found %s , lineno: %d\n" , yytext ,yylineno) ; 
	store_token_name("FLOAT",yytext,yylineno);
	yylval.string_value =  new std::string(yytext) ;
	return FLOAT ; 
}
{VOID} {
	//printf("void keyword found %s , lineno: %d\n" , yytext ,yylineno) ; 	
	store_token_name("VOID",yytext,yylineno);
	yylval.string_value =  new std::string(yytext) ;
	return VOID ; 
}
{PRINT} {
	//printf("void keyword found %s , lineno: %d\n" , yytext ,yylineno) ; 	
	store_token_name("PRINT",yytext,yylineno);
	yylval.string_value =  new std::string(yytext) ;
	return PRINT ; 
}
{RETURN} {
	//printf("void keyword found %s , lineno: %d\n" , yytext ,yylineno) ; 	
	store_token_name("RETURN",yytext,yylineno);
	yylval.string_value =  new std::string(yytext) ;
	return RETURN ; 
}
{WHILE} {
	store_token_name("WHILE",yytext,yylineno) ;
	// yylval. = new std::string(yytext) ;
	return WHILE ;
}
{IF} {
	store_token_name("IF",yytext,yylineno) ;
	// yylval.double_value = new std::string(yytext) ;
	return IF ;
}
{ELSE} {
	store_token_name("ELSE",yytext,yylineno) ;
	// yylval.double_value = new std::string(yytext) ;
	return ELSE ;
}
{DO} {
	store_token_name("DO",yytext,yylineno) ;
	// yylval.double_value = new std::string(yytext) ;
	return DO ;
}


{metachar} {
	//printf("metachar found %s , lineno: %d\n" , yytext ,yylineno) ; 	
	store_token_name("META CHAR",yytext,yylineno);
	yylval.string_value =  new std::string(yytext) ;
	return (int) yytext[0] ; 
}
{name} {
	//printf("variable name found %s , lineno: %d\n" , yytext ,yylineno) ; 
	store_token_name("NAME",yytext,yylineno);
	yylval.string_value = new std::string(yytext) ;
	string var_name =  "main" ;  
	//if(strcmp( ( char *) *(yylval.string_value), "main") != 0)
	if(*(yylval.string_value) != "main")
	{
		*(yylval.string_value) = *(yylval.string_value) + "_" ; 
	}
	return NAME; 
}
{boolop} {
	if(strcmp(yytext,"&&") == 0)
	{
		store_token_name("AND",yytext,yylineno) ;
		yylval.lop = _logical_and ;
		return AND ; 
	}
	else if(strcmp(yytext,"||") == 0)
	{	
		store_token_name("OR",yytext,yylineno) ;
		yylval.lop = _logical_or;
		return OR ; 
	}
	else if(strcmp(yytext,"!") == 0)
	{
		store_token_name("NOT",yytext,yylineno) ;
		yylval.lop = _logical_not;
		return NOT ; 
	}
}

{assignment} {
	//printf("assignment found %s , lineno: %d\n" , yytext ,yylineno) ; 
	store_token_name("ASSIGN_OP",yytext,yylineno);
	yylval.string_value =  new std::string(yytext) ;
	return ASSIGN ; 
}
{arithoperator} {
	//printf("Arithoperator found %s , lineno: %d\n" , yytext ,yylineno) ; 
	store_token_name("ARITHOP",yytext,yylineno);
	yylval.string_value =  new std::string(yytext) ;
	return (int) yytext[0] ; 
}
{num} {
	//printf("NUM found %s , lineno: %d\n" , yytext ,yylineno) ; 
	store_token_name("NUM",yytext,yylineno);
	yylval.integer_value = atoi(yytext) ;
	return INTEGER_NUMBER ; 
}
{fnum} {
	//printf("FNUM found %s , lineno: %d\n" , yytext ,yylineno) ; 
	//yyvalue.integer_value = atoi(yytext) ;
	store_token_name("FNUM",yytext,yylineno);
	yylval.double_value = atof(yytext) ;
	return DOUBLE_NUMBER ;
}
{relop} { 

	if(!strcmp(yytext, "=="))
	{
		store_token_name("EQUAL",yytext,yylineno) ;
		yylval.relop = equalto ;
		return RELOP1 ; 
	}
	else if(!strcmp(yytext,"!="))
	{
	    store_token_name("NOT_EQUAL",yytext,yylineno) ;
	    yylval.relop = not_equalto ;
	    return RELOP1 ; 
	 }
	else if(!strcmp(yytext,"<="))
	  {
		store_token_name("LESS_THAN_EQUAL",yytext,yylineno) ;
	 	yylval.relop = less_equalto ;
	 	return RELOP2 ; 
	  }
	else if(!strcmp(yytext,"<"))
	  {
		store_token_name("LESS_THAN",yytext,yylineno) ;
		yylval.relop = less_than ;
		return RELOP2 ; 
	}
	else if(!strcmp(yytext,">="))
	{
		store_token_name("GREATER_THAN_EQUAL",yytext,yylineno) ;
		yylval.relop = greater_equalto ;
		return RELOP2 ; 
	  }
	else if(!strcmp(yytext,">"))
	  {
	    store_token_name("GREATER_THAN",yytext,yylineno) ;
	    yylval.relop = greater_than  ;
	    return RELOP2 ; 
	  }
	
}


{comment} {}
[\t ]* {}
. { printf("cs316: Error\n") ;   }
\n {
	
}
%%
