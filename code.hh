# include <stdio.h>
# include <assert.h>
# include <argp.h>
# include <iostream>
# include <string>
# include <list>
# include <sstream>
# include <map>

using namespace std;

/*

Base: Operand
	|			|			|				|
   Ptr(ID)		INT			FLOAT			COMPLEX_INT

generate TAC
*/




class Term;

class Statement 
{	
	Term * lhs;
	op_type  op;
	Term * opd1;
	Term * opd2;

   public:
	Statement() { }
	Statement(Term *l, op_type o, Term *o1, Term *o2) { lhs=l; op=o; opd1=o1; opd2=o2;}
	Statement(Term *l, op_type o, Term *o1) { lhs=l; op=o; opd1=o1; opd2=NULL;}
	Statement(Term *l, Term *o1) { lhs=l; op=COPY; opd1=o1; opd2=NULL;}
	Statement(Term *o1) { lhs=NULL; op=COPY; opd1=o1; opd2=NULL;}
	~Statement();

	void print_stmt();
};



class Code
{
	list <Statement *> * stmt_list;

  public:
	Code() { stmt_list = new list <Statement *>; }
	~Code();

	void append_statement(Statement *s) { stmt_list->push_back(s); }
	void append_list (Code *c);
	list <Statement *> * get_list()	{ return stmt_list; }

	void print_code(); 
};
