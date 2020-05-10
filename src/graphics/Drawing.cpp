#include "Drawing.h"

void Drawing::draw(Moveable &myShape) {
    glPushMatrix();

    //Translate
    PhysicalVector pos (myShape.position());

    glBegin(GL_POINTS);                      // Select points as the primitive
    glVertex3f(pos.x(), pos.y(), pos.z());    // Specify a point
    glEnd();

    glTranslatef(pos.x(), pos.y(), pos.z());

    //Rotate
    glMultMatrixf( (const GLfloat*) myShape.getOrientationMat().get());

    //Scale
    float scale = myShape.scale();
    glScalef(scale, scale, scale);

    glColor3fv(myShape.getColor().vec);

    int numPts = 16;
    switch (myShape.getType()) {
        case box:
            glutWireCube( 1.0 );
            break;
        case circle:
            glutSolidSphere( 1, numPts, numPts);
//            glutWireSphere( 1, numPts, numPts);
            break;
    }
    glPopMatrix();

}
