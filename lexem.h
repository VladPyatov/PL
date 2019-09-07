#pragma once
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Exept: public std::exception {
	public:
        virtual const char* what() const throw (){ return where.c_str();}
		string where, w;
        char  c;
        int str;
		Exept(int cur_str, char c, string where) {
            this->str=cur_str;
		    this->c=c;
            this->where=where;
	    }

        ~Exept() _NOEXCEPT {} //or _GLIBCXX_USE_NOEXCEPT
        /*Exept(const char* c, const char * w) {
		    strcpy(this->c,c);
            strcpy(this->w,w);
	    }*/
};
//all available lexems
enum LexType {
    L_NULL,
    L_AND, L_STRING, L_BOOL, L_BREAK, L_ELSE, L_IF, L_FALSE, L_INT,
    L_NOT, L_OR, L_PROGRAM, L_READ, L_THEN, L_TRUE, L_WHILE, L_WRITE, L_FOR,
    L_FIN, //18
    L_SEMICOLON, L_COMMA, L_COLON, L_ASSIGN, L_LPAREN, L_RPAREN, L_LFIG, L_RFIG, L_EQ,
    L_LSS, L_GTR, L_PLUS, L_MINUS, L_TIMES, L_SLASH, L_PRC, L_LEQ, L_NEQ, L_GEQ, //37
    L_NUM, //38
    L_STR,//39
    L_ID, //40
    P_LABEL, //41
    P_ADDR, //42
    P_GO, //43
    P_FGO //44
};

///// Lex
class Lex {
    LexType type;
    int i_value;
    string s_value;
public:
    Lex(LexType t=L_NULL, int v=0):type(t),i_value(v),s_value(""){};
    Lex(LexType t, string v):type(t), i_value(0), s_value(v){};
    LexType GetType() const;
    int GetIValue() const;
    string GetSValue() const;
    friend ostream & operator<<(ostream &s, Lex l);
};

///// Identifier
class ID {
    bool assign;
    int i_value;
    string s_value;
    string name;
    bool declare;
    LexType type;
public:
    ID() { 
      declare = false; 
      assign = false; 
    }
    ID(const string n) {
        name = n;
        declare = false; 
        assign = false;
    }
    //met-
    string get_name () const { return name; }
    bool get_declare () const { return declare; }
    void put_declare () { declare = true; }
    LexType get_type () const { return type; }
    void put_type ( LexType t ) { type = t; }
    bool get_assign () const { return assign; }
    void put_assign (){ assign = true; }
    int  get_i_value () const { return i_value; }
    void put_i_value (int v){ i_value = v; }
    //-ods

    bool operator==(const string& s) const { return name == s; }
};

/////ID table
extern vector<ID> TID;

//put ID element in vector
int put ( const string & buf );

/////Load
class Load {
    FILE * fp;
    char c;
    int look ( const string buf, const char ** list );
    void gc () { c = fgetc (fp); }
    int curr_str;
public:
    static const char * TW [], * TD [];
    Load ( const char * prog ):curr_str(1) { fp = fopen ( prog, "r" );}
    int get_cur(){return curr_str;}
    Lex get_lex();
};

