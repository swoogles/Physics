/*
 * Circle.h
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "RoundShape.h"

class Circle: public RoundShape {
  private:
    GLdouble radius;
  public:
    Circle();
    virtual ~Circle();

    matrix<double> getPts();

    float getMarkerSize();
    void scaleMembers(float);

    void drawScale() const;
    void drawUnit() const;

    float getRadius();
    void setRadius(float);

    float getMomentOfInertia();

    int getType();
};
#endif /* CIRCLE_H_ */
