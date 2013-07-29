This is where I should put stuff to read.

Building PLIB
// Apparently this is necessary to tell PLIB where to find my gl libraries
 sudo ln -s /usr/lib/libXi.so.6 /usr/lib/libXi.so
  --Courtesy of this forum
  http://hi.baidu.com/szmneo/item/400e02ac59c477ae29ce9de2

Boost libraries:
sudo apt-get install libboost-dev

For jpeglib.h:
sudo apt-get install libjpeg62-dev

For plib libaries to work on 32 bit linux
-Run makePlibLinks.sh
