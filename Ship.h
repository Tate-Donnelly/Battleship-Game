/*
 * Ship.h
 *
 *  Created on: Mar 11, 2021
 *      Author: tedon
 */

#ifndef SHIP_H_
#define SHIP_H_

class Ship {
public:
	Ship();
	virtual ~Ship();
	char getSymbol();
	short getLength();
	char getName();
	void setSymbol(char set);
	void setLength(short set);
	void setName(char set);
private:
	char symbol;
	short length;
	char name;
};

#endif /* SHIP_H_ */
