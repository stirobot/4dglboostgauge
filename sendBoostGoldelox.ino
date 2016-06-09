#define DisplaySerial Serial

void setup() {
  DisplaySerial.begin(9600);
  //delay(10000);
}

int currentBoost = 0;
void loop() {
  DisplaySerial.flush(); 
  while(!DisplaySerial.available()) ; 
  if (DisplaySerial.read() == '!'){
    int iBoost = getBoost();  
    DisplaySerial.write('B');
    DisplaySerial.write(0x00);
    DisplaySerial.write(lowByte(iBoost));
  }
}

int getBoost(){
  if (currentBoost <= 21){
    currentBoost++;  
  }
  else {
    currentBoost = 0;
  }
  //return random(0,20);
  return currentBoost;
}

/*
 * TODOs:
 * -send all meter readings all the time
 * -be able to send a character to switch screens
 * -be able to send a character to reset peaks?
 * -warnings that are inverses
 * -try animating by hitting a frame in the middle of two readings
 * -how about only sending what is updated when sending??
 */
