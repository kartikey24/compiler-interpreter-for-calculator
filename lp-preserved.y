%{
# include <stdio.h>
# include <argp.h>
# include <iostream>
# include <string>
# include <sstream>
# include <map>
extern "C" void yyerror(char *s);

extern int yylex(void);

using namespace std;

map <string, int> symtab;

typedef enum {
		compiler,
		interpreter
	} lpmode;

lpmode mode;

extern char yytext[];

int temp_count=0;

stringstream output_buffer;

%}
%union{
	string * name;
	struct composite {
		int val;
		string * res;
	} value;	
	string *code;
}

%token <name> NUM 
%token <name> ID
%left '+' '-'
%left '*' '/'
%right UMINUS
%type <value> Expr
%type <code> Stmt
%type <code> StmtList

%start Start
%%

Start : StmtList
      		{
			if (mode == compiler)
				cout << "The three address code generated for the input is\n" << *$1;
		}
	;

StmtList : Stmt
	 	{
			if (mode == compiler)
				$$ = $1;
		}
	| StmtList Stmt
		{ 
			if (mode == compiler)
				$$ = new string(*$1 + *$2);
		}
	;

Stmt : ID '=' Expr  ';'
		{  
	 	   	if (mode == interpreter)
		 	{ 
		   		symtab[*$1] = $3.val; 
		   		cout << "> " << *$1 << " = " << $3.val << endl;
			}
			else if (mode == compiler)
			{
				output_buffer << *$1 << " = " << *$3.res << endl;
				$$ = new string(output_buffer.str());
				output_buffer.str("");		// It is important to empty this string object because it is global and there is double accumulation, once here and then in the recursive rule of StmtList
			}
			else
			{
				cerr << "Wrong mode" << endl;
				exit(1);
			}
        	}
	| Expr ';'
		{ 
	 	   	if (mode == interpreter)
		   		cout << "> " << $1.val << endl;
			else if (mode == compiler)
			{
				output_buffer << *$1.res << endl;
				$$ = new string(output_buffer.str());
			}
			else
			{
				cerr << "Wrong mode" << endl;
				exit(1);
			}
        	}
	;

Expr : Expr '+' Expr			
		{ 
	 	   	if (mode == interpreter)
				$$.val = $1.val + $3.val; 
			else if (mode == compiler)
			{
				string * temp = new string ("t" + to_string(temp_count++));
				output_buffer << *temp << " = " << *$1.res << " + " << *$3.res << endl;
				$$.res = temp;
			}
			else
			{
				cerr << "Wrong mode" << endl;
				exit(1);
			}
		}
	| Expr '*' Expr 		
		{ 
	 	   	if (mode == interpreter)
				$$.val = $1.val * $3.val; 
			else if (mode == compiler)
			{
				string * temp = new string ("t" + to_string(temp_count++));
				output_buffer << *temp << " = " << *$1.res << " * " << *$3.res << endl;
				$$.res = temp;
			}
			else
			{
				cerr << "Wrong mode" << endl;
				exit(1);
			}
		}
	| Expr '/' Expr 		
		{ 
	 	   	if (mode == interpreter)
				$$.val = $1.val / $3.val; 
			else if (mode == compiler)
			{
				string * temp = new string ("t" + to_string(temp_count++));
				output_buffer << *temp << " = " << *$1.res << " / " << *$3.res << endl;
				$$.res = temp;
			}
			else
			{
				cerr << "Wrong mode" << endl;
				exit(1);
			}
		}
	| Expr '-' Expr 		
		{ 
	 	   	if (mode == interpreter)
				$$.val = $1.val - $3.val; 
			else if (mode == compiler)
			{
				string * temp = new string ("t" + to_string(temp_count++));
				output_buffer << *temp << " = " << *$1.res << " - " << *$3.res << endl;
				$$.res = temp;
			}
			else
			{
				cerr << "Wrong mode" << endl;
				exit(1);
			}
		}
	| '-' Expr	%prec UMINUS	
		{ 
	 	   	if (mode == interpreter)
				$$.val = -$2.val; 
			else if (mode == compiler)
			{
				string * temp = new string ("t" + to_string(temp_count++));
				output_buffer << *temp << " = -" << *$2.res << endl;
				$$.res = temp;
			}
			else
			{
				cerr << "Wrong mode" << endl;
				exit(1);
			}
		}
	| NUM
		{ 
	 	   	if (mode == interpreter)
				$$.val = stoi(*$1); 
			else if (mode == compiler)
			{
				$$.res = $1;
			}
			else
			{
				cerr << "Wrong mode" << endl;
				exit(1);
			}
		}
	| ID
		{ 
	 	   	if (mode == interpreter)
				$$.val = symtab[*$1]; 
			else if (mode == compiler)
			{
				$$.res = $1;
			}
			else
			{
				cerr << "Wrong mode" << endl;
				exit(1);
			}
		}
	;
	
%%
static int parse_opt (int key, char *arg, struct argp_state *state)
{
	switch (key)
	{
		case 'i': 
			mode = interpreter;
			break;
		case 'c': 
			mode = compiler;
			break;
	}
	return 0;
}

int main (int argc, char **argv)
{
	struct argp_option options[] = {
		{ 0, 'c', 0, 0, "Compile the input into three address code and print it"},
		{ 0, 'i', 0, 0, "Interpret the input and print result"},
		{ 0 }
		};

	struct argp argp = {options, parse_opt };

	argp_parse (&argp, argc, argv, 0, 0, 0);
	return yyparse();
}

