#include "syntax.h"

void Handler::gl(){
    curr_lex=program.get_lex();
    curr_type=curr_lex.GetType();
    if(curr_type==L_STR){
        curr_ivalue=0;
        curr_svalue=curr_lex.GetSValue();
    } else {
        curr_svalue="";
        curr_ivalue=curr_lex.GetIValue();
    }
}

//syntax...
void Handler::Main(){
    cout<<"Main start"<<endl;
    gl();
    P();
    if(curr_type!=L_FIN) throw curr_lex;
    cout<<"Main end"<<endl;
    for(Lex l: poliz)
        cout<<l;
    
}

void Handler::P(){
    cout<<"P start"<<endl;
    if(curr_type==L_PROGRAM) gl();
    else throw curr_lex;
    if(curr_type==L_LFIG);
    else throw curr_lex;
    D1();
    if(curr_type==L_RFIG) gl();
    else{
        B1();
        if(curr_type==L_RFIG) gl();
        else throw curr_lex;
    };
    cout<<"P end"<<endl;
}

void Handler::D1(){
    cout<<"D1 start"<<endl;
    do{
        gl();
        D();
        //gl();
    }while(curr_type==L_SEMICOLON);
    cout<<"D1 end"<<endl;
}

void Handler::D(){
    cout<<"D start"<<endl;
    t=L_NULL;
    T();
    V();
    while(curr_type==L_COMMA){
        gl();
        V();
    }
    dec(t);
    t=L_NULL;
    cout<<"D end"<<endl;
}

void Handler::T(){
    cout<<"T start"<<endl;
    if(curr_type==L_INT){
        t=L_INT;
        gl();
    }
    else if(curr_type==L_BOOL){
        t=L_BOOL;
        gl();
    }
    else if(curr_type==L_STRING){
        t=L_STRING;
        gl();
    }
    cout<<"T end"<<endl;
}

void Handler::V(){
    cout<<"V start"<<endl;
    if(t!=L_NULL){
        if(curr_type==L_ID){
            st_int.push(curr_ivalue);
            gl();
            if(curr_type==L_ASSIGN){
                gl();
                //semantic if
                if(curr_type==L_NUM && t==L_INT) gl();
                else if(curr_type==L_STR && t==L_STRING) gl();
                else if((curr_type==L_TRUE || curr_type==L_FALSE) && t==L_BOOL) gl();
                else throw curr_lex;
            }
        }
    }
    cout<<"V end"<<endl;
    
}

void Handler::B1(){//???
    cout<<"B1 start"<<endl;
    int flag=0;
    do{
        if(flag) gl();
        else flag=1;
        B();
    }while(curr_type==L_SEMICOLON);
    //cout<<curr_lex<<endl;
    cout<<"B1 end"<<endl;
}

void Handler::B(){ //
    int P0, P1, P2, P3;
    cout<<"B start"<<endl;
    if(curr_type==L_ID){
        check_id();
        poliz.push_back(Lex(P_ADDR,curr_ivalue));
        gl();
        //cout<<4<<endl;
        if(curr_type==L_ASSIGN){
            gl();
            E();
            eq_type();
            poliz.push_back(Lex(L_ASSIGN));
        } else throw curr_lex;

    } else if(curr_type==L_IF){
        gl();
        if(curr_type==L_LPAREN) {gl();E();}
        else throw curr_lex;
        if(curr_type!=L_RPAREN) throw curr_lex;
        eq_bool();
        //p_s
        P2=poliz.size();
        poliz.push_back(Lex());
        poliz.push_back(Lex(P_FGO));
        //p_e
        gl();
        //cout<<2<<endl;
        B();
        //cout<<3<<endl;
        //p_s
        P3=poliz.size();
        poliz.push_back(Lex());
        poliz.push_back(Lex(P_GO));
        poliz[P2]=Lex(P_LABEL,poliz.size());
        //p_e
        if(curr_type==L_ELSE){
            //cout<<5<<endl;
            gl();
            B();
            poliz[P3]=Lex(P_LABEL,poliz.size());
        }

    } else if(curr_type==L_WHILE){
        P0=poliz.size();
        gl();
        if(curr_type==L_LPAREN) {gl(); E();}
        else throw curr_lex;
        if(curr_type!=L_RPAREN) throw curr_lex;
        eq_bool();
        P1=poliz.size();
        poliz.push_back(Lex());
        poliz.push_back(Lex(P_FGO));
        gl();
        B();
        poliz[P0]=Lex(P_LABEL);
        poliz.push_back(Lex(P_GO));
        poliz[P1]=Lex(P_LABEL,poliz.size());

    } else if(curr_type==L_READ){
        gl();
        if(curr_type==L_LPAREN){
            gl();
            if(curr_type==L_ID){
                check_id_in_read();
                poliz.push_back(Lex(P_ADDR,curr_ivalue));
                gl();
            } else throw curr_lex;
            if(curr_type==L_RPAREN){
                gl();
                poliz.push_back(Lex(L_READ));
            }
            else throw curr_lex;
        } else throw curr_lex;
    } else if(curr_type==L_WRITE){
        gl();
        if(curr_type==L_LPAREN){
            gl();
            E();
            /*while(curr_type==L_COMMA){
                gl();
                E();
            }*/
            if(curr_type==L_RPAREN){ 
                gl();
                poliz.push_back(Lex(L_WRITE));
            }
            else throw curr_lex;
        } else throw curr_lex;

    } else if(curr_type==L_FOR){
        gl();
        if(curr_type==L_LPAREN){
            gl();
            E();
            if(curr_type==L_SEMICOLON){
                gl();
                E();
                if(curr_type==L_SEMICOLON){
                    gl();
                    E();
                    if(curr_type!=L_RPAREN) throw curr_lex;
                    else {
                        gl();
                        B();
                    }
                } else throw curr_lex;
            } else throw curr_lex;
        } else throw curr_lex;
    } else if(curr_type==L_BREAK){
        gl();
        if(curr_type!=L_SEMICOLON) throw curr_lex;
    }else F();

    cout<<"B end"<<endl;

}

