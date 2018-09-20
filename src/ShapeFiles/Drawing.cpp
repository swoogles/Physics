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
    myShape->drawScale();

    vecPtr color(myShape->getColor());
    glColor3fv(color->vec);

    myShape->drawUnit();
    glPopMatrix();
}

void Drawing::drawShapes(compressed_vector<shapePointer_t> shapes) {
    foreach_ ( shapePointer_t curShape, shapes ) {
        draw(curShape);
    }
}
