/*
 * BS.h
 *
 *  Created on: Mar 11, 2021
 *      Author: tedon
 */
#include "Ship.h"
#ifndef BS_H_
#define BS_H_

class BS:public Ship{
public:
	BS();
	virtual ~BS();
	char getSymbol();
	short getLength();
	char* getName();
private:
	char symbol='b';
	short length=4;
	char name='Battleship';
};

#endif /* BS_H_ */
