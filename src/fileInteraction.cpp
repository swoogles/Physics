/*
 * fileInteraction.cpp
 *
 *  Created on: Aug 7, 2011
 *      Author: brasure
 */

#include "fileInteraction.h"

void saveShapes(char * fileName, std::vector<shared_ptr<Moveable>> shapes) {
	shared_ptr<Moveable> curShape;
	ofstream myfile;
	myfile.open (fileName);
	myfile << shapes.size() << endl;
	for (unsigned int i = 0; i < shapes.size(); i++) {
		curShape = shapes.at(i);

		myfile << curShape->getType() << endl;


		PhysicalVector pos(curShape->position());
		vecFilePrint(myfile, pos);

		myfile << curShape->mass().value() << endl;

		auto momentum = curShape->momentum();
		vecFilePrint(myfile, momentum);

		PhysicalVector angularMomenum = curShape->angularMomentum();
		vecFilePrint(myfile, angularMomenum);

		myfile << curShape->density() << endl;

		auto color = curShape->getColor();
		vecFilePrint(myfile, color);

	}
	myfile.close();

}

void openShapes(char * fileName, std::vector<shared_ptr<Moveable>> shapes) {
	ifstream curFile;
	float curVar;
	shared_ptr<Moveable> curShape;

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
			PhysicalVector pos(vecFileRead(curFile));

			//Set start mass
			float mass;
			curFile >> mass;
			kilogram_t typedMass = kilogram_t(mass);

			//Set start momentum
			PhysicalVector momentum(vecFileRead(curFile));

			//Set start angular momentum
			PhysicalVector angularMomentum(vecFileRead(curFile));

			//Set start density
			float density;
			curFile >> density;
			kilograms_per_cubic_meter_t typedDensity = kilograms_per_cubic_meter_t(density);

			//Set start color
			PhysicalVector color; // TODO decide whether to serialize. Probably yes.
			vecFileRead(curFile);

			shared_ptr<Moveable> curShape = std::make_shared<Particle>(
					pos,
					typedMass,
					momentum,
					typedDensity,
					color
					);
            curShape->setAngularMomentum(angularMomentum);
			shapes.push_back(curShape);
		}
	}
}

PhysicalVector vecFileRead(ifstream &curFile) {
	float values[3];
	for (int i = 0; i < 3; i++)
		curFile >> values[i];
	return PhysicalVector(values);
}

void vecFilePrint(ofstream &myfile, PhysicalVector outputVec) {
	myfile << outputVec.x() << " ";
	myfile << outputVec.y() << " ";
	myfile << outputVec.z() << endl;
}
