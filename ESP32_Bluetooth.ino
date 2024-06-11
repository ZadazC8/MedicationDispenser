#include <BluetoothSerial.h>
#include <LiquidCrystal_I2C.h>

BluetoothSerial SerialBT;

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte degreeSymbol[8] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

#define servoPill1 12
#define servoPill2 14
#define servoPill3 27
#define servoPill4 26
#define servoPill5 25
#define servoPill6 13

void setup() {
  pinMode(servoPill1, OUTPUT);
  pinMode(servoPill2, OUTPUT);
  pinMode(servoPill3, OUTPUT);
  pinMode(servoPill4, OUTPUT);
  pinMode(servoPill5, OUTPUT);
  pinMode(servoPill6, OUTPUT);

  lcd.init();
  lcd.backlight();

  Serial.begin(115200);
  SerialBT.begin("ESP32_PillDispenser"); // Nombre del dispositivo Bluetooth
  Serial.println("El dispositivo está listo para emparejarse.");
}

void loop() {
  lcd.setCursor(3, 0);
  lcd.print("BIENVENIDO");
  lcd.setCursor(5, 1);  
  lcd.print("PILLC");

  if (SerialBT.available()) {
    Serial.println("App Conectada");
    int data = SerialBT.read();
    Serial.println(data);
    switch (data) {
      case 1:
        Serial.println("Recibido");
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("DISPENSANDO");
        lcd.setCursor(2, 1);
        lcd.print("MEDICAMENTO 1");
        delay(5000);

        for (int i =0; i < 4 ; i++){
          digitalWrite (servoPill1, HIGH); 
          delayMicroseconds(1946);             
          digitalWrite (servoPill1,LOW);
          delay(13);
        }
        lcd.clear();
        break;
      
      case 2:
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("DISPENSANDO");
        lcd.setCursor(2, 1);
        lcd.print("MEDICAMENTO 2");
        delay(5000);

        for (int i =0; i < 4 ; i++){
          digitalWrite (servoPill2, HIGH); 
          delayMicroseconds(1600);             
          digitalWrite (servoPill2,LOW);
          delay(6);
        }
        lcd.clear();
        break;

      case 3:
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("DISPENSANDO");
        lcd.setCursor(2, 1);
        lcd.print("MEDICAMENTO 3");
        delay(5000);

        for (int i =0; i < 4 ; i++){
          digitalWrite (servoPill3, HIGH); 
          delayMicroseconds(1610);             
          digitalWrite (servoPill3,LOW);
          delay(18);
        }
        lcd.clear();
        break;

      case 4:
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("DISPENSANDO");
        lcd.setCursor(2, 1);
        lcd.print("MEDICAMENTO 4");
        delay(5000);

        for (int i =0; i < 4 ; i++){
          digitalWrite (servoPill4, HIGH); 
          delayMicroseconds(1610);             
          digitalWrite (servoPill4,LOW);
          delay(18);
        }
        lcd.clear();
        break;
      
      case 5:
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("DISPENSANDO");
        lcd.setCursor(2, 1);
        lcd.print("MEDICAMENTO 5");
        delay(5000);

        for (int i =0; i < 4 ; i++){
          digitalWrite (servoPill5, HIGH); 
          delayMicroseconds(1610);             
          digitalWrite (servoPill5,LOW);
          delay(18);
        }
        lcd.clear();
        break;
      
      case 6:
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("DISPENSANDO");
        lcd.setCursor(2, 1);
        lcd.print("MEDICAMENTO 6");
        delay(5000);

        for (int i =0; i < 5 ; i++){
          digitalWrite (servoPill6, HIGH); 
          delayMicroseconds(2300);             
          digitalWrite (servoPill6,LOW);
          delay(18);
        }
        lcd.clear();
        break;

      default:
        Serial.println("Comando no reconocido");
        break;
    } 
  }
}
