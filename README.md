# Projekt: Kalkulator ONP Liczb Zespolonych

## Opis Projektu

Projekt zakłada stworzenie kalkulatora działającego zgodnie z odwrotną notacją polską (ONP) dla liczb zespolonych. Program implementuje struktury do obsługi liczb zespolonych oraz stosu (kolejki LIFO - Last In First Out), umożliwiając wykonywanie operacji arytmetycznych na liczbach zespolonych (+, -, *, /).

## Funkcjonalności programu:

1. **Struktura na liczbę zespoloną**  
   Definiujemy strukturę `LiczbaZespolona`, która przechowuje część rzeczywistą i urojoną liczby zespolonej.

2. **Struktura – element stosu**  
   Tworzymy strukturę `ElementStosu`, która przechowuje liczbę zespoloną oraz wskaźnik na poprzedni element stosu, tworząc dynamiczną strukturę stosu.

3. **Obsługa stosu (LIFO)**  
   Implementacja podstawowych operacji na stosie, takich jak:
   - **Push (Połóż)**: Dodaje nową liczbę zespoloną na stos.
   - **Pop (Zdejmij)**: Usuwa liczbę zespoloną ze stosu.
   - **Czy pusty?**: Sprawdza, czy stos jest pusty.
   - **Wyczyść stos**: Usuwa wszystkie elementy ze stosu.
   - **Wyświetl stos**: Wyświetla zawartość stosu.

4. **Kalkulator liczb zespolonych działający według ONP**  
   Program obsługuje podstawowe operacje matematyczne na liczbach zespolonych, zgodnie z odwrotną notacją polską (ONP). Użytkownik może wprowadzać liczby i operatory, a kalkulator oblicza wynik, wykorzystując stos.
