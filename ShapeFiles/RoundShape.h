/*
 * RoundShape.h
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#ifndef ROUNDSHAPE_H_
#define ROUNDSHAPE_H_

#include "MyShape.h"

class RoundShape: public MyShape {
public:
	RoundShape();
	virtual ~RoundShape();

	void scaleMembers(float);
	void scaleMembers(float, float);
};

#endif /* ROUNDSHAPE_H_ */
