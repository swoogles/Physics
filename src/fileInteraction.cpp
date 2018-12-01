/*
 * fileInteraction.cpp
 *
 *  Created on: Aug 7, 2011
 *      Author: brasure
 */

#include "fileInteraction.h"

void saveShapes(char * fileName, vectorT shapes) {
	shapePointer_t curShape;
	ofstream myfile;
	myfile.open (fileName);
	myfile << shapes.size() << endl;
	for (unsigned int i = 0; i < shapes.size(); i++) {
		curShape = shapes.at(i);

		myfile << curShape->getType() << endl;


		VecStruct pos(curShape->getPos());
		vecFilePrint(myfile, pos);

		myfile << curShape->getMass().value() << endl;

		auto momentum = curShape->getMomentum();
		vecFilePrint(myfile, momentum);

		VecStruct angularMomenum = curShape->getAngMomentum();
		vecFilePrint(myfile, angularMomenum);

		myfile << curShape->getDensity() << endl;

		auto color = curShape->getColor();
		vecFilePrint(myfile, color);

	}
	myfile.close();

}

void openShapes(char * fileName, vectorT shapes) {
	ifstream curFile;
	float curVar;
	shapePointer_t curShape;

	float newDT;
	int type;
	int numShapes;

	curFile.open(fileName);

	curFile >> newDT;

	curFile >> numShapes;

	shapes.resize(numShapes);

	for (int i = 0; i < numShapes; i++) {

		curFile >> type;
		if (type == 1) {

			//Set start position
			VecStruct pos(vecFileRead(curFile));

			//Set start mass
			float mass;
			curFile >> mass;
			kilogram_t typedMass = kilogram_t(mass);

			//Set start momentum
			VecStruct momentum(vecFileRead(curFile));

			//Set start angular momentum
			VecStruct angularMomentum(vecFileRead(curFile));

			//Set start density
			float density;
			curFile >> density;
			kilograms_per_cubic_meter_t typedDensity = kilograms_per_cubic_meter_t(density);

			//Set start color
			VecStruct color; // TODO decide whether to serialize. Probably yes.
			vecFileRead(curFile);

			shapePointer_t curShape = std::make_shared<Particle>(
					pos,
					typedMass,
					momentum,
					typedDensity,
					color
					);
			curShape->setAngMomentum(angularMomentum);
			shapes.push_back(curShape);
		}
	}
	glutPostRedisplay();
}

VecStruct vecFileRead(ifstream &curFile) {
	float values[3];
	for (int i = 0; i < 3; i++)
		curFile >> values[i];
	return VecStruct(values);
}

void vecFilePrint(ofstream &myfile, VecStruct outputVec) {
	myfile << outputVec.x() << " ";
	myfile << outputVec.y() << " ";
	myfile << outputVec.z() << endl;
}
