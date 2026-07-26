#ifndef PHYSICS_GLOBALAPPLICATION_H
#define PHYSICS_GLOBALAPPLICATION_H

#include <memory>

class FullApplication;

/*! The running application, reached by the GLUT and plib callbacks that have
 *  nowhere else to look. Defined in GlobalApplication.cpp rather than main.cpp
 *  so the test binary, which has its own main, still links.
 */
extern std::unique_ptr<FullApplication> globalFullApplication;

#endif //PHYSICS_GLOBALAPPLICATION_H
