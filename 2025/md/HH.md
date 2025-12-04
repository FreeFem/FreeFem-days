## An Artificial Neural Network for Predicting the Roles of Contacting Bodies in Computational Contact Mechanics 
## Dr Houssam Houssein    houssam.houssein@airthium.com

## Airthium 


*Abstract:* The resolution of mechanical contact between two bodies using numerical methods, such as the finite element method (FEM), requires the prior assignment of contacting roles--slave or master--to each body, which is performed before solving the contact problem. When a body is designated as the slave, non-penetration constraints are imposed to prevent penetration into the master. To ensure accurate results, each body must be assigned the correct role, a task that is complex and often impractical to perform manually. To address this, artificial neural networks (ANNs) are proposed in this presentation for three 2D mechanical contact problems to automatically assign the appropriate roles, thereby improving accuracy while reducing computational cost compared to the symmetric formulation, where no role assignment is required. A data generation strategy, essential for training the ANNs and implemented using FreeFEM, is also presented.