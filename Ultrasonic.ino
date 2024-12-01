#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define trigPin 10 // trigger pin
#define echoPin 9  // echo pin

long duration;
float distance; // ใช้ float เพื่อรองรับค่าทศนิยม
int numSamples = 10; // จำนวนการวัดที่ต้องการเฉลี่ย
float getAverageDistance(int numSamples) {
  float totalDistance = 0;
  
  for (int i = 0; i < numSamples; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH); // รับค่า duration
    float tempDistance = duration * 0.034 / 2;  // คำนวณระยะทางหน่วยเซนติเมตร (cm)
    totalDistance += tempDistance;
    delay(50); // หน่วงระหว่างการวัด
  }

  return totalDistance / numSamples; // คำนวณค่าเฉลี่ย
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Online....");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  delay(2000);
}

void loop() {
  // หาค่าเฉลี่ยจากการวัดหลายครั้ง
  distance = getAverageDistance(numSamples);  

  lcd.clear();

  // ตรวจสอบขอบเขตของระยะทาง
  if (distance > 400 || distance < 2) { // เกินช่วงการวัด 2-400 cm
    lcd.setCursor(0, 0);
    lcd.print("Out of Range");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Distance");
    lcd.setCursor(0, 1);
    lcd.print(distance, 3); // แสดงผลทศนิยม 1 ตำแหน่ง
    lcd.print(" CM");
  }
}
