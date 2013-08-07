#include <iostream>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/multi_array.hpp>
#include <cassert>

using namespace boost;
using namespace std;

//using namespace std;

#define INVALID_OCTREE_INDEX -1

int main(int argcp, char **argv) 
{
    typedef multi_array<float, 3> array_type;
    typedef array_type::index index;

    array_type  * quadOctreeMine = new array_type(boost::extents[2][2][2]);
    cout << "quadOctreeMine: " << quadOctreeMine << endl;
    quadOctreeMine = new array_type(boost::extents[2][2][2]);
    cout << "quadOctreeMine: " << quadOctreeMine << endl;
    quadOctreeMine = new array_type(boost::extents[2][2][2]);
    cout << "quadOctreeMine: " << quadOctreeMine << endl;
    quadOctreeMine = new array_type(boost::extents[2][2][2]);
    cout << "quadOctreeMine: " << quadOctreeMine << endl;
    quadOctreeMine = new array_type(boost::extents[2][2][2]);
    cout << "quadOctreeMine: " << quadOctreeMine << endl;
}

