File of expert season of freefem++ day

1) explanation of how to compile freefem++ 

2) add function to freefem++ 
  try a add expert fonction just for the fun
   2 function expert 
    one on value and on with poits. 

double expert( double x) {
    cout << " expert " << x << endl;
    return 42.;
}

double expert( double *x) {
    *x=42;
    cout << " expert " << *x << endl;
    return 42.;
}
void static init(){
    // warning with choose of priority 
    Global.Add("expert","(",new OneOperator1<double,double>(expert,1));
    Global.Add("expert","(",new OneOperator1<double,double*>(expert,2));
}

to compile do on terminal
ff-c++ expert-2015.cpp
to check
FreeFem++ expert-2015.edp

3) add a new bubble finite element P1 of triangle spitt in 3 triangles with barycenter.

Remarque first with use a copy of P1b function element and after we change 
de definition of the basic function. 





