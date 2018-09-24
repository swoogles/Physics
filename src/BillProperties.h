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
    explicit BillProperties(string fileName);

    bool static isValidProperty( string line );
    string at( const char target[] );
};
#endif /* BILL_PROPERTIES_H_ */

