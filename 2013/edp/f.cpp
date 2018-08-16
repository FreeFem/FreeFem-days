#include "ff++.hpp"
//#include <ctype>

double f0(long   t) { return t;}
double f0_(const double &t) { return t;}
double f0s(Stack s,const  double & t) { return t;}
bool ReadEnviFile(KNM<double> * A, string *filename)
{
  
  cout << " filename =" << *filename << " A = "<< (*A)(0,0) << endl;
  ifstream f(filename->c_str());
  ffassert(f); 
  string l,l1,l2,l3,x;
  int  c, n=-1, m=1, band=-1 ;
  while ((c=f.peek()))
    {
      ffassert(c != EOF);
      if (c ==';') 
	{
	  c = f.get();// eat 
	  while ((c=f.peek()))
	    { 
	      if( isalpha(c))
		{
		  f >> l; 
		  if(l == "File" ) 
		    { // ; File Dimensions: 10 samples x 11  lines x 1 band
		      f >> l ;
		      if( l == "Dimensions:") 
			{  
			  f >> m >> l1 >> x >> n >> l2 >> x >> band ;
			  ffassert(band ==1 && l1 =="samples" && x =="x" );
			  
			}
		    }
		  getline(f,l);
		  //cout << " skip " << l << endl; 
		  break;
		} 
	      else if (c==' ') c = f.get();
	      else { getline(f,l);
		//cout << " Skip " << l << endl;
		break;}
	    }
	}
      else if (isdigit(c))  break;
      else  c = f.get();
    }
  
  ffassert( n> 0 && m > 0); 
  A->resize(n,m);
  for(int i=0;i<n; ++i)
    for(int j=0;j<m; ++j)
      {
	f >> (*A)(i,j);
	assert(f.good());
      }
  
  return  0; 
}

//  OneOperator1<A,B>  : A f( B)   
//  OneOperator1_<A,B>  : A f(const  B& )   
//  OneOperator1s_<A,B>  : A f(Stack *s, const  B& )   
void init(){
  Global.Add("f0","(",new OneOperator1<double,long>(f0));
  Global.Add("f00","(",new OneOperator1_<double,double>(f0_));
  Global.Add("f0s","(",new OneOperator1s_<double,double>(f0s));
  Global.Add("ReadEnviFile","(",new OneOperator2<bool,KNM<double> *,string *>(ReadEnviFile));

};

 LOADFUNC(init);
