#ifndef BILL_PROPERTIES_H_
#define BILL_PROPERTIES_H_

#include <string>
#include <map>
#include <fstream>

using namespace std;

class BillProperties
{
    map<string, string> properties;
  public:
    // Using char * instead of string gets rid of heap allocation and dynamic initialization
    static const char FORCE_CALCULATION_METHOD[];
    static const char  SIMULATION_DT[];
    static const char  PARALLEL[];

    bool static isValidProperty( string line );
    void readProperties();
    string at( const char target[] );
};
#endif /* BILL_PROPERTIES_H_ */

