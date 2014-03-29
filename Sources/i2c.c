/*
 * i2c.c
 *
 *  Created on: Mar 22, 2014
 *      Author: Netdark
 */


#include "derivative.h"
#include "i2c.h"

void Init_I2C(void)
{
  SIM_SCGC5 = SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTB_MASK;
  SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
  PORTB_PCR0 = PORT_PCR_MUX(2) | PORT_PCR_PE_MASK;
  PORTB_PCR1 = PORT_PCR_MUX(2) | PORT_PCR_PE_MASK;
  I2C0_F  = 0x20;
  I2C0_C1 = I2C_C1_IICEN_MASK;
}

/* Inicia  Transmicion i2c
 * SlaveID: 1010 {A0-A2}
 * "Mode" define  Read (1)  Write (0)
 */
void IIC_StartTransmission (char SlaveID, char Mode)
{
  SlaveID = SlaveID << 1;
  SlaveID |= (Mode & 0x01);
  i2c_Start();
  i2c_write_byte(SlaveID);
}

void Pause(void){
    int n;
    for(n=1;n<50;n++) {
      asm("nop");
    }
}

/* Lee un Registro I2C
 * SlaveID: 
 * RegisterAddress: Direccion del Registro
 * Regresa valor del registro
 */
char I2CReadRegister(char SlaveID, char RegisterAddress)
{
  char result;
//Recorrer la direccion >> , despues &0xff
  IIC_StartTransmission(SlaveID,MWSR);
  i2c_Wait();
  i2c_write_byte(RegisterAddress>>8); 
  i2c_Wait();
  i2c_write_byte(RegisterAddress&0XFF); 
  i2c_Wait();
  i2c_RepeatedStart();
  i2c_write_byte((SlaveID << 1) | 0x01);
  i2c_Wait();
  i2c_EnterRxMode();
  i2c_DisableAck(); 
  result = I2C0_D ; // lectura
  i2c_Wait();
  i2c_Stop(); 
  result = I2C0_D ; 
  Pause();
  return result;
}


void I2CWriteRegister(char SlaveID, char RegisterAddress, char Data)
{
  IIC_StartTransmission(SlaveID,MWSR);
  i2c_Wait();
  i2c_write_byte(RegisterAddress>>8);
  i2c_Wait();
  i2c_write_byte(RegisterAddress&0XFF);
  i2c_Wait();
  i2c_write_byte(Data);
  i2c_Wait();
  i2c_Stop();
  Pause();
}

/* Lee un Registro I2C
 * SlaveID: 
 * RegisterAddress: Direccion del Registro
 * Regresa valor del registro
 * MEM8
 */
char I2CReadRegister8(char SlaveID, char RegisterAddress)
{
  char result;
//Recorrer la direccion >> , despues &0xff
  IIC_StartTransmission(SlaveID,MWSR);
  i2c_Wait();
  i2c_write_byte(RegisterAddress); 
  i2c_Wait();
  i2c_RepeatedStart();
  i2c_write_byte((SlaveID << 1) | 0x01);
  i2c_Wait();
  i2c_EnterRxMode();
  i2c_DisableAck(); // Deshabilita ACK 
  result = I2C0_D ; // lectura
  i2c_Wait();
  i2c_Stop(); // Envia STOP 
  result = I2C0_D ; 
  Pause();
  return result;
}

/* Escribe un valor
 * SlaveID:
 * RegisterAddress: 
 * Data: 
 */
void I2CWriteRegister8(char SlaveID, char RegisterAddress, char Data)
{
  IIC_StartTransmission(SlaveID,MWSR);
  i2c_Wait();
  i2c_write_byte(RegisterAddress);
  i2c_Wait();
  i2c_write_byte(Data);
  i2c_Wait();
  i2c_Stop();
  Pause();
}
