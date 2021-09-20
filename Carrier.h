/*
 * Carrier.h
 *
 *  Created on: Mar 11, 2021
 *      Author: tedon
 */
#include "Ship.h"
#ifndef CARRIER_H_
#define CARRIER_H_

class Carrier: public Ship{
public:
	Carrier();
	virtual ~Carrier();
private:
	char symbol='c';
	short length=5;
	char name='Carrier';
};

#endif /* CARRIER_H_ */
