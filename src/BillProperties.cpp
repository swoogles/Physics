#include "BillProperties.h"

const char BillProperties::FORCE_CALCULATION_METHOD[] = "forceCalculationMethod";
const char BillProperties::SIMULATION_DT[] = "dt";
const char BillProperties::PARALLEL[] = "parallel";
const char BillProperties::OUTPUT_DIRECTORY[] = "outputDirectory";
const char BillProperties::NUM_SHAPES[] = "numShapes";

string BillProperties::at( const char target[]  )
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
  string line;
  string propName, propValue;
  ifstream propertiesFile;
  // TODO Create separate instances, rather than this static hard-coded value.
  propertiesFile.open("simulation.properties", ios::in);
  int equalsPosition;
  while( getline(propertiesFile, line) )
  {
    if ( BillProperties::isValidProperty( line ) )
    {
      equalsPosition = line.find('=');
      propName=line.substr(0,equalsPosition);
      propValue=line.substr(equalsPosition+1);
      properties.insert( make_pair( propName, propValue ) );
    }
  }
  propertiesFile.close();
}
