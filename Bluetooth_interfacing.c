#include "LPC214X.h"

void Uart0Init(unsigned int baudrate)     
{ 
	unsigned int FDiv;              	   
  	PINSEL0 |= 0x00000005;           //Enable RxD0 and TxD0                    
    U0LCR = 0x83;                   // 8 bits, no Parity, 1 Stop bit
	FDiv = (15000000 / 16 ) / baudrate ; //
	U0DLM = FDiv /256;					//0x00;
	U0DLL = FDiv %256;					//0x97;        
    U0LCR = 0x03; 					// DLAB = 0   
}

unsigned char UART0_PutChar(unsigned char Ch)
{
  	while(!(U0LSR & 0x20));
//	if(Ch == '\n')
//		Ch = 0x0D;
	U0THR = Ch;
	return Ch;
}

void UART0_SendString(char* str) /* A function to send string on UART0 */
{
	int i = 0;
	while( str[i] != '\0' )
	{
		UART0_PutChar(str[i]);
		i++;
	}
}

unsigned char UART0_GetChar(void)
{
	while(!(U0LSR & 0x01));
	return(U0RBR);
}

int main (void)
{
	char data_in;
	Uart0Init(9600);
	UART0_SendString("All OK")
	IO0DIR = IO0DIR | 0x00000010;	/* Configure P0.4 as output */
	while(1)
	{
		data_in = UART0_GetChar();	/* Receive data from bluetooth */
		if ( data_in == '1' )	/* If data received is 1 */
		{
			IO0PIN = ( IO0PIN | 0x00000010 );
			UART0_PutChar('H');

		}
		else if ( data_in == '0' )	/* If data received is 0 */
		{
			IO0PIN = ( IO0PIN & 0xFFFFFFEF );
			UART0_PutChar('L');
		}
		else
			UART0_PutChar('W');

	}
}