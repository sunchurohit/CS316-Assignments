%{
#include "storetokens.hh"
#include "parser.tab.h"
%}

INTEGER int
FLOAT float
VOID void
letter  [a-zA-Z_]
digit [0-9]
arithoperator [-+*/]
metachar [,;(){}]
assignment = 
fnum {digit}+\.{digit}+
num {digit}+
name {letter}({letter}|{digit})*
comment \/\/.*
%%


{INTEGER} {
	//printf("int keyword found %s , lineno: %d\n" , yytext ,yylineno) ; 
	store_token_name("INTEGER",yytext,yylineno);
	yylval.string_value =  new std::string(yytext) ;
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
	return NAME; 
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
{comment} {}
[\t ]* {}
. { printf("Error: unknown character found at line no. %d\n" , yylineno) ;    }
\n {
	 yylineno++ ; 
}
%%
