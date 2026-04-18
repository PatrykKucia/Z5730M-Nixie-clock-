#include "Nixie.h"

// --- Implementacja NixieTube ---

NixieTube::NixieTube(uint8_t pinA, uint8_t pinB, uint8_t pinC, uint8_t pinD) {
    _pinA = pinA;
    _pinB = pinB;
    _pinC = pinC;
    _pinD = pinD;
}

void NixieTube::begin() {
    pinMode(_pinA, OUTPUT);
    pinMode(_pinB, OUTPUT);
    pinMode(_pinC, OUTPUT);
    pinMode(_pinD, OUTPUT);
    blank(); // Domyślnie wygaszamy lampę po restarcie
}

void NixieTube::displayDigit(uint8_t digit) {
    if (digit > 9) {
        blank(); // Zabezpieczenie: jeśli chcemy wyświetlić np. 10, po prostu wygaszamy
        return;
    }
    
    // Operacje bitowe maskujące odpowiednie bity dla wejść A(LSB), B, C, D(MSB)
    digitalWrite(_pinA, (digit & 0x01) ? HIGH : LOW);
    digitalWrite(_pinB, (digit & 0x02) ? HIGH : LOW);
    digitalWrite(_pinC, (digit & 0x04) ? HIGH : LOW);
    digitalWrite(_pinD, (digit & 0x08) ? HIGH : LOW);
}

void NixieTube::blank() {
    // Stan 1111 (15 dziesiętnie) na układzie 74141 powoduje brak wyjścia = zgaszona lampa
    digitalWrite(_pinA, HIGH);
    digitalWrite(_pinB, HIGH);
    digitalWrite(_pinC, HIGH);
    digitalWrite(_pinD, HIGH);
}


// --- Implementacja NixieDisplay ---

NixieDisplay::NixieDisplay(NixieTube t1, NixieTube t2, NixieTube t3, NixieTube t4) 
    : _tubes{t1, t2, t3, t4} {}

void NixieDisplay::begin() {
    for (int i = 0; i < 4; i++) {
        _tubes[i].begin();
    }
}

void NixieDisplay::setTime(uint8_t hours, uint8_t minutes) {
    if (hours > 99 || minutes > 99) return; 

    _tubes[0].displayDigit(hours / 10);      // Dziesiątki godzin
    _tubes[1].displayDigit(hours % 10);      // Jednostki godzin
    _tubes[2].displayDigit(minutes / 10);    // Dziesiątki minut
    _tubes[3].displayDigit(minutes % 10);    // Jednostki minut
}

void NixieDisplay::setNumber(uint16_t number) {
    if (number > 9999) return;

    _tubes[0].displayDigit(number / 1000);             // Tysiące
    _tubes[1].displayDigit((number / 100) % 10);       // Setki
    _tubes[2].displayDigit((number / 10) % 10);        // Dziesiątki
    _tubes[3].displayDigit(number % 10);               // Jednostki
}

void NixieDisplay::blankAll() {
    for (int i = 0; i < 4; i++) {
        _tubes[i].blank();
    }
}