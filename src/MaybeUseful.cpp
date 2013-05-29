/*
 * MaybeUseful.cpp
 *
 *  Created on: Aug 6, 2011
 *      Author: brasure
 */


/*
 *  // Distance from point to line
AB[0] = (aPos[0]+aMomentum[0]) - aPos[0];
AB[1] = (aPos[1]+aMomentum[1]) - aPos[1];
AB[2] = (aPos[2]+aMomentum[2]) - aPos[2];

AP[0] = hitPt[0] - aPos[0];
AP[1] = hitPt[1] - aPos[1];
AP[2] = hitPt[2] - aPos[2];
sgVectorProductVec3(crossed, AB, AP);

a = sgLengthVec3(crossed);
a /= sgLengthVec3(AB);

cout << "Distance: " << a << endl;

cout << "aMomentum: " << aMomentum << endl;
sgScaleVec4(aMomentum, a);
cout << "aAngMomentum: " << aMomentum << endl;
*/
