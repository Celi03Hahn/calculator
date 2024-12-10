#include <limits.h>  // Um den maximalen Wert von int zu erhalten

void setup() {
    // Seriellen Monitor starten
    Serial.begin(9600);
}

void loop() {
    // Warten, bis Daten vom PC verfügbar sind
    if (Serial.available() > 0) {
        // Eingabedaten vom PC lesen
        String input = Serial.readStringUntil('\n');
        
        // Entfernen von führenden/trailing Leerzeichen
        input.trim();
        
        // Berechnung durchführen
        long result = calculate(input);  // Verwenden von 'long' für größere Ergebnisse
        
        // Wenn das Ergebnis den maximalen Wert überschreitet
        if (result > INT_MAX || result < INT_MIN) {
            Serial.println("Fehler: Ergebnis überschreitet den maximal darstellbaren Wert!");
        } else {
            // Ergebnis zurück an den PC senden
            Serial.println(result);
        }
    }
}

// Funktion, die eine einfache Berechnung durchführt (Addition, Subtraktion, Multiplikation, Division)
long calculate(String input) {
    long num1, num2;
    char op;
    
    // Versuchen, die Eingabe zu parsen (z. B. "5 + 3")
    int parsed = sscanf(input.c_str(), "%ld %c %ld", &num1, &op, &num2);
    
    if (parsed != 3) {
        return 0;  // Fehlerbehandlung: 0 zurückgeben, wenn das Parsen fehlschlägt
    }
    
    if (parsed == 3) {
        // Berechnung basierend auf dem Operator
        if (op == '+') {
            return num1 + num2;
        } else if (op == '-') {
            return num1 - num2;
        } else if (op == '*') {
            return num1 * num2;  // Multiplikation
        } else if (op == '/') {
            if (num2 != 0) {
                return num1 / num2;  // Division (mit Überprüfung auf Division durch Null)
            } else {
                Serial.println("Fehler: Division durch Null!");
                return 0;  // Fehlerfall für Division durch Null
            }
        }
    }
    
    return 0;  // Fehlerbehandlung: 0 zurückgeben, wenn der Operator nicht erkannt wird
}



