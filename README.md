# currentSense
 Bare Bones C on AtMega328p current sensing PCB
 
 
 ## Interface with AtMega-Demo Current Sensing Boards
 
 These demo boards use a 12MHz oscillator, making programming via typical means difficult.
 
 This is the working method I have found for using the ISP header with STK500 programmer
 
 
 1. Use use the minicore bootloader. Access this via the boards manager (need to add minicore to board manager URLs first) see README for [minicore](https://github.com/MCUdude/MiniCore)
 2. Programmer - STK500 as ISP(minicore)
 3. Clock - External 12MHz
 4. UART0
 5. Port COM 7
 6. BOD (brownout detection) Disabled.
 
 
 ## Programming using Raspi & UART with AVRdude
 
 
 
