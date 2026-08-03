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
  make && make test && ./build/Tests && ./build/PhysicsSandbox --record

  // What will this config do, without running it?
  ./build/PhysicsSandbox --config configs/cold-collapse.properties --print-setup

  // Sample, render the best one, upload it, print the link - one command
  tools/autopost --archetype chaotic --runs 8
  tools/autopost --resume batches/<batch> --dry-run   // just pick and plan

  // Sample a batch of variations, rank them, render the winners
  tools/run_batch.py sample --archetype chaotic --runs 12
  tools/run_batch.py report batches/<batch> --contact-sheet --reel
  tools/run_batch.py render batches/<batch> --exclude chaotic-02 --top 3 --res 1440p

  // Put the first 60s of a run on YouTube, described from its own run report
  tools/youtube-upload WorthyVideos/latest.mp4 60s --dry-run   # see the metadata first
  tools/youtube-upload WorthyVideos/latest.mp4 60s
  tools/youtube-upload --status 81HrUpoqQLY                    # done processing yet?

Starting arrangements are data now, not code: see SCENARIOS.md for the scenario
types, the knobs worth turning, and how batching and scoring work. Recorded runs
write a .json report next to the video that is enough to reproduce them exactly,
and tools/YOUTUBE.md covers the one-time Google setup that uploading needs.

tools/autopost chains all of that together without asking anything: it samples
previews, picks a winner, renders it, cuts the clip where the merging finishes,
uploads it and prints the link. Every decision is a rule applied to the numbers
in the run reports, so it repeats. Three things it knows that are easy to get
wrong by hand:

--particles is per group, so the same number means 56k particles in a 3-group
scenario and 171k in an 8-group one, and render cost climbs superlinearly with
density. autopost asks for a total particle --budget and solves for the flag
from the ratio the preview measured.

Score is not enough to pick a winner. It rewards collapsing hard and building a
dominant body and ignores how long that takes, so it will happily choose a run
that is over in fifteen seconds. autopost measures each preview's arc - how far
in 95% of the merging is done - and takes the best score among the runs that
last at least --min-arc seconds. Watch for the compression note: scaling
particles up a long way from the preview scales the merge settings with them,
so the rendered arc comes out shorter than the preview's.

A render that is going to fail does not look slow at the start - it starts at
3.5s a frame and decays to fifteen minutes a frame. autopost watches the frame
rate against --stall and --deadline, and kills the whole process group, because
killing the wrapper alone leaves the simulator running on nine cores forever.
 
  
Turning output images into a video:

     ffmpeg  -i outFrame%05d.jpg -framerate 1 -c:v libx264 -crf 18 -pix_fmt yuv420p -vf scale=-2:1440:flags=neighbor -r 30 ../WorthyVideos/current.mp4 && vlc ../WorthyVideos/current.mp4
     
     
 Full Build Time as of 2020-06-07: 2m20s

  
