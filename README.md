Physics 
=======

This is my main personal project. I have been contributing with varying intensity over the last 7 years.
Once when I was trying to get my 1st real job, right out of school.
2 years later when I wanted to re-visit it with Git and some reasonable software project knowledge.
And 5 years later, after moving to CB, switching jobs, experimenting with multiple languages, getting into functional programming, and generally getting deeper into software.

When it was originally conceived, I was only planning to figure out some graphics basics. At that point, I wasn't even using any of the OpenGL primitives, but was drawing everything point-by-point and transformation-by-transformation just for the sake of going through that process myself. After I was able to draw simple shapes, I decided a long-term direction for the project as physics simulation software.

Over the next few months, I added many features:
- Physical properties for my shape objects
- Naive (n^2) force calculation methods for all active objects
- The ability to output every frame of a simulation for later compositing into videos
- Rudimentary saving/loading for lists of objects
- An Observer class for manipulating the vantage point when running simulations
- UI that allows
 - Pausing/Unpausing simulations
 - Slowing down/Speeding up simulations
 - Changing the viewing position
 - Creating new objects 
 
More recently, I've made more significant improvements, including:
- Removed almost all global variables
- Expanded physical properties of my shape objects
- n log(n) force calculation method for active objects

Future improvements and additions
- Use Boost serialization libraries to get more formal saving/loading capabilities.
- Improve Makefiles
- Make it easier to tweak Quadrant attributes.
- Output straight to video file and skip intermediate image stage
- Parallelize calculation code.
- Get rid of sgVec's and use a more popular and mature vector library, most likely Boost.

Installation requirements:
sudo apt install libjpeg-dev
  -Correction:
  sudo apt install libjpeg8-dev
	pacman -S libjpeg9

  // Some combination of these??
sudo apt-get install libgraphicsmagick++1-dev
sudo apt-get install libgraphicsmagick1-dev


sudo pacman -S plib boost
sudo pacman -S grc

Important Commands
  // This is the command I should be executing as I develop
  make && make test && ./build/Tests && ./build/PhysicsSandbox r 1
 
  
  
