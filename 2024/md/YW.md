## Optimal Flow Control for the Reconstruction of C. elegans Locomotion

## *Dr Yongxing Wang*

## University of Leeds


*Abstract:* C. elegans is a ~1 mm-long worm that exhibits rich locomotor behaviors in 3D, including bending, twisting, and stretching along its body. Understanding its locomotion is not only crucial for gaining insights into the C. elegans neuromuscular system but also provides foundational principles applicable to other biological organisms and engineering systems. At Leeds WormLab, we have recorded extensive 3D video footage of C. elegans, enabling us to extract trajectories (midline) of the worm’s motion. However, how the worm locally twists remains unknown.

To address this, I recently developed an optimal flow control model that incorporates angular velocity ω as a control variable on the worm’s surface, alongside the experimentally measured linear velocity u at the worm's midline. By minimising viscous dissipation during locomotion, this framework enables precise calculation of the worm’s local frame and the forces exerted by its muscles during locomotion. In this approach, the local frame is expressed as u+ω×r, where r is the radius of the body’s cross-section. Solving the optimal control problem allows us to compute the muscle forces and curvature as post-processing outputs, providing deeper insights into the biomechanics of C. elegans locomotion.

There are related animations of the simulation using FreeFem++: https://yongxingwang.github.io/rod/.