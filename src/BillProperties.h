#ifndef BILL_PROPERTIES_H_
#define BILL_PROPERTIES_H_

#include <string>
#include <map>
#include <fstream>
#include <vector>

using namespace std;

class BillProperties
{
  public:
    explicit BillProperties(string fileName);

    bool static isValidProperty( string line );
    string at( const char target[] ) const;

    //! True when the key was present in the file.
    bool has( const string & target ) const;

    //! Value for the key, or defaultValue when the key isn't present.
    string get( const string & target, const string & defaultValue ) const;

    //! Every key/value pair, for echoing a run's config into its report.
    const map<string, string> & all() const;

    string static trim( const string & value );
private:
    map<string, string> properties;
    const string fileName;

    void readProperties();
};
#endif /* BILL_PROPERTIES_H_ */

