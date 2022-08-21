#include "common-headers.hh"
#include "code.hh"
#include "term-types.hh"

extern lpmode mode;
int temp_count=0;
map <string, Term*> symtab;

Expr_Attribute * process_ID(Term * term)
{
	Expr_Attribute * ea;

	Code * code = new Code();
	symtab[term->get_string()];
	// Term *v = new ptr_term(symtab.find(*name));
	if (mode == interpreter)
		ea = new Expr_Attribute(symtab[term->get_string()], NULL);
	else if (mode == compiler)
		ea = new Expr_Attribute(term, code);
	return ea;
}

// Expr_Attribute * process_FLOAT(Term* term)
// {
// 	Expr_Attribute * ea;

// 	Code * code = new Code();
// 	if (mode == interpreter)
// 		ea = new Expr_Attribute(term, NULL);
// 	else if (mode == compiler)
// 		ea = new Expr_Attribute(term, code);
// 	return ea;
// }

// Expr_Attribute * process_COMPLEX(string * name1, string *name2)
// {
// 	Expr_Attribute * ea;

// 	Code * code = new Code();
// 	Term *v = new complex_term(stoi(*name1),stoi(*name2));
// 	if (mode == interpreter)
// 		ea = new Expr_Attribute(v, NULL);
// 	else if (mode == compiler)
// 		ea = new Expr_Attribute(v, code);
// 	return ea;
// }

Term *itoc(Term *x, Term *y){
    return new complex_term(((int_term *)x)->get_value(), ((int_term *)y)->get_value());
}


Expr_Attribute * process_NUM(Term *term)
{
	Expr_Attribute * ea;

	Code * code = new Code();
	// Term *v = new int_term(stoi(*name));
	if (mode == interpreter)
		ea = new Expr_Attribute(term, NULL);
	else if (mode == compiler)
		ea = new Expr_Attribute(term, code);
	return ea;
}


string operator_name(op_type op)
{
	string name; 
	switch (op)
	{
		case PLUS: 	name = " + "; break;
		case MINUS:	name = " - "; break;
		case MULT:	name = " * "; break;
		case DIV:	name = " / "; break;
		case UMINUS:	name = " - "; break;
		default:
				cerr << "Wrong operator type" << endl;
				exit(1);
				break;
	}
	return name;
}


Expr_Attribute * process_Expr(Expr_Attribute *left, op_type op, Expr_Attribute *right)
{
	Expr_Attribute * ea;
	Term *result;

	if (mode == interpreter)
	{
		switch (op)
		{
			case PLUS: 	result = (*left->get_value() + right->get_value());  break;
			case MINUS:	result = (*left->get_value() - right->get_value()); break;
			case MULT:	result = (*left->get_value() * right->get_value()); break;
			case DIV:	result = (*left->get_value() / right->get_value()); break;
			case UMINUS:
				if (right != NULL)
				{
					cerr << "Right operand must be NULL for Unary Minus" << endl;
					exit(1);
				}
				else	result =  - (*left->get_value());
				break;
			default:
				cerr << "Wrong operator type" << endl;
				exit(1);
				break;
		}
		// cout<<left->get_value()->get_string()<<" "<<right->get_value()->get_string()<<endl;
		ea = new Expr_Attribute(result, NULL);
	}
	else if (mode == compiler)
	{
		string  temp = "t" + to_string(temp_count++);
		symtab[temp];
		Term *v = new name_term(temp);
		Statement * stmt;
		Code * code; 

		if (op == UMINUS)
		{
			stmt = new Statement(v, op, left->get_value());

			code = left->get_code();
			code->append_statement(stmt);
		}
		else 
		{
			
			stmt = new Statement(v, op, left->get_value(), right->get_value());
			code = left->get_code();
			code->append_list(right->get_code());
			code->append_statement(stmt);
		}
		ea = new Expr_Attribute(v, code);
	}
	return ea;
}

Code * process_Asgn(Term *lhs, Expr_Attribute *rhs)
{
	Expr_Attribute * ea;
	Statement * stmt;
	Code *code; 
	
	if (mode == interpreter)
	{
		if (lhs == NULL)
		   	cout << "> " << rhs->get_value()->get_string() << endl;
		else
		{
			
			symtab[lhs->get_string()] = rhs->get_value();
			// cout<<rhs->get_value()->get_string()<<" error here "<<endl;
			cout << "> " <<lhs->get_string()  << " = " << rhs->get_value()->get_string() << endl;
		}
		code = new Code();
	}
	else if (mode == compiler)
	{
	

		if (lhs == NULL)
		{
			stmt = new Statement (rhs->get_value());
			code = rhs->get_code();
			code->append_statement(stmt);
		}
		else
		{
			stmt = new Statement (lhs, COPY, rhs->get_value());
			code = rhs->get_code();
			code->append_statement(stmt);
		}
	}
	return code;
}

void process_finish(Code * code)
{ 	if (mode == compiler) 
	{
		cout << "The three address code generated for the input is\n"; 
		code->print_code();
	}
}

Code * process_Stmt (Code * code)
{ 
	if (mode == compiler) 
		return code; 
	else return new Code();
}

Code * process_Stmt_List(Code *list, Code *code)
{ 
	if (mode == compiler) 
	{
		list->append_list(code);
		return  list;
	}
	else return new Code();
}
