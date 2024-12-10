#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

HANDLE openSerialPort(const wstring& portName) {
    HANDLE hSerial = CreateFileW(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        cerr << "Fehler beim ÷ffnen des seriellen Ports!" << endl;
        return INVALID_HANDLE_VALUE;
    }
    return hSerial;
}

bool configureSerialPort(HANDLE hSerial) {
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        cerr << "Fehler beim Abrufen der seriellen Parameter!" << endl;
        return false;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        cerr << "Fehler beim Setzen der seriellen Parameter!" << endl;
        return false;
    }
    return true;
}

void sendData(HANDLE hSerial, const string& data) {
    DWORD bytesWritten;
    if (!WriteFile(hSerial, data.c_str(), data.length(), &bytesWritten, NULL)) {
        cerr << "Fehler beim Schreiben in den seriellen Port!" << endl;
    }
}

string receiveData(HANDLE hSerial) {
    char buffer[256];
    DWORD bytesRead;
    string result;

    if (ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, NULL)) {
        result.append(buffer, bytesRead);
    }
    else {
        cerr << "Fehler beim Lesen vom seriellen Port!" << endl;
    }

    return result;
}

int main() {
    wstring portName = L"COM3";  // Beispiel f¸r COM-Port (anpassen!)
    HANDLE hSerial = openSerialPort(portName);
    if (hSerial == INVALID_HANDLE_VALUE) {
        return 1;
    }

    if (!configureSerialPort(hSerial)) {
        CloseHandle(hSerial);
        return 1;
    }

    string input;
    char continueCalculation = 'y'; // Variable f¸r fortlaufende Berechnungen

    while (continueCalculation == 'y' || continueCalculation == 'Y') {
        // Benutzereingabe f¸r Berechnung
        cout << "Gib die Berechnung ein (z.B. 5 + 3): ";
        getline(cin, input);

        // Eingabedaten an den Mikrocontroller senden
        sendData(hSerial, input);

        // Anzeige, dass der Mikrocontroller jetzt rechnet
        cout << "Der Mikrocontroller rechnet...\n";

        // Ergebnis vom Mikrocontroller empfangen
        string result = receiveData(hSerial);

        // Ergebnis anzeigen
        cout << "Ergebnis vom Mikrocontroller: " << result << endl;

        // Benutzer fragen, ob er eine weitere Berechnung durchf¸hren mˆchte
        cout << "Willst du eine weitere Berechnung machen? (y/n): ";
        cin >> continueCalculation;
        cin.ignore();  // Entfernt das newline-Zeichen aus dem Eingabepuffer nach der Auswahl
    }

    // Seriellen Port schlieﬂen
    CloseHandle(hSerial);

    cout << "Programm beendet." << endl;

    return 0;
}

