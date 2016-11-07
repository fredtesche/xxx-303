// External clock functions

void handleExtClock(byte clockData) {

  switch (clockData) {
    case 248: // Tick tick, mother father!
      // Determine the currently playing pattern. 48 ticks in a pattern.
      playingPattern = (int)(ticks / 48);

      // Determine which led to light up. This is basically magic.
      seqPos = (ticks / 6) % 16;

      // If we've received ticks, then refresh the led array and play a note
      if (ticks % 6 == 0) {
        seqLedRefresh = 1;
        playNote();
      }

      ticks++;

      // Reset the ticks based on patternAmount
      if (ticks > (patternAmount * 48)) {
        ticks = 0;
      }
      break;
    case 250: // Start
      ticks = 0;
      seqStart();
      break;
    case 251: // Continue
      ticks = 0;
      seqContinue();
      break;
    case 252: // Stop
      ticks = 0;
      seqStop();
      break;
  }
  //MIDI.sendTimeCodeQuarterFrame(248);

}
