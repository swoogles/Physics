#include "Timer.h"

void Timer::startTiming()
{
  // Clock::time_point t1 = Clock::now();
  start = boost::chrono::system_clock::now();
}

void Timer::stopTiming()
{
  // Clock::time_point t1 = Clock::now();
  boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
  std::cout << "Process took: " << sec.count() << " seconds\n";
}
  // 
  // //Chrono stuff
  // boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();
  // for ( long i = 0; i < 10000000; ++i )
  //   std::sqrt( 123.456L ); // burn some time

  // boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
  // std::cout << "took " << sec.count() << " seconds\n";

  // typedef boost::chrono::duration<long, boost::ratio<60> > minutes;
  // minutes m1(3);                 // m1 stores 3
  // minutes m2(2);                 // m2 stores 2
  // minutes m3 = m1 + m2;          // m3 stores 5

  // cout << "Minutes1: " << m1 << endl; 
  // cout << "Minutes2: " << m2 << endl; 
  // cout << "Minutes3: " << m3 << endl; 

