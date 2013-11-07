/*
 * fileInteraction.h
 *
 *  Created on: Aug 7, 2011
 *      Author: brasure
 */

#ifndef FILEINTERACTION_H_
#define FILEINTERACTION_H_

 #include <iostream>
#include <fstream>

#include <boost/numeric/ublas/vector_sparse.hpp>
#include <plib/sg.h>
#include "ShapeFiles/Circle.h"
#include "ShapeFiles/Box.h"

#include "Physics/WorldSettings.h"
#include "Physics/Simulations.h"

/*  README: Out of all the functions written so far, I believe the save/open functions are the ones that need the most hardening
 *  Right now they are very particular about what they can do and how they can do it, with NO error checking of any kind in play.
 *
 *  //TODO Turn these into functions that might actually be a little safer juggling knives.
 */


/*! \brief Saves the info in WorldSettings and current shapes in MyShape::shapes to a file
 *
 *  Header information:
 *  \n -Timestep (float)
 *  \n -Number of shapes (int)
 *
 *  Only set up for Circle class right now; save format:
 *  \n -Type (int)
 *  \n -Pos (int int int)
 *  \n -Mass (float)
 *  \n -Radius (float)
 *  \n -Momentum (float float float)
 *  \n -Angular Momentum (float float float)
 *  \n -Density (float)
 *  \n -Color (float float float)
 */
void saveShapes(char * fileName);


/*! \brief Opens a saved file and loads WorldSettings info and shapes info
 *
 *  Header information:
 *  \n -Timestep (float)
 *  \n -Number of shapes (int)
 *
 *  Only set up for Circle class right now; save format:
 *  \n -Type (int)
 *  \n -Pos (int int int)
 *  \n -Mass (float)
 *  \n -Radius (float)
 *  \n -Momentum (float float float)
 *  \n -Angular Momentum (float float float)
 *  \n -Density (float)
 *  \n -Color (float float float)
 *
 */
void openShapes(char * fileName);

/*! \interface sgVec
 *  \brief Vector class used in several places throughout the project
 */

/*! \relates sgVec
 *  \brief Reads in next 3 numbers from file to retVec
 *
 *  No error checking is done currently; no guarantee made for what happens if format doesn't match
 */
void vecFileRead(ifstream & curFile, sgVec4 retVec);

/*! \relates sgVec
 * \brief Prints first three numbers from outputVec on one line with a single space in between each
 *
 */
void vecFilePrint(ofstream &, sgVec4 outputVec);


#endif /* FILEINTERACTION_H_ */
