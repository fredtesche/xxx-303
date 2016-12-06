byte midiChannel = 9;     // The global MIDI channel
//byte tempo = 120;         // Set the tempo
bool clockSource = 0;     // Internal clock (0), external clock (1)
bool playing = 0;         // Are we playing a tune?
bool paused = 0;          // Are we paused?
bool stopped = 1;         // Are we stopped? (Must init to 1)
//volatile unsigned long ticks = 0;   // A tick of the Teensy's internal clock
int ticks = 0;            // A tick of the clock
byte songPosition = 0;    // A place to store the current MIDI song position
byte lastNote;            // A place to remember the last MIDI note we played
byte shiftA = B00000001;  // A buffer to store the values in the right shift register
byte shiftB = B00000001;  // A buffer to store the values in the left shift register
byte seqPos = 0;          // What position in the sequence are we in?
bool seqLedRefresh = 1;   // Should we refresh the LED array?
int playingPattern = 0;  // The currently playing pattern, 0-7
byte patternAmount = 2;   // How many patterns will play

byte data;
int counter;

// Bytes to hold values received from port expanders
byte PortExp0BankA;
byte PortExp0BankB;
byte PortExp1BankA;
byte PortExp1BankB;
byte PortExp0BankA_Old;
byte PortExp0BankB_Old;
byte PortExp1BankA_Old;
byte PortExp1BankB_Old;

// Determine how to play a step
// -1: restart
// 0: mute
// 1: play
// 2: accent
char stepPlay[8][16] = {
  {1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// Slide note: 1
// Normal note: 0
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

// The MIDI values of the notes in each step.
// C3 = 40
byte stepNote[8][16] = {
  {40, 40, 0, 0, 42, 42, 43, 40, 40, 0, 0, 38, 38, 0, 40, 11},
  {40, 40, 0, 0, 42, 42, 43, 40, 40, 0, 0, 38, 38, 0, 40, 11},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
