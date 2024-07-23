/*
 * Lab_0.c
 *
 * Created: 4/07/2024 22:51:33
 * Author : luisa
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h> //Función para usar Delays
#include <avr/interrupt.h>

void setup(void);
char Numero(uint8_t Valor); 
void initTimer1(void);
void Comienzo(void); 

const t1qvalue = 0; //Valor para que el timer 1 genere pulsos de 1 s
volatile uint8_t contador1 = 0;
volatile uint8_t contador2 = 0; 
volatile uint8_t magic = 0;
volatile uint8_t start = 0;
volatile uint8_t winner = 0; 
 
const uint8_t milista[6] = {0x3F, 0x6, 0x5B, 0x4F, 0x66, 0x6D}; //Lista de conteo de 0 a 5

int main(void)
{
    setup(); 
	
    while (1) 
    {
		if(magic == 1){
			Comienzo();
			magic = 0;
			start++;
			contador1 = 0;
			contador2 = 0;
			
		}else if(start == 1){
			switch(contador1){
				case(0):
				PORTC = 0b00010000;
				break;
				case(1):
				PORTC = 0b00010001;
				break;
				case(2):
				PORTC = 0b00010010;
				break;
				case(3):
				PORTC = 0b00010100;
				break;
				case(4):
				PORTC = 0b0011000;
				winner = 1;
				break;  
			}
			switch (contador2){
				case(0):
				PORTB = 0b00000011;
				break;
				case(1):
				PORTB = 0b00000111;
				break;
				case(2):
				PORTB = 0b00001011;
				break;
				case(3):
				PORTB = 0b00010011;
				break;
				case(4):
				PORTB = 0b00100011;
				winner = 2;
				break; 
			}
			
		}
		if(winner == 1){
			PORTC = 0b0011111;
			PORTD = milista[1];
			start = 0;
			winner = 0;
		}
		else if(winner == 2){
			PORTB = 0b00111111;
			PORTD = milista[2];
			start = 0;
			winner = 0;
		}
		
    }
}

void Comienzo(){
	for (int i = 5; i >=0; i--){
		PORTD = milista[i];
		_delay_ms(1000); 
	}
}
void setup(void){
	cli();
	DDRB &= ~(1<<DDB0);
	PORTB |= (1<<PORTB0);
	DDRB &= ~(1<<DDB1);
	PORTB |= (1<<PORTB1);
	
	DDRC &= ~(1<<DDC4);
	PORTC |= (1<<PORTC4);
	
	          //PB0 y PB1 con interrupción de cambio de estado 
	PCMSK0 |= (1<<PCINT1)|(1<<PCINT0); 
	PCICR |= (1<< PCIE0); 
	
				//PC4 
	PCMSK1 |= (1<<PCINT12);
	PCICR |= (1<<PCIE1); 
	
	DDRD |= 0xFF; //Puerto D como salidas
	PORTD = 0; 
	UCSR0B = 0; //Enable al pin PD0 "RTX0"
	
	DDRC |= (1<<DDC3)|(1<<DDC2)|(1<<DDC1)|(1<<DDC0);
	PORTC &= ~(1<<PORTC3)|~(1<<PORTC2)|~(1<<PORTC1)|~(1<<PORTC0); //lEDS JUGADOR 1
	
	DDRB |= (1<<DDB2)|(1<<DDB3)|(1<<DDB4)|(1<<DDB5);
	PORTD &= ~(1<<PORTD2)|~(1<<PORTD3)|~(1<<PORTD4)|~(1<<PORTD5); //Leds Jugador 2
	
	
	sei();
}



ISR (PCINT0_vect){
	if ((PINB & (1<<PINB0)) == 0){
		contador1++;
		
		if (contador1 > 5){
			contador1 = 0;
		}
	}
	if((PINB & (1<<PINB1)) == 0){
		contador2++;
		
		if (contador2 > 5){
			contador2 = 0;
		}
	}
	PCIFR &= ~(1<<TOV0);
	
}

ISR (PCINT1_vect){
	if((PINC & (1<<PINC4)) == 0){
		magic++;
	}
	PCIFR &= ~(1<<TOV1);
}
