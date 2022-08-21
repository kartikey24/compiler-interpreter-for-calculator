%{
#include "common-headers.hh"
#include "code.hh"
#include "parse.tab.h"

%}
%option noyywrap

digit  [0-9]
ws	[ \t\n]*
operator [-+*/=;(),{}]
letter [a-zA-Z_]

%%
{digit}+ 	{ 	yylval.term= new int_term(yytext); return INT; }
{digit}+\.{digit}+ 	{ 	yylval.term= new float_term(yytext); return FLOAT; }

{operator}	{	return yytext[0];	}

{letter}({letter}|{digit})* { yylval.term= new name_term(yytext); return ID; }
		
{ws}		;
.	{ 	
		string error_message;
		error_message =  "Illegal character `" + *yytext;
		error_message += "' on line " + yylineno;
	}

