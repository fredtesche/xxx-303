#include <SPI.h>
#include <MIDI.h>
#include <Bounce.h>
#include <TimerThree.h>
#include <LiquidCrystalFast.h>
#include <Encoder.h>
#include <AltSoftSerial.h>
#include "pins.h"
#include "defines.h"
#include "globals.h"
#include "functions.h"
#include "bpm.h"
#include "int_clock.h"
#include "ext_clock.h"

Bounce stopButton = Bounce(7, 5);
Bounce playButton = Bounce(8, 5);

LiquidCrystalFast lcd(LCD_RS, LCD_RW, LCD_Enable, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
AltSoftSerial altSerial;

void setup() {
  pinMode(Button_00, INPUT_PULLUP);
  pinMode(Button_01, INPUT_PULLUP);
  pinMode(Button_02, INPUT_PULLUP);
  pinMode(Button_03, INPUT_PULLUP);
  pinMode(Button_04, INPUT_PULLUP);
  pinMode(Button_05, INPUT_PULLUP);
  pinMode(Button_06, INPUT_PULLUP);
  pinMode(Button_07, INPUT_PULLUP);
  pinMode(Button_08, INPUT_PULLUP);
  pinMode(Button_09, INPUT_PULLUP);
  pinMode(Button_10, INPUT_PULLUP);
  pinMode(Button_11, INPUT_PULLUP);
  pinMode(Button_12, INPUT_PULLUP);
  pinMode(Button_13, INPUT_PULLUP);
  pinMode(SlideEnable, OUTPUT);
  pinMode(SawLevel, OUTPUT);
  pinMode(SquareLevel, OUTPUT);
  pinMode(DecayLevel, OUTPUT);
  pinMode(SRSelect, OUTPUT);
  pinMode(PE1InterruptA, OUTPUT);
  pinMode(PE1InterruptB, OUTPUT);
  pinMode(PortExp0Sel, OUTPUT);
  pinMode(PortExp1Sel, OUTPUT);
  pinMode(DigPot1Select, OUTPUT);
  pinMode(DigPot2Select, OUTPUT);
  pinMode(DigPot3Select, OUTPUT);
  pinMode(DigPot4Select, OUTPUT);

  Timer3.initialize(bpm126);
  Timer3.attachInterrupt(handleIntClock);
  Timer3.stop();

  // Handle incoming MIDI events
  MIDI.setHandleClock(handleExtClock);
  MIDI.setHandleStart(handleExtStart);
  MIDI.setHandleContinue(handleExtContinue);
  MIDI.setHandleStop(handleExtStop);

  altSerial.begin(31250);
  Serial.begin(31250);
  MIDI.begin(midiChannel);
  digitalWrite(LCD_RW, LOW);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("xxx-303");
  SPI.begin();

  // Port Expander 0 configs (buttons)
  spiWrite(PortExp0Sel, PortExp0, IOCONA,   B00101000);           // Chip config
  spiWrite(PortExp0Sel, PortExp0, IOCONB,   B00101000);
  spiWrite(PortExp0Sel, PortExp0, IODIRA,   B11111111);           // Pin direction
  spiWrite(PortExp0Sel, PortExp0, IODIRB,   B11111111);
  spiWrite(PortExp0Sel, PortExp0, GPPUA,    B11111111);           // Pullup resistor state
  spiWrite(PortExp0Sel, PortExp0, GPPUB,    B11111111);
  spiWrite(PortExp0Sel, PortExp0, GPINTENA, B00000000);           // Enable interrupts
  spiWrite(PortExp0Sel, PortExp0, GPINTENB, B00000000);
  spiWrite(PortExp0Sel, PortExp0, DEFVALA,  B11111111);           // Default pin values
  spiWrite(PortExp0Sel, PortExp0, DEFVALB,  B11111111);

  // Port Expander 1 configs (encoders)
  spiWrite(PortExp1Sel, PortExp1, IOCONA,   B00101000);           // Chip config
  spiWrite(PortExp1Sel, PortExp1, IOCONB,   B00101000);
  spiWrite(PortExp1Sel, PortExp1, IODIRA,   B11111111);           // Pin direction
  spiWrite(PortExp1Sel, PortExp1, IODIRB,   B11111111);
  spiWrite(PortExp1Sel, PortExp1, GPPUA,    B11111111);           // Pullup resistor state
  spiWrite(PortExp1Sel, PortExp1, GPPUB,    B11111111);
  spiWrite(PortExp1Sel, PortExp1, GPINTENA, B11111111);           // Enable interrupts
  spiWrite(PortExp1Sel, PortExp1, GPINTENB, B11111111);
  spiWrite(PortExp1Sel, PortExp1, INTCONA,  B11111111);           // Interrupt Configuration
  spiWrite(PortExp1Sel, PortExp1, INTCONB,  B11111111);
  spiWrite(PortExp1Sel, PortExp1, DEFVALA,  B00000000);           // Default pin values
  spiWrite(PortExp1Sel, PortExp1, DEFVALB,  B00000000);

}

void loop() {

  playButton.update();
  stopButton.update();

  /*
      Poll the port expander that holds pushbutton switches.
      These switches are to select each step. When the switch is
      pressed, the step is selected and then the step can be
      modified with the other controls. These don't need to be
      interrupts because step selection isn't as time sensitive
      as some other controls.

      Not sure why these switches don't need to be debounced but
      hey whatever man. Also, move this to a function for
      cleanliness at some point.
  */

  PortExp0BankA = spiRead(PortExp0Sel, 0, GPIOA);
  PortExp0BankB = spiRead(PortExp0Sel, 0, GPIOB);

  for (byte i = 0; i < 8; i++) {
    if (bitRead(PortExp0BankA, i) != bitRead(PortExp0BankA_Old, i)) {
      if (bitRead(PortExp0BankA, i) < bitRead(PortExp0BankA_Old, i)) {
        Serial.println("Push");
      } else {
        Serial.println("Release");
      }
    }
    if (bitRead(PortExp0BankB, i) != bitRead(PortExp0BankB_Old, i)) {
      if (bitRead(PortExp0BankB, i) < bitRead(PortExp0BankB_Old, i)) {
        Serial.println("Push");
      } else {
        Serial.println("Release");
      }
    }
  }

  PortExp0BankA_Old = PortExp0BankA;
  PortExp0BankB_Old = PortExp0BankB;

  switch (clockSource) { // Using internal clock or external clock?
    case 0:
      if (playButton.fallingEdge()) {
        Serial.println("Play pressed");
        if (playing == 0) { // Only do stuff if the sequencer is not running
          if (paused == 1 && stopped == 0) {
            Timer3.resume();
            seqContinue();
          } else if (paused == 0 && stopped == 1) {
            Timer3.restart();
            seqStart();
          }
        }
      } // End play button logic

      if (stopButton.fallingEdge()) {
        Serial.println("Stop pressed");
        if (stopped == 0) { // Only do stuff if we are paused or playing
          if (playing == 0 && paused == 1) {
            Timer3.stop();
            usbMIDI.sendNoteOff(lastNote, 0, midiChannel);
            seqStop();
          }
          if (playing == 1 && paused == 0) {
            Timer3.stop();
            usbMIDI.sendNoteOff(lastNote, 0, midiChannel);
            seqPause();
          }

        }
      } // End stop button logic
      break;
    case 1:
      MIDI.read();
      usbMIDI.read();

      if (usbMIDI.read()) {
      }

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

    shiftWrite(SRSelect, shiftA, shiftB);
    seqLedRefresh = 0;
  }

}
