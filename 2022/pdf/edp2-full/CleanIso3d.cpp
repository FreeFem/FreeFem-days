#include "ff++.hpp"
#include "AFunction_ext.hpp"
#include <array>
/*
template< class MMesh >
AnyType LapSmothing< MMesh >(Stack stack,const MMesh * const & pTh, long & const inter,  double & const omega)
{
  MeshPoint *mp(MeshPointStack(stack)), mps = *mp;
  MMesh *pTh = GetAny< MMesh * >((*eTh)(stack));
  ffassert(pTh);
  MeshPoint *mpp(MeshPointStack(stack));

  typedef typename MMesh::Element T;
  typedef typename MMesh::BorderElement B;
  typedef typename MMesh::Vertex V;
  MMesh &Th = *pTh;

  // arguments
  KN< long > zzempty;
  KN< long > nrT(arg(1, 5, stack, zzempty));
  KN< long > nrB(arg(2, 6, stack, zzempty));
  double precis_mesh(arg(3, stack, 1e-7));
  long orientation(arg(4, stack, 1L));
  bool cleanmesh(arg(7, stack, true));
  bool removeduplicate(arg(8, stack, false));
  bool rebuildboundary=false;//(arg(9, stack, false));

  KN< int > takemesh(Th.nv);
  takemesh = 0;
  if (verbosity > 5)
    cout << "before movemesh: Vertex " << Th.nv << " Triangles " << Th.nt << " Edges " << Th.nbe
         << endl;

  // map to change lab
  ffassert(nrT.N( ) % 2 == 0);
  ffassert(nrB.N( ) % 2 == 0);

  map< int, int > mapBref;
  for (int i = 0; i < nrB.N( ); i += 2)
    if (nrB[i] != nrB[i + 1]) mapBref[nrB[i]] = nrB[i + 1];

  map< int, int > mapTref;
  for (int i = 0; i < nrT.N( ); i += 2)
    if (nrT[i] != nrT[i + 1]) mapTref[nrT[i]] = nrT[i + 1];

  // copy to clean proprely the stack
  V *v = new V[Th.nv];
  T *t = new T[Th.nt];
  B *b = new B[Th.nbe];

  // apply the geometric transfo and copy vertices
  if( Th.nt ==0 && Th.nbe==0)
  { // mesh with only vertex
      for (int iv = 0; iv < Th.nv; ++iv)
      {
    
          R3 P;
          P = Th(iv);
          mpp->set(P.x,P.y,P.z);
          if (xx)
              v[iv].x = GetAny< double >((*xx)(stack));
          else
              v[iv].x = mpp->P.x;
          if (yy)
              v[iv].y = GetAny< double >((*yy)(stack));
          else
              v[iv].y = mpp->P.y;
          if (zz)
              v[iv].z = GetAny< double >((*zz)(stack));
          else
              v[iv].z = mpp->P.z;
      }
  }
  else
  {
  // loop over elements
  for (int it = 0; it < Th.nt; ++it) {
    const T &K(Th.elements[it]);
    for (int iv = 0; iv < T::nv; iv++) {
      int i = Th.operator( )(K[iv]);
      if (takemesh[i] == 0) {
        mpp->setP(&Th, it, iv);
        if (xx)
          v[i].x = GetAny< double >((*xx)(stack));
        else
          v[i].x = mpp->P.x;
        if (yy)
          v[i].y = GetAny< double >((*yy)(stack));
        else
          v[i].y = mpp->P.y;
        if (zz)
          v[i].z = GetAny< double >((*zz)(stack));
        else
          v[i].z = mpp->P.z;
        v[i].lab = Th.vertices[i].lab;
        takemesh[i] = takemesh[i] + 1;
      }
    }
  }
  }

  // loop over border elements
  for (int ibe = 0; ibe < Th.nbe; ++ibe) {
    const B &K(Th.be(ibe));
    for (int j = 0; j < B::nv; j++) {
      int i = Th.operator( )(K[j]);
      if (takemesh[i] == 0) {
        mpp->set(Th.vertices[i].x, Th.vertices[i].y, Th.vertices[i].z);
        if (xx) v[i].x = GetAny< double >((*xx)(stack));
        else v[i].x = mpp->P.x;
        if (yy) v[i].y = GetAny< double >((*yy)(stack));
        else v[i].y = mpp->P.y;
        if (zz) v[i].z = GetAny< double >((*zz)(stack));
        else v[i].z = mpp->P.z;
        v[i].lab = Th.vertices[i].lab;
        takemesh[i] = takemesh[i] + 1;
      }
    }
  }

  // copy elements
  for (int it = 0; it < Th.nt; ++it) {
    const T &K = (Th[it]);
    int iv[T::nea];
    for (int i = 0; i < T::nea; i++) iv[i] = Th.operator( )(K[i]);
    // cp element
    t[it].set(v, iv, K.lab);
  }
  // copy border elements
  for (int ibe = 0; ibe < Th.nbe; ++ibe) {
    const B &K(Th.be(ibe));
    int iv[B::nea];
    for (int i = 0; i < B::nea; i++) iv[i] = Th.operator( )(K[i]);
    // cp border element
    b[ibe].set(v, iv, K.lab);
  }
  // build the moved mesh and apply option
  MMesh *T_Th = new MMesh(Th.nv, Th.nt, Th.nbe, v, t, b, cleanmesh, removeduplicate,
                          rebuildboundary, orientation, precis_mesh);

  // apply the change of elements and borderelements references
  if (nrT.N( ) > 0)
    for (int i = 0; i < T_Th->nt; i++) {
      const T &K(T_Th->elements[i]);
      int lab = K.lab;
      T_Th->elements[i].lab = ChangeLab(mapTref, lab);
    }
  if (nrB.N( ) > 0)
    for (int i = 0; i < T_Th->nbe; i++) {
      const B &K(T_Th->be(i));
      int l0, l1 = ChangeLab(mapBref, l0 = K.lab);
      T_Th->be(i).lab = l1;
    }
  T_Th->BuildGTree( );
  finalize(T_Th);
   /*if(verbosity>5) cout << " bb box "<< T_Th->Pmin << " ; " << T_Th->Pmax <<endl;
  Add2StackOfPtr2FreeRC(stack, T_Th);
  *mp = mps;
  return T_Th;
}
*/

