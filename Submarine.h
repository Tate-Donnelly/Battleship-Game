/*
 * Submarine.h
 *
 *  Created on: Mar 11, 2021
 *      Author: tedon
 */
#include "Ship.h"
#ifndef SUBMARINE_H_
#define SUBMARINE_H_

class Submarine: public Ship {
public:
	Submarine();
	virtual ~Submarine();
private:
	char symbol='s';
	short length=3;
	char name='Submarine';
};

#endif /* SUBMARINE_H_ */
