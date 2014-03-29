
#include "derivative.h" 
#include "i2c.h"
 
char result;
char slave,memory;
char dato=0;
int main(void)
{
slave=0x50;
dato=1;
memory=0;

	Init_I2C();
	
	for (;;)
	{
	I2CWriteRegister(slave,memory,dato);
	result=I2CReadRegister(slave,memory);
	dato++;
	memory++;
	}
	
	
return 0;
}
 
 
