#include "Quadrant.h"

using namespace std;

ostream& operator<<(ostream& os, sgVec4 outputVec) {
	cout << "<" << outputVec[0] << ", " << outputVec[1] << ", " << outputVec[2] << ">";
	return os;
}

//void Quadrant::calcForcesForAll()
//{
//}

Quadrant::~Quadrant()
{
  // MyShape::removeShapeFromList( MyShape::shapes(0) );
  MyShape::removeShapeFromList( borders );
  // cout << "Function:" << BOOST_CURRENT_FUNCTION << endl;
}


Quadrant::Quadrant(int numCells, int level, sgVec4 pos, sgVec4 dimensions)
  : quadOctree(boost::extents[2][2][2])
{
  // cout << "Function:" << BOOST_CURRENT_FUNCTION << endl;
  this->level = level;

  isLeaf=true;
  containsBody=false;
  mass = 0;

  setPos( pos );
  sgCopyVec4( this->dimensions, dimensions );

  borders = make_shared<Box>();
  borders->setPos(pos);

  sgVec3 newColor;
  float redAmount = level*.10; 
  float greenAmount = (1-level*.10);
  float blueAmount = (1-level*.10);
  newColor[0]=redAmount;
  newColor[1]=greenAmount;
  newColor[2]=blueAmount;
  borders->setColor( newColor );

  borders->setSideLength( dimensions[0] );

  MyShape::addShapeToList( borders );
}

boost::shared_ptr<Quadrant> Quadrant::getQuadrantFromCell( int x, int y, int z )
{
  return quadOctree[x][y][z];
}

// x,y, & z must have values of 0|1
void Quadrant::subDivide( int x, int y, int z, int numCells )
{
  sgVec4 newPos;
  sgVec4 newDimensions;

  int xFactor;
  int yFactor;
  int zFactor;

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

  newDimensions[0] = dimensions[0]/2;
  newDimensions[1] = dimensions[1]/2;
  newDimensions[2] = dimensions[2]/2;


  //sgVec4 off
  newPos[0]=pos[0]+(xFactor*dimensions[0]/4.0);
  newPos[1]=pos[1]+(yFactor*dimensions[1]/4.0);
  newPos[2]=pos[2]+(zFactor*dimensions[2]/4.0);
  //newPos[3]=0;

  quad_pointer insertionQuadrant;


  // Quadrant
  //TODO Calculate new position
  // quadOctree[x][y][z] = boost::make_shared<Quadrant>( Quadrant( numCells, this->level + 1, newPos, newDimensions ) );
  quadOctree[x][y][z] = boost::make_shared<Quadrant>( numCells, this->level + 1, boost::ref(newPos), boost::ref(newDimensions) );
}

void Quadrant::subDivideAll( int levels, int numCells )
{
  sgVec4 newPos;
  sgVec4 newDimensions;

  int xFactor;
  int yFactor;
  int zFactor;

  // Quadrant
  quad_pointer targetQuadrant;

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

          newPos[0]=pos[0]+(xFactor*dimensions[0]/4.0);
          newPos[1]=pos[1]+(yFactor*dimensions[1]/4.0);
          newPos[2]=pos[2]+(zFactor*dimensions[2]/4.0);
          newPos[3]=1;

          newDimensions[0] = dimensions[0]/2;
          newDimensions[1] = dimensions[1]/2;
          newDimensions[2] = dimensions[2]/2;

          quad_pointer insertionQuadrant;
          quadOctree[x][y][z] = boost::make_shared<Quadrant>( numCells, this->level + 1, boost::ref(newPos), boost::ref(newDimensions) );
          targetQuadrant = quadOctree[x][y][z];
          targetQuadrant->subDivideAll(levels, 4);

        }
      }
    }

  }
}

void Quadrant::getWeightedPosition(sgVec4 centerOfMass)
{
	sgCopyVec4(centerOfMass, this->weightedPosition);
}
void Quadrant::setWeightedPosition(sgVec4 centerOfMass)
{
  sgCopyVec4(this->weightedPosition, centerOfMass);
}

void Quadrant::getCenterOfMass(sgVec4 centerOfMass)
{
	sgCopyVec4(centerOfMass, this->weightedPosition);
  sgScaleVec4( centerOfMass, 1.0/mass );
}

void Quadrant::setCenterOfMass( sgVec4 centerOfMass )
{
  sgCopyVec4(this->weightedPosition, centerOfMass);
  sgScaleVec4( this->weightedPosition, mass);
}

