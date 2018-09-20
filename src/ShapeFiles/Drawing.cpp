//
// Created by bfrasure on 19/09/18.
//

#include "Drawing.h"

void Drawing::draw(shapePointer_t myShape){
    glPushMatrix();

    //Translate
    vecPtr pos (myShape->getPosNew());
    glTranslatef(pos->x(), pos->y(), pos->z());

    //Rotate
    glMultMatrixf( (const GLfloat*) myShape->getOrientationMat());

    //Scale
    float scale = myShape->getScale();
    glScalef(scale, scale, scale);

    vecPtr color(myShape->getColor());
    glColor3fv(color->vec);

    int numPts = 16;
    switch (myShape->getType()) {
        case box: glutWireCube( 1.0 );
        case circle:glutWireSphere( 1, numPts, numPts);
    }
    glPopMatrix();
}

void Drawing::drawShapes(compressed_vector<shapePointer_t> shapes) {
    foreach_ ( shapePointer_t curShape, shapes ) {
        draw(curShape);
    }
}
