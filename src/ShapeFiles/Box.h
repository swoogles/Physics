#ifndef BOX_H_
#define BOX_H_

#include "../Dimensions/Moveable.h"

class Box: public Moveable {
public:
	Box(PhysicalVector pos, float sideLength, PhysicalVector color);

	double scale() override;

	virtual float getWidth();
private:
	float sideLength;
};

#endif
