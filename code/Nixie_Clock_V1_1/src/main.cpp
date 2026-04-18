#include <Arduino.h>
#include "WurthLED.h" // Importujemy naszą własną "bibliotekę"
#include "Nixie.h"

#define T1_A 4
#define T1_B 5
#define T1_C 6
#define T1_D 7

// Lampa 2 (U2)
#define T2_A 8
#define T2_B 9
#define T2_C 10
#define T2_D 11

// Lampa 3 (U3)
#define T3_A 12
#define T3_B 13
#define T3_C 14
#define T3_D 15

// Lampa 4 (U4)
#define T4_A 21
#define T4_B 18
#define T4_C 17
#define T4_D 16

// Inicjalizacja pojedynczych lamp
NixieTube tube1(T1_A, T1_B, T1_C, T1_D);
NixieTube tube2(T2_A, T2_B, T2_C, T2_D);
NixieTube tube3(T3_A, T3_B, T3_C, T3_D);
NixieTube tube4(T4_A, T4_B, T4_C, T4_D);

// Inicjalizacja kontrolera wyświetlacza
NixieDisplay display(tube1, tube2, tube3, tube4);

const int ONLINE_PIN = 36;
const int ERROR_PIN = 37;
const int PGOOD_PIN = 2; 
const int NIXIE_HV_EN_PIN = 35; 

// Zmienne do przełączania efektów
int currentEffect = 1; 
uint32_t lastStatusPrint = 0;
uint32_t lastNixieUpdate = 0;
uint32_t lastBlinkUpdate = 0; // Timer dla migania LED ONLINE
uint16_t nixieCounter = 0;    // Licznik do wyświetlania
bool onlineLedState = false;  // Stan diody migającej
bool hvState = false;

bool isPowerGood() {
    // Odczytujemy stan pinu IO2 
    // Zazwyczaj HIGH oznacza, że zasilanie jest OK
    return digitalRead(PGOOD_PIN) == HIGH;
}

void setNixiePower(bool active) {
    if (active) {
        digitalWrite(NIXIE_HV_EN_PIN, HIGH);
    } else {
        digitalWrite(NIXIE_HV_EN_PIN, LOW);
    }
}

void setup() {
    Serial.begin(115200);
    delay(2000); 
    Serial.println("\n--- BOOT: ESP32-S3 Start ---");

    pinMode(NIXIE_HV_EN_PIN, OUTPUT);
    digitalWrite(NIXIE_HV_EN_PIN, LOW); // 170V off

    pinMode(LED_PIN, OUTPUT); // LED_PIN jest brane z WurthLED.h
    digitalWrite(LED_PIN, LOW);
    
    pinMode(ONLINE_PIN, OUTPUT);
    pinMode(ERROR_PIN, OUTPUT);

    pinMode(PGOOD_PIN, INPUT);

    Serial.println("System: Test diod statusowych...");
    digitalWrite(ONLINE_PIN, HIGH);
    digitalWrite(ERROR_PIN, HIGH);
    delay(500);
    digitalWrite(ERROR_PIN, LOW);

    display.begin();
    
    Serial.println("System: Setup zakonczony. Wchodze w loop().");
   
}

void loop() {
    uint32_t currentMillis = millis();

    // 1. EFEKT LED (Kolorki działają cały czas niezależnie na pełnej szybkości pętli)
    runEffect(1); 
    //turnOffWurthLed();
    // 2. MIGANIE DIODY ONLINE CO 2 SEKUNDY (2000 ms)
    if (currentMillis - lastBlinkUpdate >= 2000) {
        lastBlinkUpdate = currentMillis;
        onlineLedState = !onlineLedState; // Odwrócenie stanu (HIGH -> LOW -> HIGH)
        digitalWrite(ONLINE_PIN, onlineLedState ? HIGH : LOW);
    }

    // 3. MONITOROWANIE PGOOD I PRINTOWANIE (Co 1 sekundę)
    if (currentMillis - lastStatusPrint >= 1000) {
        lastStatusPrint = currentMillis;
        
        // bool pgoodStatus = isPowerGood(); 
        bool pgoodStatus = true; // Zmienna tymczasowa dla testów
        
        Serial.print("Status Systemu -> PGOOD: ");
        if (pgoodStatus) {
            Serial.println("[ OK ] ");
            digitalWrite(ERROR_PIN, LOW);
        } else {
            Serial.println("[ BRAK SYGNALU - SPRAWDZ ZASILANIE 170V! ] ");
            digitalWrite(ERROR_PIN, HIGH);
        }
    }
   //display.blankAll();

   //display.setNumber(1);

    // 4. OBSŁUGA LAMP NIXIE - ciągłe iterowanie licznika (np. co 100 ms)
    if (currentMillis - lastNixieUpdate >= 1000) {
        lastNixieUpdate = currentMillis;

        display.setNumber(nixieCounter);
        
        nixieCounter++;
        if (nixieCounter > 9) {
            nixieCounter = 0; // Zerowanie po przekroczeniu 4 cyfr
        }
    }
}

    // Zmiana efektu na następny co 5 sekund
    // if (currentMillis - lastEffectChange > 5000) {
    //     lastEffectChange = currentMillis;
    //     currentEffect++;
    //     if (currentEffect > 3) {
    //         currentEffect = 1; 
    //     }
        
    //     Serial.print("Zmieniam efekt na: ");
    //     Serial.println(currentEffect);
    // }

    // // Wywołanie efektu z naszej biblioteki
    // runEffect(currentEffect);
