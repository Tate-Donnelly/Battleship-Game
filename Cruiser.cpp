/*
 * Cruiser.h
 *
 *  Created on: Mar 11, 2021
 *      Author: tedon
 */
#include "Ship.h"
#ifndef CRUISER_H_
#define CRUISER_H_

class Cruiser: public Ship {
public:
	Cruiser();
	virtual ~Cruiser();
private:
	char symbol='r';
	short length=3;
	char name='Cruiser';
};

#endif /* CRUISER_H_ */
