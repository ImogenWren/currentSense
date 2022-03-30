# currentSense
 Bare Bones C on AtMega328p current sensing PCB
 
 --------
 ## Interface with AtMega-Demo Current Sensing Boards
 
 These demo boards use a 12MHz oscillator, making programming via typical means difficult.
 
 This is the working method I have found for using the ISP header with STK500 programmer
 
 
 1. Use use the minicore bootloader. Access this via the boards manager (need to add minicore to board manager URLs first) see README for [minicore](https://github.com/MCUdude/MiniCore)
 2. Programmer - STK500 as ISP(minicore)
 3. Clock - External 12MHz
 4. UART0
 5. Port COM 7
 6. BOD (brownout detection) Disabled.
 
-------- 
 ## Programming using Raspi & UART with AVRdude
 
 This was a little tricky, and there were several workarounds I needed to make this work. I wont go into every detail here, 
 but I will try and reference the areas that were sticking for me.
 
 NOTE: these instructions were tested on an ARDUINO NANO, not the currentSense board with the 12MHz clock. - I am sure they can be adapted but I wanted to write down
 my current best working method while it was fresh.
 
 ### Method
 
 1. Install arduino CLI following instructions available online. [Instructions](https://siytek.com/arduino-cli-raspberry-pi/)
 2. Modify arduino avr tools to account for working with tty/UART pins. [Instructions](https://siytek.com/raspberry-pi-gpio-arduino/)
 3. NOTE: I had to edit the (new) avrdude file & remove `sudo` from the start of the `strace` line.
 4. I also had to modify user permissions to make each file executable by any user
      
        sudo chmod ugo+rwx avrdude
        sudo chmod ugo+rwx avrdude-original
        sudo chmod ugo+rwx autoreset
     
u = User level <br>
g = group level <br>
o = something <br>
 
 5. Add /home/pi/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin to PATH ( Please someone tell me there is a way to perminantly do this)

Navigate to the directory to be added to PATH

     cd /home/pi/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin

enter the command:

     PATH="$(pwd):$PATH"

This only works untill raspi is rebooted, I would like to find a perminant method for doing this.
 
 5. Compile .ino file inside arduino sketch folder using command:

        arduino-cli compile --fqbn arduino:avr:nano currentSense

6. Upload compiled hex file to board:

       arduino-cli upload --port /dev/ttyAMA0 --fqbn arduino:avr:nano ~/currentSense

Ignore `strace: | autoreset: Broken pipe` it is an unavoidable xconcequence of the sheer number of workarounds this required.


 
### Other Things I tried:
 I dont _think_ these steps had an impact on my eventual success, however they may be worth trying if lost.
 
 1. Changing global access to GPIO Pins: [Raspi Hardware Permissions](https://roboticsbackend.com/raspberry-pi-hardware-permissions/)
    - Especially `sudo chmod g+rw /dev/gpiomem`
 3. Adding `/home/pi/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin` to PATH$ in `/home/.bashrc` file. I dont think this worked, it was an attempt for a perminant fix for step 5 above.
 
