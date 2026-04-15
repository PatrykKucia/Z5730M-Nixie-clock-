#include <Arduino.h> // Wymagane w PlatformIO, w Arduino IDE można pominąć

// Definicja pinów dla diod
const int led1Pin = 36;
const int led2Pin = 37;

void setup() {
  // Skonfigurowanie pinów jako wyjścia (OUTPUT)
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
}

void loop() {
  // Zaświecenie obu diod (stan wysoki)
  digitalWrite(led1Pin, HIGH);
  digitalWrite(led2Pin, HIGH);
  
  delay(500); // Czekaj 500 milisekund (pół sekundy)

  // Zgaszenie obu diod (stan niski)
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
  
  delay(500); // Czekaj kolejne 500 milisekund
}