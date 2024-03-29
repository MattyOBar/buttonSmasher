#include <Arduino.h>
#include <ezButton.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>

//Button pins
ezButton button1(34);
ezButton button2(35);
ezButton button3(36);
ezButton button4(37);
ezButton button5(38);
ezButton startButton(40);
ezButton cancelButton(41);

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
const int rs = 42, en = 43, d4 = 44, d5 = 45, d6 = 46, d7 = 47;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Joystick Pins
const int joystickY = A10;
const int joystickX = A9;

//Joystick values
#define up  0
#define down 1
#define right 2
#define left 3
#define none 4

int lastJoyRead;

// Networking
byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0xA8, 0x18 }; // mac address on ethernet shield
byte ip[] = { 10, 0, 0, 177 };  // arduino ip address
byte server[] = {10, 0, 0, 9};
char serverChar[] = "10.0.0.9"; // LAN IP address for host machine 
char pathNameAdd[] = "/add/game";

EthernetClient client;

//Variables
int duration = 0;
bool duration15 = false;
bool duration30 = false;
const int seconds15 = 15000;
const int seconds30 = 30000;
int score = 0;
bool gameOn = false;
bool finished = false;
unsigned long time_now = 0;
int delayTime = 1000;
int cursorColumn = 0;
int character = 0;
char name[17] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'};
char alphabet[37] = {' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 
                    'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
                    'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3',
                    '4', '5', '6', '7', '8', '9', '0'};
int gameId = 0;

//Setup function
void setup() {
  Serial.begin(115200);
  
  lcd.begin(16, 2);
  lcd.blink();
  lcd.print("Loading...");
  
  Ethernet.begin(mac);
  Serial.println(Ethernet.begin(mac));
  button1.setDebounceTime(50);
  button2.setDebounceTime(50);
  button3.setDebounceTime(50);
  button4.setDebounceTime(50);
  button5.setDebounceTime(50);
  startButton.setDebounceTime(50);

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

  randomSeed(analogRead(0));

  lcd.begin(16, 2);
  lcd.print("Press the green");
  lcd.setCursor(0, 1);
  lcd.print("button to start!");

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

//This function lets the user pick between a game duration of 15 or 30 seconds.
void startMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Select Duration:");
  lcd.setCursor(0,1);
  lcd.print("15  30  Seconds");
  lcd.setCursor(2, 1);
  lcd.blink();
  while (startButton.getStateRaw() != 0) {
    int joystick = readJoystick();

    switch (joystick) {
      case 2:
        lcd.noBlink();
        lcd.setCursor(0, 1);
        lcd.print("15  30< Seconds");
        lcd.setCursor(6, 1);
        lcd.blink();
        duration15 = false;
        duration30 = true;
        break;
      
      case 3:
        lcd.noBlink();
        lcd.setCursor(0, 1);
        lcd.print("15< 30  Seconds");
        lcd.setCursor(2, 1);
        lcd.blink();
        duration15 = true;
        duration30 = false;
        break;
      
      default:
        break;
      }
  
  }

}

