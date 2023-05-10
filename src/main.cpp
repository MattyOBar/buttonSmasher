#include <Arduino.h>
#include <ezButton.h>
#include <LiquidCrystal.h>

//Button pins
ezButton button1(34);
ezButton button2(35);
ezButton button3(36);
ezButton button4(37);
ezButton button5(38);
ezButton startButton(40);

//Green LED pins
const int greenLed1 = 27;
const int greenLed2 = 28;
const int greenLed3 = 29;
const int greenLed4 = 30;
const int greenLed5 = 31;

//Red LED pins
const int redLed1 = 22;
const int redLed2 = 23;
const int redLed3 = 24;
const int redLed4 = 25;
const int redLed5 = 26;

//LCD Pins
const int rs = 52, en = 53, d4 = 46, d5 = 47, d6 = 48, d7 = 49;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Joystick Pins
const int joystickY = A7;
const int joystickX = A6;

//Joystick values
#define up  0
#define down 1
#define right 2
#define left 3
#define none 4

int lastJoyRead;

//Variables
int score = 0;
bool gameOn = false;
bool finished = false;
int cursorColumn = 0;
int character = 0;
unsigned long time_now = 0;
int delayTime = 1000;
String name = "";

//Setup function
void setup() {
  Serial.begin(115200);
  button1.setDebounceTime(50);
  button2.setDebounceTime(50);
  button3.setDebounceTime(50);
  button4.setDebounceTime(50);
  button5.setDebounceTime(50);
  startButton.setDebounceTime(50);

  lcd.begin(16, 2);
  lcd.print("Press the green");
  lcd.setCursor(0, 1);
  lcd.print("button to start!");

  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);  
  pinMode(24, OUTPUT);  
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(30, OUTPUT);
  pinMode(31, OUTPUT);
}

//This function handles all of the LEDs and buttons during the gameplay
void gamePlay() {

  int randomButtonNum = random(1,5);
  
  switch(randomButtonNum) {
    case 1:
      digitalWrite(greenLed1, HIGH);
      digitalWrite(greenLed2, LOW);
      digitalWrite(greenLed3, LOW);
      digitalWrite(greenLed4, LOW);
      digitalWrite(greenLed5, LOW);
      digitalWrite(redLed1, LOW);
      digitalWrite(redLed2, HIGH);
      digitalWrite(redLed3, HIGH);
      digitalWrite(redLed4, HIGH);
      digitalWrite(redLed5, HIGH);
      
      while(button1.getStateRaw() == 1) {
      }

      break;
  
  case 2:
      digitalWrite(greenLed1, LOW);
      digitalWrite(greenLed2, HIGH);
      digitalWrite(greenLed3, LOW);
      digitalWrite(greenLed4, LOW);
      digitalWrite(greenLed5, LOW);
      digitalWrite(redLed1, HIGH);
      digitalWrite(redLed2, LOW);
      digitalWrite(redLed3, HIGH);
      digitalWrite(redLed4, HIGH);
      digitalWrite(redLed5, HIGH);
      
      while(button2.getStateRaw() == 1) {
      }

      break;

  case 3:
      digitalWrite(greenLed1, LOW);
      digitalWrite(greenLed2, LOW);
      digitalWrite(greenLed3, HIGH);
      digitalWrite(greenLed4, LOW);
      digitalWrite(greenLed5, LOW);
      digitalWrite(redLed1, HIGH);
      digitalWrite(redLed2, HIGH);
      digitalWrite(redLed3, LOW);
      digitalWrite(redLed4, HIGH);
      digitalWrite(redLed5, HIGH);
      
      while(button3.getStateRaw() != 0) {
      }
      
      break;

  case 4:
      digitalWrite(greenLed1, LOW);
      digitalWrite(greenLed2, LOW);
      digitalWrite(greenLed3, LOW);
      digitalWrite(greenLed4, HIGH);
      digitalWrite(greenLed5, LOW);
      digitalWrite(redLed1, HIGH);
      digitalWrite(redLed2, HIGH);
      digitalWrite(redLed3, HIGH);
      digitalWrite(redLed4, LOW);
      digitalWrite(redLed5, HIGH);
      Serial.print(button4.getState());
      while(button4.getStateRaw() != 0) {
      }

      break;

  case 5:
      digitalWrite(greenLed1, LOW);
      digitalWrite(greenLed2, LOW);
      digitalWrite(greenLed3, LOW);
      digitalWrite(greenLed4, LOW);
      digitalWrite(greenLed5, HIGH);
      digitalWrite(redLed1, HIGH);
      digitalWrite(redLed2, HIGH);
      digitalWrite(redLed3, HIGH);
      digitalWrite(redLed4, HIGH);
      digitalWrite(redLed5, LOW);
      

      while(button5.getStateRaw() != 0) {
      }

      break;
  
  default:

    break;
  }
}

