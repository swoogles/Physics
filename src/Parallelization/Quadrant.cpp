#include "Quadrant.h"

typedef shared_ptr<Quadrant> QuadrantPointer_t;

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

shapePointer_t Quadrant::getShapeInQuadrant()
{
  return shapeInQuadrant;
}

Quadrant::~Quadrant()
{
    // TODO Make sure I have a working implementation here.
    // TODO maybe a leak now that I've pulled this out?
    shapeInQuadrant.reset();
//  shapes.removeShapeFromList(borders);
//  shapes.removeShapeFromList(centerOfMassRepresentation);
}


Quadrant::Quadrant(int level, unique_ptr<VecStruct> pos, unique_ptr<VecStruct> dimensions)
  :isLeaf(true)
  ,containsBody(false)
  ,level(level)
//  ,borders(make_shared<Box>(pos, dimensions[0], (sgVec3){ (float) (level*.10), (float) (1-level*.10), (float) (1-level*.10)} ) )
  ,quadOctree(extents[2][2][2])
  ,dimensions()
  
{

  sgVec4 boxPos;
  sgCopyVec4(boxPos, pos->vec);
  // TODO Can I get this back inside the nifty constructor syntax?
  borders = make_shared<Box>(boxPos, dimensions->vec[0], (sgVec3){ (float) (level*.10), (float) (1-level*.10), (float) (1-level*.10)} );
  // TODO Require shapeToInsert in constructor.
  weightedPosition[0]=0.0;
  weightedPosition[1]=0.0;
  weightedPosition[2]=0.0;
  // TODO Can this be a more direct move?
  setPos( pos->vec );
  sgCopyVec4( this->dimensions, dimensions->vec );

  sgVec3 CoMColor = { 0, 1, 0 };
  sgVec4 comPos = { 0, 0, 0, 1};
  float comMass = 0;
  float comRadius = 1.0;
  sgVec4 comMomentum = { 0, 0, 0 };
  float comDensity = 1.0;

  /*
  centerOfMassRepresentation = make_shared<Circle>(
          comPos,
          comMass,
          comRadius,
          comMomentum,
          comDensity,
          CoMColor
  );
   */

}

