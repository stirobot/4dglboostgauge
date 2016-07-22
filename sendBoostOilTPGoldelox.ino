#define DisplaySerial Serial


void setup() {
  DisplaySerial.begin(9600);
  //buttons set up as two buttons as per (http://tronixstuff.com/2011/01/11/tutorial-using-analog-input-for-multiple-buttons/)
  //using a 4.7 k ohm resistor
  //  no buttons is about 1012
  //  button A is about 127
  //  button B is about 14
  pinMode(A5, INPUT_PULLUP);
  //delay(10000);
}

int currentBoost = 0;
int currentOilT = 0;
int currentOilP = 0;
char mode = 'B';
int buttonV;
void loop() {
  buttonV = analogRead(A5);
  while ( (buttonV > 1000) && (buttonV < 1024) ) { //no buttons pressed
    buttonV = analogRead(A5);
    if ((buttonV > 10)  && (buttonV < 17)) { //centered around 14
      while (((analogRead(A5) > 10)  && (analogRead(A5) < 17))) { //debounce
      }
      if (mode == 'B') {
        mode = 'T';
      }
      else if (mode == 'T') {
        mode = 'P';
      }
      else if (mode == 'P') {
        mode = 'B';
      }
      //if (mode == 'P'){mode = 'D';}
      //else {mode == 'P';}
    }
    DisplaySerial.flush();
    while (!DisplaySerial.available()) ;
    if (DisplaySerial.read() == '!') {
      if (mode == 'B') {
        sendBoost();
      }
      if (mode == 'T') {
        sendOilT();
      }
      if (mode == 'P') {
        sendOilP();
      }
      if (mode == 'D') {
        sendBoth();
      }
    }
  }
}

void sendBoost() {
  int iBoost = getBoost();
  DisplaySerial.write('B');
  DisplaySerial.write(0x00);
  DisplaySerial.write(lowByte(iBoost));
  return;
}

void sendOilP() {
  int iPSI = getOilPSI();
  DisplaySerial.write('P');
  DisplaySerial.write(0x00);
  DisplaySerial.write(lowByte(iPSI));
  return;
}

void sendOilT() {
  int iTemp = getOilTemp();
  DisplaySerial.write('T');
  DisplaySerial.write(0x00);
  DisplaySerial.write(lowByte(iTemp)); //TODO: needs logic for dealing with values greater than 255 and printing nulls
}

void sendBoth() {
  int iPSI = getOilPSI();
  int iTemp = getOilTemp();
  DisplaySerial.write('D');
  DisplaySerial.write(lowByte(iPSI));
  DisplaySerial.write(lowByte(iTemp));
  //TODO: figure this part out
  return;
}

int getBoost() {
  if (currentBoost <= 21) {
    currentBoost++;
  }
  else {
    currentBoost = 0;
  }
  //return random(0,20);
  return currentBoost;
}

int getOilPSI() {
  if (currentOilP <= 120) {
    currentOilP++;
  }
  else {
    currentOilP = 0;
  }
  //return random(0,20);
  return currentOilP;
}

int getOilTemp() { //will have to pre-scale this one
  if (currentOilT <= 255) {
    currentOilT++;
  }
  else {
    currentOilT = 0;
  }
  //return random(0,20);
  return currentOilT;
}

/*
 * TODOs:
 * 1) Port over oil temp and psi code
 * 2) Port over boost code
 * 3) Add button logic
 *
 * -send all meter readings all the time
 * -be able to send a character to switch screens
 * -be able to send a character to reset peaks?
 * -warnings that are inverses
 * -try animating by hitting a frame in the middle of two readings
 * -how about only sending what is updated when sending??
 */
