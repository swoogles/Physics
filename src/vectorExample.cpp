#include <boost/numeric/ublas/vector.hpp>                                                                                    

using namespace std;                                                                                                         
using namespace boost::numeric::ublas;                                                                                       
using boost::numeric::ublas::vector;


int main(int argcp, char **argv) {                                                                                           
  boost::numeric::ublas::vector<char> chars;                                                                                           
  boost::numeric::ublas::vector<char>::size_type index = 1;
  boost::numeric::ublas::vector<char>::iterator iter;
  int curSize = chars.size();
  char curChar = 'a';
  chars.resize(curSize + 1);
  chars(curSize) = curChar++;

  cout << "chars.size: " << chars.size() << endl;
  cout << "chars.curChar: " << chars(chars.size()-1) << endl;

  curSize = chars.size();
  chars.resize(curSize + 1);
  chars(curSize) = curChar++;

  cout << "chars.size: " << chars.size() << endl;
  cout << "chars.curChar: " << chars(chars.size()-1) << endl;

  curSize = chars.size();
  chars.resize(curSize + 1);
  chars(curSize) = curChar++;

  cout << "chars.size: " << chars.size() << endl;
  cout << "chars.curChar: " << chars(chars.size()-1) << endl;

  int target = 1;
  int count = 0;
  iter = chars.begin();
  while ( iter != chars.end() && count != target )
  {
    iter++;
    count++;
  }
  chars.erase_element( index );
  // chars.erase_element( chars.size()- 2 );
  cout << "chars.size after erase: " << chars.size() << endl;

  chars.resize( chars.size() - 1, true );

  for ( int i = 0; i < chars.size(); i ++)
  {
    cout << "chars[" << i << "]: " << chars(i) << endl;
  }

  return NULL;
}

