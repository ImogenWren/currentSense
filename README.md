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
 ## Programming Arduino Nano using Raspi & UART with AVRdude
 
 This was a little tricky, and there were several workarounds I needed to make this work. I wont go into every detail here, 
 but I will try and reference the areas that were sticking for me.
 
 NOTE: these instructions were tested on an ARDUINO NANO, not the currentSense board with the 12MHz clock. - I am sure they can be adapted but I wanted to write down
 my current best working method while it was fresh.
 
 ### Method
 
 #### Connections:
 Pin Numbers
 |Nano	|Rpi PinName		| Rpi PinNo.	|
 |---	|---			|---		|
 | 3v3	| 3v3			| 1		|
 | GND	| GND			| 6		|
  | Rx0	| GPIO 14 (UART0 TxD)	| 8 		|
 | Tx1	| GPIO 15 (UART0 RxD)	| 10		|
 | RST	| GPIO 18		| 12		|
 
 
##### 1. Install arduino CLI following instructions available online. [Instructions](https://siytek.com/arduino-cli-raspberry-pi/)
##### 2. Modify arduino avr tools to account for working with tty/UART pins. [Instructions](https://siytek.com/raspberry-pi-gpio-arduino/)


##### 3. NOTE: I had to edit the (new) avrdude file & remove `sudo` from the start of the `strace` line.

###### Note:
To try and replicate the double tap reset required for Nano IoT33, going to modify the new autoreset avrdude python file
`nano /home/pi/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin/autoreset`

added these lines to the bottom of `def reset()`
```
#added lines guessing at required delays
  time.sleep(0.5)
  GPIO.output(pin, GPIO.HIGH)
  time.sleep(0.32)
  GPIO.output(pin, GPIO.LOW)
  time.sleep(1)
```
Hopefully this will be enough to trigger programming mode for the IoT33, added a sleep at the end to
try and make it work cleanly with non 33 IoT boards as well. Will need experimentation

THIS DID NOT WORK:
I think because it trys to look for comms before running the autoreset script, so I am going to put it in
a new python file and call it
first in the bash script I wrote to simplify the uploading process. 


hopefully the 2nd reset from autoreset will not undo this step. first I can test with just the python script to see if it even goes into programming mode.

YES double tapping reset using GPIO worked BUT then AVRdude autoreset takes it out of programming mode BUT

SOLUTION: Actually it just seems to work most of the time 2/3 at least




##### 4. I also had to modify user permissions to make each file executable by any user [User Permissions](https://www.pluralsight.com/blog/it-ops/linux-file-permissions)
      
        sudo chmod ugo+rwx avrdude
        sudo chmod ugo+rwx avrdude-original
        sudo chmod ugo+rwx autoreset
     
u = User level <br>
g = group level <br>
o = something <br>
 
##### 5. Add /home/pi/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin to PATH ( Please someone tell me there is a way to perminantly do this)

Navigate to the directory to be added to PATH

     cd /home/pi/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin

enter the command:

     PATH="$(pwd):$PATH"

This only works untill raspi is rebooted, I would like to find a perminant method for doing this.

-------
## My Hack Way of Perminantly Doing this
_bash script it on startup_

Step 1: Save this file in currentSense as `addAVRpath.sh`
```
#!/bin/sh
# addAVRpath.sh
# Navigate to home directory, then to this directory.
# Then add directory to path



cd /
cd /home/pi/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin
PATH="$(pwd):$PATH"
cd /

```

Step 2: Make Launcher Executable
`chmod 755 addAVRpath.sh`

Script can be tested with

sh addAVRpath.sh


Step 4: Add task to crontab file
`crontab -e` (use `sudo crontab -e`) for tasks that require sudo privilages.

Crontab Task with Logs redirect and sleep before execution

`@reboot sleep 30 && sh /home/pi/currentSense/addAVRpath.sh >/home/pi/logs/cronlog 2>&1`

