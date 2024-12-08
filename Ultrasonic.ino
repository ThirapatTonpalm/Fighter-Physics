#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// LCD และ Ultrasonic
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define trigPin 10 // Trigger pin
#define echoPin 9  // Echo pin

// ตัวแปรหลัก
int simpleValue = 35;
int timer = 0;
int i = 0;
bool hasCounted = false; // ตัวแปรสำหรับตรวจสอบว่าเริ่มนับเวลาหรือยัง

long duration;
float distance; // ใช้ float เพื่อรองรับค่าทศนิยม
int numSamples = 10; // จำนวนการวัดที่ต้องการเฉลี่ย

// ค่าคงที่สปริง
const float springConstant = 3.0; // N/m (ค่าคงที่ของสปริง)

// ฟังก์ชันหาค่าเฉลี่ยของระยะทาง
float getAverageDistance(int numSamples) {
  float totalDistance = 0;

  for (int i = 0; i < numSamples; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH); // รับค่า duration
    float tempDistance = duration * 0.034 / 2; // คำนวณระยะทาง (cm)
    totalDistance += tempDistance;
    delay(50); // หน่วงระหว่างการวัด
  }

  return totalDistance / numSamples; // คำนวณค่าเฉลี่ย
}

// ฟังก์ชันคำนวณน้ำหนัก
float calculateWeight(float springConstant, float time) {
  const float g = 9.81; // ความเร่งเนื่องจากแรงโน้มถ่วง (m/s^2)
  float mass = (time * time * springConstant) / (4 * 3.1416 * 3.1416); // มวล (kg)
  return mass * g; // น้ำหนัก (N)
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
  lcd.setCursor(0, 0);

  if (distance < 35 || distance == 36 || distance == 37) {
    distance = 35;
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.setCursor(0, 1);
    lcd.print("Timer: Stopped");
    hasCounted = false; // รีเซ็ตสถานะหากกลับมาในช่วงนี้
  } 
  else if (distance > 40) {
    if (!hasCounted) {
      lcd.print("Distance: >40");
      lcd.setCursor(0, 1);
      lcd.print("Timer Counting");
      while (distance > 40) {
        i++; // จับเวลาในหน่วยวินาที
        delay(1000);
        distance = getAverageDistance(numSamples); // อัปเดตระยะทางในลูป
        Serial.println(distance);
      }
      hasCounted = true; // ตั้งสถานะว่านับเวลาแล้ว

      // คำนวณน้ำหนักเมื่อสิ้นสุดการจับเวลา
      float weight = calculateWeight(springConstant, i);
      Serial.print("Weight: ");
      Serial.print(weight);
      Serial.println(" N");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Weight: ");
      lcd.print(weight);
      lcd.print(" N");
    }
  } else {
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.setCursor(0, 1);
    lcd.print("Stable");
  }

  delay(500); // หน่วงเวลาเพื่อป้องกันการกระพริบบน LCD
}
