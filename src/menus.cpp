/*
 * menus.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "menus.h"

void demo_menu(int id) {
	switch (id) {
	case 1: exit(0);
			break;
	case 2: cout << "You picked option 2!";
			break;
	case 3: cout << "You picked option 3!";
			break;
	}
}

void size_menu(int id) {
	switch (id) {
	case 2: cout << "You picked option 2!";

			break;
	case 3: cout << "Move em up!";
			//moveShapes(0,.5,0);
			break;
	}
}
void top_menu(int id) {
	switch(id) {
	case 1: exit(0);
			break;
	}
}
