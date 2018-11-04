#include "MyShape.h"
#include "Quadrant.h"

bool withinBoundaries( sgVec3 insertPos, sgVec3 minBoundaries, sgVec3 maxBoundaries )
{
  bool withinBoundaries = true;
  for ( int i = 0; i < 3 && withinBoundaries; i++ )
  {
    if (
            /* THIS WAS IT!!! THIS WAS WHAT KILLED ME!!
                I had an AND here instead of an OR, so this check was only working if the shape
                happened to go out of bounds in all dimensions at the same time (basically never)
                Otherwise, it would recurse itself to death by continuing to create Quadrants that
                would NEVER contain the shape.
                */
        (insertPos[i] < minBoundaries[i] ) ||
        ( insertPos[i] > maxBoundaries[i] ) 
      ) {
      withinBoundaries = false;
    }
  }
  return withinBoundaries;
}

shapePointer_t Quadrant::getShapeInQuadrant()
{
  return shapeInQuadrant;
}


Quadrant::Quadrant(int level, VecStruct &pos, float width)
  :isLeaf(true)
  ,containsBody(false)
  ,level(level)
//  ,borders(make_shared<Box>(pos, dimensions[0], (sgVec3){ (float) (level*.10), (float) (1-level*.10), (float) (1-level*.10)} ) )
  ,quadOctree(extents[2][2][2])
{
    // TODO Get this in a more idiomatic form
  this->setMass(0);
  sgVec4 boxPos;
  sgCopyVec4(boxPos, pos.vec);
  // TODO Can I get this back inside the nifty constructor syntax?
  borders = make_shared<Box>(boxPos, width, (sgVec3){ (float) (level*.10), (float) (1-level*.10), (float) (1-level*.10)} );
  // TODO Require shapeToInsert in constructor.
  weightedPosition[0]=0.0;
  weightedPosition[1]=0.0;
  weightedPosition[2]=0.0;
  // TODO Can this be a more direct move?
  setPos( pos.vec );
  sgVec3 dimensions{width, width, width};
  sgCopyVec4( this->dimensions, dimensions);

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
	if (mass != 0) sgScaleVec4( centerOfMass, 1.0/mass );
}

void Quadrant::setCenterOfMass( sgVec4 centerOfMass )
{
  sgCopyVec4( this->weightedPosition, centerOfMass );
  if (mass != 0) sgScaleVec4( this->weightedPosition, mass );
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
  if (!shapeIsInQuadrantBoundaries(insertedShape)) {

    /* TODO Either
     *  Mark the shape for deletion in some way
     *  Expand the Quadrant until it *does* include the shape
     */
    return;
  }

  // 1.
  if ( !containsBody )
  {
    shapeInQuadrant = insertedShape;
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
    targetQuadrant->insertShape( insertedShape );
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
    // 3.c
    targetQuadrant->insertShape( insertedShape );
    targetQuadrantB->insertShape( shapeInQuadrant );
    // TODO I should be removing shapeInQuadrant here.
  }

  // 3.d
  sgVec4 CoMPosition;
  this->getCenterOfMass( CoMPosition );
  //  centerOfMassRepresentation->setPos( CoMPosition );
}


// Guaranteed to hand back an instantiated Quadrant
// TODO This is scary. A determination shouldn't create anything.
QuadrantPointer_t Quadrant::determineShapeQuadrant( shape_pointer shapeToInsert )
{
  VecStruct insertPos = *shapeToInsert->getPosNew();

  VecStruct newPos;
  VecStruct newDimensions;
  sgVec4 offsets;
  int targets[]{INVALID_OCTREE_INDEX, INVALID_OCTREE_INDEX, INVALID_OCTREE_INDEX};

  // Each dimension is cut in half as you go down
  sgScaleVec4 ( newDimensions.vec, dimensions, .5 );

  sgScaleVec3( offsets, dimensions, .25 );

  sgCopyVec3( newPos.vec, pos );

  for ( int i = 0; i < 3; i++ )
  {
    if ( insertPos.vec[i] < pos[i] )
    {
      targets[i] = 0;
      newPos.vec[i] -= offsets[i];
    }
    else
    {
      targets[i] = 1;
      newPos.vec[i] += offsets[i];
    }
  }

  QuadrantPointer_t insertionQuadrant = quadOctree[targets[0]][targets[1]][targets[2]];

  if ( insertionQuadrant == nullptr )
  {
//    cout << "Creating a new subquadrant for insertion" << endl;
    insertionQuadrant = std::make_shared<Quadrant>( this->level + 1, newPos, newDimensions.vec[0] ) ;
    quadOctree[targets[0]][targets[1]][targets[2]] = insertionQuadrant;
  }

  return insertionQuadrant;
}

bool Quadrant::shapeIsInQuadrantBoundaries(shapePointer_t newShape) {
  VecStruct insertPos = *newShape->getPosNew();
//    vecPtr insertPos(newShape->getPosNew());

    VecStruct newPos;
    VecStruct newDimensions;

    // Each dimension is cut in half as you go down
    sgScaleVec4 ( newDimensions.vec, dimensions, .5 );

    // Boundaries [0]=min : [1]=central : [2]=max
    sgVec3 maxBoundaries;
    sgAddVec3( maxBoundaries, pos, newDimensions.vec );

    sgVec3 minBoundaries;
    sgSubVec3( minBoundaries, pos, newDimensions.vec );

    sgCopyVec3( newPos.vec, pos );

    // TODO make one function that will take 2 vectors and return
    // true if one falls completely within the bounds of another
    return withinBoundaries( insertPos.vec, minBoundaries, maxBoundaries );
}

void Quadrant::adjustMass(float dMass) {
	mass += dMass;
}

vector<shared_ptr<Quadrant>> Quadrant::children() {
  vector<shared_ptr<Quadrant>>liveChildren ;
  // TODO This should *really* be captured inside the Quadrant class. WTF should Simulations know about these shitty indexes?
  for ( int x = 0; x < 2; x++ ) {
    for ( int y = 0; y < 2; y++ ) {
      for ( int z = 0; z < 2; z++ ) {
        QuadrantPointer_t targetQuadrant = this->getQuadrantFromCell( x, y, z );
        if ( targetQuadrant != nullptr ) {
            cout << "Return child at quadrant: " << x << ", " << y << ", " << z << endl;
          liveChildren.push_back(targetQuadrant);
          auto nextChildren = targetQuadrant->children();
          liveChildren.insert(std::end(liveChildren), std::begin(nextChildren), std::end(nextChildren));
        }
      }
    }
  }
  return liveChildren;
}
