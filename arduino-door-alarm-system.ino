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
bool kapiOncekiDurum = false;

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  pinMode(doorSensorPin, INPUT_PULLUP); // Kapı kapalıyken LOW
}

void loop() {
  bool kapiAcik = digitalRead(doorSensorPin) == HIGH;

  // Kapı yeni açıldıysa alarmı tetikle
  if (kapiAcik && !kapiOncekiDurum) {
    Serial.println("Kapı açıldı! Alarm aktif!");
    alarmTriggered = true;
  }

  kapiOncekiDurum = kapiAcik;

  // Alarm durumu
  if (alarmTriggered) {
    tone(buzzerPin, 1000);
  } else {
    noTone(buzzerPin);
  }

  // Keypad okuma
  char key = keypad.getKey();
  if (key) {
    Serial.print("Tuş: ");
    Serial.println(key);

    if (key == '#') {  // Şifre kontrol
      if (input == password) {
        Serial.println("Şifre doğru. Alarm kapatıldı.");
        alarmTriggered = false;
      } else {
        Serial.println("Yanlış şifre!");
      }
      input = "";
    } 
    else if (key == '*') {
      input = ""; // Giriş temizle
    } 
    else {
      input += key;
    }
  }
}
