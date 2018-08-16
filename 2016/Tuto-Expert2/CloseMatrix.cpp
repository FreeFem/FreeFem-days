// Example C++ function "Trivail matrix test .. ", dynamically loaded into "load.edp"
// ---------------------------------------------------------------------
// $Id$


#include "ff++.hpp"

static bool debug = false;

class R2close {
public:
    double *data;// minimal points+ delta
    typedef double  * Point;
    int n,nx,offset;
    Point *P;
    const double EPSILON;
    double x0,y0,x1,y1, coef;// boundin box
    R2close() :data(0), n(0), nx(1000000), P(new Point[nx]),EPSILON(1e-6),offset(0)  {InitialiserListe();}
    R2close(double *dd,int mx,double eps=1e-6,int offsett=1) :
    data(dd), n(0), nx(mx), P(new Point[nx]),EPSILON(eps),offset(offsett)
    {InitialiserListe();}
    Point  operator[](int i) const { return P[i];}
    
    int N,m;  // le nombre de code = ncase()%n
    int *head; // pointeur tableau de dimension  m    contenant les tete de liste pour chaque code
    int *next; // pointeur tableau de dimension nx donnant le point suivant de mÍme code
    static const int NotaPoint =-1;
    void InitialiserListe()
    {
        if(data)
        {
            x0=data[0];
            y0=data[1];
            x1=data[2];
            y1=data[3];
            
        }
        else
        {
            x0=0;
            y0=1;
            x1=0;
            y1=1;
        }
        
        coef  = 1./max(x1-x0,y1-y0);
        if(verbosity>10)
            cout << "     bounding box ClosePoints  Pmin=[" << x0 << ", "<< y0 << "], Pmax=[ " << x1 << " " << y1 << "] " << "eps= " <<  EPSILON <<endl;
        
        N=max(sqrt(nx),10.);
        m=max(nx/10,100);
        next = new int[nx];
        head = new int[m];
        for(int i=0; i< m;++i)
            head[i]=NotaPoint;
    }
    int AddSimple(double *p)
    {
        double x = p[0], y=p[offset];
        assert(n<nx);
        P[n]=p;
        
        int k=ncase(x,y)%m;
        assert(k>=0);
        next[n]=head[k];
        head[k]=n;
        if(debug)   cout << "  AddSimple " << n << " <- " << k << " / " << x << " " << y << " / "<< offset << endl;
        return n++;
    }
private:
    Point * Exist(double *p) const
    {
        double x = p[0], y=p[offset];
        
        for(int i=0;i<n;++i)
            if(Equivalent(x,y,P[i]))
                return P+i;
        return 0;
    }
public:
    bool Equivalent(double x0,double y0,const Point Q) const
    {
        return  ((x0-Q[0])*(x0-Q[0])+(y0-Q[offset])*(y0-Q[offset])) < EPSILON*EPSILON;
    }
    
    Point * Exist(double x, double y,int k) const
    {
        for(int i=head[k%m] ; i!=NotaPoint; i=next[i] )
        {   //cout << i << " ";
            if(Equivalent(x,y,P[i]))
                return P+i;
        }
        return 0;
    }
    
    int Add(double *p)
    {
        Point *q=Exist(p);
        if( q) return  q-P; // numero du point
        else return AddSimple(p);
    }
    
    int  FindAll(double x,double y,int * p)
    {
        int nbp=0;
        if(debug)   cout << " Find " << x << " "<< y  << " " << EPSILON << " " << ncase(x,y) << ": " ;
        // double x = p[0], y=p[offset];
        double eps=EPSILON/2;
        Point *q=0;
        int kk[9],k=0,nc;
        for(int i=-1;i<2;++i)
            for(int j=-1;j<2;++j)
            {
                nc = ncase(x+eps*i,y+eps*j);
                //cout <<x+eps*i << " " << y+eps*j << " " << nc << " . ";
                if(nc>=0)
                    for(int i=0;i<k;++i) // remove double cas e..
                        if(kk[i]==nc)
                        {
                            nc=-1;
                            break;
                        }
                if(nc>=0) kk[k++]=nc;
            }
        if(k>4) {
            cout << "   ClosePoints: FindAll Bug ??? : " << k << " : " ;
            for(int i=0; i<k;++i)
                cout << " " << kk[i];
            cout << endl;
        }
        assert(k <=4);
        for(int i=0;i<k;++i)
        {
            int k=kk[i];
            for(int i=head[k%m] ; i!=NotaPoint; i=next[i] )
            {
                if(Equivalent(x,y,P[i]))
                    p[nbp++]=i;
            }
        }
        return nbp;
    }
    
