#ifndef BILL_PROPERTIES_H_
#define BILL_PROPERTIES_H_

#include <string>
#include <map>
#include <fstream>

using namespace std;

class BillProperties
{
  public:
    explicit BillProperties(string fileName);

    bool static isValidProperty( string line );
    string at( const char target[] ) const;
private:
    map<string, string> properties;
    const string fileName;

    void readProperties();
};
#endif /* BILL_PROPERTIES_H_ */

