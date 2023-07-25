/*

  Audio player, non blocking.
  read 8bit mono .wav file, up to 4 channels
  use Audacity to convert your audio file
  Author : AloyseTech with subsequent modifications by various people including,Hydronics and Gambalunga

  03/17/19: https://github.com/hydronics2/SamdAudio
    updated to work with Adafruit Qaud Flash memory boards.
    This will work with any SAMD21 chip using SPI flash with a little tinkering
     - itsyBitsy M0 Express, 
     - Feather M0 Express, 

   23/07/2023:
    Updated to work with the current version of the Adafruit_CPFS library.
    This library require that CircuitPython is first loaded on the Feather M0 Express.
    Once executed files can be uploaded from an attached computer to the CircuitPython "drive".
    The computer sees that attached Feather as a USB flash card.
    
     - Will not compile with Adafruit Feather M4 Express and other boards with the SAMD51 processor 

    

*/

#include <Adafruit_CPFS.h>
#include <SamdAudio_CPFS.h>

FatVolume *fs = NULL;  // CIRCUITPY flash filesystem, as a FAT pointer

SamdAudio AudioPlayer;

#define NUM_AUDIO_CHANNELS 1  //could be 1,2 or 4 for sound

#define AUDIO_BUFFER_SIZE 1024  //512 works fine for 22.05kh, use 1024 for 32khz and 44.1khz

//indicate sample rate here (use audacity to convert your wav)
const unsigned int sampleRate = 22050;  //hz


//************************************************************************

void setup() {
  // Start the CIRCUITPY flash filesystem FIRST. Very important!
  fs = Adafruit_CPFS::begin();
  // An optional initial true/false flag passed to begin() selects whether
  // the flash filesystem is presented to a USB-attached host computer (true),
  // or accessible only to code on the microcontroller (false).
  // For "Haxpress" boards (small M0 boards retrofitted with SPI flash),
  // a chip-select pin and/or SPI instance can follow like so:
  // fs = Adafruit_CPFS::begin(true, SS1, &SPI1); // QT Py M0 Haxpress

  // Start Serial AFTER Adafruit_CPFS, or CIRCUITPY won't show on computer.
  Serial.begin(115200);
  //while(!Serial);

  pinMode(LED_BUILTIN, OUTPUT);

  if (fs == NULL) {  // If CIRCUITPY filesystem is missing or malformed...
    // Show error message & blink LED to indicate problem. Full stop.
    Serial.println("Can't access board's CIRCUITPY drive.");
    Serial.println("Has CircuitPython been previously installed?");
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }  // else valid CIRCUITPY drive, proceed...

  // It's good to pause here -- a LOT happensin begin() when the filesystem is connected to USB,
  // and many rapid-fire change events occur. Allow a couple seconds to settle...
  delay(2500);

  Serial.println("Mounted filesystem!");
  // end fash setup

  Serial.print("Initializing Audio Player...");
  if (AudioPlayer.begin(sampleRate, NUM_AUDIO_CHANNELS, AUDIO_BUFFER_SIZE) == -1) {
    Serial.println(" failed!");
    return;
  }
  Serial.println(" done.");
/* // Commented out because if it runs in setup it can block the USB
  AudioPlayer.play("sfx1_8bit_22khz.wav", 0);
  Serial.println("Playing file.....");
  */
}

void loop() {

  if (Serial.available()) {
    char c = Serial.read();
    /*
    // inserted to use files already loaded
    if ( c == 'a') 
    {
      AudioPlayer.play("Alarm.wav", 0); //playing file  on channel 1
      Serial.println("playing audio file on channel 1");
    }
    */
    if (c == 'o') {
      AudioPlayer.play("sfx1_8bit_22khz.wav", 0);  //playing file on channel 0
      Serial.println("playing audio file on channel 0");
    }
    if (c == 'p') {
      AudioPlayer.play("sfx2_8bit_22khz.wav", 1);  //playing file  on channel 1
      Serial.println("playing audio file on channel 1");
    }
    if (c == 'k') {
      AudioPlayer.play("sfx2_8bit_22khz.wav", 2);  //playing file on channel 2
      Serial.println("playing audio file on channel 2");
    }
    if (c == 'l') {
      AudioPlayer.play("sfx4_8bit_22khz.wav", 3);  //playing file on channel 3
      Serial.println("playing audio file on channel 3");
    } else if (c == '1') {
      AudioPlayer.stopChannel(0);
      Serial.println("ch0 off!");
    } else if (c == '2') {
      AudioPlayer.stopChannel(1);
      Serial.println("ch1 off!");
    } else if (c == '3') {
      AudioPlayer.stopChannel(2);
      Serial.println("ch2 off!");
    } else if (c == '4') {
      AudioPlayer.stopChannel(3);
      Serial.println("ch3 off!");
    }
  }
}
