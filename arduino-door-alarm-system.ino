#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {2, 3, 4, 5}; 
byte colPins[COLS] = {6, 7, 8};    

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int buzzerPin = 10;
const int doorSensorPin = 11;

String password = "1453";
String input = "";
bool alarmTriggered = false;
bool kapiAcik = false; // Kapının mevcut durumu
bool kapiOncekiDurum = true; // HIGH = kapalı

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  pinMode(doorSensorPin, INPUT_PULLUP); // Kapalıyken HIGH
}

void loop() {
  kapiAcik = digitalRead(doorSensorPin) == HIGH;

  // Kapı yeni açıldıysa (önce kapalıydı, şimdi açık)
  if (kapiAcik && kapiOncekiDurum == true) {
    Serial.println("Kapı açıldı! Alarm aktif!");
    alarmTriggered = true;
  }

  // Alarm durumu
  if (alarmTriggered) {
    tone(buzzerPin, 1000);
  } else {
    noTone(buzzerPin);
  }

  // Şifre girişi
  char key = keypad.getKey();
  if (key) {
    Serial.print("Tuş: ");
    Serial.println(key);
    if (key == '#') {
      if (input == password) {
        Serial.println("Doğru şifre! Alarm sustu.");
        alarmTriggered = false;
      } else {
        Serial.println("Yanlış şifre!");
      }
      input = "";
    } else {
      input += key;
    }
  }

  // Önceki kapı durumu güncellenir (bir sonraki loop için)
  kapiOncekiDurum = !kapiAcik; // true = kapalı, false = açık
  
}
