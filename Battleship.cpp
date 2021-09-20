//============================================================================
// Name        : Battleship.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "Tests.h"
#include "Production.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	Production* p=new Production();
	Tests* t=new Tests();
	if(t->test()){
		if(p->prod(argc, argv)){
			printf("Production passed\n");
		}else{
			printf("Production failed\n");
		}
	}
	return 0;
}
