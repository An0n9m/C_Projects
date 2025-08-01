#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "st7920lib.h"


int main(void) {
	//DDRAM_test(); 	  // Testcase #1 Display Data RAM Test
	//GDRAM_test(); 	  // Testcase #2 Graphic RAM Test
	DDRAM_and_GDRAM_test(); // Testcase #3 using both GDRAM and DDRAM (pong simulation)
	//pong_test();		  // Testcase #4 simple pong simulation using GDRAM		

}
// written by an0n9m ( Davit Jalagonia ) 