//This function is used to turn off all of the LEDs.
void clearLeds() {
  digitalWrite(greenLed1, LOW);
  digitalWrite(greenLed2, LOW);
  digitalWrite(greenLed3, LOW);
  digitalWrite(greenLed4, LOW);
  digitalWrite(greenLed5, LOW);
  digitalWrite(redLed1, LOW);
  digitalWrite(redLed2, LOW);
  digitalWrite(redLed3, LOW);
  digitalWrite(redLed4, LOW);
  digitalWrite(redLed5, LOW);

}

//This function handles reading the analog input for the joystick
int readJoystick() {
  int output = none;
  int xPosition = analogRead(joystickX);
  int yPosition = analogRead(joystickY);
  xPosition = map(xPosition, 0, 1023, 1023, 0);
  yPosition = map(yPosition, 0, 1023, 1023, 0);

  if (xPosition >= 900) {
    output = right;
  } else if (xPosition <= 100) {
    output = left;
  } else if (yPosition >= 900) {
    output = up;
  } else if (yPosition <= 100) {
    output = down;
  }
  return output;
}

//This function lets the user input their initials
void initials() {
  String characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ "; 
  int index = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Input Name: ");
  lcd.setCursor(0, 1);
  lcd.blink();
  lcd.write(characters.indexOf(0));
  lcd.setCursor(0, 1);

  finished = false;

  while (finished == false) {
    time_now = millis();
    int currentJoyRead = readJoystick();

    if (currentJoyRead != lastJoyRead) {
      lastJoyRead = currentJoyRead;
    }

    switch(currentJoyRead) {
      case up:
      if (index < 28) {
        index += 1;
        lcd.write(characters.indexOf(index));
        lcd.setCursor(cursorColumn, 1);
        while(millis() < time_now + delayTime){}
      }
        
        break;
      case down:
      if (index > 0) {
        index -= 1;
        lcd.write(characters.indexOf(index));
        lcd.setCursor(cursorColumn, 1);
        while(millis() < time_now + delayTime){}
      }
        
        break;
      case left:
        if(cursorColumn > 0) {
          cursorColumn = cursorColumn - 1;
          lcd.setCursor(cursorColumn, 1);
          while(millis() < time_now + delayTime){}
        }

        break;
      case right:
        if (cursorColumn < 15) {
          cursorColumn = cursorColumn + 1;
          lcd.setCursor(cursorColumn, 1);
          while(millis() < time_now + delayTime){}
        }

        break;
      default: break;
      }
  }
}

//Main loop function
void loop() {
  button1.loop();
  button2.loop();
  button3.loop();
  button4.loop();
  button5.loop();
  startButton.loop();

  if(startButton.getStateRaw() == 0){
    gameOn = true;
    score = 0;
  }

  if (gameOn == true) {
    score = 0;
    while (score < 10) {
      lcd.clear();  
      lcd.setCursor(0,0);
      lcd.print("Score: ");
      lcd.setCursor(0,1);
      lcd.print(score);
      gamePlay();  
      score = score + 1;

    }
    gameOn = false;
    clearLeds();
    lcd.clear();  
    lcd.setCursor(0,0);
    lcd.print("You win!");
    time_now = millis();
    while(millis() < time_now + 2000){}
    initials();
  }
}





