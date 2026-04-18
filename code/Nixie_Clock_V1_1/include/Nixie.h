#ifndef NIXIE_H
#define NIXIE_H

#include <Arduino.h>

// Klasa reprezentująca pojedynczą lampę Nixie z dekoderem 74141
class NixieTube {
public:
    // Konstruktor przyjmujący piny A, B, C, D z ESP32
    NixieTube(uint8_t pinA, uint8_t pinB, uint8_t pinC, uint8_t pinD);
    
    // Inicjalizacja pinów (wywoływana w setup)
    void begin();
    
    // Wyświetla cyfrę od 0 do 9. Cyfry > 9 wygaszają lampę.
    void displayDigit(uint8_t digit);
    
    // Całkowite wygaszenie lampy (wysyła stan 1111 na dekoder)
    void blank();

private:
    uint8_t _pinA;
    uint8_t _pinB;
    uint8_t _pinC;
    uint8_t _pinD;
};

// Klasa zarządzająca wszystkimi 4 lampami jako zegarem/wyświetlaczem
class NixieDisplay {
public:
    // Konstruktor przyjmujący 4 obiekty lamp
    NixieDisplay(NixieTube t1, NixieTube t2, NixieTube t3, NixieTube t4);
    
    void begin();
    
    // Ustawia czas (np. HH:MM)
    void setTime(uint8_t hours, uint8_t minutes);
    
    // Wyświetla dowolną liczbę 4-cyfrową (od 0 do 9999)
    void setNumber(uint16_t number);
    
    // Wygasza wszystkie lampy
    void blankAll();

private:
    NixieTube _tubes[4];
};

#endif