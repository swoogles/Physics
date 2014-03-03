#include "Quadrant.h"

using namespace std;

bool withinBoundaries( sgVec3 insertPos, sgVec3 minBoundaries, sgVec3 maxBoundaries )
{

  bool withinBoundaries = true;
  // X coordinate checking
  for ( int i = 0; i < 3 && withinBoundaries == true; i++ )
  {
    if ( 
        (insertPos[i] < minBoundaries[i] ) && 
        ( insertPos[i] > maxBoundaries[i] ) 
      )
    {
      withinBoundaries = false;
    }
  }
  return withinBoundaries;
}

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
  float blueAmount = greenAmount;
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

boost::shared_ptr<Quadrant> Quadrant::operator() ( const sgVec3 targets )
{
  if ( targets[0] == INVALID_OCTREE_INDEX &&
      targets[1] == INVALID_OCTREE_INDEX &&
      targets[2] == INVALID_OCTREE_INDEX ) 
  {

    return nullptr;
  }
  else {
    return quadOctree[targets[0]][targets[1]][targets[2]];
  }
}

void Quadrant::subDivideAll( int levels, int numCells )
{
  sgVec4 newPos;
  sgVec4 newDimensions;
  sgVec4 scaleFactors;
  sgVec4 halfNewDimensions;
  sgVec3 crossProduct;

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
           
          sgScaleVec3  ( newDimensions, dimensions, .5 );
          sgScaleVec3  ( halfNewDimensions, newDimensions, .5 );
          
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
  sgVec4 offsets;
  sgVec3 targets;

  shapeToInsert->getPos( insertPos ); 

  // Each dimension is cut in half as you go down
  sgScaleVec4 ( newDimensions, dimensions, .5 );

  sgScaleVec3( offsets, dimensions, .25 );

  // Boundaries [0]=min : [1]=central : [2]=max
  sgVec3 maxBoundaries;
  sgAddVec3( maxBoundaries, pos, newDimensions );

  sgVec3 minBoundaries;
  sgSubVec3( minBoundaries, pos, newDimensions );

  sgCopyVec3( newPos, pos );

  targets[0] = INVALID_OCTREE_INDEX;
  targets[1] = INVALID_OCTREE_INDEX;
  targets[2] = INVALID_OCTREE_INDEX;

  // TODO make one function that will take 2 vectors and return
  // true if one falls completely within the bounds of another
  bool validInsertPosition = withinBoundaries( insertPos, minBoundaries, maxBoundaries );

  if ( validInsertPosition )
  {
    for ( int i = 0; i < 3; i++ )
    {
      if ( insertPos[i] < pos[i] )
      {
        targets[i] = 0; 
        newPos[i] -= offsets[i];
      }
      else
      {
        targets[i] = 1; 
        newPos[i] += offsets[i];
      }
    }
  }

  quad_pointer insertionQuadrant;

  // Only proceed if we have determined that the provided shape does actually
  // belong in this Quadrant
  if ( targets[0] != INVALID_OCTREE_INDEX &&
      targets[1] != INVALID_OCTREE_INDEX &&
      targets[2] != INVALID_OCTREE_INDEX )
  {
    int numCells = 8;

    // quadOctree(targets);
    insertionQuadrant = quadOctree[targets[0]][targets[1]][targets[2]];

    if ( insertionQuadrant == nullptr )
    {
      insertionQuadrant = boost::make_shared<Quadrant>( numCells, this->level + 1, boost::ref(newPos), boost::ref(newDimensions) ) ;
      quadOctree[targets[0]][targets[1]][targets[2]] = insertionQuadrant;
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
