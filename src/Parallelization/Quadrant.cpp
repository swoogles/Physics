#include "Quadrant.h"

using namespace std;

boost::shared_ptr<MyShape> Quadrant::getShapeInQuadrant()
{
  return shapeInQuadrant;
}

Quadrant::~Quadrant()
{
  MyShape::removeShapeFromList( borders );
  MyShape::removeShapeFromList( centerOfMassRepresentation );
}


Quadrant::Quadrant(int numCells, int level, sgVec4 pos, sgVec4 dimensions)
  :isLeaf(true)
  ,containsBody(false)
  ,level(level)
  // ,weightedPosition({0,0,0})
  ,borders(make_shared<Box>() )
  ,quadOctree(extents[2][2][2])
  
{
  weightedPosition[0]=0.0;
  weightedPosition[1]=0.0;
  weightedPosition[2]=0.0;
  setPos( pos );
  sgCopyVec4( this->dimensions, dimensions );

  borders->setPos(pos);

  float redAmount = level*.10; 
  float greenAmount = (1-level*.10);
  float blueAmount = (1-level*.10);
  sgVec3 newColor = { redAmount, greenAmount, blueAmount };

  borders->setColor( newColor );
  borders->setSideLength( dimensions[0] );

  MyShape::addShapeToList( borders );

  sgVec3 CoMColor = { 0, 1, 0 };

  centerOfMassRepresentation = make_shared<Circle>();
  centerOfMassRepresentation->setPos( 0,0,0 );
  centerOfMassRepresentation->setRadius( 1.0 );
  centerOfMassRepresentation->setColor(CoMColor);

  if ( level == 1 )
  {
    MyShape::addShapeToList( centerOfMassRepresentation );
  }

}

boost::shared_ptr<Quadrant> Quadrant::getQuadrantFromCell( int x, int y, int z )
{
  return quadOctree[x][y][z];
}

void Quadrant::subDivideAll( int levels, int numCells )
{
  sgVec4 newPos;
  sgVec4 newDimensions;
  sgVec4 scaleFactors;
  sgVec4 offsets;
  sgVec4 halfNewDimensions;
  sgVec3 crossProduct;

  int xFactor, yFactor, zFactor;

  quad_pointer targetQuadrant;

  if ( this->level < levels )
  {

    for ( int x = 0; x < 2; x++ )
    {
      for ( int y = 0; y < 2; y++ )
      {
        for ( int z = 0; z < 2; z++ )
        {
          scaleFactors[0] = ( x ) ?  1 : -1;
          scaleFactors[1] = ( y ) ?  1 : -1;
          scaleFactors[2] = ( z ) ?  1 : -1;

          // sgScaleVec3( offsets, scaleFactors, 
          // newPos[0]=pos[0]+(xFactor*dimensions[0]/4.0);
           
          sgScaleVec3  ( newDimensions, dimensions, .5 );
          sgScaleVec3  ( halfNewDimensions, newDimensions, .5 );

          
              // void sgVectorProductVec3 ( sgVec3 dst, sgVec3 a, sgVec3 b )
          sgVectorProductVec3 ( crossProduct, scaleFactors, halfNewDimensions );

          newPos[0]=pos[0]+( crossProduct[0] );
          newPos[1]=pos[1]+( crossProduct[1] );
          newPos[2]=pos[2]+( crossProduct[2] );
          newPos[3]=1;


          quad_pointer insertionQuadrant;
          quadOctree[x][y][z] = make_shared<Quadrant>( numCells, this->level + 1, boost::ref(newPos), boost::ref(newDimensions) );
          targetQuadrant = quadOctree[x][y][z];
          targetQuadrant->subDivideAll(levels, 4);

        }
      }
    }

  }
}

void Quadrant::getWeightedPosition( sgVec4 weightedPosition )
{
	sgCopyVec4( weightedPosition, this->weightedPosition );
}
void Quadrant::setWeightedPosition( sgVec4 weightedPosition )
{
  sgCopyVec4( this->weightedPosition, weightedPosition );
}

void Quadrant::getCenterOfMass( sgVec4 centerOfMass )
{
	sgCopyVec4( centerOfMass, this->weightedPosition );
  sgScaleVec4( centerOfMass, 1.0/mass );
}

void Quadrant::setCenterOfMass( sgVec4 centerOfMass )
{
  sgCopyVec4( this->weightedPosition, centerOfMass );
  sgScaleVec4( this->weightedPosition, mass );
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
  }
  else if ( ! isLeaf )
  {
    this->adjustMass( insertedShape->getMass() );

    sgVec4 quadrantWeightedPosition;
    this->getWeightedPosition( quadrantWeightedPosition );

    sgVec4 shapeWeightedPosition;
    insertedShape->getPos( shapeWeightedPosition );
    sgScaleVec4( shapeWeightedPosition, insertedShape->getMass() );

    sgAddVec4( quadrantWeightedPosition, shapeWeightedPosition );
    this->setWeightedPosition( quadrantWeightedPosition );

    quad_pointer targetQuadrant = this->determineShapeQuadrant( insertedShape );
    if ( targetQuadrant != nullptr )
    {
      targetQuadrant->insertShape( insertedShape );
    }
  }
  else
  {
    this->adjustMass( insertedShape->getMass() );
    isLeaf = false;
    quad_pointer targetQuadrant = this->determineShapeQuadrant( insertedShape );
    quad_pointer targetQuadrantB = this->determineShapeQuadrant( shapeInQuadrant );

    sgVec4 quadrantWeightedPosition;
    this->getWeightedPosition( quadrantWeightedPosition );

    sgVec4 shapeWeightedPosition;
    insertedShape->getPos( shapeWeightedPosition );
    sgScaleVec4( shapeWeightedPosition, insertedShape->getMass() );

    sgAddVec4(quadrantWeightedPosition, shapeWeightedPosition );
    this->setWeightedPosition( quadrantWeightedPosition );

    if ( targetQuadrant != nullptr && targetQuadrantB != nullptr )
    {
      targetQuadrant->insertShape( insertedShape );
      targetQuadrantB->insertShape( shapeInQuadrant );
    }
    shapeInQuadrant.reset();
  }

  sgVec4 CoMPosition;
  this->getCenterOfMass( CoMPosition );

  centerOfMassRepresentation->setPos( CoMPosition );
}


