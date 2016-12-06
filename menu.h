// Menu logic
//
// Control
// -1: Previous
// 0: Next
// 1: Enter
//
// Output
// false: Menu has not changed
// true: Menu has changed
//
// "Control" only modifies id.
// id is passed to logic to determine where to go next.

byte id = 0;
byte previd = 0;

bool menu(char control) {

  switch (control) {
    case -1: // Prev
      id--;
      break;
    case 1: // Enter
      break;
    case 2: // Next
      id++;
      break;
  }

  switch (id) {
    case 1:
      lcdA = "Screen One";
      break;
    case 2:
      lcdA = "Screen Two";
      break;
    case 3:
      lcdA = "Clock Source";
      switch(clockSource){
        case 0:
        lcdB = "Internal";
        break;
        case 1:
        lcdB = "External";
        break;
      }
      break;
  }

  if (id < 0) {
    id = 0;
  }
  
  if (id > 3) {
    id = 3;
  }

  return true;
}