void Quadrant::insertShape( shape_pointer insertedShape )
{
  if ( !containsBody )
  {
    shapeInQuadrant = insertedShape;
    containsBody = true;
    this->setMass( insertedShape->getMass() );
    sgVec4 pos;
    insertedShape->getPos(pos);
    this->setCenterOfMass( pos );
    // cout << "Level " << level << " Mass: " << this->getMass() << endl;
  }
  else if ( ! isLeaf )
  {
    this->adjustMass( insertedShape->getMass() );

    // TODO Update centerOfMass
    sgVec4 quadrantWeightedPosition;
    this->getWeightedPosition( quadrantWeightedPosition );

    sgVec4 shapeWeightedPosition;
    insertedShape->getPos( shapeWeightedPosition );
    sgScaleVec4( shapeWeightedPosition, insertedShape->getMass() );

    sgAddVec4(quadrantWeightedPosition, shapeWeightedPosition );
    this->setWeightedPosition( quadrantWeightedPosition );

    quad_pointer targetQuadrant = this->determineShapeQuadrant( insertedShape );
    if ( targetQuadrant != NULL )
    {
      targetQuadrant->insertShape( insertedShape );
      this->adjustMass( insertedShape->getMass() );
      // cout << "Level " << level << " Mass: " << this->getMass() << endl;
    }
  }
  else
  {
    isLeaf = false;
    quad_pointer targetQuadrant = this->determineShapeQuadrant( insertedShape );
    quad_pointer targetQuadrantB = this->determineShapeQuadrant( shapeInQuadrant );
    // this->adjustMass( insertedShape->getMass() );
    // sgVec4 originalCenterOfMass;
    // this->getCenterOfMass( originalCenterOfMass );

    sgVec4 quadrantWeightedPosition;
    this->getWeightedPosition( quadrantWeightedPosition );

    sgVec4 shapeWeightedPosition;
    insertedShape->getPos( shapeWeightedPosition );
    sgScaleVec4( shapeWeightedPosition, insertedShape->getMass() );

    sgAddVec4(quadrantWeightedPosition, shapeWeightedPosition );
    this->setWeightedPosition( quadrantWeightedPosition );

    // cout << "Level " << level << " Mass: " << this->getMass() << endl;
    if ( targetQuadrant != NULL && targetQuadrantB != NULL )
    {
      targetQuadrant->insertShape( insertedShape );
      targetQuadrantB->insertShape( shapeInQuadrant );
    }
  }

}


// Guaranteed to hand back an instantiated Quadrant
boost::shared_ptr<Quadrant> Quadrant::determineShapeQuadrant( shape_pointer shapeToInsert )
{
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

      // Z coordinate checking
      if ( insertZ >= minZBoundary ) //Within min Z boundary
      {
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

  quad_pointer insertionQuadrant;

  // Only proceed if we have determined that the provided shape does actually
  // belong in this Quadrant
  if ( targetX != INVALID_OCTREE_INDEX &&
      targetY != INVALID_OCTREE_INDEX &&
      targetZ != INVALID_OCTREE_INDEX )
  {
    int numCells = 8;

    insertionQuadrant = quadOctree[targetX][targetY][targetZ];
    // insertionQuadrant = getQuadrantFromCell( targetX, targetY, targetZ );


    if ( insertionQuadrant == NULL )
    {
      //subDivide( targetX, targetY, targetZ, numCells );

      insertionQuadrant = boost::make_shared<Quadrant>( numCells, this->level + 1, boost::ref(newPos), boost::ref(newDimensions) ) ;
      quadOctree[targetX][targetY][targetZ] = insertionQuadrant;
    }
  }

  return insertionQuadrant;
}

void Quadrant::getPos(sgVec4 retVec) {
  sgVec4 centerOfMass;
  this->getCenterOfMass( centerOfMass );
  // sgScaleVec4( centerOfMass, 1.0/mass );

	sgCopyVec4(retVec, centerOfMass);
}

void Quadrant::calcForceOnShape( shape_pointer curShape, sgVec3 netForceFromQuadrant, float dt )
{
  if ( isLeaf )
  {
    if ( curShape == shapeInQuadrant )
    {
      cout << "Matched shape!" << endl;
    }
  }
  else
  {
    sgVec4 sepVec;
    // getVectorToObject2( curShape, this, sepVec );

    sgVec4 pos1, pos2;
    curShape->getPos(pos1 );
    this->getPos(pos2 );
    sgSubVec4(sepVec, pos2, pos1);
  }

}
