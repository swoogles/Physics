#include <iostream>                                                                                                          
#include <boost/numeric/ublas/vector.hpp>                                                                                    
#include <boost/shared_ptr.hpp>                                                                                    
#include <boost/scoped_ptr.hpp>                                                                                    
#include <boost/make_shared.hpp>                                                                                    
#include <boost/foreach.hpp>
#include "ShapeFiles/Circle.h"                                                                                                  
#include "ShapeFiles/MyShape.h"                                                                                                  
#define foreach_  BOOST_FOREACH

using namespace std;                                                                                                         
using namespace boost::numeric::ublas;                                                                                       
using boost::numeric::ublas::vector;

void removeItemFromList( char deleteLetter )
{
}

int main(int argcp, char **argv) {                                                                                           
  // shared_pointe
  boost::shared_ptr<int> x(new int);
  boost::shared_ptr<int> y = boost::make_shared<int>();
  // boost::shared_ptr<int> z = boost::make_shared<int>( y );
  boost::shared_ptr<int> z = y;

  *y = 5;

  cout << "y: " << *y << endl;
  cout << "z: " << *z << endl;


  boost::numeric::ublas::vector<char> chars;                                                                                           
  boost::numeric::ublas::vector<char>::size_type index = 1;
  boost::numeric::ublas::vector<char>::iterator iter;
  int curSize = chars.size();
  char curChar = 'a';
  chars.resize(curSize + 1);
  chars(curSize) = curChar++;

  curSize = chars.size();
  chars.resize(curSize + 1);
  chars(curSize) = curChar++;

  curSize = chars.size();
  chars.resize(curSize + 1);
  chars(curSize) = curChar++;

  int target = 1;
  int count = 0;
  iter = chars.begin();
  // chars.erase_element( index );
  // chars.erase_element( chars.size()- 2 );
  cout << "chars.size after erase: " << chars.size() << endl;

  // chars.resize( chars.size() - 1, true );

  for ( int i = 0; i < chars.size(); i ++)
  {
    cout << "chars[" << i << "]: " << chars(i) << endl;
  }

  boost::numeric::ublas::vector<char> charsCopy;
  charsCopy.resize( chars.size() - 1 );

  int curIteration = 0;
  iter = chars.begin();
  while ( iter != chars.end() )
  {
    if ( *iter != 'b' )
    {
      charsCopy(curIteration) = *iter;
      curIteration++;
    }
    cout << "Iterating..." << endl;
    cout << "Iter: " << *iter << endl;
    iter++;
  }

  for ( int i = 0; i < charsCopy.size(); i ++)
  {
    cout << "charsCopy[" << i << "]: " << charsCopy(i) << endl;
  }

  int charIndex = 0; 
  foreach_( char curChar, charsCopy )
  {
    cout << "charsCopy[" << charIndex << "]: " << curChar << endl;
  }

  return 0;
}


