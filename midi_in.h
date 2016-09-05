// Scratch pad

void handleClock() {

  if (playing == 1) {

    // Determine the currently playing pattern. This is basically magic.
    playingPattern = (int)(ticks / playingPattern);

    // Determine which led to light up. This is also basically magic.
    seqPos = (ticks / 6) % 16;

    // If there have been 6 ticks, then refresh the led array
    if (ticks % 6 == 0) {
      seqLedRefresh = 1;
    }

    ticks++;

    // Reset the ticks based on patternAmount
    if (ticks > ((patternAmount * 96) - 1)) {
      ticks = 0;
    }

  }

}

void seqStart() {
  paused = 0;
  ticks = 0;
  playing = 1;
}

void seqContinue() {
  paused = 0;
  playing = 1;
}

void seqPause() {
  playing = 0;
  paused = 1;
}

void seqStop() {
  playing = 0;
  ticks = 0;
  paused = 0;
}

void handleSongPosition(unsigned int beats) {

  // Only is sent when not playing
  if (playing == 0) {

    if (beats < 16) {
      playingPattern = 0;
    } else if (beats > 15 && beats < 32) {
      playingPattern = 1;
    } else if (beats > 31 && beats < 48) {
      playingPattern = 2;
    } else if (beats > 47 && beats < 64) {
      playingPattern = 3;
    } else if (beats > 63 && beats < 80) {
      playingPattern = 4;
    } else if (beats > 79 && beats < 96) {
      playingPattern = 5;
    } else if (beats > 95 && beats < 112) {
      playingPattern = 6;
    } else if (beats > 111 && beats < 128) {
      playingPattern = 7;
    }

  }

  switch (beats) {
    case 0: case 16: case 32: case 48: case 64: case 80: case 96: case 112:
      seqPos = 0;
      seqLedRefresh = 1;
      break;
    case 1: case 17: case 33: case 49: case 65: case 81: case 97: case 113:
      seqPos = 1;
      seqLedRefresh = 1;
      break;
    case 2: case 18: case 34: case 50: case 66: case 82: case 98: case 114:
      seqPos = 2;
      seqLedRefresh = 1;
      break;
    case 3: case 19: case 35: case 51: case 67: case 83: case 99: case 115:
      seqPos = 3;
      seqLedRefresh = 1;
      break;
    case 4: case 20: case 36: case 52: case 68: case 84: case 100: case 116:
      seqPos = 4;
      seqLedRefresh = 1;
      break;
    case 5: case 21: case 37: case 53: case 69: case 85: case 101: case 117:
      seqPos = 5;
      seqLedRefresh = 1;
      break;
    case 6: case 22: case 38: case 54: case 70: case 86: case 102: case 118:
      seqPos = 6;
      seqLedRefresh = 1;
      break;
    case 7: case 23: case 39: case 55: case 71: case 87: case 103: case 119:
      seqPos = 7;
      seqLedRefresh = 1;
      break;
    case 8: case 24: case 40: case 56: case 72: case 88: case 104: case 120:
      seqPos = 8;
      seqLedRefresh = 1;
      break;
    case 9: case 25: case 41: case 57: case 73: case 89: case 105: case 121:
      seqPos = 9;
      seqLedRefresh = 1;
      break;
    case 10: case 26: case 42: case 58: case 74: case 90: case 106: case 122:
      seqPos = 10;
      seqLedRefresh = 1;
      break;
    case 11: case 27: case 43: case 59: case 75: case 91: case 107: case 123:
      seqPos = 11;
      seqLedRefresh = 1;
      break;
    case 12: case 28: case 44: case 60: case 76: case 92: case 108: case 124:
      seqPos = 12;
      seqLedRefresh = 1;
      break;
    case 13: case 29: case 45: case 61: case 77: case 93: case 109: case 125:
      seqPos = 13;
      seqLedRefresh = 1;
      break;
    case 14: case 30: case 46: case 62: case 78: case 94: case 110: case 126:
      seqPos = 14;
      seqLedRefresh = 1;
      break;
    case 15: case 31: case 47: case 63: case 79: case 95: case 111: case 127:
      seqPos = 15;
      seqLedRefresh = 1;
      break;
  }

}
