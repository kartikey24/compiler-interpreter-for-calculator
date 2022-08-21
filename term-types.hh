# include <stdio.h>
# include <assert.h>
# include <argp.h>
# include <iostream>
# include <string>
# include <list>
# include <sstream>
# include <map>
# include <vector>
# include <algorithm>

using namespace std;
#ifndef TM
#define TM 1
class Term{
public:
    Term(){};
	virtual string get_string(){ return ""; }
    virtual Term *operator + (Term *const &obj) {
         return NULL;
    }
	virtual Term *operator - (Term *const &obj) {
         return NULL;
    }

	virtual Term *operator * (Term *const &obj) {
         return NULL;
    }

	virtual Term *operator / (Term *const &obj) {
         return NULL;
    }
    virtual Term *operator - (){
        return NULL;
    }
    
};

class complex_term:public Term{
	int p,q;

public:
	complex_term(int x=0, int y=0){p = x; q = y; }
	Term* operator + (Term* const &obj) {
        
         complex_term *res = new complex_term();
         res->p = p + ((complex_term *)obj)->p;
         res->q = q + ((complex_term *)obj)->q;
         return res;
    }
	Term* operator - (Term* const &obj) {
         complex_term *res = new complex_term();
         res->p = p - ((complex_term *)obj)->p;
         res->q = q - ((complex_term *)obj)->q;
         return res;
    }

	Term* operator * (Term* const &obj) {
         complex_term *res = new complex_term();
         res->p = p * ((complex_term *)obj)->p - q * ((complex_term *)obj)->q;
         res->q = p * ((complex_term *)obj)->q + q * ((complex_term *)obj)->p;
         return res;
    }

	Term* operator / (Term* const &obj) {
         
		 complex_term *res = (complex_term *)this->operator*(obj);
         int tmp = res->p * p + res->q *q;
		 res->p/=tmp;
		 res->q/=tmp;
         return res;
    }
    Term* operator - (){
        return new complex_term(-p,-q);
    }

	string get_string(){
		return to_string(p)+"+"+to_string(q)+"i";
	}
};

class int_term:public Term{
    int x;
public:
	int_term(int p=0){x=p;  }
    int_term(string p="0"){x=stoi(p); }
	Term *operator + (Term* const &obj) {
         return new int_term(x+((int_term *)obj)->x);
    }
	Term *operator - (Term* const &obj) {
         return new int_term(x-((int_term *)obj)->x);
    }

	Term *operator * (Term* const &obj) {
         return new int_term(x*((int_term *)obj)->x);
    }

	Term *operator / (Term* const &obj) {
         return new int_term(x/((int_term *)obj)->x);
    }
    Term *operator - (){
        return new int_term(-x);
    }
    int get_value(){ return x;}
	string get_string(){
		return to_string(x);
	}
};

class float_term:public Term{
    float x;
public:
	float_term(float p=0){x=p;  }
    float_term(string p="0"){x=stof(p);}
	Term *operator + (Term* const &obj) {
         return new float_term(x+((float_term *)obj)->x);
    }
	Term *operator - (Term* const &obj) {
         return new float_term(x-((float_term *)obj)->x);
    }

	Term *operator * (Term* const &obj) {
         return new float_term(x*((float_term *)obj)->x);
    }

	Term *operator / (Term* const &obj) {
         return new float_term(x/((float_term *)obj)->x);
    }
    Term *operator - (){
        return new float_term(-x);
    }
	string get_string(){
		return to_string(x);
	}
};

class name_term: public Term{
    string ID;
    map <string, Term*>::iterator itr;
public:
    name_term(string s):ID(s){}
    string get_string(){return ID;}
};


class set_term: public Term{
    vector<int> v;
public:
    set_term(vector<int> v1):v(v1) {
        sort(v.begin(),v.end());
    }


    Term *u(Term *const &obj){
        set_term *ob = (set_term *)obj;
        vector<int> res(v.size()+ob->v.size());
        auto it=set_union(v.begin(),v.end(),ob->v.begin(), ob->v.end(),res.begin());
        res.resize(it-res.begin());
        return new set_term(res);
    }
    Term *i(Term *const &obj){
        set_term *ob = (set_term *)obj;
        vector<int> res(v.size()+ob->v.size());
        auto it=set_intersection(v.begin(),v.end(),ob->v.begin(), ob->v.end(),res.begin());
        res.resize(it-res.begin());
        return new set_term(res);
    }
    Term *d(Term *const &obj){
        set_term *ob = (set_term *)obj;
        vector<int> res(v.size()+ob->v.size());
        auto it=set_difference(v.begin(),v.end(),ob->v.begin(), ob->v.end(),res.begin());
        res.resize(it-res.begin());
        return new set_term(res);
    }
    Term *sd(Term *const &obj){
        set_term *ob = (set_term *)obj;
        vector<int> res(v.size()+ob->v.size());
        auto it=set_symmetric_difference(v.begin(),v.end(),ob->v.begin(), ob->v.end(),res.begin());
        res.resize(it-res.begin());
        return new set_term(res);
    }

    string get_string(){
        string tmp = "{ ";
        for(auto i:v)
           tmp+=to_string(i)+", ";
        tmp.pop_back();
        tmp.pop_back();
        tmp+=" }";
        return tmp;
    }
};



#endif