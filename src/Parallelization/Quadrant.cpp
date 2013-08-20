#include "Quadrant.h"

using namespace std;

ostream& operator<<(ostream& os, sgVec4 outputVec) {
	cout << "<" << outputVec[0] << ", " << outputVec[1] << ", " << outputVec[2] << ">";
	return os;
}

//void Quadrant::calcForcesForAll()
//{
//}


Quadrant::Quadrant(int numCells, int level, sgVec4 pos, sgVec4 dimensions)
  : quadOctreeMine(boost::extents[2][2][2])
{
  this->level = level;
  int length = 2;

  isLeaf=true;
  containsBody=false;

  setPos( pos );
  sgCopyVec4( this->dimensions, dimensions );

  bordersNew = boost::make_shared<Box>();
  bordersNew->setPos(pos);
  borders = new Box();
  borders->setPos(pos);

  sgVec3 newColor;
  float redAmount = level*.15; 
  float greenAmount = (1-level*.15);
  float blueAmount = (1-level*.15);
  newColor[0]=redAmount;
  newColor[1]=greenAmount;
  newColor[2]=blueAmount;
  borders->setColor( newColor );

  int numShapes = MyShape::shapes.size();
  borders->setSideLength( dimensions[0] );

  MyShape::shapes.resize(numShapes + 1);
	MyShape::shapes(numShapes) = borders;


  // quadOctreeMine(boost::extents[2][2][2]);

  // quadOctreeMine = array_type(boost::extents[2][2][2]);
  //quadOctreeMine[0][0][0] = new Quadrant( numCells, this->level + 1, pos, dimensions );
  // quadOctreeMine[0][0][0] = NULL;


  // boost::numeric::ublas::vector <Quadrant *> newQuadrants(8);
  // newQuadrants.insert_element( values++, newQuadrant );
 // cout << "Constructor> quadOctreeMine: " <<  & quadOctreeMine << endl;


}



Quadrant * Quadrant::getQuadrantFromCell( int x, int y, int z )
{
  // PHYS-3
  return quadOctreeMine[x][y][z];
  // return quadOctree->at( x, y, z );
}

// x,y, & z must have values of 0|1
void Quadrant::subDivide( int x, int y, int z, int numCells )
{
  sgVec4 newPos;
  sgVec4 newDimensions;

  int xFactor;
  int yFactor;
  int zFactor;

  // PHYS-3
  // Remove this if block
  // if ( quadOctree == NULL )
  // {
  //   quadOctree = new Octree<Quadrant * >(numCells);
  // }

  if ( x ) {
    xFactor=1;
  }
  else {
    xFactor =-1;
  }
  if ( y ) {
    yFactor=1;
  }
  else {
    yFactor =-1;
  }
  if ( z ) {
    zFactor=1;
  }
  else {
    zFactor =-1;
  }

  // cout << "xFactor: " << xFactor << endl;
  // cout << "yFactor: " << yFactor << endl;
  // cout << "zFactor: " << zFactor << endl;

  newDimensions[0] = dimensions[0]/2;
  newDimensions[1] = dimensions[1]/2;
  newDimensions[2] = dimensions[2]/2;


  // cout << "Ein? " << endl;
  cout << "original Pos: " << pos << endl;
  //sgVec4 off
  newPos[0]=pos[0]+(xFactor*dimensions[0]/4.0);
  newPos[1]=pos[1]+(yFactor*dimensions[1]/4.0);
  newPos[2]=pos[2]+(zFactor*dimensions[2]/4.0);
  //newPos[3]=0;

  Quadrant * insertionQuadrant;


  // Quadrant
  //TODO Calculate new position
  // cout << "Ein? " << endl;
  // PHYS-3
  quadOctreeMine[x][y][z] = new Quadrant( numCells, this->level + 1, newPos, newDimensions );
  // quadOctree->set( x, y, z, new Quadrant( numCells, this->level + 1, newPos, newDimensions ) );
  cout << "New Pos: " << newPos << endl;
  cout << "New Dimensions: " << newDimensions << endl;
  // cout << "Ein? " << endl;
}

