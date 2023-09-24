//
// Created by sotom on 5/09/2023.
//

#ifndef EXTENDIBLE_HASHING_RECORD_H
#define EXTENDIBLE_HASHING_RECORD_H

#include <iostream>
#include <string>
#include <limits>


using namespace std;

class Record {
public:
    int id;
    int Price;
    int Year;
    char Vin[19];
    char State[20];
    char Make[50];




    void showData(){
        cout << "ID: " << id << endl;
        cout << "Price: " << Price << endl;
        cout << "Year: " << Year << endl;
        cout << "Vin: " << Vin << endl;
        cout << "State: " << State << endl;
        cout << "Make: " << Make << endl;
    }

    void set_data() {
        cout << "ID: ";
        cin >> this->id;

        cout << "Price: ";
        cin >> this->Price;

        cout << "Year: ";
        cin >> this->Year;

        cout << "VIN: ";
        cin.ignore(); // Limpia el carácter de nueva línea pendiente después de Year
        cin.get(this->Vin, sizeof(this->Vin));

        // Limpiar el búfer del teclado después de leer el VIN
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "State: ";
        cin.getline(this->State, sizeof(this->State));

        cout << "Make: ";
        cin.getline(this->Make, sizeof(this->Make));
    }



};


#endif //EXTENDIBLE_HASHING_RECORD_H
