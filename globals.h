byte midiChannel = 9;     // The global MIDI channel
byte tempo = 120;         // Set the tempo
bool playing = 0;         // Are we playing a tune?
bool paused = 0;          // Are we paused?
bool stopped = 1;         // Are we stopped? (Must init to 1)
volatile unsigned long  ticks = 0;            // A place to store the amount of MIDI ticks received
byte songPosition = 0;    // A place to store the current MIDI song position
byte lastNote;            // A place to remember the last MIDI note we played

byte shiftA = B00000001;  // A buffer to store the values in the right shift register
byte shiftB = B00000001;  // A buffer to store the values in the left shift register
byte seqPos = 0;          // What position in the sequence are we in?
bool seqLedRefresh = 1;   // Should we refresh the LED array?
byte playingPattern = 0;  // The currently playing pattern, 0-7
byte patternAmount = 2;   // How many patterns will play



// Default setting that will play each step in the first pattern and skip the remaining patterns.
// states:
// -1: skip
// 0: mute
// 1: play
// 2: accent

char stepPlay[8][16] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// Is this note slided? Default is 0
bool stepSlide[8][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

/*
  // Holds the note values. Default should be 60, which is C3
  // Why the hell doesn't this work?
  byte stepNotes[8][16] = {
  {37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,},
  {37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
*/
byte pattern0[] = {37, 37, 37, 37, 82, 37, 37, 37, 37, 37, 37, 37, 82, 37, 37, 37};
byte pattern1[] = {37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37};
byte pattern2[16];
byte pattern3[16];
byte pattern4[16];
byte pattern5[16];
byte pattern6[16];
byte pattern7[16];
