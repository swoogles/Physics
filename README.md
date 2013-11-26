Physics
=======

Physics Simulations and Sandbox
======= 

This is my main personal project. The one I go to when I've had a rough day, I'm feeling lonely, or I just need to create something.

When it was originally conceived, I was only planning to figure out some graphics basics. At that point, I wasn't even using any of the OpenGL primitives, but was drawing everything point-by-point just for the sake of going through that process myself. After I was able to draw simple shapes, I decided a long-term direction for the project as physics simulation software.

Over the next few months, I added many features:
-Physical properties for my shape objects
-Naive (n^2) force calculation methods for all active objects
-The ability to output every frame of a simulation for later compositing into videos
-Rudimentary saving/loading for lists of objects
-An Observer class for manipulating the vantage point when running simulations
-UI that allows
 -Pausing/Unpausing simulations
 -Slowing down/Speeding up simulations
 -Changing the viewing position
 -Creating new objects 
 
More recently, I've made more significant improvements, including:
- I added physical properties to my shape objects, naive force calculation methods to act on them,...
