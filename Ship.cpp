/*
 * Ship.cpp
 *
 *  Created on: Mar 11, 2021
 *      Author: tedon
 */

#include "Ship.h"

Ship::Ship() {
	// TODO Auto-generated constructor stub

}

Ship::~Ship() {
	// TODO Auto-generated destructor stub
}
void Ship::setSymbol(char set){
	symbol=set;
}
void Ship::setLength(short set){
	length=set;
}
void Ship::setName(char set){
	name=set;
}
char Ship::getSymbol(){
	return this->symbol;
}
short Ship::getLength(){
	return this->length;
}
char Ship::getName(){
	return this->name;
}
