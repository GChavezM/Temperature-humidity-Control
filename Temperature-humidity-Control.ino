#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>

#define FAN 41
#define BOMB 39

#define TEMP 26
#define HUM 70

const int rs = 53, en = 51, d4 = 49, d5 = 47, d6 = 45, d7 = 43;
//const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define DHTPINA 33
#define DHTPINB 31
#define DHTPINC 29

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
  lcd.setCursor(0,0);
  lcd.print("Start");
  
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

  bool sensorA = checkValue(h1, t1);
  bool sensorB = checkValue(h2, t2);
  bool sensorC = checkValue(h3, t3);

  bool error = !sensorA || !sensorB || !sensorC;

  if(error){
    printErrorMessage(sensorA, sensorB, sensorC);
  } else{
    float temp = (t2 + t3) / 2;
    float hum = (h2 + h3) / 2;
    printValues(h1, hum, t1, temp);
    controlTemp(temp);
    controlHum(hum);
  }
  
  delay(2000);
}

void printValues(float h1, float hum, float t1, float temp){
  String room1 = "T:"+ String(t1, 1) + "C H:" + String(h1,1) + "%";
  String room2 = "T:"+ String(temp, 1) + "C H:" + String(hum, 1) + "%";
  Serial.println("R1 -> " + room1);
  Serial.println("R2 -> " + room2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(room1);
  lcd.setCursor(0,1);
  lcd.print(room2);
}

void printErrorMessage(bool sA, bool sB, bool sC){
  String message = "Error sensor:";
  String sensors = "";
  if(!sA){
    sensors += " A";
  }
  if(!sB){
    sensors += " B";
  }
  if(!sC){
    sensors += " C";
  }
  Serial.println(message + sensors);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(message);
  lcd.setCursor(3,1);
  lcd.print(sensors);
}

bool checkValue(float val1, float val2){
  return (isnan(val1) || isnan(val2)) ? false : true;
}

void controlTemp(float val){
  if(val > TEMP){
    digitalWrite(FAN, HIGH);
    Serial.println("Fan is On");
  } else{
    digitalWrite(FAN, LOW);
    Serial.println("Fan is Off");
  }
}
void controlHum(float val){
  if(val < HUM){
    digitalWrite(BOMB, HIGH);
    Serial.println("Bomb is On");
  } else{
    digitalWrite(BOMB, LOW);
    Serial.println("Bomb is Off");
  }
}
