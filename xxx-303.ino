/*
   xxx-303
   A step sequencer that runs on Teensy 2.0. It's built to drive the Open Labs x0x-heart.

   Fred Tesche 2016
   www.fakecomputermusic.com
   fred@fakecomputermusic.com

   Features:
   8 patterns of 16 steps each
   16 LEDs to show the currently playing step
   Support for USB MIDI or DIN MIDI
   Support for internal or external clock
   Most controls on the front panel
   MIDI control of many knobs
   2x16 LCD

   Required hardware:
   Teensy 2.0 (by PJRC)
   x0x-heart (by Open Labs)
   Midi2CV Mk2 (by MidiSizer)
   UART -> MIDI interface
   2x16 LCD
   2 SN74HC595 shift registers
   Some buttons and knobs and crap

   TODO:
   Add MIDI in/out
   Build UI
   Figure out how digital pots work
   Get the altsoftserial stuff working
   Build out micros -> bpm table and move to program memory

*/


#include <SPI.h>
#include <MIDI.h>
#include <Bounce.h>
#include <TimerOne.h>
#include "globals.h"
#include "functions.h"
#include "clock.h"
#include "bpm.h"

#define ss 0

Bounce playButton = Bounce(21, 5);
Bounce stopButton = Bounce(20, 5);

void setup() {
  Timer1.initialize(bpm126);
  Timer1.attachInterrupt(handleClock);
  Timer1.stop();
  pinMode(20, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);
  pinMode(ss, OUTPUT);
  SPI.begin();
  Serial.begin(9600);

  // Handle incoming MIDI events
  //MIDI.setHandleClock(handleClock);
  //MIDI.setHandleStart(seqStart);
  //MIDI.setHandleContinue(seqContinue);
  //MIDI.setHandleStop(seqStop);
  //MIDI.setHandleSongPosition(handleSongPosition);

  MIDI.begin(9);
}

void sendMidiClock() {
  MIDI.sendRealTime(Clock);
}

void loop() {
  MIDI.read();
  playButton.update();
  stopButton.update();

  if (playButton.fallingEdge()) {
    if (playing == 0) { // Only do stuff if the sequencer is not running
      if (paused == 1 && stopped == 0) {
        //MIDI.sendSongPosition(math to determine song position, based on playingPattern and seqPos);
        //MIDI.sendRealTime(Start);
        Timer1.resume();
        seqContinue();
      } else if (paused == 0 && stopped == 1) {
        //MIDI.sendSongPosition(0);
        //MIDI.sendRealTime(Start);
        Timer1.restart();
        seqStart();
      }

    }
  } // End play button logic

  if (stopButton.fallingEdge()) {
    if (stopped == 0) { // Only do stuff if we are paused or playing
      if (playing == 0 && paused == 1) {
        //MIDI.sendRealTime(Stop);
        //MIDI.sendSongPosition(0);
        Timer1.stop();
        usbMIDI.sendNoteOff(lastNote, 0, midiChannel);
        seqStop();
      }
      if (playing == 1 && paused == 0) {
        //MIDI.sendRealTime(Stop);
        Timer1.stop();
        usbMIDI.sendNoteOff(lastNote, 0, midiChannel);
        seqPause();
      }

    }
  } // End stop button logic

  if (seqLedRefresh == 1) {

    if (seqPos < 8) {
      shiftB = B00000001 << seqPos;
      shiftA = B00000000;
    }
    if (seqPos > 7) {
      shiftB = B00000000;
      shiftA = B00000001 << (seqPos - 8);
    }
    seqPos++;
    if (seqPos > 15) {
      seqPos = 0;
    }

    digitalWrite(ss, LOW);
    SPI.transfer(shiftA);
    SPI.transfer(shiftB);
    digitalWrite(ss, HIGH);
    seqLedRefresh = 0;
  }


}



