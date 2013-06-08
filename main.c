//Drives a 7seg using CD4511 BCD to 7 Seg Display Driver and uses an LFSR to genearate random numbers
//Coded on : 7th Dec`12
//Remarks: Wiring part is a bit touchy..
//Datasheet:http://www.datasheetcatalog.org/datasheets/150/109579_DS.pdf
//Coded By: Rohit Gupta
//Works like fab!!
#include <msp430g2231.h>
#define D BIT4
#define S BIT5
#define B BIT6
#define A BIT7
#define SWC BIT3
volatile unsigned int lfsr = 0xFF98u; //Source: Wikipedia
volatile unsigned int bit;
volatile unsigned int period = 0;
volatile unsigned int val=0;

void CD4511(int numb)
{
switch(numb){
				case 0:P1OUT&=~(B+A+S+D);
				break;
				case 1:P1OUT|=(A);
				       P1OUT&=~(B+S+D);
                break;
				case 2:P1OUT|=(B);
				       P1OUT&=~(A+S+D);
                break;
				case 3:P1OUT|=(A+B);
				       P1OUT&=~(S+D);
                break;
				case 4:P1OUT|=(S);
				       P1OUT&=~(A+D+B);
                break;
				case 5:P1OUT|=(A+S);
				       P1OUT&=~(B+D);
                break;
				case 6:P1OUT|=(S+B);
				       P1OUT&=~(A+D);
                break;
				case 7:P1OUT|=(S+B+A);
				       P1OUT&=~(D);
                break;
				case 8:P1OUT|=(D);
				       P1OUT&=~(B+S+A);
                break;
				case 9:P1OUT|=(A+D);
				       P1OUT&=~(B+S);
                break;
				default:P1OUT&=~(B+A+S+D);
				break;
}
}
void main(void)
{   WDTCTL = WDTPW + WDTHOLD;//Stop Watchdog!!
	P1DIR|=(D+S+B+A); // all as outputs!
	P1DIR|=BIT0; //LED 1 as output to show a toggle
	P1IE|=SWC;  //interrupt enable on switch
    P1IFG&=~SWC; //clear interrupt flag for interrupt to proceed
   _bis_SR_register(GIE); //global interrupts enable

	while(1)
{
//Forever Loop
}
}
#pragma vector=PORT1_VECTOR  //ISR Sub-routine
__interrupt void Port_1(void)
{
	            /* taps: 16 15 14 13; feedback polynomial: x^16 + x^15 + x^12+ x^10 + 1 */
			   bit  = ((lfsr >> 0) ^ (lfsr >> 1) ^ (lfsr >> 4) ^ (lfsr >> 6) ) & 1;
			   lfsr =  (lfsr >> 1) | (bit << 15);
			   val=lfsr/10000;

    if(val>6) val=5;
	if(val<=0) val=1;

	CD4511(val);
	P1OUT^=BIT0;
	P1IFG&=~SWC;	 //clear flag
}

