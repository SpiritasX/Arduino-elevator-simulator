#include <elapsedMillis.h>
#include <Bounce2.h>

const uint8_t buttonPins[] = {2, 3, 4, 5};
const uint8_t pressedLED[] = {10, 11, 12, 13};
const uint8_t movingLED[] = {6, 7, 8, 9};
const uint8_t panicPin = 22;
const uint8_t NPN = 23;
int binaryNum[] = {1, 0, 0, 0};
bool shutdownChecker = false;
int startPos = 0;
Bounce * buttons = new Bounce[4];
Bounce panic = Bounce();

void setup() {
  for(int i = 0; i < 4; i++) {
    pinMode(pressedLED[i], OUTPUT);
    pinMode(movingLED[i], OUTPUT);
    digitalWrite(pressedLED[i], LOW);
    digitalWrite(movingLED[i], binaryNum[i]);
    buttons[i].attach(buttonPins[i], INPUT_PULLUP);
    buttons[i].interval(25);
  }
  panic.attach(panicPin, INPUT_PULLUP);
  panic.interval(25);
  pinMode(NPN, OUTPUT);
  digitalWrite(NPN, HIGH);
}

void binary(int n) {
  for(int i = 0; i < 4; i++) {
    binaryNum[i] = n % 2;
    n /= 2;
  }
}

void completeShutdown() {
  shutdownChecker = !shutdownChecker;
  if(shutdownChecker) {
    for(int i = 0; i < 4; i++) {
      digitalWrite(movingLED[i], LOW);
      digitalWrite(pressedLED[i], LOW);
      binaryNum[i] = 0;
    }
    digitalWrite(NPN, LOW);
  } else {
    binaryNum[0] = 1;
    digitalWrite(movingLED[0], HIGH);
    startPos = 0;
    digitalWrite(NPN, HIGH);
  }
}

void moving(int endPos) {
  if(endPos > startPos)
    for(int i = startPos + 1; i < endPos + 1; i++) {
      binary(i + 1);
      for(int i = 0; i < 4; i++)
        digitalWrite(movingLED[i], binaryNum[i]);
      for(int j = 0; j < 2; j++)
        pressedButton();
      if(shutdownChecker)
        return;
      if(digitalRead(pressedLED[i]) == HIGH) {
        for(int j = 0; j < 10; j++)
          pressedButton();
        if(shutdownChecker)
          return;
        digitalWrite(pressedLED[i], LOW);
      } else
        for(int j = 0; j < 5; j++)
          pressedButton();
        if(shutdownChecker)
          return;
    }
  else if(startPos > endPos)
    for(int i = startPos + 1; i > endPos + 1; i--) {
      binary(i - 1);
      for(int i = 0; i < 4; i++)
        digitalWrite(movingLED[i], binaryNum[i]);
      for(int j = 0; j < 2; j++)
        pressedButton();
      if(shutdownChecker)
        return;
      if(digitalRead(pressedLED[i - 1]) == HIGH) {
        for(int j = 0; j < 10; j++)
          pressedButton();
        if(shutdownChecker)
          return;
        digitalWrite(pressedLED[i - 1], LOW);
      } else
        for(int j = 0; j < 5; j++)
          pressedButton();
        if(shutdownChecker)
          return;
    }
  digitalWrite(pressedLED[endPos], LOW);
  startPos = endPos;
}

int pressedButton() {
  elapsedMillis counter;
  while(counter < 100)
    for(int i = 0; i < 4; i++) {
      buttons[i].update();
      panic.update();
      if(buttons[i].rose() && !shutdownChecker)
        digitalWrite(pressedLED[i], HIGH);
      if(panic.rose())
        completeShutdown();
    }
}

void loop() {
  int endPos;
  pressedButton();
   
  for(int i = 0; i < 4; i++) {
    digitalWrite(movingLED[i], binaryNum[i]);
    if(digitalRead(pressedLED[i]) == HIGH && !shutdownChecker) {
      endPos = i;
      moving(endPos);
    }
  }
}
