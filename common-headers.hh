# include <stdio.h>
# include <argp.h>
# include <iostream>
# include <string>
# include <sstream>
# include <map>
# include "term-types.hh"
using namespace std;

class Code;

class Expr_Attribute
{	
	Term *value;
	Code * code;

   public:
	Expr_Attribute() { }
	Expr_Attribute(Term *v, Code *c) { value= v;  code=c;}
	~Expr_Attribute();

	Term *get_value() 	{ return value; }
	string * get_place() 	{ return new string(value->get_string()); }
	Code * get_code()	{ return code; }
};

typedef enum {
		compiler,
		interpreter
	} lpmode;

typedef enum {
	PLUS,
	MINUS,
	MULT,
	DIV,
	UMINUS,
	COPY
	} op_type;

Expr_Attribute * process_ID(Term *term);
Expr_Attribute * process_NUM(Term *term);
Term *itoc(Term *x, Term *y);
// Expr_Attribute * process_FLOAT(string * name);
// Expr_Attribute * process_COMPLEX(string * name1, string *name2);
Expr_Attribute * process_Expr(Expr_Attribute *left, op_type op, Expr_Attribute *right);
Code * process_Asgn(Term *lhs, Expr_Attribute *rhs);
void process_finish(Code * code);
Code * process_Stmt (Code * code);
Code * process_Stmt_List(Code *list, Code *stmt);
string operator_name(op_type op);


#if 0
// Code to test the class. Need to rename the file to .cc to compile 
// into an executable.
//
int main()
{

	string * p = new string ("t0");
	string * c = new string ("This is my code");

	Expr_Attribute * ea = new Expr_Attribute(10, p, c);

	Expr_Attribute * eb = ea;

	cout << "Value is " << eb->get_value() << endl;
	cout << "Place is " << *eb->get_place() << endl;
	cout << "Code is " << *eb->get_code() << endl;

	return 0;
}

#endif