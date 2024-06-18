/*#include <LiquidCrystal_I2C.h>

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

#define btn1 32
#define btn2 35
#define btn3 34
#define btn4 15
#define btn5 2
#define btn6 4



int pwmValue = 1023;
int accionamientoServo = 25; // Duración del giro en milisegundos

void setup() {
  pinMode(servoPill1, OUTPUT);
  pinMode(servoPill2, OUTPUT);
  pinMode(servoPill3, OUTPUT);
  pinMode(servoPill4, OUTPUT);
  pinMode(servoPill5, OUTPUT);
  pinMode(servoPill6, OUTPUT);

  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(btn3, INPUT);
  pinMode(btn4, INPUT);
  pinMode(btn5, INPUT);
  pinMode(btn6, INPUT); 

  lcd.init();
  lcd.backlight();

  Serial.begin(115200);
}

void loop() {
  
  lcd.setCursor(3, 0);
  lcd.print("BIENVENIDO");
  lcd.setCursor(5, 1);
  lcd.print("PILLC");
  int tiempoInicioGiro = 0; // Tiempo de inicio del giro

  if (digitalRead(btn1)) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("DISPENSANDO");
    lcd.setCursor(2, 1);
    lcd.print("MEDICAMENTO 1");
    delay(5000);

    for (int i =0; i < 4 ; i++){
    digitalWrite (servoPill1, HIGH); 
    delayMicroseconds(2300);             
    digitalWrite (servoPill1,LOW);
    delay(18);
    }
    lcd.clear();
  } 

  else if (digitalRead(btn2)){
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("DISPENSANDO");
    lcd.setCursor(2, 1);
    lcd.print("MEDICAMENTO 2");
    delay(5000);

    for (int i =0; i < 4 ; i++){
    digitalWrite (servoPill2, HIGH); 
    delayMicroseconds(2300);             
    digitalWrite (servoPill2,LOW);
    delay(18);
    }
    lcd.clear();
  }
  else if (digitalRead(btn3)){
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("DISPENSANDO");
    lcd.setCursor(2, 1);
    lcd.print("MEDICAMENTO 3");
    delay(5000);

    for (int i =0; i < 4 ; i++){
    digitalWrite (servoPill3, HIGH); 
    delayMicroseconds(2300);             
    digitalWrite (servoPill3,LOW);
    delay(18);
    }
    lcd.clear();
  }
  else if (digitalRead(btn4)){
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
  }
  else if (digitalRead(btn5)){
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
  }
  else if (digitalRead(btn6)){
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
  }
}
*/

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
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("DISPENSANDO");
        lcd.setCursor(2, 1);
        lcd.print("MEDICAMENTO 1");
        delay(5000);

        for (int i =0; i < 4 ; i++){
          digitalWrite (servoPill1, HIGH); 
          delayMicroseconds(1885);             
          digitalWrite (servoPill1,LOW);
          delay(20);
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
          delayMicroseconds(1565);             
          digitalWrite (servoPill2,LOW);
          delay(18);
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
          delayMicroseconds(1809);             
          digitalWrite (servoPill3,LOW);
          delay(20);
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
          delayMicroseconds(1562);             
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
          delayMicroseconds(1775);             
          digitalWrite (servoPill5,LOW);
          delay(21);
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

        for (int i =0; i < 4 ; i++){
          digitalWrite (servoPill6, HIGH); 
          delayMicroseconds(1735);             
          digitalWrite (servoPill6,LOW);
          delay(22);
        }
        lcd.clear();
        break;

      default:
        Serial.println("Comando no reconocido");
        break;
    } 
  }
}
