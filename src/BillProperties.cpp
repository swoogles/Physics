#include "BillProperties.h"

BillProperties::BillProperties(string fileName)
: fileName(fileName) {
  readProperties();
}

string BillProperties::at( const char target[]  ) const
{
  return properties.at( target );
}

bool BillProperties::has( const string & target ) const
{
  return properties.find( target ) != properties.end();
}

string BillProperties::get( const string & target, const string & defaultValue ) const
{
  const auto found = properties.find( target );
  return found == properties.end() ? defaultValue : found->second;
}

const map<string, string> & BillProperties::all() const
{
  return properties;
}

string BillProperties::trim( const string & value )
{
  const auto first = value.find_first_not_of(" \t\r\n");
  if ( first == string::npos ) {
    return "";
  }
  const auto last = value.find_last_not_of(" \t\r\n");
  return value.substr(first, last - first + 1);
}

bool BillProperties::isValidProperty( string line )
{
  const string trimmed = trim( line );
  if ( trimmed.empty() ) {
    return false;
  } else if ( trimmed[0] == '#' ) {
    return false;
  } else if ( trimmed.find('=') == string::npos ) {
    return false;
  } else {
    return true;
  }
}

void BillProperties::readProperties()
{
  ifstream propertiesFile;
  propertiesFile.open(fileName, ios::in);

  string line;
  while( getline(propertiesFile, line) )
  {
    if ( BillProperties::isValidProperty( line ) )
    {
      unsigned long equalsPosition = line.find('=');
      string propName = trim( line.substr(0,equalsPosition) );
      string propValue = trim( line.substr(equalsPosition+1) );
      properties.insert( make_pair( propName, propValue ) );
    } else {

    }
  }
  propertiesFile.close();
}