void Quadrant::subDivideAll( int levels, int numCells )
{
  sgVec4 newPos;
  sgVec4 newDimensions;

  int xFactor;
  int yFactor;
  int zFactor;

  // Quadrant
  Quadrant * targetQuadrant;

  // if ( quadOctree == NULL )
  // {
  //   quadOctree = new Octree<Quadrant * >(numCells);
  // }

  if ( this->level < levels )
  {

    for ( int x = 0; x < 2; x++ )
    {
      for ( int y = 0; y < 2; y++ )
      {
        for ( int z = 0; z < 2; z++ )
        {
          if ( x ) {
            xFactor=1;
          }
          else {
            xFactor =-1;
          }
          if ( y ) {
            yFactor=1;
          }
          else {
            yFactor =-1;
          }
          if ( z ) {
            zFactor=1;
          }
          else {
            zFactor =-1;
          }

          //sgVec4 offset;
          //sgScaleVec4( pos
          newPos[0]=pos[0]+(xFactor*dimensions[0]/4.0);
          newPos[1]=pos[1]+(yFactor*dimensions[1]/4.0);
          newPos[2]=pos[2]+(zFactor*dimensions[2]/4.0);
          newPos[3]=1;

          newDimensions[0] = dimensions[0]/2;
          newDimensions[1] = dimensions[1]/2;
          newDimensions[2] = dimensions[2]/2;

          Quadrant * insertionQuadrant;
          // PHYS-3
          quadOctreeMine[x][y][z] = new Quadrant( numCells, this->level + 1, newPos, newDimensions );
          // quadOctree->set( x, y, z, new Quadrant( numCells, this->level + 1, newPos, newDimensions ) );
          targetQuadrant = quadOctreeMine[x][y][z];
          // targetQuadrant = quadOctree->at(x,y,z);
          targetQuadrant->subDivideAll(levels, 4);

        }
      }
    }

  }
}

void Quadrant::getCenterOfMass(sgVec4 centerOfMass)
{
	sgCopyVec4(centerOfMass, this->preCenterOfMass);
  sgScaleVec4( centerOfMass, 1.0/mass );
}

void Quadrant::setCenterOfMass( sgVec4 centerOfMass )
{
  sgCopyVec4(this->preCenterOfMass, centerOfMass);
  sgScaleVec4( this->preCenterOfMass, mass);
}

void Quadrant::insertShape( MyShape * insertedShape )
{
  if ( !containsBody )
  // if ( shapeInQuadrant == NULL && quadOctree == NULL )
  {
    // cout << "step 1 in" << endl;
    shapeInQuadrant = insertedShape;
    containsBody = true;
    // cout << "step 1 out" << endl;
  }
  else if ( ! isLeaf )
  // else if ( shapeInQuadrant == NULL && quadOctree != NULL )
  {
    // cout << "step 2 in" << endl;
    this->adjustMass( insertedShape->getMass() );

    // TODO Update centerOfMass
    int levels = 2;
    Quadrant * targetQuadrant = this->determineShapeQuadrant( insertedShape );
    targetQuadrant->insertShape( insertedShape );
    // cout << "step 2 out" << endl;
  }
  else
  {
    // cout << "step 3 in" << endl;
    isLeaf = false;
    Quadrant * targetQuadrant = this->determineShapeQuadrant( insertedShape );
    targetQuadrant->insertShape( insertedShape );

    targetQuadrant = this->determineShapeQuadrant( shapeInQuadrant );
    targetQuadrant->insertShape( shapeInQuadrant );


    shapeInQuadrant = NULL;
    // cout << "step 3 out" << endl;
  }

}


