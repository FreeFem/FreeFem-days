#include <cstdio>
#include <string>
using namespace std; 
#include "ff++.hpp"
#include "AFunction_ext.hpp" 
string * sprintf(Stack s, string * const & fmt,long const & l)
{
  char buf[1024];
  sprintf(buf,fmt->c_str(),l);
  return Add2StackOfPtr2Free(s,new string(buf) );
}
void inittt()
{
  Global.Add("sprintf","(",new OneOperator2s_<string*,string *,long> ( sprintf )) ;
}
LOADFUNC(inittt);
