%{
#include "common-headers.hh"
#include "code.hh"
extern "C" void yyerror(char *s);
extern int yylex(void);

%}
%union{
	Term *term;
	Expr_Attribute_IA * expression_attributes;
	Code_IA *code;
}
%token <term> INT
%token <term> FLOAT
%token <term> ID
%left '+' '-'
%left '*' '/'
%right Uminus
%type <term> COMPLEX
%type <term> NUM
%type <expression_attributes> Expr
%type <code> Stmt
%type <code> StmtList

%start Start
%%

Start : StmtList 			{ 
      						process_finish($1); $1->IA = true;
      					}
	;

StmtList : Stmt 			{ $$ = process_Stmt($1); $1->IA = $$->IA;}
	| StmtList Stmt 		{ $$ = process_Stmt_List($1,$2); $1->IA =$2->IA = $$->IA;}
	;

Stmt : ID '=' Expr  ';' 		{ $$ = process_Asgn($1, $3); $3->IA = $$->IA;}
	| Expr ';' 			{ $$ = process_Asgn(NULL, $1); $1->IA = $$->IA;}
	| if '(' Boolean ')' then '{' StmtList '}' else '{' StmtList '}' {$$ = process_Condition($3, $7, $11); $7->IA = $3; $11->IA = $3}
	;

Expr : Expr '+' Expr			{ $$ = process_Expr($1, PLUS, $3); $1->IA = $3->IA = $$->IA;}
	| Expr '*' Expr 		{ $$ = process_Expr($1, MULT, $3); $1->IA = $3->IA = $$->IA;}
	| Expr '/' Expr 		{ $$ = process_Expr($1, DIV, $3); $1->IA = $3->IA = $$->IA;}
	| Expr '-' Expr 		{ $$ = process_Expr($1, MINUS, $3); $1->IA = $3->IA = $$->IA;}
	| '-' Expr	%prec Uminus	{ $$ = process_Expr($2, UMINUS, NULL); $2->IA  = $$->IA;}
	| NUM 				{ $$ = process_NUM($1); }
	| ID 				{ $$ = process_ID($1); }
	;
	
NUM : INT
	| FLOAT
	| COMPLEX
	;


Boolean : Expr '<' Expr          { $$ = process_Boolean($1, LESS, $3); $1->IA = $3->IA = $$->IA;}
		| Expr '>' Expr          { $$ = process_Boolean($1, GRTR, $3);$1->IA = $3->IA = $$->IA;}
		| Expr '==' Expr          { $$ = process_Boolean($1, EQ, $3);$1->IA = $3->IA = $$->IA;}
		| Expr '<=' Expr          { $$ = process_Boolean($1, LEQ, $3);$1->IA = $3->IA = $$->IA;}
		| Expr '>=' Expr          { $$ = process_Boolean($1, GEQ, $3);$1->IA = $3->IA = $$->IA;}
		| Expr '!=' Expr 		{ $$ = process_Boolean($1, NEQ, $3);$1->IA = $3->IA = $$->IA;}
		;



COMPLEX : '(' INT ',' INT ')'		{$$ =  itoc($2, $4);}
%%
