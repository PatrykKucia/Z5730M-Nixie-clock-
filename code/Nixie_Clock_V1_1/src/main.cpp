#include <Arduino.h>
#include "WurthLED.h" // Importujemy naszą własną "bibliotekę"

const int ONLINE_PIN = 36;
const int ERROR_PIN = 37;
const int PGOOD_PIN = 2; 
const int NIXIE_HV_EN_PIN = 35; 

// Zmienne do przełączania efektów
int currentEffect = 1; 
uint32_t lastEffectChange = 0;
uint32_t lastPowerToggle = 0;
uint32_t lastStatusPrint = 0;
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
    
    Serial.println("System: Setup zakonczony. Wchodze w loop().");
   
}

void loop() {
    uint32_t currentMillis = millis();

    // 1. EFEKT LED (Tęcza działa cały czas niezależnie)
    runEffect(1);

    // 2. LOGIKA PRZEŁĄCZANIA 170V (Co 5 sekund)
    if (currentMillis - lastPowerToggle > 5000) {
        lastPowerToggle = currentMillis;
        hvState = !hvState; // Zmiana stanu na przeciwny
        
        setNixiePower(hvState);
        
        // Wizualizacja stanu na diodzie ONLINE
        digitalWrite(ONLINE_PIN, hvState ? HIGH : LOW);
        
        Serial.print("\n>>> ZMIANA STANU HV: ");
        Serial.println(hvState ? "ON (WLACZONE)" : "OFF (WYLACZONE)");
    }

    // 3. MONITOROWANIE PGOOD I PRINTOWANIE (Co 1 sekundę)
    if (currentMillis - lastStatusPrint > 1000) {
        lastStatusPrint = currentMillis;
        
        bool pgoodStatus = isPowerGood();
        
        Serial.print("Status Systemu -> PGOOD: ");
        if (pgoodStatus) {
            Serial.print("[ OK ] ");
            digitalWrite(ERROR_PIN, LOW);
        } else {
            Serial.print("[ BRAK SYGNALU ] ");
            digitalWrite(ERROR_PIN, HIGH);
        }
        
        Serial.print("| Cel HV: ");
        Serial.println(hvState ? "POWINNO BYC 170V" : "POWINNO BYC 0V");
        
        // Logika bezpieczeństwa: Jeśli chcemy HV, a PGOOD zniknie - zapal błąd
        if (hvState && !pgoodStatus) {
            Serial.println("!!! ALARM: Przetwornica powinna pracowac, ale PGOOD jest NISKI!");
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
}