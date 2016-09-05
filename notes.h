/*
  #include <MIDI.h>

  #define midiChannel 5
  #define slidePin 3
  #define accentPin 8
*/

/*
  byte currentPattern = 0;
  byte currentStep = 0;
  byte lastNote = 0;

  void setup() {
  Serial.begin(31250);
  pinMode(slidePin, OUTPUT);
  pinMode(accentPin, OUTPUT);
  }

  void tick() {

  // Check for slide
  switch (stepSlide[currentPattern][currentStep]) {
    case 0:
      digitalWrite(slidePin, LOW);
      break;
    case 1:
      digitalWrite(slidePin, HIGH);
      break;
  }

*/
// Play a note
void playPattern(byte seqPos, byte playingPattern) {

  // Turn the last note off, play a new note that is accented, and remember what note we played
  // digitalWrite(accentPin, HIGH);
  MIDI.sendNoteOff(lastNote, 127, midiChannel);
  MIDI.sendNoteOn(pattern0[seqPos], 127, midiChannel);
  lastNote = pattern0[seqPos];


  /*
    switch (stepPlay[playingPattern][seqPos]) {
    case -1:
      // Skip to the next pattern by triggering the next logic
      seqPos = 16;
      break;
    case 0:
      // Turn off the previous note
      MIDI.sendNoteOff(lastNote, 127, midiChannel);
      break;
    case 1:
      // Turn the last note off, play a new note, and remember what note we played
      // digitalWrite(accentPin, LOW);
      MIDI.sendNoteOff(lastNote, 127, midiChannel);
      MIDI.sendNoteOn(stepNotes[playingPattern][seqPos], 127, midiChannel);
      lastNote = stepNotes[playingPattern][seqPos];
      break;
    case 2:
      // Turn the last note off, play a new note that is accented, and remember what note we played
      // digitalWrite(accentPin, HIGH);
      MIDI.sendNoteOff(lastNote, 127, midiChannel);
      MIDI.sendNoteOn(stepNotes[playingPattern][seqPos], 127, midiChannel);
      lastNote = stepNotes[playingPattern][seqPos];
      break;
    }

    if (currentStep < 16) {       // If we are not finished with the pattern
      currentStep++;              // Then move to the next step
    } else {                      // If we are finished with the pattern
      currentStep = 0;            // Reset to the first step in a sequence
      currentPattern++;           // Move to the next pattern

      if (currentPattern > 7) {   // If we are finished with all the patterns
        currentStep = 0;          // Start over a sequence
        currentPattern = 0;       // Start over the patterns
      }
    }
    }
  */
}

