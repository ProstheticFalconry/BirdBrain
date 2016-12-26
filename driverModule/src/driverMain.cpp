#include <stdio.h>
#include <testdep.h>

Test test1;

int main(){
	test1.printTest();
	printf("The test int value is %d",test1.intTest);
}
