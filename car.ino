#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Motor control pins
#define motorPin1 2 // IN1
#define motorPin2 3 // IN2
#define motorPin3 4 // IN3
#define motorPin4 5 // IN4

char currentCommand = 'S'; // Default to 'Stop'

void forward() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}

void backward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void stop() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

void turnLeft() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void turnRight() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}

void carController(char command) {
  if (command != currentCommand) { // Update only if the command changes
    currentCommand = command;
    lcd.clear();
    switch (command) {
      case 'F':
        lcd.setCursor(0, 0);
        lcd.print("Move forward");
        forward();
        break;
      case 'B':
        lcd.setCursor(0, 0);
        lcd.print("Move backward");
        backward();
        break;
      case 'L':
        lcd.setCursor(0, 0);
        lcd.print("Move left");
        turnLeft();
        break;
      case 'R':
        lcd.setCursor(0, 0);
        lcd.print("Move right");
        turnRight();
        break;
      default:
        lcd.setCursor(0, 0);
        lcd.print("Stop");
        stop();
        break;
    }
  }
}

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Set motor pins as output
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  // Start serial communication
  Serial.begin(9600);
  lcd.print("Bluetooth ready!");
}

void loop() {
  if (Serial.available()) {
    char receivedChar = Serial.read();
    carController(receivedChar);
  } else {
    carController(currentCommand); // Repeat the current command
  }
}
