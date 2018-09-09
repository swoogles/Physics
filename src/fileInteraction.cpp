/*
 * fileInteraction.cpp
 *
 *  Created on: Aug 7, 2011
 *      Author: brasure
 */

#include "fileInteraction.h"

void saveShapes(char * fileName) {
	shape_pointer curShape;
	sgVec4 curField;
	ofstream myfile;
	myfile.open (fileName);
	myfile << MyShape::shapes.size() << endl;
	for (unsigned int i = 0; i < MyShape::shapes.size(); i++) {
		curShape = MyShape::shapes(i);

		myfile << curShape->getType() << endl;

		curShape->getPos(curField);
		vecFilePrint(myfile, curField);

		myfile << curShape->getMass() << endl;

		curShape->getMomentum(curField);
		vecFilePrint(myfile, curField);

		curShape->getAngMomentum(curField);
		vecFilePrint(myfile, curField);

		myfile << curShape->getDensity() << endl;

		curShape->getColor(curField);
		vecFilePrint(myfile, curField);

	}
	myfile.close();

}

void openShapes(char * fileName) {
	ifstream curFile;
	sgVec4 curVec;
	float curVar;
	shape_pointer curShape;

	float newDT;
	int type;
	int numShapes;

	curFile.open(fileName);

	curFile >> newDT;

	curFile >> numShapes;

	MyShape::shapes.resize(numShapes);

	for (int i = 0; i < numShapes; i++) {

		curFile >> type;
		if (type == 1) {

			//Set start position
			sgVec4 pos;
			vecFileRead(curFile, pos);
			pos[3] = 1;

			//Set start mass
			float mass;
			curFile >> mass;

			//Set start momentum
			sgVec4 momentum;
			vecFileRead(curFile, momentum);
			momentum[3] = 0;

			//Set start angular momentum
			sgVec4 angularMomentum;
			vecFileRead(curFile, angularMomentum);
			angularMomentum[3] = 0;

			//Set start density
			float density;
			curFile >> density;

			float bogusRadius = 1; // TODO Infer from density and mass

			//Set start color
			sgVec4 color;
			vecFileRead(curFile, color);

			shape_pointer curShape = boost::make_shared<Circle>(
					pos,
					mass,
					bogusRadius,
					momentum,
					density,
					color
					);
			curShape->setAngMomentum(angularMomentum);
			MyShape::shapes(i) = curShape;
		}
	}
	glutPostRedisplay();
}

void vecFileRead(ifstream & curFile, sgVec4 retVec) {
	for (int i = 0; i < 3; i++)
		curFile >> retVec[i];
}

void vecFilePrint(ofstream & myfile, sgVec4 outputVec) {
	myfile << outputVec[0] << " ";
	myfile << outputVec[1] << " ";
	myfile << outputVec[2] << endl;
}
