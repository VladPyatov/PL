#include "lexem.h"

LexType Lex::GetType() const { return type;}
int Lex::GetIValue() const {return i_value;}
string Lex::GetSValue() const {return s_value;}

vector<ID> TID;

int put ( const string & buf ){
    vector<ID>::iterator k;

    if ( (k = find(TID.begin(), TID.end(), buf)) != TID.end())
            return k - TID.begin();
    TID.push_back(ID(buf));
    return TID.size() - 1;
}//put

int Load::look ( const string buf, const char ** list ) {
    int i = 0;
    while (list[i]) {
        if (buf == list[i])
            return i;
        ++i;
    }
    return 0;
}//look



//table of keywords
const char * Load::TW    [] = {"", "and", "string", "bool", "break", "else", "if", //6
                                "false", "int", "not", "or", "program", //11
                                "read", "then", "true", "while", "write", "for"};//17

//table of "ogranichitel"
const char * Load::TD    [] = {"EOF", ";", ",", ":", "=", "(", ")", //6
                                 "{", "}", "==", "<", ">","+", "-", //13
                                 "*", "/", "%", "<=", "!=", ">="}; //19

Lex Load::get_lex(){
    enum state { HEAD, IDENT, NUMB, STR, COM, ALE, NEQ };
    state S = HEAD;
    string buf, sample="@$!%^&*()-_=+;:/?<>[]{}|\\";
    int num, i,minus;
    do {
        gc();
        switch(S){
            case HEAD: if(c==' ' || c=='\n' || c=='\r' || c=='\t'){if(c=='\n') {curr_str+=1;}}
                        else if (c == EOF) return Lex(L_FIN,0);
                        else if ( isalpha(c) ) {
                            buf.push_back(c);
                            S = IDENT;
                        }
                        else if ( isdigit(c) ) {
                            num = c - '0';
                            S = NUMB;
                        }
                        else if (c=='"') S=STR;
                        else if(c=='-'){
                            buf.push_back(c);
                            gc();
                            while(c==' ') gc();
                            if(isdigit(c)){
                                num=c-'0';
                                S=NUMB;
                            } else {
                                ungetc(c,fp);
                                i = look ( buf, TD);
                                return Lex(L_MINUS,i);
                            }
                        }
                        else if ( c=='#' ) S = COM;
                        else if ( c=='=' || c=='<' || c=='>' ) { 
                             buf.push_back(c);
                             S = ALE; 
                        }
                        else if (c == '!') {
                             buf.push_back(c);
                             S = NEQ;
                        }
                        else {
                            buf.push_back(c);
                            if ( (i = look ( buf, TD)) )
                                return Lex ( (LexType)(i+(int)L_FIN), i );
                            else
                                throw Exept(curr_str,c,string("HEAD"));
                        }
            break;//HEAD
            case IDENT: if (isdigit(c) || isalpha(c)) buf.push_back(c);
                        else {
                            ungetc(c,fp);
                            if((i=look(buf,TW))) return Lex((LexType)i,i);
                            else{
                                i=put(buf);
                                return Lex(L_ID,i);
                            }
                        }
            break;//IDENT
            case NUMB: if(isdigit(c)) num=num*10+(c-'0');
                        else{
                            ungetc(c,fp);
                            return Lex(L_NUM,num);
                        }
            break;//NUMB
            case STR: if(isdigit(c) || isalpha(c) || sample.find(c)!=string::npos) buf.push_back(c);       
                        else if (c=='"') return Lex(L_STR,buf);
                        else throw Exept(curr_str,c,string("STR"));
            break;//STR
            case COM: if(c=='\n') S=HEAD;
            break;//COM
            case ALE: if(c=='='){
                        buf.push_back(c);
                        i = look (buf,TD);
                        return Lex ((LexType)(i+(int)L_FIN), i);
                      }
                      else {
                          ungetc (c, fp);
                          i = look (buf, TD);
                          return Lex ((LexType)(i+(int)L_FIN), i);
                      }
            break;//ALE
            case NEQ: if (c == '=') {
                        buf.push_back(c);
                        i = look (buf, TD);
                        return Lex (L_NEQ, i);
                      }
                      else
                          throw Exept(curr_str,'!',string("NEQ"));
            break;//NEQ
        }//switch
    } while(true);
}// Lex Load::get_lex()

ostream & operator<< (ostream &s, Lex l){
    string t;
    if (l.type <= 17)
        t = Load::TW[l.type];
    else if (l.type >= 18 && l.type <= 37)
        t = Load::TD[l.type-18];
    else if (l.type == 38)
        t = "NUMB";
    else if (l.type == 39)
        t = "STR";
    else if (l.type == 40)
          t = TID[l.i_value].get_name();

    
    s << '(' << t << ',';
    if(l.type==L_STR) cout <<l.s_value;
    else cout<<l.i_value;
    cout << ");" << endl;
    return s;
}
