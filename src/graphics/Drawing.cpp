#include "Drawing.h"

#include "ShapeFiles/Particle.h"

#include <GL/gl.h>
#include <GL/glut.h>


void Drawing::draw(const Moveable &myShape) const {
//    cout << "drawing shape: " << &myShape <<  " with mass: " << myShape.mass() << " and momentum: " << myShape.momentum() <<  endl;
    glPushMatrix();

    //Translate
    PhysicalVector pos (myShape.position());

    glTranslatef(pos.x(), pos.y(), pos.z());

    //Rotate

    //Scale
    float scale = myShape.scale();
    glScalef(scale, scale, scale);

    glColor3fv(myShape.getColor().vec);

    int numPts = 16;
    switch (myShape.getType()) {
        case box:
//            cout << "printing quadrant box with scale: " << scale << endl;
//            cout << "printing quadrant box with pos: " << myShape.position() << endl;
//            cout << "printing quadrant box with color: " << myShape.getColor() << endl;
//            glColor3f(1.0f, 0.0f, 0.0f);
            glutWireCube( 1.0 );
            break;
        case circle:
            glMultMatrixf( (const GLfloat*) myShape.getOrientationMat().get());
            glutSolidSphere( 1, numPts, numPts);
//            glutWireSphere( 1, numPts, numPts);
            break;
    }
    glPopMatrix();

    if (myShape.getType() == circle) {
        glPushMatrix();

        /* The sphere above is drawn at true physical scale, which at these
         * distances is a small fraction of a pixel even for the largest body.
         * This point is what you actually see, so its size has to carry the
         * mass - otherwise every particle is the same dot and a body holding
         * a fifth of the system reads as having disappeared. */
        glPointSize(Particle::pointSizeFor(myShape.mass().value()));

        glBegin(GL_POINTS);                      // Select points as the primitive
        glVertex3f(pos.x(), pos.y(), pos.z());    // Specify a point
        glEnd();

        glPointSize(1.0f);
        glPopMatrix();
    }

}