QuadrantPointer_t  Quadrant::getQuadrantFromCell( int x, int y, int z )
{
  return quadOctree[x][y][z];
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

/* Description of insertion algorithm

  1. If node x does not contain a body, put the new body b here.

  2. a. If node x is an internal node, update the center-of-mass and total mass of x. 
     b. Recursively insert the body b in the appropriate quadrant.

  3. a. If node x is an external node, say containing a body named c, then there are two bodies b and c in the same region. 
     b. Subdivide the region further by creating four children. Then, recursively insert both b and c into the appropriate quadrant(s). Since b and c may still end up in the same quadrant, there may be several subdivisions during a single insertion.      c. Finally, update the center-of-mass and total mass of x.
*/

void Quadrant::insertShape(shapePointer_t insertedShape)
{
  // TODO use new function here
  if (!shapeIsInQuadrantBoundaries(insertedShape)) {
    cout << "Shape has strayed too far and cannot be placed inside this Quadrant! \n Unpredictable results ahead!" << endl;
    return;
  }

  if (insertedShape.use_count() > 58000) {
    cout << "Unplaceable shape : " << insertedShape.get() << ". I'm going to recurse myself to death now." << endl;
  }
  // 1.
  if ( !containsBody )
  {
    shapeInQuadrant = insertedShape;
//    cout << "insertShape.1 use_count: " << insertedShape.use_count() << endl;
    containsBody = true;
    this->setMass( insertedShape->getMass() );
    vecPtr pos(insertedShape->getPosNew());
    this->setCenterOfMass( pos->vec );
  }
  // 2. a
  else if ( ! isLeaf )
  {
    this->adjustMass( insertedShape->getMass() );

    sgVec4 quadrantWeightedPosition;
    this->getWeightedPosition( quadrantWeightedPosition );

    vecPtr shapeWeightedPosition(insertedShape->getPosNew());
    sgScaleVec4( shapeWeightedPosition->vec, insertedShape->getMass() );

    sgAddVec4( quadrantWeightedPosition, shapeWeightedPosition->vec );
    this->setWeightedPosition( quadrantWeightedPosition );

    // 2. b
    QuadrantPointer_t targetQuadrant = this->determineShapeQuadrant( insertedShape );
    if ( targetQuadrant != nullptr )
    {
      targetQuadrant->insertShape( insertedShape );
    }
  }
  // 3. a
  else
  {
    this->adjustMass( insertedShape->getMass() );
    isLeaf = false;
    // 3. b
    // I deviate from the described algorithm here, because I only make the
    // Quadrants necessary, rather than all 4 up front.
    QuadrantPointer_t targetQuadrant = this->determineShapeQuadrant( insertedShape );
    QuadrantPointer_t targetQuadrantB = this->determineShapeQuadrant( shapeInQuadrant );

    sgVec4 quadrantWeightedPosition;
    this->getWeightedPosition( quadrantWeightedPosition );

    vecPtr shapeWeightedPosition(insertedShape->getPosNew());
    sgScaleVec4( shapeWeightedPosition->vec, insertedShape->getMass() );

    sgAddVec4(quadrantWeightedPosition, shapeWeightedPosition->vec );
    this->setWeightedPosition( quadrantWeightedPosition );

    // Consider removing this check. It's only an issue when you're out of
    // memory, and it's not like I'm checking for that situation everywhere
    // else in the program.
    if ( targetQuadrant != nullptr && targetQuadrantB != nullptr )
    {
      // 3.c
      targetQuadrant->insertShape( insertedShape );
      targetQuadrantB->insertShape( shapeInQuadrant );
//      cout << "insertShape.3c insertedShape use_count: " << insertedShape.use_count() << endl;
//      cout << "insertShape.3c shapeInQuadrant use_count: " << shapeInQuadrant.use_count() << endl;
    }
    shapeInQuadrant.reset();
  }

  // 3.d
  sgVec4 CoMPosition;
  this->getCenterOfMass( CoMPosition );

//  centerOfMassRepresentation->setPos( CoMPosition );
}


// Guaranteed to hand back an instantiated Quadrant
QuadrantPointer_t Quadrant::determineShapeQuadrant( shape_pointer shapeToInsert )
{
  vecPtr insertPos(shapeToInsert->getPosNew());

  vecPtr newPos = make_unique<VecStruct>();
  vecPtr newDimensions = make_unique<VecStruct>();
  sgVec4 offsets;
  sgVec3 targets;

  // Each dimension is cut in half as you go down
  sgScaleVec4 ( newDimensions->vec, dimensions, .5 );

  sgScaleVec3( offsets, dimensions, .25 );

  sgCopyVec3( newPos->vec, pos );

  targets[0] = INVALID_OCTREE_INDEX;
  targets[1] = INVALID_OCTREE_INDEX;
  targets[2] = INVALID_OCTREE_INDEX;

  // TODO make one function that will take 2 vectors and return
  // true if one falls completely within the bounds of another
  bool validInsertPosition = shapeIsInQuadrantBoundaries(shapeToInsert);

  if ( validInsertPosition )
  {
    for ( int i = 0; i < 3; i++ )
    {
      if ( insertPos->vec[i] < pos[i] )
      {
        targets[i] = 0; 
        newPos->vec[i] -= offsets[i];
      }
      else
      {
        targets[i] = 1; 
        newPos->vec[i] += offsets[i];
      }
    }
  } else {
      cout << "NOT A GOOD INSERTION POINT. ABOUT TO DIE" << endl;
  }

  QuadrantPointer_t insertionQuadrant;

  // Only proceed if we have determined that the provided shape does actually
  // belong in this Quadrant
  if ( targets[0] != INVALID_OCTREE_INDEX &&
      targets[1] != INVALID_OCTREE_INDEX &&
      targets[2] != INVALID_OCTREE_INDEX )
  {

    insertionQuadrant = quadOctree[targets[0]][targets[1]][targets[2]];

    if ( insertionQuadrant == nullptr )
    {
      cout << "Quadrant level: " << level << endl;
      insertionQuadrant = std::make_shared<Quadrant>( this->level + 1, std::move(newPos), std::move(newDimensions) ) ;
      quadOctree[targets[0]][targets[1]][targets[2]] = insertionQuadrant;
    }
  }

  return insertionQuadrant;
}

bool Quadrant::shapeIsInQuadrantBoundaries(shapePointer_t newShape) {
   vecPtr insertPos(newShape->getPosNew());

//  sgVec4 newPos;
  vecPtr newPos = make_unique<VecStruct>();
  vecPtr newDimensions = make_unique<VecStruct>();

  // Each dimension is cut in half as you go down
  sgScaleVec4 ( newDimensions->vec, dimensions, .5 );

  // Boundaries [0]=min : [1]=central : [2]=max
  sgVec3 maxBoundaries;
  sgAddVec3( maxBoundaries, pos, newDimensions->vec );

  sgVec3 minBoundaries;
  sgSubVec3( minBoundaries, pos, newDimensions->vec );

  sgCopyVec3( newPos->vec, pos );

  // TODO make one function that will take 2 vectors and return
  // true if one falls completely within the bounds of another
  return withinBoundaries( insertPos->vec, minBoundaries, maxBoundaries );
}