template<class R,class A=R,class CODE=E_F_F0s_<R,A> >
class  OneOperator1spref_ : public OneOperator {
    typedef  R (*func)(Stack stack, const A &) ;
    func  f;
    public:
    E_F0 * code(const basicAC_F0 & args) const
    {     if ( args.named_parameter && !args.named_parameter->empty()  )
    CompileError( " They are used Named parameter ");
     return  new CODE(f,t[0]->CastTo(args[0]));}
    OneOperator1spref_(func  ff,int ppref=10):
    OneOperator(map_type[typeid(R).name()],map_type[typeid(A).name()]),f(ff){pref=ppref;}
};

 long  SortIsolines(Stack stack, KNM<double>*const &ppoints,KN<long>*const &pbe,double const & eps,KN< KN< long > > * const & ppbp)
{
    
    typedef   v_fes3::FESpace FESpace;
    typedef double K;
    double hseuil= eps;// ????
    typedef Mesh3::Element Element;
    typedef Element::Vertex Vertex;
    KNM<double> &pts=*ppoints;
    double * ppts = pts;
    int istep=0,jstep=0,nbpts =0;
    if( pts.N() == 3) { nbpts=pts.M(); istep = pts.step * pts.shapei.step;jstep = pts.step * pts.shapej.step;}
    else if ( pts.M() == 3) { nbpts=pts.N(); istep = pts.step * pts.shapej.step ;jstep = pts.step * pts.shapei.step ;}
    
    cout << " SortIsolines  nbpts = "<<  nbpts << " istep =" << istep << " " << jstep<< endl;
    ffassert( istep && jstep  && nbpts) ;
    KN<long> &be=*pbe;
    
    R3 Pmin(1e100,1e100,1e100),Pmax(-1e100,-1e100,-1e100);
    
    KN_<double>  X(ppts,nbpts,jstep),Y(ppts+istep,nbpts,jstep),Z(ppts+2*istep,nbpts,jstep);
    
    int nbpx = be.N()*2;
    KN<KN<long>> *prenum =ppbp;
    
    KN<Vertex> Pf(nbpx);
    int nbi =be.N()-1; //  nb de iso
    for(int ib=0; ib< nbi; ++ib)
    for(int ie=0; ie<2; ++ie)
    {
        int i = be[ib+ie]-ie;
        ffassert(i>=0 && i < nbpts);
        R3 P(X(i),Y(i),Z(i));
        ffassert(Z(i) <= 1.);
        Pmin.x = Min(Pmin.x,P.x);
        Pmin.y = Min(Pmin.y,P.y);
        Pmin.z = Min(Pmin.z,P.z);
        Pmax.x = Max(Pmax.x,P.x);
        Pmax.y = Max(Pmax.y,P.y);
        Pmax.z = Max(Pmax.z,P.z);
    }
    cout << " bb  : ("<< Pmin << ") (" << Pmax <<")"<<  endl;
    EF23::GTree<Vertex> *gtree=new EF23::GTree<Vertex>(Pf,Pmin,Pmax,0);
    hseuil = 2./gtree->coef; //  seuil minimal dans gtree
    if(verbosity>9) cout << " hseuil minimal "<< hseuil << endl;
    long ihseuil=gtree->coef*hseuil;
    ffassert(ihseuil);
    int nbarc =0,nbp=0;
    //verbosity=10;
    typedef std::array<int,2> Arc;
    vector<Arc> arc;
    double hseuil2=hseuil*hseuil;
    int iempty=0;
    for(int ib=0; ib< be.N()-1; ++ib)
    {
        int ip[2];
        for(int ie=0; ie<2; ++ie)
        {
            int i = be[ib+ie]-ie;
            int i2 = ie;
           
            R3 P(X(i),Y(i),Z(i));
            Vertex * pvi=gtree->ToClose(P,hseuil,true);
            
            if(!pvi && nbp)
            {
                pvi=gtree->NearestVertex(P,true);
                int j = pvi - Pf;
                double l2 = R3(*pvi,P).norme2();
                if(l2 > hseuil2) pvi=0;
                else    if( l2 < hseuil2) {
                    if(verbosity>9) cout << " bug in ToClose ??? "  << " "<< i  << " == " << j << " :  " << P << " j " << Pf[j] << " / " << l2 <<endl;
                }
                
            }
            if(0) //  force brute ???
                for(int j=0; j< nbp; ++j)
            {
                double l2 = R3(P,Pf[j]).norme2();
                if( l2 < hseuil2) {
                    if(verbosity>9) cout << " bug  "<< i  << " == " << j << " :  " << P << " j " << Pf[j] << " / " << l2 <<endl;
                }
                
            }
            if(! pvi)
            {
                ffassert(nbp<nbpx);
                Pf[nbp].x = P.x;
                Pf[nbp].y = P.y;
                Pf[nbp].z = P.z;
                Pf[nbp].lab = i ;// nb sommet de creation ..
                gtree->Add(Pf[nbp]);
                pvi = &Pf[nbp];
                nbp++;
            }
            
            ip[i2]= pvi- (Vertex *) Pf;
        }
        
        int i0= ip[0],i1=ip[1];
        if( i0 != i1)
        {
            Arc A={i0,i1};
            arc.push_back(A);
        }
        else iempty++;
        
    }
    if(verbosity)
        cout << " nb arc vide "<< iempty << endl;
    ffassert(arc.size()+iempty == be.N()-1);
    // in ArcF(i0,i1) i \in [i0,i1], if i%5==4 => i/5 numéro thétraèdre else (i%5) numéro de face dans le tétraèdre i/5
    
    delete gtree;
    sort(arc.begin(),arc.end());
    int nbua=unique(arc.begin(),arc.end())-arc.begin();
    if(verbosity>9) cout << " nbua "<< nbua << " " << arc.size() << " nbp " << nbp << " / " << nbpx << endl;
    arc.resize(nbua);
    vector<vector<int>> av(nbp);
    if(verbosity>9) cout << " nb arc = "<< arc.size() << " nbp " << nbp << endl;
    for(int i=0;i<arc.size(); ++i)
    {
        if(verbosity>99) cout << " arc "<< i <<" " <<  arc[i][0] <<" " << arc[i][1] << endl;
        for(int j=0;j<2; ++j)
        {
            
            av[arc[i][j]].push_back(2*i+j);
        }
    }
    if(verbosity>9)
    {
        int bizzare=0;
        for(int i=0; i< nbp ; ++i)
        {
            if( av[i].size()!=2 && av[i].size())
            {
                bizzare++;
                int ibb = Pf[i].lab/2, i22 = Pf[i].lab%2;
                cout << " -- point ?? "<< i << " " << (R3) Pf[i] << " (" << ibb << "," << i22 << ") nb arc = "<< av[i].size() << " : ";
                for(int j=0; j < av[i].size() ; ++j )
                cout << av[i][j] << " " ;
                cout << endl;
                
            }
        }
    }
    // recherche des banches du graphe et stockage dans pbb
    vector<int> ca(arc.size(),0);
    vector<int> bb,pbb;
    vector<pair<double,vector<int>>> ppbv;
    bb.reserve(arc.size());// arc et le sans par brancj
    pbb.push_back(0);
    auto which = [&arc] (int a,int v) {ffassert( arc[a][0] ==v ||  arc[a][1] ==v); return arc[a][0] ==v ? 0 : 1 ;};
    auto nexta = [&arc,&av] (int i,int a) {
        int aa=-1;
        if(av[i].size() ==2)
        {
            if(verbosity>99) cout << i << " " << a << " : " << av[i][0]/2 << " "<< av[i][1]/2 << endl;
            int k = av[i][1]/2 !=  a ;
            
            ffassert( av[i][1-k]/2 == a);
            aa = av[i][k];
        }
        return aa;};
    auto branch = [&ca , &arc,&Pf,  &which, &nexta , &av,&ppbv, &bb,&pbb](int a,int i) {
        // depart a, sommet ai de a
        // for  k=[pb[ib], bbe[ib+1][ indice de la branch ib dans bb...
        //     bb[k] = 2*a+sens  arc de la branche et sens de parcours ...
        if( ca[a]) return 0;//  deja faire on saute ..
        vector<int> pbv;
        int ia=which(a,i);
        int lg=1;
        // depart de branche ..
        double lgb =0;
        
        bb.push_back(2*a+ia);
        ca[a]=1;
        int v0 =i;
        pbv.push_back(i);
        int v = arc[a][1-ia];
        
        R3 Pv= Pf[v],Pv0=Pv,Pvv=Pv;;
        if(verbosity>9) cout << " branch "<< a << " " << ia << " s= "<< i << " o= " << av[i].size() << " -> "<< v << " " ;
        while(1) {
            pbv.push_back(v);
            int aa = nexta(v,a);
            if(aa <0) { if(verbosity>9) cout << " fin branch s= " <<v << " o=  " << av[v].size()
                              << " lg = " << lg << " ||" << (Pv0-Pv).norme()<< "||" << pbv.size() << endl;
                break;
            }
            if( ca[aa/2]) { if(verbosity>9 ) cout << " fin loop "<< aa/2 << " s= " << v <<  " o=" << av[v].size()   << " lg = " << lg << " lgb " << lgb << " " << pbv.size() << endl;
                break;
            }
            a =aa/2;
            ia = aa%2;
            bb.push_back(a*2+1-ia);
            int vv = arc[a][1-ia];
             Pvv= Pf[vv];
        
            lgb += (Pvv-Pv).norme();
            Pv = Pvv;
            if(verbosity>99) cout<< " -> " << vv << endl;
            v = vv;
            lg++;
            ca[a]=1;
        }
        if(lg <= 1) { lg=0; }
        if(lg>1)
        {
        
          //ffassert(lg == pbv.size());
          pbb.push_back(bb.size());// fin de la branche
         double zz = Pv.z;
          ppbv.push_back(make_pair(zz,pbv));
        }
        return lg;//  longueurs de la branch ..
    };
    
    int nbb =0, lbb=0;
    for( int i=0; i<nbp; ++i)
    if( av[i].size()>0 && av[i].size()!=2 ) {
        // un depart possible
        
        if(verbosity>9) cout << " depart a "<< av[i].size() << " a= " <<av[i][0]/2<< " " << endl;
        for(int j=0; j<av[i].size();++j)
        nbb += branch(av[i][j]/2,i)>0;
    }
    
    //  recheche des autre depart (les boucle)
    for( int aa=0; aa<arc.size(); ++aa)
    {
        if(ca[aa]==0) // no colorie , un depart possible dans le 2 sens ????
        {
            if(verbosity>9) cout << " depart boucle " << aa << arc[aa][0] << endl;
            nbb += branch(aa,arc[aa][0])>0;
        }
    }
    
    cout << " nba = "<< arc.size() << " nbp "<< nbp << endl;
    cout << " nbranch = "<< nbb << " == " << pbb.size() -1 <<  endl;
    ffassert(nbb == pbb.size()-1);
    
    //  KNM<double> points(nbp+b.size(),3);
    // KN<long> benw;
    int nbe=0;
    int nbc=0;
    int nbkk=0;
    sort(ppbv.begin(),ppbv.end());
    prenum->resize(ppbv.size());
    KN< KN< long > >& Lp(*prenum);
     for(int b=0; b< ppbv.size();++b)
    {
        Lp[b].resize(ppbv[b].second.size());
        for(int i=0; i< ppbv[b].second.size(); ++i)
          Lp[b][i] = Pf[ppbv[b].second[i]].lab ;
    }
    long nb =ppbv.size();
    return nb;
    
}
KN<double> * isobord(Stack stack,pmesh  pTh,KN_<long> l)
{
    KN<double> * pr=0;
    if(pTh)
    {
        const Mesh &Th=*pTh;
         pr=new KN<double>(Th.nv);
        KN<double> & r =*pr;
        r = 1.;
        
        if( l.N())
        {
            set<long> sl;
            for(int i=0; i< l.N();++i)
            sl.insert(l[i]);
            for(int i=0; i<Th.neb; ++i)
              if( sl.find(Th.be(i).lab) == sl.end())
              {
                  r[Th(Th.be(i)[0])]=0;
                  r[Th(Th.be(i)[1])]=0;
              }
        }
        else
            for(int i=0; i<Th.neb; ++i)
            {
             r[Th(Th.be(i)[0])]=0;
             r[Th(Th.be(i)[1])]=0;
            }

        Add2StackOfPtr2Free(stack, pr);
    }
    return pr;
}
KN<double> * isobord(Stack stack,pmesh const & pTh)
{
    KN<long> l(0);
    return isobord(stack,pTh,l);
}
KN<double> * isobord(Stack stack,pmesh const & pTh,long const & ll)
{
    KN<long> l(1,ll);
    return isobord(stack,pTh,l);
}
template<class Mesh>
double dist(Stack stack,Mesh * const &pTh)
{ // version 3d sep 2021
    typedef typename Mesh::RdHat RdHat;
    typedef typename Mesh::Rd Rd;
    typedef typename Mesh::Element Element;
    if(pTh == 0) return 0.;
    RdHat PHat;
    bool outside;

    MeshPoint & mp = *MeshPointStack(stack);

    if((const void *) pTh == (const void *) mp.Th) return 0.;
    Rd P(&mp.P.x);
    const Element  * K=pTh->Find(P,PHat,outside);
    if (!outside)
        mp.set(*pTh,P,PHat,*K,K->lab);
    else {
        Rd Pb = (*K)(PHat);
        return (P-Pb).norme();
    }
    return 0.;
}
template<class Mesh>
double signeddist(Stack stack,Mesh * const &pTh)
{ // version 3d sep 2021
    typedef typename Mesh::RdHat RdHat;
    typedef typename Mesh::Rd Rd;
    typedef typename Mesh::Element Element;
    if(pTh == 0) return 0.;
    RdHat PHat;
    bool outside;

    MeshPoint & mp = *MeshPointStack(stack);

    if((const void *) pTh == (const void *) mp.Th) return 0.;
    Rd P(&mp.P.x);
    const Element  * K=pTh->Find(P,PHat,outside);
    if (!outside)
        mp.set(*pTh,P,PHat,*K,K->lab);
    else {
        // normal to K ???
        Rd Pb = (*K)(PHat);
        Rd e =P-Pb, Nt=K->NormalTUnitaire();
        double en = e.norme();
        return (e,Nt) >0 ? en : -en;
    }
    return 0.;
}
static void inittt( ) {
    Global.Add("SortIsolines", "(",new OneOperator4s_<long,KNM<double> *,KN<long> *,double,KN< KN< long > >* >(SortIsolines));
    Global.Add("isobord", "(",new OneOperator1spref_<KN<double> *,pmesh>(isobord));
    Global.Add("isobord", "(",new OneOperator2s_<KN<double> *,pmesh,long>(isobord));

     Global.Add("dist", "(", new OneOperator1spref_<double,pmeshL,E_F_F0s_<double,pmeshL,E_F0mps> >(dist));// sep 2021  FH function distance to mesh
      Global.Add("dist", "(", new OneOperator1spref_<double,pmeshS,E_F_F0s_<double,pmeshS,E_F0mps> >(dist));// sep 2021  FH function distance to mesh
    Global.Add("signeddist", "(", new OneOperator1spref_<double,pmeshL,E_F_F0s_<double,pmeshL,E_F0mps> >(signeddist));// sep 2021  FH function distance to mesh
      Global.Add("signeddist", "(", new OneOperator1spref_<double,pmeshS,E_F_F0s_<double,pmeshS,E_F0mps> >(signeddist));// sep 2021  FH function distance to mesh

    Global.Add("dist", "(", new OneOperator1spref_<double,pmesh3,E_F_F0s_<double,pmesh3,E_F0mps> >(dist));// sep 2021  FH function distance to mesh
    Global.Add("dist", "(", new OneOperator1spref_<double,pmesh,E_F_F0s_<double,pmesh,E_F0mps> >(dist)); // oct 2017 FH  FH function distance to mesh


}


LOADFUNC(inittt);
