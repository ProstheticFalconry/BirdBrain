#include <stdio.h>
#include <testdep.h>

Test::Test(){
	this->testStr="hello world";
	this->intTest=1;
	this->intTest2=2;
}

void Test::printTest(){
	printf("%s \n");
}

