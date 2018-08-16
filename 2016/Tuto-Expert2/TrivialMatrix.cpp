// Example C++ function "Trivail matrix test .. ", dynamically loaded into "load.edp"
// ---------------------------------------------------------------------
// $Id$


#include "ff++.hpp"

class CloseMatrix :  public E_F0 { public:
  typedef Matrice_Creuse<R> * Result;
  Expression expTh,expeps;
  CloseMatrix(const basicAC_F0 & args)
  {   
    
    args.SetNameParam();
    expTh= to<pmesh>(args[0]);
    expeps= CastTo<double>(args[1]);
    
  }
  
  ~CloseMatrix()
  {  
  }     
  
  static ArrayOfaType  typeargs() { return  ArrayOfaType(atype<pmesh>(),atype<double>());}
  static  E_F0 * f(const basicAC_F0 & args){ return new CloseMatrix(args);}
  AnyType operator()(Stack s) const ;
  
};

/*
   Juste a trivial test buil dasn diagobal matrix with 4 value (esp)
 */



AnyType CloseMatrix::operator()(Stack stack) const
{
   Matrice_Creuse<R> * sparse_mat = new Matrice_Creuse<R>;
    cout <<"Matrice_Creuse<R> * " << sparse_mat << endl;
    // the code Add2StackOfPtr2Free is to remove memory after the usage of the pointeur ..
    pmesh pTh= GetAny<pmesh>((*expTh)(stack));
    double eps=GetAny<double>((*expeps)(stack));
   Add2StackOfPtr2Free(stack,sparse_mat);
   R ta[]={eps,eps,eps,eps} ; //
   MatriceMorse<R>  * amorse =new MatriceMorse<R>(4,ta);
   sparse_mat->Uh=UniqueffId();
   sparse_mat->Vh=UniqueffId();
   sparse_mat->A.master(amorse);
   sparse_mat->typemat=(amorse->n == amorse->m) ? TypeSolveMat(TypeSolveMat::GMRES) : TypeSolveMat(TypeSolveMat::NONESQUARE); //  none square matrice (morse)  *mp=mps;
  
   if(verbosity>3) { cout << "   CloseMatrix : " << amorse << endl;}
    return sparse_mat;
}

 static void Load_Init()
   {
     cout << " lood: init CloseMatrix " << endl;
     Global.Add("CloseMatrix","(", new OneOperatorCode<CloseMatrix >( ));
   }
LOADFUNC(Load_Init)
