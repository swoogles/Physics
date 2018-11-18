#include "BillProperties.h"

BillProperties::BillProperties(string fileName)
: fileName(fileName) {
  readProperties();
}

string BillProperties::at( const char target[]  ) const
{
  return properties.at( target );
}

bool BillProperties::isValidProperty( string line )
{
  if ( line[0] == '#' ) {
    return false;
  } else if ( line.find('=') == string::npos ) {
    return false;
  } else {
    return true;
  }
}

void BillProperties::readProperties()
{
  ifstream propertiesFile;
  // TODO Create separate instances, rather than this static hard-coded value.
  propertiesFile.open(fileName, ios::in);

  string line;
  while( getline(propertiesFile, line) )
  {
    if ( BillProperties::isValidProperty( line ) )
    {
      unsigned long equalsPosition = line.find('=');
      string propName = line.substr(0,equalsPosition);
      string propValue = line.substr(equalsPosition+1);
      properties.insert( make_pair( propName, propValue ) );
    } else {

    }
  }
  propertiesFile.close();
}
