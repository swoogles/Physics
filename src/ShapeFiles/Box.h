#ifndef BOX_H_
#define BOX_H_

#include "../Dimensions/Moveable.h"

class Box: public Moveable {
private:
	float sideLength;
public:
	Box(PhysicalVector pos, float sideLength, PhysicalVector color);

	double scale() override;

    virtual float getWidth();
};

#endif
