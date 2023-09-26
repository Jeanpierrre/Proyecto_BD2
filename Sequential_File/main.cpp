#include <iostream>
#include "sequential_file.h"
using namespace std;

int Menu_SequentialFile() {
    int op;
    while (true) {
        cout << endl << "///// Menu ///// " << endl;
        cout << "1. Load .csv" << endl;
        cout << "2. Add record" << endl;
        cout << "3. Remove record - x" << endl;
        cout << "4. Search record" << endl;
        cout << "5. Search by range - x" << endl;
        cout << "6. Show all records" << endl;
        cout << "0. Exit" << endl;
        cout << endl;
        cout << "--> ";
        cin >> op;

        if (op == 0 || op == 1 || op == 2 || op == 3 || op == 4 || op == 5 || op == 6) {
            return op;
            break;
        } else {
            cout << "Not valid option" << endl;
            Sleep(2000);
            system("clear");
        }
    }
}

void titulo () {
    cout << "  ____                          ____  " << endl;
    cout << " / ___|_ __ _   _ _ __   ___   | ___| " << endl;
    cout << "| |  _| '__| | | | '_ \\ / _ \\  |___ \\ " << endl;
    cout << "| |_| | |  | |_| | |_) | (_) |  ___) |" << endl;
    cout << " \\____|_|   \\__,_| .__/ \\___/  |____/ " << endl;
    cout << "                 |_|                  " << endl;
}

int main () {
    titulo();
    Sequential_File seq("data.dat");

    while (true) {
        int op = Menu_SequentialFile();

        if (op == 1) {
            cout << "Cargando data from .cvs ... " << endl << endl;
            auto start = std::chrono::high_resolution_clock::now(); 

            read_dataset_count("tc20171021.csv", 500, seq);

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            cout << endl;
            cout << "ADD FROM CSV TIME: " << duration.count() << "s";
            cout << endl;

        } else if (op == 2) {
            cout << "Add record ... " << endl << endl;
            Sequential_Record rx;
            auto start = std::chrono::high_resolution_clock::now();
            
            rx.setData();
            seq.add(rx);

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            cout << endl;
            cout << "Data añadida a base de datos ..." << endl;
            cout << "SINGLE ADD TIME: " << duration.count() << "s";
            cout << endl;

        } else if (op == 3) {
            cout << "Remove record from data ... " << endl << endl;
            cout << "Not implemented" << endl;

        } else if (op == 4) {
            cout << "Search a record" << endl << endl;
            string sh;
            cout << "Inser key of record: "; cin >> sh;
            auto start = std::chrono::high_resolution_clock::now();

            seq.search(sh).showData();

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            cout << endl;
            cout << "TIME TO SEARCH A RECORD: " << duration.count() << "s";
            cout << endl;

        } else if (op == 5) {
            cout << "Search by range" << endl << endl;
            cout << "Not implemented" << endl;

        } else if (op == 6) {
            cout << endl;
            cout << "Show all record in data" << endl << endl;
            auto start = std::chrono::high_resolution_clock::now();

            seq.showRecords();

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            cout << endl;
            cout << "SHOW RECORDS TIME: " << duration.count() << "s";
            cout << endl;

        } else {
            break;
        }
    }
    cout << "Gracias!" << endl;
}