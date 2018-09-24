#ifndef BILL_PROPERTIES_H_
#define BILL_PROPERTIES_H_

#include <string>
#include <map>
#include <fstream>

using namespace std;

class BillProperties
{
    map<string, string> properties;
    const string fileName;

    void readProperties();
  public:
    BillProperties(string fileName);
    static const char FORCE_CALCULATION_METHOD[];
    static const char  NUM_SHAPES[];

    bool static isValidProperty( string line );
    string at( const char target[] );
};
#endif /* BILL_PROPERTIES_H_ */

