#include "WurthLED.h"

// Definicja pinu (tylko w jednym miejscu!)
const int LED_PIN = 3;

// Zmienne globalne do efektów (ukryte w tym pliku)
uint32_t lastEffectUpdate = 0;
uint16_t effectStep = 0;

// --- FUNKCJA POMOCNICZA: Opóźnienie ---
static inline void delay_cycles(uint32_t cycles) {
    uint32_t start = ESP.getCycleCount();
    while (ESP.getCycleCount() - start < cycles);
}

// --- FUNKCJA POMOCNICZA: Wysyłanie bitu ---
void IRAM_ATTR sendBit(bool bitVal) {
    uint32_t t_high = bitVal ? 190 : 45;
    uint32_t t_low  = bitVal ? 45 : 190;

    GPIO.out_w1ts = (1 << LED_PIN); 
    delay_cycles(t_high);
    GPIO.out_w1tc = (1 << LED_PIN); 
    delay_cycles(t_low);
}

// --- FUNKCJA POMOCNICZA: Wysyłanie 16 bitów ---
void IRAM_ATTR send16Bits(uint8_t gain, uint16_t pwm) {
    uint16_t data = ((gain & 0x0F) << 12) | (pwm & 0x0FFF);
    for (int i = 15; i >= 0; i--) {
        sendBit(data & (1 << i));
    }
}

// --- FUNKCJA: Tęcza ---
void setRainbowColor(uint16_t wheelPos) {
    uint16_t r = 0, g = 0, b = 0;
    if (wheelPos < 1365) {
        r = 4095 - wheelPos * 3;
        g = wheelPos * 3;
        b = 0;
    } else if (wheelPos < 2730) {
        wheelPos -= 1365;
        r = 0;
        g = 4095 - wheelPos * 3;
        b = wheelPos * 3;
    } else {
        wheelPos -= 2730;
        r = wheelPos * 3;
        g = 0;
        b = 4095 - wheelPos * 3;
    }
    setWurthLed(true, r, g, b); // Tutaj nie podajemy gain, użyje domyślnego 0x0F z .h
}

// ====================================================================
// GŁÓWNA FUNKCJA OBSŁUGI LEDa (Bez gain = 0x0F w definicji!)
// ====================================================================
void setWurthLed(bool state, uint16_t r, uint16_t g, uint16_t b, uint8_t gain) {    //4095 max value
    if (!state) { r = 0; g = 0; b = 0; }
    noInterrupts(); 
    send16Bits(gain, r);
    send16Bits(gain, g);
    send16Bits(gain, b);
    interrupts(); 
    delayMicroseconds(300); 
}

// ====================================================================
// GŁÓWNA FUNKCJA EFEKTÓW
// ====================================================================
void runEffect(int effectMode) {
    uint32_t currentMillis = millis();

    switch (effectMode) {
        case 1: // Tęcza
            if (currentMillis - lastEffectUpdate > 10) {
                lastEffectUpdate = currentMillis;
                effectStep = (effectStep + 10) % 4095;
                setRainbowColor(effectStep);
            }
            break;
        case 2: // Oddychanie
            if (currentMillis - lastEffectUpdate > 5) {
                lastEffectUpdate = currentMillis;
                int brightness = (abs((int)(currentMillis % 2000) - 1000) * 4095) / 1000;
                setWurthLed(true, 0, brightness, 0); 
            }
            break;
        case 3: // Stroboskop
            if (currentMillis - lastEffectUpdate > 80) { 
                lastEffectUpdate = currentMillis;
                effectStep++;
                if (effectStep % 2 == 0) { setWurthLed(true, 4095, 0, 0); } 
                else { setWurthLed(true, 0, 0, 4095); }
            }
            break;
        default:
            setWurthLed(false, 0, 0, 0); 
            break;
    }
}