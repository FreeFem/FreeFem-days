## Indefinite direct solver for primal-dual interior method by FreeFEM script

### *Atsushi Suzuki*

##  D3 Center Osaka University/RIKEN Center for Computational Science

*Abstract*: A variational inequality problem is a basis formulation of contact problem and inequality constraint can be treated by Lagrange multiplier.
 The primal-dual active set algorithm iterates updating procedure for both multiplier and active/inactive parts 
 of the computational domain to resolve nonlinear and noncontinuous conditions. On the other hand, interior point method relaxes 
 the constraint by introducing the logarithmic barrier function with a sequence of parameters. 
 To apply a Newton iteration for finding a minimizer of the augmented energy with a fixed parameter, indefinite Hessian matrix 
 with primal and dual unknowns need to be considered. The second block the the coefficient corresponding to dual unknowns becomes 
 closed to be singular due to relaxed complementary conditions on the primal and dual unknowns. Standard software packages 
 use powerful direct solver for the matrix system with high condition number on primal unknowns after elimination of dual unknowns.
  However, the global indefinite system keeps to be invertible whenever iteration procedure is progressing. A FreeFEM script solves 
  directly primal-dual system will be shown.
