#ifndef TIMER_H_
#define TIMER_H_

#include <boost/numeric/ublas/vector_sparse.hpp>
#include <GL/glut.h>
#include <iostream>
#include <plib/sg.h>

#include <boost/chrono.hpp>
#include <boost/chrono/time_point.hpp>
#include <boost/chrono/system_clocks.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <cmath>

#include "../ShapeFiles/Moveable.h"

using namespace boost;

class Timer
{
  private:
    boost::chrono::system_clock::time_point start;
    // boost::posix_time::time_duration duration;
    boost::chrono::duration<double> duration; 
    // boost::chrono::duration duration;
    
  public:
    void startTiming();
    void stopTiming();
    boost::chrono::duration<double> getDuration();
};
#endif
