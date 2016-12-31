#include <stdio.h>
#include <testdep.h>
#include <string.h>

Test::Test(){
        char teststr[]="hello world";
	strcpy(this->testStr,teststr);
	this->intTest=1;
	this->intTest2=2;
}

void Test::printTest(){
	printf("%s \n",testStr);
}

