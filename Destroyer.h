/*
 * Destroyer.h
 *
 *  Created on: Mar 11, 2021
 *      Author: tedon
 */
#include "Ship.h"
#ifndef DESTROYER_H_
#define DESTROYER_H_

class Destroyer: public Ship {
public:
	Destroyer();
	virtual ~Destroyer();
private:
	char symbol='d';
	short length=2;
	char name='Destroyer';
};

#endif /* DESTROYER_H_ */