// Guaranteed to hand back an instantiated Quadrant
Quadrant * Quadrant::determineShapeQuadrant( MyShape * shapeToInsert )
{
  //cout << "Determining shape Quadrant" << endl;
  sgVec4 insertPos;
  shapeToInsert->getPos( insertPos ); 
  float insertX = insertPos[0];
  float insertY = insertPos[1];
  float insertZ = insertPos[2];

  sgVec4 newPos;
  sgVec4 newDimensions;
  sgCopyVec4( newDimensions, dimensions );
  sgScaleVec4 ( newDimensions, .5 );

  float xOffset = dimensions[0]/4;
  float minXBoundary = pos[0] - newDimensions[0];
  float centralXBoundary = pos[0];
  float maxXBoundary = pos[0] + newDimensions[0];

  float yOffset = dimensions[1]/4;
  float minYBoundary = pos[1] - newDimensions[1];
  float centralYBoundary = pos[1];
  float maxYBoundary = pos[1] + newDimensions[1];

  float zOffset = dimensions[2]/4;
  float minZBoundary = pos[2] - newDimensions[2];
  float centralZBoundary = pos[2];
  float maxZBoundary = pos[2] + newDimensions[2];

  int targetX = INVALID_OCTREE_INDEX;
  int targetY = INVALID_OCTREE_INDEX;
  int targetZ = INVALID_OCTREE_INDEX;


  // X coordinate checking
  if ( insertX >= minXBoundary ) //Within min X boundary
  {
    //cout << "X Checking" << endl;
    if ( insertX < centralXBoundary ) //Less than central X boundary
    {
      targetX = 0; 
      newPos[0] = centralXBoundary - xOffset;
    }
    else if ( insertX <= maxXBoundary ) //Less than max X boundary
    {
      targetX = 1;
      newPos[0] = centralXBoundary + xOffset;
    }

    // Y coordinate checking
    if ( insertY >= minYBoundary ) //Within min Y boundary
    {
      //cout << "Y Checking" << endl;
      if ( insertY < centralYBoundary ) //Less than central Y boundary
      {
        targetY = 0; 
        newPos[1] = centralYBoundary - yOffset;
      }
      else if ( insertY <= maxYBoundary ) //Less than max Y boundary
      {
        targetY = 1;
        newPos[1] = centralYBoundary + yOffset;
      }

      //cout << "insertZ: " << insertZ << endl;
      //cout << "minZBoundary: " << minZBoundary << endl;

      // Z coordinate checking
      if ( insertZ >= minZBoundary ) //Within min Z boundary
      {
        //cout << "Z Checking" << endl;
        //cout << "centralZBoundary" << centralZBoundary << endl;
        //cout << "maxZBoundary" << maxZBoundary << endl;
        if ( insertZ < centralZBoundary ) //Less than central Z boundary
        {
          newPos[2] = centralZBoundary - zOffset;
          targetZ = 0; 
        }
        else if ( insertZ <= maxZBoundary ) //Less than max Z boundary
        {
          targetZ = 1;
          newPos[2] = centralZBoundary + zOffset;
        }
      }
    }
  }

  Quadrant * insertionQuadrant;

  //cout << "Ok" << endl;

  // Only proceed if we have determined that the provided shape does actually
  // belong in this Quadrant
  if ( targetX != INVALID_OCTREE_INDEX &&
      targetY != INVALID_OCTREE_INDEX &&
      targetZ != INVALID_OCTREE_INDEX )
  {
    // cout << "Valid" << endl;
    int numCells = 8;
    // if ( quadOctree == NULL )
    // {
    //   quadOctree = new Octree<Quadrant * >(numCells);
    // }


    // PHYS-3
    insertionQuadrant = quadOctreeMine[targetX][targetY][targetZ];
    // insertionQuadrant = getQuadrantFromCell( targetX, targetY, targetZ );

    //cout << "Should insert shape in quadrant[" 
      //<< targetX << ","
      //<< targetY << ","
      //<< targetZ << "]" << endl;

    if ( insertionQuadrant == NULL )
    {
      // cout << "Creating a new quadrant for insertion" << endl;
      //cout << endl;
      //subDivide( targetX, targetY, targetZ, numCells );

      insertionQuadrant = new Quadrant( numCells, this->level + 1, newPos, newDimensions );
      // PHYS-3
      quadOctreeMine[targetX][targetY][targetZ] = insertionQuadrant;
      // quadOctree->set( targetX, targetY, targetZ, insertionQuadrant );
    }
    else
    {
      //cout << "Position requires further dividing of an existant Quadrant" << endl;
      //cout << endl;
      //subDivide( targetX, targetY, targetZ, numCells );
    }
  }

  return insertionQuadrant;
  
}
