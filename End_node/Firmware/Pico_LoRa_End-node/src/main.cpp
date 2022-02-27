#include <Arduino.h>

UART Serial2(8,9);

const int led_builtin = 25;
const int led_ext = 28;
const int sensor_1 = 2;
const int sensor_2 = 3;

const long delay_s = 0;

unsigned long previousMillis = 0;
unsigned long previousDisplay = 0;

byte baca_A;
byte baca_B;
int a;
int b;
int c;
int d;

unsigned long masuk = 0;
unsigned long keluar = 0;
unsigned long total = 0;

bool masukState = false;
bool keluarState = false;
String jointer;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(9600);
  //Serial2.begin(9600);

  pinMode(led_builtin, OUTPUT);
  pinMode(sensor_1, INPUT_PULLUP);
  pinMode(sensor_2, INPUT_PULLUP);
  pinMode(led_ext, OUTPUT);

}

void bacaMasuk() {

  if (baca_A == 0 && baca_B == 1 && a == 0 && b == 0 && c == 0) {
    a = 1;
  }
  if (baca_A == 1 && baca_B == 0 && a == 1 && b == 0) {
    a = 2;
  }

  if (baca_A == 1 && baca_B == 1 && a == 2 && b == 0) {
    c = 1;
  }
  if (a == 2 && c == 1) {

    //masuk++;
    // EEPROM.write(rom, masuk);
    // EEPROM.commit();
    //masukState = true;

    masuk++;
    total++;

    //total++;
    // EEPROM.write(rom3, total);
    // EEPROM.commit(); 
    
    a = 0;
    b = 0;
  }
  //else {

  //masukState = false;
  //}
  //return masukState;
}

void bacaKeluar() {

  if (baca_A == 1 && baca_B == 0 && a == 0 && b == 0 && c == 0) {
    b = 1;
  }
  if (baca_A == 0 && baca_B == 1 && a == 0 && b == 1) {
    b = 2;
  }
  if (baca_A == 1 && baca_B == 1 && a == 0 && b == 2) {
    d = 1;
  }
  if (b == 2 && d == 1) {

    //keluar++;
    // EEPROM.write(rom2, keluar);
    // EEPROM.commit();

    //keluarState = true;

    keluar++;
    total--;

    //total--;
    // EEPROM.write(rom3, total);
    // EEPROM.commit();
    
    a = 0;
    b = 0;
  }
  //else {
   // keluarState = false;
  //}
  //return keluarState;
}

void readSensor() {

  bacaMasuk();
  c = 0;
  bacaKeluar();
  d = 0;

  // if (masukState == true) { 
  //   masuk++; 
  //   }
  // if (keluarState == true) { 
  //   keluar++; 
  //   }

  // total = masuk - keluar;

  if ( total < 0) {
    total = 0;
    // EEPROM.write(rom3, total);
    // EEPROM.commit();
  }

  if ( total > 30000000) {

    total = 0;
  }
}

void loop() {

  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

  //if ((currentMillis - previousMillis) >= delay_s) {

    baca_A = digitalRead(sensor_1);
    baca_B = digitalRead(sensor_2);

    if (baca_A == 0 || baca_B == 0) {
      digitalWrite(led_ext, HIGH);
      digitalWrite(led_builtin, HIGH);
    }
    else {
      digitalWrite(led_ext, LOW);
      digitalWrite(led_builtin, LOW);
    }

    readSensor();
    jointer = "#" + String(masuk) + "#" + String(keluar) + "#" + String(total) + "#$";
    Serial.println(jointer);

    if ((currentMillis - previousDisplay) >= 1000) {

      // Serial.print("Masuk: ");
      // Serial.print(masuk);
      // //Serial1.write(masuk); //sending to gateway

      // Serial.print(" Keluar: ");
      // Serial.print(keluar);
      // //Serial1.write(keluar);

      // Serial.print(" Total: ");
      // Serial.println(total);
      previousDisplay = currentMillis;
      Serial1.println(jointer);
    }
  //}

}