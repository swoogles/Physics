#include "Drawing.h"

void Drawing::draw(Moveable &myShape) {
//    cout << "drawing shape: " << &myShape <<  " with mass: " << myShape.mass() << " and momentum: " << myShape.momentum() <<  endl;
    glPushMatrix();

    //Translate
    PhysicalVector pos (myShape.position());

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
            cout << "printing quadrant box with scale: " << scale << endl;
            cout << "printing quadrant box with pos: " << myShape.position() << endl;
            cout << "printing quadrant box with color: " << myShape.getColor() << endl;
            glColor3f(1.0f, 0.0f, 0.0f);
            glutWireCube( 1.0 );
            break;
        case circle:
            glutSolidSphere( 1, numPts, numPts);
//            glutWireSphere( 1, numPts, numPts);
            break;
    }
    glPopMatrix();

    if (myShape.getType() == circle) {
        glPushMatrix();
        glBegin(GL_POINTS);                      // Select points as the primitive
        glVertex3f(pos.x(), pos.y(), pos.z());    // Specify a point
        glEnd();
        glPopMatrix();
    }

}
