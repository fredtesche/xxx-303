#include <SPI.h>
#include <MIDI.h>
#include <Bounce.h>
#include <TimerOne.h>
#include "globals.h"
#include "functions.h"
#include "int_clock.h"
#include "ext_clock.h"
#include "bpm.h"

#define ss 0

Bounce playButton = Bounce(21, 5);
Bounce stopButton = Bounce(20, 5);

byte nothing;

void setup() {
  Timer1.initialize(bpm126);
  Timer1.attachInterrupt(handleIntClock);
  Timer1.stop();
  pinMode(20, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);
  pinMode(ss, OUTPUT);
  SPI.begin();
  Serial.begin(31250);

  // Handle incoming MIDI events
  MIDI.setHandleClock(handleExtClock);
  MIDI.setHandleStart(handleExtStart);
  MIDI.setHandleContinue(handleExtContinue);
  MIDI.setHandleStop(handleExtStop);
  //MIDI.setHandleSongPosition(songPosition);
  //usbMIDI.setHandleRealTimeSystem(handleExtUSBTransport);
  //bookmark
  usbMIDI.setHandleSongPosition(test); // Trying to figure out how to make song position work..... hmm
  MIDI.begin(midiChannel);
}

void test(byte what) {
  nothing = what;
}

void loop() {

  playButton.update();
  stopButton.update();

  switch (clockSource) { // Using internal clock or external clock?
    case 1:
      MIDI.read();
      usbMIDI.read();

      if (usbMIDI.read()) {
        Serial.println(nothing);
      }

      break;
    case 0:
      if (playButton.fallingEdge()) {
        if (playing == 0) { // Only do stuff if the sequencer is not running
          if (paused == 1 && stopped == 0) {
            Timer1.resume();
            seqContinue();
          } else if (paused == 0 && stopped == 1) {
            Timer1.restart();
            seqStart();
          }

        }
      } // End play button logic

      if (stopButton.fallingEdge()) {
        if (stopped == 0) { // Only do stuff if we are paused or playing
          if (playing == 0 && paused == 1) {
            Timer1.stop();
            usbMIDI.sendNoteOff(lastNote, 0, midiChannel);
            seqStop();
          }
          if (playing == 1 && paused == 0) {
            Timer1.stop();
            usbMIDI.sendNoteOff(lastNote, 0, midiChannel);
            seqPause();
          }

        }
      } // End stop button logic
      break;
  }

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



