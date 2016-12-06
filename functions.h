void spiWrite(byte select, byte chip, byte addr, byte data) {
  digitalWrite(select, LOW);
  SPI.transfer(B01000000 + chip); // Send the opcode and the chip address
  SPI.transfer(addr); // Select the register
  SPI.transfer(data); // Select the pin to read (with 0 and 1)
  digitalWrite(select, HIGH);
}

void shiftWrite(byte select, byte shiftA, byte shiftB) {
  digitalWrite(select, LOW);
  SPI.transfer(shiftA);
  SPI.transfer(shiftB);
  digitalWrite(select, HIGH);
}

int spiRead(byte select, byte chip, byte reg) {
  digitalWrite(select, LOW);
  chip = chip | B00000001; // Add read bit to chip address
  SPI.transfer(B01000000 + chip); // Send the opcode and chip address
  SPI.transfer(reg);      // Select which register to read
  data = SPI.transfer(0x00); // Spi library tx/rx simultaneously, so tx any data to rx
  digitalWrite(select, HIGH);
  return data;
}

// Various functions

void seqStart() {
  playing = 1;
  paused = 0;
  stopped = 0;
}

void seqContinue() {
  playing = 1;
  paused = 0;
  stopped = 0;
}

void seqPause() {
  playing = 0;
  paused = 1;
  stopped = 0;
}

void seqStop() {
  ticks = 0;
  seqPos = 0;
  playing = 0;
  paused = 0;
  stopped = 1;
  seqLedRefresh = 1;
}

// Play a note
void playNote() {
  //Serial.println(stepNote[playingPattern][seqPos]); // Debug

  switch (stepPlay[playingPattern][seqPos]) {
    case -1:
      // Skip the remaining notes
      seqPos = 16;
      break;
    case 0:
      // Don't play a note
      break;
    case 1:
      // Turn off the previous note and play a new note.
      // digitalWrite(accentPin, LOW);
      usbMIDI.sendNoteOff(lastNote, 0, midiChannel);
      usbMIDI.sendNoteOn(stepNote[playingPattern][seqPos], 127, midiChannel);
      lastNote = stepNote[playingPattern][seqPos];
      break;
    case 2:
      // Turn off the previous note, and play a new accented note
      // digitalWrite(accentPin, HIGH);
      usbMIDI.sendNoteOff(lastNote, 0, midiChannel);
      usbMIDI.sendNoteOn(stepNote[playingPattern][seqPos], 127, midiChannel);
      lastNote = stepNote[playingPattern][seqPos];
      break;
  }
}
