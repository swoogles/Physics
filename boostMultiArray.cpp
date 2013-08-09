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

    array_type * quadOctreeMine = new array_type(boost::extents[2][2][2]);
    cout << "quadOctreeMine: " << quadOctreeMine << endl;
    quadOctreeMine = new array_type(boost::extents[2][2][2]);
    cout << "quadOctreeMine: " << quadOctreeMine << endl;
    quadOctreeMine = new array_type(boost::extents[2][3][4]);
    cout << "quadOctreeMine: " << quadOctreeMine << endl;

    cout << "numDimensions" << quadOctreeMine->num_dimensions() << endl;

    for ( int i = 0; i < quadOctreeMine->num_dimensions(); i++ )
    {
      cout << "Dimensions[" << i << "] size: " <<  quadOctreeMine->shape()[i] << endl;
      cout << "Strides[" << i << "] size: " <<  quadOctreeMine->strides()[i] << endl;
      cout << endl;
    }

    cout << "NumElements: " << quadOctreeMine->num_elements() << endl;
    cout << "size: " << quadOctreeMine->size() << endl;

    quadOctreeMine->data()[0] = 5.0;

    cout << "Data: " << ( *quadOctreeMine->data() )<< endl;

    array_type localOctree(boost::extents[2][2][2]);

    localOctree[0][0][0] = 1.0;
    localOctree[1][1][1] = 1.0;

    cout << "LocalOctree[0][0][0]" << localOctree[0][0][0]  << endl;
    cout << "localOctree[1][1][1] " << localOctree[1][1][1] << endl;

    array_type localOctree2(boost::extents[2][2][2]);

    localOctree2[0][0][0] = 5.0;
    localOctree2[1][1][1] = 5.0;

    cout << "LocalOctree2[0][0][0]: " << localOctree2[0][0][0]  << endl;
    cout << "localOctree2[1][1][1]: " << localOctree2[1][1][1] << endl;

    cout << "LocalOctree[0][0][0]" << localOctree[0][0][0]  << endl;
    cout << "localOctree[1][1][1] " << localOctree[1][1][1] << endl;

    //quadOctreeMine[0][0][0] = 5.0;

    //iterator curIt = quadOctreeMine->begin();

}

