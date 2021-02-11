#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <Wire.h>

#define ADDRESS 0b1010000

const int rs = 13, en = 12, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'7','8','9','/'},
  {'4','5','6','*'},
  {'1','2','3','-'},
  {'C','0','=','+'}
};
byte rowPins[ROWS] = {22, 23, 24, 25};
byte colPins[COLS] = {26, 27, 28, 29};

Keypad  customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

int currentMode = 1;
int defaultMode = 1;
int pWT = 60;
int wT = 60;
int wWT = 60;
int dT = 60;
char key;

void setup(){
  Serial.begin(9600);
  Serial.println("def mode before read from mem");
  Serial.println(defaultMode);
  lcd.begin(16,2);
  lcd.clear();
  Wire.begin();
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  currentMode = memoryRead();
  Serial.println("curr mode after read from mem");
  Serial.println(currentMode);
  if(currentMode > 4){
    currentMode = 1;
  }
  defaultMode = currentMode;
  Serial.println("def mode after assigned");
  Serial.println(defaultMode);
}

void loop(){
  int timer = 0;
  int defaultTime = 0;
  int LEDpin = 0;
  if(currentMode == 1){
    defaultTime = pWT;
    LEDpin = 7;
  }else if(currentMode == 2){
    defaultTime = wT;
    LEDpin = 6;
  }else if(currentMode == 3){
    defaultTime = wWT;
    LEDpin = 5;
  }else if(currentMode == 4){
    defaultTime = dT;
    LEDpin = 4;
  }
  digitalWrite(LEDpin, HIGH);
  while(timer < defaultTime){   
    key = customKeypad.getKey();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Default Mode:");
    lcd.print(currentMode);
    lcd.setCursor(0,1);
    lcd.print("Remaining:");
    lcd.print(defaultTime - timer);
    if(key == '='){    
      pause();
    }else if (key == 'C'){
      changeDefaultMode();
    }else if(key == '+'){   
      changeTimer();
    }
    delay(100);
    timer++;
  }
  digitalWrite(LEDpin, LOW);
  currentMode++;
  if(currentMode > 4){    
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(4, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("End of Wash!");
    pause();
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);
    currentMode = memoryRead();
    if(currentMode > 4){
      currentMode = 1;
    }
    defaultMode = currentMode;
  }
}
//---------------------------------------------------------------------------------------------------------------
void memoryWrite(int data){
  Wire.beginTransmission(ADDRESS);
  Wire.write(0x00);
  Wire.write(0xAA);
  Wire.write(data);
  Wire.endTransmission();
}

int memoryRead(){
  Wire.beginTransmission(ADDRESS);
  Wire.write(0x00);
  Wire.write(0xAA);
  Wire.endTransmission();
  Wire.requestFrom(ADDRESS, 1);
  int temp = Wire.read();
  return temp;
}

void pause(){
  key = customKeypad.waitForKey();
  while(key != '=')
    key = customKeypad.waitForKey();
}

void changeDefaultMode(){
  Serial.println("im in change mode");
  key = customKeypad.waitForKey();
  Serial.println(key);
  if(key == '1'){
    memoryWrite(1);
    defaultMode = 1;
  }else if(key == '2'){
    memoryWrite(2);
    defaultMode = 2;
  }else if(key == '3'){
    memoryWrite(3);
    defaultMode = 3;
  }else if(key == '4'){
    memoryWrite(4);
    defaultMode = 4;
  }
}

void changeTimer(){
  Serial.println("im in change timer ");
  key = customKeypad.waitForKey();
  Serial.println(key);
  if(key == '1'){
    key = customKeypad.waitForKey();
    if(key  == '*'){
      pWT *= 2;
      Serial.println("pWT");
      Serial.println(pWT);
    }else if(key == '/'){
      pWT /= 2;
      Serial.println("pWT");
      Serial.println(pWT);
    }
  }else if(key == '2'){
    key = customKeypad.waitForKey();
    if(key  == '*'){
      wT *= 2;
      Serial.println("wT");
      Serial.println(wT);
    }else if(key == '/'){
      wT /= 2;
      Serial.println("wT");
      Serial.println(wT);
    }
  }else if(key == '3'){
    key = customKeypad.waitForKey();
    if(key  == '*'){
      wWT *= 2;
      Serial.println("wWT");
      Serial.println(wWT);
    }else if(key == '/'){
      wWT /= 2;
      Serial.println("wWT");
      Serial.println(wWT);
    }
  }else if(key == '4'){
    key = customKeypad.waitForKey();
    if(key  == '*'){
      dT *= 2;
      Serial.println("dT");
      Serial.println(dT);
    }else if(key == '/'){
      dT /= 2;
      Serial.println("dT");
      Serial.println(dT);
    }
  }
}
