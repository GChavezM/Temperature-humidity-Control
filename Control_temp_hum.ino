#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>

#define FAN 5
#define BOMB 6

const int rs = 53, en = 51, d4 = 49, d5 = 47, d6 = 45, d7 = 43;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define DHTPINA 2
#define DHTPINB 3
#define DHTPINC 4

#define DHTTYPE DHT11

DHT dhtA(DHTPINA, DHTTYPE);
DHT dhtB(DHTPINB, DHTTYPE);
DHT dhtC(DHTPINC, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Start");

  pinMode(FAN, OUTPUT);
  pinMode(BOMB, OUTPUT);

  digitalWrite(FAN, LOW);
  digitalWrite(BOMB, LOW);

  lcd.begin(16,2);
  
  dhtA.begin();
  dhtB.begin();
  dhtC.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  float h1 = dhtA.readHumidity();
  float h2 = dhtB.readHumidity();
  float h3 = dhtC.readHumidity();

  float t1 = dhtA.readTemperature();
  float t2 = dhtB.readTemperature();
  float t3 = dhtC.readTemperature();

  checkValue(h1, t1, "A");
  checkValue(h2, t2, "B");
  checkValue(h3, t3, "C");

  delay(2000);
}

void checkValue(float val1, float val2, String sensor){
  if(isnan(val1) || isnan(val2)) {
    Serial.println("Sensor " + sensor + " is not working");
    lcd.clear()
    lcd.cursor(0, 1)
    lcd.print("Error sensor " + sensor)
  }
}
