#include <LiquidCrystal_I2C.h>

#define A_SOIL_HUMI A0
#define O_PUMP_A 6
#define O_PUMP_B 5
#define O_RGB_R 10
#define O_RGB_G 9
#define O_RGB_B 11

#define On_Time 500

#define LCD_I2C_ADDR 0x27

LiquidCrystal_I2C lcd(LCD_I2C_ADDR, 16, 2);

int soilHumidity;

// 출력 핀 초기화 
void initPin() {
  pinMode(O_PUMP_A, OUTPUT);
  pinMode(O_PUMP_B, OUTPUT);
  pinMode(O_RGB_R, OUTPUT);
  pinMode(O_RGB_G, OUTPUT);
  pinMode(O_RGB_B, OUTPUT);

  digitalWrite(O_RGB_R, LOW);
  digitalWrite(O_RGB_G, LOW);
  digitalWrite(O_RGB_B, LOW);
  analogWrite(O_PUMP_A, 0);
  analogWrite(O_PUMP_B, 0);
}

// LCD INTRO 출력 
void introLcd() {
  lcd.print("SmartFarm 1.0");
  lcd.setCursor(0, 1);
  lcd.print("CreativeMakers");
}

// 토양습도 LCD 출력
void printLcd() {
  //lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(0, 0);
  if(soilHumidity < 20) lcd.print("Water Pump Start");
  else if(soilHumidity < 40) lcd.print("Soil is Dry");
  else if(soilHumidity < 70) lcd.print("Soil is Normal");
  else if(soilHumidity < 90) lcd.print("Soil is Wet");
  else lcd.print("In the Water");
  
  lcd.setCursor(0, 1);
  lcd.print("Moisture : ");
  lcd.print(soilHumidity);
  lcd.print("%");
}

// LCD 초기화 
void initLcd() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  introLcd();
}

// 토양습도 계산 
void calcSoilHumidity() {
  soilHumidity = map(analogRead(A_SOIL_HUMI), 1000, 400, 0, 100);
  if(soilHumidity > 100) soilHumidity = 100;
  else if(soilHumidity < 0) soilHumidity = 0;
}

void writeRGB(bool R, bool G, bool B) {
  digitalWrite(O_RGB_R, R);
  digitalWrite(O_RGB_G, G);
  digitalWrite(O_RGB_B, B);
}

void setup() {
  initPin();
  initLcd();
  //RGB LED를 보라색(빨강+파랑)으로 출력합니다.
  delay(2000);
  writeRGB(HIGH, LOW, HIGH);
}

void loop() {
  calcSoilHumidity();
  printLcd();
  /* 습도가 낮아지면, 펌프가 동작되고
     습도가 높아지면, 펌프가 꺼집니다. */
  
  if (soilHumidity < 20) {
    writeRGB(HIGH, LOW, LOW);
    delay(2000);
    lcd.clear();
    lcd.noBacklight();
    for (int i = 0; i < 230; i++) 
    {
      analogWrite(O_PUMP_A, i);
      delay(5);
    }
    delay(On_Time);
    analogWrite(O_PUMP_A, 0);
    analogWrite(O_PUMP_B, 0);
    delay(100);
  } else {
    writeRGB(HIGH, LOW, HIGH);
    if (soilHumidity > 70) writeRGB(LOW, HIGH, LOW);
    analogWrite(O_PUMP_A, 0);
    analogWrite(O_PUMP_B, 0);
    delay(1000);
  }
}
