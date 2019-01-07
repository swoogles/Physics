#ifndef BOX_H_
#define BOX_H_

#include "../Dimensions/Moveable.h"

class Box: public Moveable {
public:
	Box(PhysicalVector pos, float sideLength, PhysicalVector color);

	double scale() const override;

	float getWidth() const;
private:
	const float sideLength;
};

#endif