// Guaranteed to hand back an instantiated Quadrant
boost::shared_ptr<Quadrant> Quadrant::determineShapeQuadrant( shape_pointer shapeToInsert )
{
  sgVec4 insertPos;
  sgVec4 newPos;
  sgVec4 newDimensions;
  sgVec4 scaleFactors;
  sgVec4 offsets;
  sgVec4 halfNewDimensions;
  sgVec3 crossProduct;

  shapeToInsert->getPos( insertPos ); 

  float insertX = insertPos[0];
  float insertY = insertPos[1];
  float insertZ = insertPos[2];

  sgCopyVec4( newDimensions, dimensions );
  sgScaleVec4 ( newDimensions, .5 );

  
  sgScaleVec3( offsets, dimensions, .25 );
  // Boundaries [0]=min : [1]=central : [2]=max
  sgVec3 xBoundaries;
  // xBoundaries[0]
  sgVec3 maxBoundaries;
  sgAddVec3( maxBoundaries, pos, newDimensions );

  sgVec3 minBoundaries;
  sgSubVec3( minBoundaries, pos, newDimensions );

  float centralXBoundary = pos[0];
  float maxXBoundary = maxBoundaries[0];

  float centralYBoundary = pos[1];
  float maxYBoundary = maxBoundaries[1];

  float centralZBoundary = pos[2];
  float maxZBoundary = maxBoundaries[2];

  int targetX = INVALID_OCTREE_INDEX;
  int targetY = INVALID_OCTREE_INDEX;
  int targetZ = INVALID_OCTREE_INDEX;


  // X coordinate checking
  if ( insertX >= minBoundaries[0] ) //Within min X boundary
  {
    if ( insertX < centralXBoundary ) //Less than central X boundary
    {
      targetX = 0; 
      newPos[0] = centralXBoundary - offsets[0];
    }
    else if ( insertX <= maxBoundaries[0] ) //Less than max X boundary
    {
      targetX = 1;
      newPos[0] = centralXBoundary + offsets[0];
    }

    // Y coordinate checking
    if ( insertY >= minBoundaries[2] ) //Within min Y boundary
    {
      if ( insertY < centralYBoundary ) //Less than central Y boundary
      {
        targetY = 0; 
        newPos[1] = centralYBoundary - offsets[1];
      }
      else if ( insertY <= maxBoundaries[1] ) //Less than max Y boundary
      {
        targetY = 1;
        newPos[1] = centralYBoundary + offsets[1];
      }

      // Z coordinate checking
      if ( insertZ >= minBoundaries[2] ) //Within min Z boundary
      {
        if ( insertZ < centralZBoundary ) //Less than central Z boundary
        {
          newPos[2] = centralZBoundary - offsets[2];
          targetZ = 0; 
        }
        else if ( insertZ <= maxBoundaries[2] ) //Less than max Z boundary
        {
          targetZ = 1;
          newPos[2] = centralZBoundary + offsets[2];
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

    if ( insertionQuadrant == nullptr )
    {
      insertionQuadrant = boost::make_shared<Quadrant>( numCells, this->level + 1, boost::ref(newPos), boost::ref(newDimensions) ) ;
      quadOctree[targetX][targetY][targetZ] = insertionQuadrant;
    }
  }

  return insertionQuadrant;
}

//Get all objects in octree and return them in list
ShapeList Quadrant::getShapesRecursive()
{
  shape_pointer curShape;
  typedef boost::shared_ptr<Quadrant> quad_pointer;

  ShapeList totalShapeList;
  ShapeList targetShapeList;

  if ( this->getShapeInQuadrant() )
  {
    totalShapeList.addShapeToList( this->getShapeInQuadrant() );
  }

  quad_pointer targetQuadrant;

  for ( int x = 0; x < 2; x++ )
  {
    for ( int y = 0; y < 2; y++ )
    {
      for ( int z = 0; z < 2; z++ )
      {

        targetQuadrant = quadOctree[x][y][z];
        if ( targetQuadrant != nullptr )
        {
          targetShapeList = targetQuadrant->getShapesRecursive();
          foreach_ ( shape_pointer curShape, targetShapeList.getShapes() )
          {
            totalShapeList.addShapeToList( curShape );
          }
        }

      }
    }
  }

  return totalShapeList;
}
