// Internal clock handlers and related functions

void handleClock() {

  if (playing == 1) {
    // Determine the currently playing pattern. 96 ticks in a pattern.
    playingPattern = (int)(ticks / 96);

    // Determine which led to light up. This is basically magic.
    seqPos = (ticks / 6) % 16;

    // If we've received ticks, then refresh the led array and play a note
    if (ticks % 6 == 0) {
      seqLedRefresh = 1;
      playNote();
    }

    ticks++;

    // Reset the ticks based on patternAmount
    if (ticks > (patternAmount * 96)) {
      ticks = 0;
    }
    //MIDI.sendTimeCodeQuarterFrame(248);

  }
}

