/*       AtMega328p - ASC712 Current Sensor - Temperature Sensor - Bare Bones C DEMO

          Imogen Heard 11/01/2021

      Green LED Connected to PD3      -     Arduino Pin D3
       Blue LED Connected to PD2      -     Arduino Pin D2
      Current Sensor         PC0      -     Arduino Pin D14/A0
      Temp Sensor            PC1      -     Arduino Pin D15/A1


    I wanted to use this sketch as a chance to learn how to interface with registers directly, rather than
    over-reliance on the Arduino Libraries and Environment.


  Using instructions found at:
  https://exploreembedded.com/wiki/AVR_I/O_Register_Configuration
  For direct port access.

  Additional Reading
  https://balau82.wordpress.com/2011/03/29/programming-arduino-uno-in-pure-c/


*/

//#define AVR_NATIVE
#define __IOT33__


#ifdef AVR_NATIVE

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>



#define BLINK_DELAY_MS 500

// Pin Definitions for NANO

#define D13 PB5



void blink(uint8_t delayOneTime = 100, uint8_t delayTwoTime = 200) {
  //   PORTD = (1 << PD2); // Set PD2 High
  PORTD = (0 << PD3) | (1 << PD2); // Set PD3 Low
  _delay_ms(2000);     // Delay
  //    PORTD = (0 << PD2); // Set PD2 Low
  PORTD = (1 << PD3) | (0 << PD2); // Set PD3 Low
  _delay_ms(1300);     // Delay
}

void blinkSetup() {
  // Use Data Direction Register to set LED pins as Output
  DDRD = (1 << PD2) | (1 << PD3);
}

void nanoBlink() {
  PORTB = (0 << PB5); // Set PD3 Low
  _delay_ms(200);     // Delay
  PORTB = (1 << PB5); // Set PD3 High
  _delay_ms(1000);     // Delay
}

void nanoBlinkSetup() {
  // Use Data Direction Register to set LED pins as Output
  DDRB = 0xff;
}





int main(void) {
  Serial.begin(9600);
  nanoBlinkSetup();
  while (1) {
    nanoBlink();
  }
}



#elif defined (__IOT33__) || defined (__AVR__)


/*
    Here program is written in more typical arduino flavour to use as a quick test for arduino hardware

*/


#define LED_PIN LED_BUILTIN

#define BLINK_HIGH 100
#define BLINK_LOW  50



void setup(){
  //  Serial.begin(115200);
  nanoBlinkSetup();  
}

void loop(){  nanoBlink(BLINK_HIGH, BLINK_LOW);
  
}

void nanoBlinkSetup() {
  pinMode(LED_PIN, OUTPUT);
}


void nanoBlink(int high_pulse_mS, int low_pulse_mS) {
  digitalWrite(LED_PIN, HIGH);
 // Serial.println("LED HIGH");
  delay(high_pulse_mS);
  digitalWrite(LED_PIN, 0);
 // Serial.println("LED LOW");
  delay(low_pulse_mS);
}


#endif






/*         Writing Direct to Registers

    Using instructions found at:
    https://exploreembedded.com/wiki/AVR_I/O_Register_Configuration

    DDRx        Data Direction Register       Configure PORT(x) as Output/Input
    PORTx       Write data to PORT(x) Pins
    PINx        Read data from PORT(x) Pins

*/


/*        DDRx Data Direction Register

   Unless port is set as output, data from registers is not sent to contoller pins

   DDRB = 0xff;   // Configure PORTB as Output

   DDRC = 0x00;   // Configure PORTD as Input

   DDRD = 0x0f;    // Configure lower nibble of PORT D as output, higher nibble as Input

   DDRD = (1<<PD0) | (1<PD3) | (1<<PD6);   // Configure PD0, PD3, PD6 as output, all others as input
*/

/*        PORTx Send data to port pins

   PORTB = 0xff;  // Set all PORTB pins High

   PORTC = 0x00;   // Set all PORTC pins Low

   PORTD = (1<<PD0) | (1<PD3) | (1<<PD6);   // Make PD0,PD3,PD6 high

*/

/*        PINx PORT Input register

    DDRB  = 0x00;  // Configure the PORTB as Input.
    PORTB = 0xFF;  // Enable the internal Pull Up resistor of PORTB.

    DDRD = 0xff;  // Configure PORTD as Output
    PORTD = PINB; // Read the data from PORTB and send it to PORTD.


*/

/*       DAC Access using registers


*/

/*        Timer register access



*/

/*        PWM Drivers Register Access



*/
