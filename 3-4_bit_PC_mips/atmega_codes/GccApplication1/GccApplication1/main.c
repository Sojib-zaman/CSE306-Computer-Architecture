 
#define F_CPU 1000000
#include <util/delay.h>
#include <avr/io.h>
// generated based on our provided serial 
// like 1 is sw and .....


#define add 0x0D
#define addi 0x03
#define sub 0x0E
#define subi 0x04
#define and 0x0B
#define andi 0x02
#define or 0x07
#define ori 0x08
#define sll 0x06
#define srl 0x00
#define nor 0x0F
#define lw 0x05
#define sw 0x01
#define beq 0x0C
#define bneq 0x0A
#define j 0x09

unsigned char memArr[32];

int main(void)
{
	MCUCSR|=(1<<JTD);
	MCUCSR|=(1<<JTD);
	
	
	DDRA=0XFF;
	DDRB=0x00;
	DDRC=0x00;
	DDRD=0x00;

	//DDRA=0X00; // ShowAddr A0 to A4; A5-> showaddrflag
	
	// reg1 BL
	// reg2 DL
	// opc BH
	// out CL
	// zero A0


	PORTA = 0b00000001;
	PORTB = 0b11111111;
	PORTD = 0b00001111;
	PORTC = 0b00001111;

	int reg1;
	int reg2;
	int operation;
	int dataOutputReg = 0x00;
	int shamt;
	int zeroFlag = 0;
	int ALUsrc;
	int beqf;
	int bneqf;

	while (1)
	{
		operation = PIND%16;
		shamt = (PIND >> 4);
		
		reg1 = PINB%16;
		reg2 =  (PINB >> 4);
		
		int c = PINC;
		ALUsrc = (c & 0b0001);
		beqf = (c & 0b0010)>>1;
		bneqf = (c & 0b0100)>>2;
		
		if(ALUsrc==1) reg2 = shamt;

		if (operation == add || operation ==addi || operation == lw || operation == sw)
		{
			dataOutputReg = (reg1 + reg2)%16;
		}
		else if (operation == sub || operation == subi || operation == beq || operation == bneq)
		{
			dataOutputReg = (reg1 - reg2 +16 )%16;
			
		}
		else if (operation == and || operation == andi)
		{
			dataOutputReg = reg1 & reg2;
		}
		else if (operation == or || operation == ori)
		{
			dataOutputReg = reg1 | reg2;
		}
		else if (operation == nor)
		{
			dataOutputReg = reg1 | reg2;
			dataOutputReg = (dataOutputReg ^ 15);
		}
		else if (operation == sll)
		{
			dataOutputReg = reg1 << reg2;
			dataOutputReg%=16;
		}
		else if (operation == srl)
		{
			dataOutputReg = reg1 >> reg2;
		}
		else if (operation == j)
		{
			dataOutputReg = 0;
		}

		if (dataOutputReg%16 == 0)
		{
			zeroFlag = 1;
		}
		else
		{
			zeroFlag = 0;
		}
		
		zeroFlag = (zeroFlag&beqf)|((!zeroFlag)&bneqf);
		
		/*PORTC = 0x00;*/
		PORTA = dataOutputReg | (zeroFlag<<4);
		//PORTC = 0xFF;
		
		
		
		_delay_ms(10);
	}
}