    Point * Find(double x,double y)
    {
        if(debug)   cout << " Find " << x << " "<< y  << " " << EPSILON << " " << ncase(x,y) << ": " ;
        // double x = p[0], y=p[offset];
        double eps=EPSILON/2;
        Point *q=0;
        int kk[9],k=0,nc;
        for(int i=-1;i<2;++i)
            for(int j=-1;j<2;++j)
            {
                nc = ncase(x+eps*i,y+eps*j);
                //cout <<x+eps*i << " " << y+eps*j << " " << nc << " . ";
                if(nc>=0)
                    for(int i=0;i<k;++i) // remove double cas e..
                        if(kk[i]==nc)
                        {
                            nc=-1;
                            break;
                        }
                if(nc>=0) kk[k++]=nc;
            }
        if(k>4) {
            cout << "   ClosePoints: Bug ??? : " << k << " : " ;
            for(int i=0; i<k;++i)
                cout << " " << kk[i];
            cout << endl;
        }
        assert(k <=4);
        for(int i=0;i<k;++i)
        {
            q=Exist(x,y,kk[i]) ;
            if(debug)  cout<< " " << kk[i]  ;
            
            if(q) break;
        }
        if(debug)    cout << " q= "<< q << endl;
        if( q)
            return  q; // numero du point
        else return 0;
    }
    Point * Find(double *p)
    {
        return Find(*p,*(p+offset));
    }
    int AddOpt(double *p)
    {
        Point *q=Find(p);
        if( q)
            return  q-P; // numero du point
        else
            return AddSimple(p);
    }
    
    long ncase(double x,double y)
    {
        if(x <x0 || x >=x1 || y <y0 || y >=y1 ) return -1; // dehors
        else return  long((x-x0)/EPSILON/2) +  long((y-y0)/EPSILON/2)*N;// indice de la case contenant (x,y).
    }
    ~R2close()
    {
        delete []P;
        delete [] head;
        delete [] next;
    }
    // pas de copie de  cette structure
private:
    R2close(const R2close&);
    R2close&operator=(const R2close&);
};

double dist2(int n,double *p,double *q)
{
    double s=0;
    for(int i=0; i<n;++n)
        s += (q[i]-p[i])*(q[i]-p[i]);
    return s;
}
long Hcode(int n,double eps,double *p,double *p0);

MatriceMorse<double> * BuildMatrix(const Mesh & Th,double eps)
{
    debug = (verbosity>999);
    int np=Th.nt;
    typedef  Mesh::Rd Rd;
    const int d=Rd::d;
    Rd g=Rd::diag(1./(d+1.));
    KNM<double>  P(d,np);
    for(int k=0; k<Th.nt; ++k)
    {
        Rd A=Th[k](g);
        for(int i=0; i<d; ++i)
            P(i,k)=A[i];
    }
    
    double *p= &P(0,0), *p01= & P(0,1) , *p10 = & P(1,0);
    cout << " " << p10 - p << " " << p01 -p << endl;
    int offset01= P.step*P.shapej.step;
    int offset10= P.step*P.shapei.step;
    double data[4];
    data[0] = data[2] = p[0];
    data[1] = data[3] = p[offset01];
    for(int i=0,k=0; i<np; ++i, k+= offset10)
    {
        data[0] = min(data[0], p[k]);
        data[2] = max(data[2], p[k]);
        data[1] = min(data[1], p[k+offset01]);
        data[3] = max(data[3], p[k+offset01]);
        
    }
    double eps2 = eps+eps;
    data[0] -=  eps2;
    data[2] +=  eps2;
    data[1] -=  eps2;
    data[3] +=  eps2;
    KN<int> lp(np);
    R2close SP(data,np,eps,offset10);
    for (int i=0; i<np;++i)
        SP.AddSimple(&P(0,i));
    
    int nnz =0;
    for (int j=0; j<np;++j)
        nnz +=SP.FindAll(P(j,0),P(j,1),lp);
    KN<int> plg(np+1),jj(nnz);
    KN<double> a(nnz);
    int l=0;
    for (int i=0; i<np;++i)
    {
        int nlp =SP.FindAll(P(i,0),P(i,1),lp);
        plg[i]=l;
        for(int k=0; k<nlp;++k,++l)
        {
            int j=lp[k];
            jj[l] =j;
            R2 Ai(P(0,i),P(1,i)), Aj(P(0,j),P(1,j)), AiAj(Ai,Aj);
            a[l] = AiAj.norme()-eps;
        }
        
    }
    plg[np]=l;
    return new MatriceMorse<double>(np,np,nnz,false,a,plg,jj);
  
}


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
    MatriceMorse<R>  * amorse =  BuildMatrix(*pTh, eps);
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
