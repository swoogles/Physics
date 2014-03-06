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
			MyShape::shapes(i) = make_shared<Circle>(); 
			curShape = MyShape::shapes(i);

			//Set start position
			vecFileRead(curFile, curVec);
			curVec[3] = 1;
			curShape->setPos(curVec);

			//Set start mass
			curFile >> curVar;
			curShape->setMass(curVar);

			//Set start momentum
			vecFileRead(curFile, curVec);
			curVec[3] = 0;
			curShape->setMomentum(curVec);

			//Set start angular momentum
			vecFileRead(curFile, curVec);
			curVec[3] = 0;
			curShape->setAngMomentum(curVec);

			//Set start density
			curFile >> curVar;
			curShape->setDensity(curVar);

			//Set start color
			vecFileRead(curFile, curVec);
			curShape->setColor(curVec);
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