//This function displyas the countdown at the start of a game
void countdownText() {
  lcd.setCursor(0, 1);
  lcd.print("READY!");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("SET!    ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("GO!!!");
  delay(1000);
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
      
      while(button1.getStateRaw() != 0) {
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
      
      while(button2.getStateRaw() != 0) {
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

  score += 1;
}

//This function handles actually playing the game.
void playGame() {
  int currentScore = 0;

  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(score);
  lcd.setCursor(0, 0);
  lcd.print("15 Seconds");
  lcd.noBlink();

  if (duration15 == true) {
    duration = 15;  
    lcd.setCursor(0, 0);
    lcd.print("15 Seconds");
      time_now = millis();
      while (millis() < time_now + seconds15) {
        gamePlay();
        if (score != currentScore) {
          currentScore = score;
          lcd.setCursor(7, 1);
          lcd.print(score);
        }
      }
    }
  if (duration30 == true) {
    duration = 30;
    lcd.setCursor(0, 0);
    lcd.print("30 Seconds");
      time_now = millis();
      while (millis() < time_now + seconds30) {
        gamePlay();
        if (score != currentScore) {
          currentScore = score;
          lcd.setCursor(7, 1);
          lcd.print(score);
        }
      }
    }
}

void endGame() {
  clearLeds();
  lcd.noBlink();
  lcd.setCursor(0, 0);
  lcd.print("Good Job!     ");
  lcd.setCursor(0, 1);
  lcd.print("Final Score: ");
  lcd.print(score);
  delay(5000);
  lcd.noDisplay();
  delay(500);
  lcd.display();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Name: ");
  lcd.blink();

  while(startButton.getStateRaw() != 0) {
    int joystick = readJoystick();

    lcd.setCursor(cursorColumn, 1);
    switch (joystick) {
      //up
      case 0:
        if (character < 36) {
          time_now = millis();
          character = character + 1;
          lcd.print(alphabet[character]);
          lcd.setCursor(cursorColumn, 1);
          name[cursorColumn] = alphabet[character];
          while (millis() < time_now + 500) {}
        }
        if (character == 36) {
          time_now = millis();
          character = 0;
          lcd.print(alphabet[character]);
          lcd.setCursor(cursorColumn, 1);
          name[cursorColumn] = alphabet[character];
          while (millis() < time_now + 500) {}


        }
        break;

      //down
      case 1:
        if (character > 0) {
          time_now = millis();
          character = character - 1;
          lcd.print(alphabet[character]);
          lcd.setCursor(cursorColumn, 1);
          name[cursorColumn] = alphabet[character];
          while (millis() < time_now + 500) {}
        }
        if (character == 0) {
          time_now = millis();
          character = 36;
          lcd.print(alphabet[character]);
          lcd.setCursor(cursorColumn, 1);
          name[cursorColumn] = alphabet[character];
          while (millis() < time_now + 500) {}
        }
        break;

      //right
      case 2:
        if (cursorColumn < 15) { 
          time_now = millis();
          cursorColumn = cursorColumn + 1;
          lcd.setCursor(cursorColumn, 1);
          while (millis() < time_now + 500) {}
        }

        break;
      
      //left
      case 3:
        if (cursorColumn > 0) {
          time_now = millis();
          cursorColumn = cursorColumn - 1;
          lcd.setCursor(cursorColumn, 1);
          while (millis() < time_now + 500) {}
        }

        break;

      default:
        break;
      }
  
  }
  cursorColumn = 0;
  character = 0;
  lcd.clear();
  lcd.noBlink();
  lcd.setCursor(0, 0);
  lcd.print("Good Game :)");
  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i, 1);
    lcd.print(name[i]);
  }

  delay(5000);
  finished = false;
  duration15 = false;
  duration30 = false;
}

void postScoresInFlask() {
  gameId += 1;  

  String dataToPost = String("gameId=") + String(gameId) + String("&duration=") + 
  String(duration) + String("&score=") + String(score) + String("&name=") + 
  String(name);

  Serial.println("Connecting...");

  if (client.connect(server, 5000)) {
    Serial.println("connected");
    Serial.println(dataToPost);
    client.println("POST " + String(pathNameAdd) + " HTTP/1.1");
    client.println("Host: " + String(serverChar));
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(dataToPost.length());
    client.println();
    client.println(dataToPost);
    
    
  } else {
    Serial.println("connection failed");
    Serial.println(client.connect(server, 5000));
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
  cancelButton.loop();


  if(startButton.getStateRaw() == 0) {
    startMenu();
    gameOn = true;
    score = 0;
  }

  if (gameOn == true) {
    if (duration15 == true) {
      lcd.clear();
      lcd.noBlink();
      lcd.setCursor(0, 0);
      lcd.print("15 SECONDS");
      countdownText();
      playGame();
      finished = true;
      gameOn = false;
    }

    if (duration30 == true) {
      lcd.clear();
      lcd.noBlink();
      lcd.setCursor(0, 0);
      lcd.print("30 SECONDS");
      countdownText();
      playGame();
      finished = true;
      gameOn = false;
    }
  }

  if (finished == true) {
    endGame();
    postScoresInFlask();
  }
}
  






