#include "lexem.h"
#include "syntax.h"
//int Load::cur=0;
int main () {
  
  try {
  	Load S("prog.txt");
    vector<Lex> L;
  	do{
		L.push_back(S.get_lex());
	}while(L.back().GetType() != L_FIN);
    //copy(L.begin(), L.end(),  ostream_iterator<Lex>(cout));
    for(Lex l: L) cout <<l;
  }
  catch (Exept ex) {
      cout << "unexpected symbol "<< ex.c<<" in " <<ex.str <<" str. ("<<ex.where<< ")"<<endl;
      return 1;
  }

 try{
    Handler inter("prog.txt");
    inter.Main();
    
  }
  catch (Exept ex) {
      cout << "unexpected symbol "<< ex.c<<" in " <<ex.str <<" str. ("<<ex.where<< ")"<<endl;
      return 1;
  }
  catch (Lex l){
    cout <<"unexpected lexem"<<endl<<"IValue: " <<l.GetIValue()<<endl<<"SValue: "<<l.GetSValue()<<endl<<"Type: "<<l.GetType()<<endl;
  }
  catch (const char *t){
    cout<<t<<endl;
  }

  return 0;
}