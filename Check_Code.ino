#include <LiquidCrystal.h>

// Pin assignments
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
const int upButton = 2;
const int downButton = 3;
const int okButton = 4;
const int exitButton = 5;
const int motorIn1 = 14;
const int motorIn2 = 16;
const int enablePin = 17;

// Motor settings
int slowStart = 1000; // in milliseconds
int forwardSpeed = 80; // percentage
int reverseSpeed = 80; // percentage
int slowStop = 1000; // in milliseconds
int duration = 20; // in seconds
bool direction = true; // true for forward, false for reverse

// Menu variables
int currentMenu = 0;
const int menuItems = 6;

// LCD initialization
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 4);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(okButton, INPUT_PULLUP);
  pinMode(exitButton, INPUT_PULLUP);
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  Serial.begin(9600);
  // Initial display
  displayMenu();
}

void loop() {
  if (digitalRead(upButton) == HIGH) {
    Serial.println("Up button pressed");
    currentMenu = (currentMenu - 1 + menuItems) % menuItems;
    displayMenu();
    delay(300);
  }
  
  if (digitalRead(downButton) == HIGH) {
    Serial.println("Down button pressed");
    currentMenu = (currentMenu + 1) % menuItems;
    displayMenu();
    delay(300);
  }
  
  if (digitalRead(okButton) == HIGH) {
    Serial.println("OK button pressed");
    modifySetting();
    displayMenu();
    delay(300);
  }
  
  if (digitalRead(exitButton) == HIGH) {
    Serial.println("Exit button pressed");
    stopMotor();
    delay(300);
  }
}

void displayMenu() {
  lcd.clear();
  Serial.println(currentMenu);
  switch (currentMenu) {
    case 0:
      lcd.print("Slow Start: ");
      lcd.print(slowStart);
      lcd.print(" ms");
      break;
    case 1:
      lcd.print("Fwd Speed: ");
      lcd.print(forwardSpeed);
      lcd.print(" %");
      break;
    case 2:
      lcd.print("Rev Speed: ");
      lcd.print(reverseSpeed);
      lcd.print(" %");
      break;
    case 3:
      lcd.print("Slow Stop: ");
      lcd.print(slowStop);
      lcd.print(" ms");
      break;
    case 4:
      lcd.print("Duration: ");
      lcd.print(duration);
      lcd.print(" sec");
      break;
    case 5:
      lcd.print("Direction: ");
      lcd.print(direction ? "Fwd" : "Rev");
      break;
  }
}

void display(){
      displayMenu();
      lcd.print(" +(up)");
      lcd.print("-(down) ");
}

void modifySetting() {
  lcd.clear();
  switch (currentMenu) {
    case 0:
      display();
      adjustValue(slowStart, 0, 5000, 10);
      break;
    case 1:
      display();
      adjustValue(forwardSpeed, 0, 100, 1);
      break;
    case 2:
      display();
      adjustValue(reverseSpeed, 0, 100, 1);
      break;
    case 3:
      display();
      adjustValue(slowStop, 0, 5000, 10);
      break;
    case 4:
    display();
      adjustValue(duration, 1, 3600, 1); // Adjust duration from 1 second to 1 hour
      break;
    case 5:
      direction = !direction;
      break;
  }
}

void adjustValue(int &value, int minValue, int maxValue, int step) {
  delay(500);
  while (true) {
    if (digitalRead(upButton) == HIGH) {
      value = constrain(value + step, minValue, maxValue);
      displayMenu();
      delay(300);
    }
    
    if (digitalRead(downButton) == HIGH) {
      value = constrain(value - step, minValue, maxValue);
      displayMenu();
      delay(300);
    }
    
    if (digitalRead(okButton) == HIGH) {
      delay(300);
      break;
    }
  }
}

void runMotor() {
  int speed = direction ? forwardSpeed : reverseSpeed;
  digitalWrite(motorIn1, direction ? HIGH : LOW);
  digitalWrite(motorIn2, direction ? LOW : HIGH);
  analogWrite(enablePin, map(speed, 0, 100, 0, 255));
  
  delay(duration * 1000);
  stopMotor();
}

void stopMotor() {
  for (int i = analogRead(enablePin); i >= 0; i -= 5) {
    analogWrite(enablePin, i);
    delay(slowStop / 50); // Adjust for smooth stop
  }
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  analogWrite(enablePin, 0);
}
