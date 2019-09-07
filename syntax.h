#pragma once
#include "lexem.h"

class Handler{
    Lex curr_lex;
    LexType curr_type;
    LexType t;
    int curr_ivalue;
    string curr_svalue;
    Load program;
    stack<int> st_int;
    stack<LexType>st_lex;
    void gl();
    //syntax
    void P();
    void D1();
    void D();
    void T();
    void V();
    //void I();
    //void K();
    //void C();
    //void G();
    //void N();
    //void S();
    //void A();
    //void L(); ?true/false
    void B1();
    void B();
    void B_();
    void F();
    void E();
    void E1();
    void E2();
    void E3();
    //semantic
    void dec(LexType t);
    void check_id();
    void check_op();
    void check_not();
    void eq_type();
    void eq_bool();
    void check_id_in_read();
public:
    vector<Lex> poliz;
    Handler(const char* prog): program(prog){};
    void Main();
};
