/*
 * Mini_Project2_StopWatch
 *  Diploma 56
 *  Created on: 14 Sept 2021
 *  Author: Elsayed Ayman Habib
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 1000000UL
unsigned char RUN_FLAG = 0; // Flag Variable for The Timer Interrupt
// Global Variables For counting Time
unsigned char g_num_1 = 0 ;
unsigned char g_num_2 = 0 ;
unsigned char g_num_3 = 0 ;
unsigned char g_num_4 = 0 ;
unsigned char g_num_5 = 0 ;
unsigned char g_num_6 = 0 ;
// Global Variables For access of six (7-segment)
unsigned char segment_1 = 0 ;
unsigned char segment_2 = 0 ;
unsigned char segment_3 = 0 ;
unsigned char segment_4 = 0 ;
unsigned char segment_5 = 0 ;
unsigned char segment_6 = 0 ;

ISR (TIMER1_COMPA_vect)
{
	// set the interrupt flag to indicate that TIMER1 COMPA occurs
	RUN_FLAG = 1 ;
}

ISR (INT0_vect) /* External INT0 Interrupt Service Routine */
{
	//Reset Set all counting Variables = 0
		g_num_1 = 0 ;
		g_num_2 = 0 ;
		g_num_3 = 0 ;
		g_num_4 = 0 ;
		g_num_5 = 0 ;
		g_num_6 = 0 ;
		segment_1 = 0 ;
		segment_2 = 0 ;
		segment_3 = 0 ;
		segment_4 = 0 ;
		segment_5 = 0 ;
		segment_6 = 0 ;
}

ISR (INT1_vect) /* External INT1 Interrupt Service Routine */
{
	// To Pause StopWatch Clear bits of (CS12=0 CS10=0 CS11=0) To stop The Clock
	if ((PIND & (1<<PD3)))
	{
		TCCR1B &= (~(1<<CS10))  & (~(1<<CS12));
	}
}

ISR (INT2_vect) /* External INT2 Interrupt Service Routine */
{
	TCCR1B |= (1<<CS10) | (1<<CS12); //Enable The clock bits CS10 CS12
}

void Timer1_CTC_mood (void) // Timer1 Interrupt Function.
{
	TCNT1 = 0 ;
	/* configure the timer
	 * for non PWM set FOC1A and FOC1B
	 * clock = F_CPU/1024 CS10=1 CS012=1 WGM12=1
	 */
	OCR1A =1000; // Compare Match Value Each ONE SECOND
	TIMSK |= (1<<OCIE1A) ;
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);
	TCCR1B = (1<<CS12)|(1<<CS10) | (1<<WGM12) ;
}
/* External INT0 enable and configuration function */
void INT0_INTERRUPET_FALLING_EDGE_RESET(void )
{
	DDRD &= (~(1<<PD2)); // Configure INT0/PD2 as input pin
	PORTD |= (1<<PD2); // Enable the internal pull up resistor at PD2 pin
	// Trigger INT0 with the falling edge
	MCUCR |= (1<<ISC01);
	MCUCR &= (~(1<<ISC00));
	GICR |= (1<<INT0); // Enable external interrupt pin INT0

}
/* External INT1 enable and configuration function */
void INT1_INTERRUPET_RAISING_EDGE_PAUSED (void )
{
	DDRD = DDRD & (~(1<<PD3)); // Configure INT1/PD3 as input pin
	MCUCR |= (1<<ISC10) | (1<<ISC11); // Trigger INT1 with the falling edge
	GICR |= (1<<INT1); // Enable external interrupt pin INT1
}
/* External INT2 enable and configuration function */
void INT2_INTERRUPET_FALLING_EDGE_RESUMED(void )
{
	DDRB = DDRB & (~(1<<PB2)); // Configure INT2/PD2 as input pin
	PORTB |= (1<<PB2); // Enable the internal pull up resistor at PD2 pin
	MCUCR |= (1<<ISC2); // Trigger INT2 with the falling edge
	GICR |= (1<<INT2); // Enable external interrupt pin INT2
}
int main (void)
{
	INT0_INTERRUPET_FALLING_EDGE_RESET (); // Enable and configure external INT0
	INT1_INTERRUPET_RAISING_EDGE_PAUSED (); // Enable and configure external INT1
	INT2_INTERRUPET_FALLING_EDGE_RESUMED (); // Enable and configure external INT2
	Timer1_CTC_mood ();
	//first 6-pins in PORTA as the enable/disable pins for the six 7-segments.
	// PINS 0,1,2,3,4,5 are output pins connecting with 6 transistors.
	DDRA =0x3F;
	PORTA =0x3F;
	//7447 decoder 4-pins to the first 4-pins in PORTC.
	DDRC = 0X0F;
	PORTC = 0XF0;
	SREG |= (1<<7);  // Enable interrupts by setting I-bit
	// PINS 0,1,3,4,5,6 are output pins connecting with 6 transistors.
	while (1)
	{
		if (RUN_FLAG==1)
		{
			if ((segment_1 & 0X0F) == 9)
			{
				g_num_1 = 0 ;
				segment_1 = ( segment_1 &  0XF0) | ( g_num_1 & 0x0F);
				g_num_2 ++ ;
				segment_2 = (segment_2 &  0XF0) | ( g_num_2 & 0x0F);
				if ((segment_2 & 0X0F) == 6)
				{
					g_num_2 = 0 ;
					segment_2 = ( segment_2 &  0XF0) | ( g_num_2 & 0x0F);
					g_num_3 ++ ;
					segment_3 = ( segment_3 &  0XF0) | ( g_num_3 & 0x0F);
					if ((segment_3 & 0X0F) == 9)
					{
						g_num_3 = 0 ;
						segment_3 = ( segment_3 &  0XF0) | ( g_num_3 & 0x0F);
						g_num_4 ++ ;
						segment_4 = ( segment_4 &  0XF0) | ( g_num_4 & 0x0F);
						if ((segment_4 & 0X0F) == 6)
						{
							g_num_4 = 0 ;
							segment_4 = ( segment_4 &  0XF0) | ( g_num_4 & 0x0F);
							g_num_5 ++ ;
							segment_5 = ( segment_5 &  0XF0) | ( g_num_5 & 0x0F);
							if ((segment_5 & 0X0F) == 9)
							{
								g_num_5 = 0 ;
								segment_5 = ( segment_5 &  0XF0) | ( g_num_5 & 0x0F);
								g_num_6 ++ ;
								segment_6 = ( segment_6 &  0XF0) | ( g_num_6 & 0x0F);
							}
						}
					}
				}
			}
			else
			{
				g_num_1 ++ ;
				segment_1 = (segment_1 & 0XF0) | (g_num_1 & 0x0f);
			}
			if (segment_1 == 9)
			{

			}
			RUN_FLAG=0; // reset the interrupt flag
		}
		PORTA = 0X01;
		PORTC = segment_1;
		_delay_ms(2);
		PORTA = 0X02;
		PORTC = segment_2;
		_delay_ms(2);
		PORTA = 0X04;
		PORTC = segment_3;
		_delay_ms(2);
		PORTA = 0X08;
		PORTC = segment_4;
		_delay_ms(2);
		PORTA = 0X10;
		PORTC = segment_5;
		_delay_ms(2);
		PORTA = 0X20;
		PORTC = segment_6;
		_delay_ms(2);
	}
}