void Handler::F(){
    cout<<"F start"<<endl;
    if(curr_type==L_LFIG){
        gl();
        B1();
        if(curr_type==L_RFIG) gl();
        else throw curr_lex;
    }

    cout<<"F end"<<endl;

}
void Handler::E(){
    cout<<"E start"<<endl;
    E1();
    if(curr_type==L_EQ || curr_type==L_LEQ || curr_type==L_GEQ||
        curr_type==L_GTR || curr_type==L_LSS || curr_type==L_NEQ){
        st_lex.push(curr_type);
        gl();
        E1();
        check_op();
    };

    cout<<"E end"<<endl;

}

void Handler::E1(){
    cout<<"E1 start"<<endl;
    E2();
    while(curr_type==L_PLUS || curr_type==L_MINUS || curr_type==L_OR){
        st_lex.push(curr_type);
        gl();
        E2();
        check_op();
    }

    cout<<"E1 end"<<endl;
}

void Handler::E2(){
    cout<<"E2 start"<<endl;
    E3();
    while(curr_type==L_TIMES || curr_type==L_SLASH || curr_type==L_AND){
        st_lex.push(curr_type);
        gl();
        E3();
        check_op();
    }

    cout<<"E2 end"<<endl;
}

void Handler::E3(){
    cout<<"E3 start"<<endl;
    if(curr_type==L_ID){
        //cout<<"da";
        check_id();
        poliz.push_back(Lex(L_ID,curr_ivalue));
        gl();
        if(curr_type==L_ASSIGN){
            //cout<<7<<endl;
            gl();
            E();
            eq_type();
            poliz.push_back(Lex(L_ASSIGN));
            //cout<<curr_lex<<endl; //
        }
    } else if(curr_type==L_NUM){
        st_lex.push(L_INT);
        poliz.push_back(curr_lex); //?
        gl();
    } else if(curr_type==L_STR){
        st_lex.push(L_STRING);
        poliz.push_back(curr_lex); //?
        gl();
    } else if(curr_type==L_TRUE){
        st_lex.push(L_BOOL);
        poliz.push_back(Lex(L_TRUE,1));
        gl();
    } else if(curr_type==L_FALSE){
        st_lex.push(L_BOOL);
        poliz.push_back(Lex(L_FALSE,0));
        gl();
    } else if(curr_type==L_NOT){
        gl();
        E3();
        check_not();
    } else if(curr_type==L_LPAREN){
        gl();
        E();
        if(curr_type==L_RPAREN) gl();
        else throw curr_lex;
    } else throw curr_lex;

    cout<<"E3 end"<<endl;

}
//semantic
void Handler::dec(LexType t){
    int i;
    while(!st_int.empty()){
        i=st_int.top();
        st_int.pop();
        if(TID[i].get_declare()) throw "declared twice";
        else{
            TID[i].put_declare();
            TID[i].put_type(t);
        }
    }
}//dec

void Handler::check_id(){
    if(TID[curr_ivalue].get_declare())
        st_lex.push(TID[curr_ivalue].get_type());
    else throw "unknown identifier";
}//check_id

void Handler::eq_type(){
    LexType t=st_lex.top();
    st_lex.pop();
    if(t!=st_lex.top())
        throw "wrong types in = operation";
    st_lex.pop();
}//eq_type

void Handler::eq_bool(){
    if(st_lex.top() != L_BOOL)
        throw "expression is not bool";
    st_lex.pop();
}//eq_bool

void Handler::check_op(){
    LexType First, Second, Op, t=L_INT,r=L_BOOL;

    Second=st_lex.top();
    st_lex.pop();
    Op=st_lex.top();
    st_lex.pop();
    First=st_lex.top();
    st_lex.pop();

    if(Op==L_PLUS && First==Second && First==L_STRING) st_lex.push(L_STRING);
    else{
        if(Op==L_PLUS || Op==L_MINUS || Op==L_TIMES || Op==L_SLASH || Op==L_PRC) r=L_INT;
        if(Op==L_OR || Op==L_AND) t=L_BOOL;
        if(First==Second && First==t) st_lex.push(r);
        else throw "wrong types are in operation";
    }
    poliz.push_back(Lex(Op));

}//check_op

void Handler::check_not(){
    if(st_lex.top()!= L_BOOL)
        throw "wrong type in NOT";
    else poliz.push_back(Lex(L_NOT));
}//check_not

void Handler::check_id_in_read(){
    if(!TID[curr_ivalue].get_declare())
        throw "not declared";
}
