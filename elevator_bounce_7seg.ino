#include <elapsedMillis.h>
#include <Bounce2.h>

const uint8_t buttonPins[] = {10, 11, 12, 13};
const uint8_t pressedLED[] = {2, 3, 4, 5};
const uint8_t movingLED[] = {6, 7, 8, 9};
int binaryNum[] = {1, 0, 0, 0};
int startPos = 0;
Bounce * buttons = new Bounce[4];

void setup() {
  for(int i = 0; i < 4; i++) {
    pinMode(pressedLED[i], OUTPUT);
    pinMode(movingLED[i], OUTPUT);
    digitalWrite(pressedLED[i], LOW);
    digitalWrite(movingLED[i], binaryNum[i]);
    buttons[i].attach(buttonPins[i], INPUT_PULLUP);
    buttons[i].interval(25);
  }
}

void binary(int n) {
  for(int i = 0; i < 4; i++) {
    binaryNum[i] = n % 2;
    n /= 2;
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
      if(digitalRead(pressedLED[i]) == HIGH) {
        for(int j = 0; j < 10; j++)
          pressedButton();
        digitalWrite(pressedLED[i], LOW);
      } else
        for(int j = 0; j < 5; j++)
          pressedButton();
    }
  else if(startPos > endPos)
    for(int i = startPos + 1; i > endPos + 1; i--) {
      binary(i - 1);
      for(int i = 0; i < 4; i++)
        digitalWrite(movingLED[i], binaryNum[i]);
      for(int j = 0; j < 2; j++)
        pressedButton();
      if(digitalRead(pressedLED[i - 1]) == HIGH) {
        for(int j = 0; j < 10; j++)
          pressedButton();
        digitalWrite(pressedLED[i - 1], LOW);
      } else
        for(int j = 0; j < 5; j++)
          pressedButton();
    }
  digitalWrite(pressedLED[endPos], LOW);
  startPos = endPos;
}

int pressedButton() {
  elapsedMillis counter;
  while(counter < 100)
    for(int i = 0; i < 4; i++) {
      buttons[i].update();
      if(buttons[i].rose())
        digitalWrite(pressedLED[i], HIGH);
    }
}

void loop() {
  int endPos;
  pressedButton();

  for(int i = 0; i < 4; i++)
    if(digitalRead(pressedLED[i]) == HIGH) {
      endPos = i;
      moving(endPos);
    }
}
