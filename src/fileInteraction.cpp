/*
 * fileInteraction.cpp
 *
 *  Created on: Aug 7, 2011
 *      Author: brasure
 */

#include "fileInteraction.h"

void saveShapes(char * fileName, vectorT shapes) {
	shapePointer_t curShape;
	sgVec4 curField;
	ofstream myfile;
	myfile.open (fileName);
	myfile << shapes.size() << endl;
	for (unsigned int i = 0; i < shapes.size(); i++) {
		curShape = shapes.at(i);

		myfile << curShape->getType() << endl;


		VecStruct pos(curShape->getPos());
		vecFilePrint(myfile, pos.vec);

		myfile << curShape->getMass().value() << endl;

		auto momentum = curShape->getMomentum();
		vecFilePrint(myfile, momentum.vec);

		VecStruct angularMomenum = curShape->getAngMomentum();
		vecFilePrint(myfile, angularMomenum.vec);

		myfile << curShape->getDensity() << endl;

		auto color = curShape->getColor();
		vecFilePrint(myfile, color.vec);

	}
	myfile.close();

}

void openShapes(char * fileName, vectorT shapes) {
	ifstream curFile;
	sgVec4 curVec;
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

			VecStruct angularMomentumVec(angularMomentum);

			//Set start density
			float density;
			curFile >> density;

			//Set start color
			sgVec4 color;
			vecFileRead(curFile, color);

			shapePointer_t curShape = std::make_shared<Circle>(
					pos,
					mass,
					momentum,
					density,
					color
					);
			curShape->setAngMomentum(angularMomentumVec);
			shapes.push_back(curShape);
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
