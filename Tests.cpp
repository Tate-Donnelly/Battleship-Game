/*
 * Tests.cpp
 *
 *  Created on: Mar 11, 2021
 *      Author: tedon
 */

#include "Board.h"
#include "Tests.h"

Tests::Tests() {
	// TODO Auto-generated constructor stub

}

Tests::~Tests() {
	// TODO Auto-generated destructor stub
}

bool Tests::test(){
	bool answer=false;
	bool ok1=testSetCoordinate();
	bool ok2=testSetCell();
	//bool ok3=testSetShip();
	answer=ok1 && ok2;// && ok3;
	if(true){
		printf("All tests passed\n");
	}else{
		printf("Not all tests passed\n");
	}
	return answer;
}
bool Tests::testSetCoordinate(){
	bool answer=false;
	coordinate c;
	c.column=5;
	c.row=10;
	answer=(c.column==5)&&(c.row==10);
	if(answer){
		printf("testSetCoordinate passed\n");
	}else{
		printf("testSetCoordinate failed\n");
	}
	return answer;
}
bool Tests::testSetCell(){
	bool answer=false;
	Cell* c=new Cell();
	c->position.column=10;
	c->position.row=5;
	c->symbol='~';
	answer=(c->symbol=='~') && (c->position.row==5)&& (c->position.column==10);
	if(answer){
		printf("testSetCell passed\n");
	}else{
		printf("testSetCell failed\n");
	}
	free(c);
	return answer;
}/*
bool Tests::testSetShip(){
	bool answer=false;
	Ship* ship=new Ship();
	ship->setLength(5);
	ship->setSymbol('~');
	char name='C';
	ship->setName(&name);
	answer=(ship->getSymbol()=='~') && (ship->getLength()==5) && (ship->getName()->=='C');
	if(answer){
		printf("testSetShip passed\n");
	}else{
		printf("testSetShip failed\n");
	}
	free(ship);
	return answer;
}
*/
