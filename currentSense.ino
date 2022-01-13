/*       AtMega328p - ASC712 Current Sensor - Temperature Sensor - DEMO

          Imogen Heard 11/01/2021

      Green LED Connected to PD3      -     Arduino Pin D3
       Blue LED Connected to PD2      -     Arduino Pin D2
      Current Sensor         PC0      -     Arduino Pin D14/A0
    ` Temp Sensor            PC1      -     Arduino Pin D15/A1


  Using instructions found at:
  https://exploreembedded.com/wiki/AVR_I/O_Register_Configuration
  For direct port access.

  Additional Reading
  https://balau82.wordpress.com/2011/03/29/programming-arduino-uno-in-pure-c/


*/



#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define BLINK_DELAY_MS 500

void blink(uint8_t delayTime = 100) {
  //   PORTD = (1 << PD2); // Set PD2 High
  PORTD = (0 << PD3) | (1 << PD2); // Set PD3 Low
  _delay_ms(100);     // Delay
  //    PORTD = (0 << PD2); // Set PD2 Low
  PORTD = (1 << PD3) | (0 << PD2); // Set PD3 Low
  _delay_ms(30);     // Delay
}



int main(void) {

  Serial.begin(9600);

  // Use Data Direction Register to set LED pins as Output
  DDRD = (1 << PD2) | (1 << PD3);

  while (1) {
    blink();
  }
}





/*  Writing Direct to Registers

    Using instructions found at:
    https://exploreembedded.com/wiki/AVR_I/O_Register_Configuration

    DDRx        Data Direction Register       Configure PORT(x) as Output/Input
    PORTx       Write data to PORT(x) Pins
    PINx        Read data from PORT(x) Pins

*/


/* DDRx Data Direction Register

   Unless port is set as output, data from registers is not sent to contoller pins

   DDRB = 0xff;   // Configure PORTB as Output

   DDRC = 0x00;   // Configure PORTD as Input

   DDRD = 0x0f;    // Configure lower nibble of PORT D as output, higher nibble as Input

   DDRD = (1<<PD0) | (1<PD3) | (1<<PD6);   // Configure PD0, PD3, PD6 as output, all others as input
*/

/* PORTx Send data to port pins

   PORTB = 0xff;  // Set all PORTB pins High

   PORTC = 0x00;   // Set all PORTC pins Low

   PORTD = (1<<PD0) | (1<PD3) | (1<<PD6);   // Make PD0,PD3,PD6 high

*/

/* PINx PORT Input register
 *  
 *  DDRB  = 0x00;  // Configure the PORTB as Input. 
    PORTB = 0xFF;  // Enable the internal Pull Up resistor of PORTB.
 
    DDRD = 0xff;  // Configure PORTD as Output
    PORTD = PINB; // Read the data from PORTB and send it to PORTD.
 *  
 *  
 */

 /*  DAC Access using registers
  * 
  * 
  */
