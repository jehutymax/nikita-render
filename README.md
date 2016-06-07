Nikita is a (admittedly rudimentary) **ray tracer** (not a path tracer just yet, but that is coming) that I've been writing.

![Stanford Bunny (69k triangles), Hall Reflection model](http://www.campos.cc/images/nikita/bunny_on_plane.png "Bunny on Plane")

### Features

#### Input & Output
* Reads XML Scene files
* Writes PNG image files

#### Geometry
* Spheres
* Cylinders
* Triangle Meshes read from SMF files

#### Acceleration
* Bouding Volume Hierarchy (BVH) tree

#### Shading
* Hall Shading Model:
	- Shadows
	- Reflections
	- Refractions
* Image Textures
* Noise-based procedural textures

#### Lights
* Point Lights
* Spot Lights


### References
1. Pharr, M. and Humphreys, G., 2004. [Physically based rendering: From theory to implementation.](http://amzn.to/1r5i6lA) Morgan Kaufmann.
2. Suffern, K.G. and Suffern, K., 2007. [Ray Tracing from the Ground up.](http://amzn.to/1P9HXyG) AK Peters.
