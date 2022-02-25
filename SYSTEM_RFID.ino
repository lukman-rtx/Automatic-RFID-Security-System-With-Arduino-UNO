#include <SPI.h> //library serial parallel interface
#include <Wire.h> //library untuk wire i2c
#include <RFID.h> //library RFID
#include <LiquidCrystal_PCF8574.h> //library penampil LCD
#include <Servo.h>
#define sda 10 //Pin Serialdata (SDA)
#define rst 9 //pin Reset
#define SALAH 7
#define BENAR 6
#define Buzzer 5
Servo servo;
LiquidCrystal_PCF8574 lcd(0x27); //0x27 dapat dicari dengan i2c scanner
RFID rfid(sda,rst);

String KodeUnik;
String Data_RFID2 = "2171252214361";
String Data_RFID1 = "20116189194166";
bool kondisi = false;

void setup() {
  Serial.begin(9600);
  servo.attach(3);
  SPI.begin();                        //Mulai komunikasi SPI
  rfid.init();                        //Mulai RFID
  pinMode(SALAH, OUTPUT);
  pinMode(BENAR, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  lcd.clear();
  lcd.begin(16,2); //ukuran LCD 16 x 2
  lcd.setBacklight(255); //menghidupkan lampu latar LCD
  lcd.setCursor (0,0);
  lcd.print("Card.Lock_System");
  lcd.setCursor (0,1);
  lcd.print("Lukman Nul Hakim");
}

void akses() {
  if(kondisi == false){
     noTone(Buzzer);
     lcd.clear();
     digitalWrite(BENAR, HIGH);
     digitalWrite(SALAH, LOW);
     Serial.println("LED ON");
     lcd.setCursor (0,0);
     lcd.print("Pintu  Terbuka !");
     lcd.setCursor (0,1);
     lcd.print("Selamat Datang !");
     tone(Buzzer, 500);
     delay(100);
     noTone(Buzzer);
     delay(100);
     tone(Buzzer, 1000);
     delay(100);
     noTone(Buzzer);
     servo.write(0);
     kondisi = true;
     }
  else {
     lcd.clear();
     lcd.setCursor (0,0);
     lcd.print("Pintu Terkunci !");
     lcd.setCursor (0,1);
     lcd.print("Kamar-mu Amaan !");
     digitalWrite(BENAR, LOW);
     tone(Buzzer, 1000);
     delay(100);
     noTone(Buzzer);
     delay(100);
     tone(Buzzer, 500);
     delay(100);
     noTone(Buzzer);
     kondisi = false;
     servo.write(90);
     } 
}

void akses_ditolak() {
  kondisi == false;
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print("?! SIAPA KAMU !?");
  lcd.setCursor (0,1);
  lcd.print("ALARM DIAKTIFKAN");
  digitalWrite(BENAR, LOW);
  digitalWrite(SALAH, HIGH);
    for(int i=0; i <= 5 ; i++) {
      tone(Buzzer, 1000);
      digitalWrite(SALAH, LOW);
      delay(500);
      noTone(Buzzer);
      digitalWrite(SALAH, HIGH);
      delay(500);
      servo.write(90);
      }
  rfid.halt();
  delay(2000);  
  }
      
void loop() {
  if(rfid.isCard() && rfid.readCardSerial()) {
      KodeUnik = String(rfid.serNum[0]) + String(rfid.serNum[1]) + String(rfid.serNum[2]) + String(rfid.serNum[3]) + String(rfid.serNum[4]);
      Serial.println(KodeUnik);
      delay(1000);
      if(KodeUnik == Data_RFID1 || KodeUnik == Data_RFID2){
          rfid.halt();
          akses();
          }
      else if(KodeUnik != Data_RFID1){
          rfid.halt();
          akses_ditolak(); 
          }
      else if(KodeUnik != Data_RFID2){
          rfid.halt();
          akses_ditolak(); 
          }
      }
  }
