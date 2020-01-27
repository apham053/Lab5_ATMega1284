/*	Author: apham053
 *  Partner(s) Name: Steven Rodriguez
 *	Lab Section:
 *	Assignment: Lab #5  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

igned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
   return (b ?  (x | (0x01 << k))  :  (x & ~(0x01 << k)) );
           
}

unsigned char GetBit(unsigned char x, unsigned char k) {
   return ((x & (0x01 << k)) != 0);
}
unsigned char U = 0x00;
unsigned char D = 0x00;
unsigned char C = 0x00;
unsigned char Bout = 0x00;        
#define A0 (~PINA & 0x01)  


enum States { Start, Wait, Up, Down, Reset, Release} State;

void tickButton() {
    switch(State) {
        case Start:
	    C = 0x00;
	    U = 1;       
	    State = Wait; 	
	    break;
	case Wait: 
		if (A0 && U) {
			State = Up;
		}
		else if (A0 && D) {
			State = Down;
		}
		else {
			State = Wait;
		}
	    break;
	case Up:   
		State = Release;
	    break;
	case Down:
		State = Release;
        break;
	case Release:
		if (!A0) {
			State = Wait;
		}
		else { 
			State = Release;
		}
	    break;
    default:
		State = Start;
		break;
    }
    
    switch(State) {
	case Start:
	    break;
	case Wait:
	    break;
	case Up: 
	    if (C < 0x06) {
                C = C + 1;
            }
	    else {
                C = 0x06;
            }
        switch(C) { 

        	case 0x01:
        		Bout = 0x01;
        		break;
        	case 0x02:
        		Bout = 0x03;
        		break;
        	case 0x03:
        		Bout = 0x07;
        		break;
        	case 0x04:
        		Bout = 0x0F;
        		break;
        	case 0x05:
        		Bout = 0x1F;
        		break;
        	case 0x06:
        		Bout = 0x3F;
        		break;
        	}
        if (Bout == 0x3F){ 
        	D = 1;
        	U = 0;
      	}

        break;

    case Down: 
	    if (C > 0x00) {
                C = C - 1;
            }
	    else {
                C = 0x00;
            }
        switch(C) { 

        	case 0x05:
        		Bout = 0x1F;
        		break;
        	case 0x04:
        		Bout = 0x0F;
        		break;
        	case 0x03:
        		Bout = 0x07;
        		break;
        	case 0x02:
        		Bout = 0x03;
        		break;
        	case 0x01:
        		Bout = 0x01;
        		break;
        	case 0x00:
        		Bout = 0x00;
        		break;
        	}
        if (Bout == 0x00){ 
        	D = 0;
        	U = 1;
      	}

        break;

    case Release:
		break;
	default:
	    break;	    
    }    
}

int main(void) {
	DDRA = 0x00;
	DDRC = 0xFF;
	PORTA = 0xFF;
	PORTC = 0x00;
	State = Start;  
	
	while (1) {
	tickButton();	
	PORTC = Bout;
	}
    
    return 1;
}