#### End of Hacky Way of Doing it
-------

 
##### 6. Compile .ino file inside arduino sketch folder using command:

        arduino-cli compile --fqbn arduino:avr:nano currentSense

##### 7. Upload compiled hex file to board:

       arduino-cli upload --port /dev/ttyAMA0 --fqbn arduino:avr:nano ~/currentSense

Ignore `strace: | autoreset: Broken pipe` output on successful upload it is an unavoidable concequence of the sheer number of workarounds this required.

## MORE SCRIPTING

I have automated the process for compiling and uploading new firmware to nano using
`uploadNano.sh`

I will add another for uploadNanoIoT.sh when I work out how to do the do.
 
### Other Things I tried:
 I dont _think_ these steps had an impact on my eventual success, however they may be worth trying if lost.
 
 1. Changing global access to GPIO Pins: [Raspi Hardware Permissions](https://roboticsbackend.com/raspberry-pi-hardware-permissions/)
    - Especially `sudo chmod g+rw /dev/gpiomem`
 2. Adding `/home/pi/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin` to PATH$ in `/home/.bashrc` file. I dont think this worked, it was an attempt for a perminant fix for step 5 above. [adding executables to PATH](https://askubuntu.com/questions/322772/how-do-i-add-an-executable-to-my-search-path)
 
 
 ### Further Reading
 
[Arduino Sketch Uploading Process - What is Actually Occuring](https://www.instructables.com/Overview-the-Arduino-sketch-uploading-process-and-/)


## Programming Arduino Nano IoT 33 - WITH Arduino IDE

The arduino nano 33 IoT with the SAMD21 MCU uses a slightly different process to the AVR boards. The COM port is software defined, so if you are having problems uploading to Arduino Nano IoT 33 Try these steps.

1. Avoid using C++ standard int main() and use the arduino standard void setup() and void loop() 
	- I assume that the precompiler is adding a fair amount of additional workarounds to the code you add to the IDE, things will stop working properly if you try and use a more typically native C topology.
2. Double press the reset button to put the board into programming mode. The LED_BUILTIN will fade to show you it is ready for programming.

How this will work when we move to UART and Raspi I do not know at this point

## Programming Arduino Nano IoT 33 - WITH Raspi & UART

adding arduino SAMD21 boards to board manager

http://downloads.arduino.cc/Hourly/samd/package_samd-hourly-build_index.json

 
 #### Connections:
 Pin Numbers
 |Nano IoT 33	|Rpi PinName		| Rpi PinNo.	|
 |---		|---			|---		|
 | 3v3		| 3v3			| 1		|
 | GND		| GND			| 6		|
 | Rx0		| GPIO 14 (UART0 TxD)	| 8 		|
 | Tx1		| GPIO 15 (UART0 RxD)	| 10		|
 | RST		| GPIO 18		| 12		|

as above with programming Nano, but changed compile and upload lines to:

##### 6. Compile .ino file inside arduino sketch folder using command:

        arduino-cli compile --fqbn arduino:samd:nano_33_iot currentSense

##### 7. Upload compiled hex file to board:

       arduino-cli upload --port /dev/ttyAMA0 --fqbn arduino:samd:nano_33_iot ~/currentSense
       
 THIS IS WORKING!!! BUUUUT
 
 Need to hit reset x2 on the board to put it into programming mode, I am wondering if the python file we modified in AVRdude can
 be further modified to provide a double tap on the reset.


------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------
# General Arduino Command Line Interface (cli) Help
[Getting Started Arduino CLI](https://create.arduino.cc/projecthub/B45i/getting-started-with-arduino-cli-7652a5)
[Arduono CLI Commands List](https://arduino.github.io/arduino-cli/0.21/commands/arduino-cli/)

To add board managers they must be added to the .yaml file found at

`/home/pi/.arduino15/arduino-cli.yaml`

Then Install must be run to install boards

Arduino board support like Leonardo/Uno
`arduino-cli core install arduino:avr`
Or if you need SAMD21/SAMD51 support:
`arduino-cli core install arduino:samd`




List attached boards:

`arduino-cli board list`
Doesnt always work

Update the core after adding new board manager URLs to
`Local\Arduino15\arduino-cli.yaml`

`arduino-cli core update-index`

then run install for added board libraries

`arduino-cli core install esp8266:esp8266`

install / search for libraries
`arduino-cli lib search LibraryName`

List All Installed Board FQBNs

`arduino-cli board listall`

## THIS ONE IS USEFUL

Open a communication port with a board.


`arduino-cli monitor [flags]`


### FQBN - Fully Qualified Board Names 


Here the FQBN stands for Fully Qualified board name, which is used to denote a particular board.
for example arduino:avr:nano is the fqbn for Arduino Nano, and arduino:avr:uno for Uno boards

|Board Name |                         FQBN |
|---	|---	|
|Adafruit Circuit Playground         |arduino:avr:circuitplay32u4cat
|Adafruit Circuit Playground Express |arduino:samd:adafruit_circuitplayground_m0
|Arduino BT                          |arduino:avr:bt
|Arduino Duemilanove or Diecimila    |arduino:avr:diecimila
|Arduino Esplora                     |arduino:avr:esplora
|Arduino Ethernet                    |arduino:avr:ethernet
|Arduino Fio                         |arduino:avr:fio
|Arduino Gemma                       |arduino:avr:gemma
|Arduino Industrial 101              |arduino:avr:chiwawa
|Arduino Leonardo                    |arduino:avr:leonardo
|Arduino Leonardo ETH                |arduino:avr:leonardoeth
|Arduino M0                          |arduino:samd:mzero_bl
|Arduino M0 Pro (Native USB Port)    |arduino:samd:mzero_pro_bl
|Arduino M0 Pro (Programming Port)   |arduino:samd:mzero_pro_bl_dbg
|Arduino MKR FOX 1200                |arduino:samd:mkrfox1200
|Arduino MKR GSM 1400                |arduino:samd:mkrgsm1400
|Arduino MKR NB 1500                 |arduino:samd:mkrnb1500
|Arduino MKR Vidor 4000              |arduino:samd:mkrvidor4000
|Arduino MKR WAN 1300                |arduino:samd:mkrwan1300
|Arduino MKR WAN 1310                |arduino:samd:mkrwan1310
|Arduino MKR WiFi 1010               |arduino:samd:mkrwifi1010
|Arduino MKR1000                     |arduino:samd:mkr1000
|Arduino MKRZERO                     |arduino:samd:mkrzero
|Arduino Mega ADK                    |arduino:avr:megaADK
|Arduino Mega or Mega 2560           |arduino:avr:mega
|Arduino Micro                       |arduino:avr:micro
|Arduino Mini                        |arduino:avr:mini
|Arduino NANO 33 IoT                 |arduino:samd:nano_33_iot
|Arduino NG or older                 |arduino:avr:atmegang
|rduino Nano                         |arduino:avr:nano
|Arduino Pro or Pro Mini             |arduino:avr:pro
|Arduino Robot Control               |arduino:avr:robotControl
|Arduino Robot Motor                 |arduino:avr:robotMotor
|Arduino Tian                        |arduino:samd:tian
|Arduino Uno                         |arduino:avr:uno
|Arduino Uno Mini                    |arduino:avr:unomini
|Arduino Uno WiFi                    |arduino:avr:unowifi
|Arduino Yún                         |arduino:avr:yun
|Arduino Yún Mini                    |arduino:avr:yunmini
|Arduino Zero (Native USB Port)      |arduino:samd:arduino_zero_native
|Arduino Zero (Programming Port)     |arduino:samd:arduino_zero_edbg
|LilyPad Arduino                     |arduino:avr:lilypad
|LilyPad Arduino USB                 |arduino:avr:LilyPadUSB
|Linino One                          |arduino:avr:one|


