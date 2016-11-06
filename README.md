xxx-303
=
**A step sequencer that runs on Teensy 2.0. It's built to drive the [Open Labs x0x-Heart](http://www.openmusiclabs.com)**

Fred Tesche 2016  
www.fakecomputermusic.com  
fred@fakecomputermusic.com  

**Features:**
- 8 patterns of 16 steps each
- 16 LEDs to show the currently playing step
- Support for USB MIDI or DIN MIDI
- Support for internal or external clock
- Most controls on the front panel
- MIDI control of many knobs
- 2x16 LCD

**Required hardware:**
- [Teensy 2.0](https://pjrc.com)
- [x0x-Heart](http://www.openmusiclabs.com)
- [Midi2CV Mk2](https://midisizer.com/midi2cv-mk2/)
- [UART -> MIDI interface](http://www.hobbytronics.co.uk/midi-breakout?keyword=midi) (or build your own)
- 2x16 LCD
- 2 SN74HC595 shift registers
- Some buttons, pots, leds, and other crap

**TODO:**
- Add MIDI in/out
- Add USB MIDI in/out
- Build UI
- Figure out how digital pots work
- Get the altsoftserial stuff working
- Build out micros -> bpm table and move to program memory
