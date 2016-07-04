#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>


#define I2C_SMBUS_READ	1
#define I2C_SMBUS_WRITE	0

// SMBus transaction types

#define I2C_SMBUS_QUICK		    0
#define I2C_SMBUS_BYTE		    1
#define I2C_SMBUS_BYTE_DATA	    2 
#define I2C_SMBUS_WORD_DATA	    3
#define I2C_SMBUS_PROC_CALL	    4
#define I2C_SMBUS_BLOCK_DATA	    5
#define I2C_SMBUS_I2C_BLOCK_BROKEN  6
#define I2C_SMBUS_BLOCK_PROC_CALL   7		/* SMBus 2.0 */
#define I2C_SMBUS_I2C_BLOCK_DATA    8

// SMBus messages

#define I2C_SMBUS_BLOCK_MAX	32	/* As specified in SMBus standard */	
#define I2C_SMBUS_I2C_BLOCK_MAX	32	/* Not specified but we use same structure */

int modread (int fd, int reg)
{
  union i2c_smbus_data data;

  if (i2c_smbus_access (fd, I2C_SMBUS_READ, reg, I2C_SMBUS_BYTE_DATA, &data))
    return -1 ;
  else
    return data.byte & 0xFF ;
}

int modread16 (int fd, int reg)
{
  union i2c_smbus_data data;

  if (i2c_smbus_access (fd, I2C_SMBUS_READ, reg, I2C_SMBUS_WORD_DATA, &data))
    return -1 ;
  else
    return data.word & 0xFFFF ;
}


void main(int argc,char* argv[])
{
	int fd,i2cAddress,j,k;
	if(argc<2)
	{
		printf("No address entered\n");
		return;
	}
	i2cAddress=atoi(argv[1]);
	
	if((fd = wiringPiI2CSetup(i2cAddress))<0)
	{
		printf("could not connect to slave address\n");
		return;
	}
	printf("setup done\n");
	while(1)
	{
		for(j=0;j<16;j++)
		{
			printf("Sending %d\t",j);
			wiringPiI2CWriteReg8(fd,0,j);
			k=wiringPiI2CReadReg8(fd,2);
			printf("data read %d\n",k);
			delay(1000);
		}
	}
}
