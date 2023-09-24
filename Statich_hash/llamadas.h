//
// Created by sotom on 23/09/2023.
//
#include "Record.h"
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;
#include "static_hash.h"
#include <chrono>

void read_dataset_ALL(string filename, static_hash &ext);
void read_dataset_count(string filename, int i, static_hash &ext);
void Interactive_Menu_ExtendibleHash()
{
    static_hash prueba("data_prubea_csv.bin",8);
    int opcion;
    bool repetir = true;

    do
    {
        cout << "\n\n        Menu " << endl;
        cout << "--------------------" << endl;
        cout << "1. Load .csv" << endl;
        cout << "2. Add record" << endl;
        cout << "3. Remove record" << endl;
        cout << "4. Search record" << endl;
        cout << "5. Show all records" << endl;
        cout << "0. End" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;

        while (opcion != 0 and opcion != 1 and opcion != 2 and opcion != 3 and opcion != 4 and opcion != 5)
        {
            cerr << "\nEnter a valid option";
            cout << "\nEnter an option: ";
            cin >> opcion;
        }
        switch (opcion)
        {
            case 1:
            {
                //ext_hash.memory_accesses = 0;
                int n;
                cout << "How many records you want to load? (0 if all)\n";
                cout<<" 0 == ALL"<<endl;
                cin>>n;
                if (n != 0)
                {
                    auto start = std::chrono::high_resolution_clock::now();
                    read_dataset_count("tc20171021.csv",n,prueba);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> duration = end - start;
                    //cout << "\nSecondary memory accesses are: " << ext_hash.memory_accesses << "\n";
                    cout << "ADD: Elapsed time: " << duration.count() << " segundos"
                         << "\n";
                }
                else
                {
                    auto start = std::chrono::high_resolution_clock::now();
                    read_dataset_ALL("tc20171021.csv",prueba);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> duration = end - start;
                    // cout << "\nSecondary memory accesses are: " << ext_hash.memory_accesses << "\n";
                    cout << "ADD: Elapsed time:  " << duration.count() << " seconds"
                         << "\n";
                }
            }
                break;
            case 2:
            {
                //ext_hash.memory_accesses = 0;
                Record record;
                record.set_data();
                auto start = std::chrono::high_resolution_clock::now();
                prueba.insert_record(record);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                //cout << "\nSecondary memory accesses are: " << ext_hash.memory_accesses << "\n";
                cout << "ADD: Elapsed time:  " << duration.count() << " seconds"
                     << "\n";
            }
                break;
            case 3:
            {
                //ext_hash.memory_accesses = 0;
                string key;
                cout << "Insert key to remove:";
                cin.ignore();
                getline(cin, key);
                auto start = std::chrono::high_resolution_clock::now();
                if (prueba.delete_Record((key))){
                    cout<<"Eliminado con exito"<<endl;
                }
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                //cout << "\nSecondary memory accesses are: " << ext_hash.memory_accesses << "\n";
                cout << "DELETE: Elapsed time:  " << duration.count() << " seconds"
                     << "\n";
            }
                break;
            case 4:
            {
                //ext_hash.memory_accesses = 0;
                string key;
                cout << "Insert key to search:\n";
                cin.ignore();
                getline(cin, key);
                Record searched;

                auto start = std::chrono::high_resolution_clock::now();
                if (prueba.search_record(key) == true)
                {
                    cout<<"Encontrado con exito "<<endl;
                    searched.showData();
                    auto end = std::chrono::high_resolution_clock::now();

                    std::chrono::duration<double> duration = end - start;

                    //cout << "\nSecondary memory accesses are: " << ext_hash.memory_accesses << "\n";
                    cout << "SEARCH: Elapsed time:  " << duration.count() << " seconds"
                         << "\n";
                }
                //  ingrese_0_para_salir(opcion);
            }
                break;
            case 5:
            {
                prueba.read_Buckets();
            }
                break;
            case 0:
            {
                repetir = false;
            }
                break;
            default:
            {
                cout << "\nInvalid option.";
            }
                break;
        }
    } while (repetir);
}









void read_dataset_count(string filename, int i, static_hash &ext) {
    int cont = 0;
    fstream fio;

    fio.open(filename, ios::in | ios::out | ios::binary);
    string line;

    getline(fio, line); // Lee la primera línea para ignorar los encabezados

    while (getline(fio, line)) {
        cout<<"count: "<<cont<<endl;
        string str;
        stringstream ss(line);
        vector<string> vec;

        while (getline(ss, str, ',')) {
            vec.push_back(str);
        }

        if (vec.size() >= 9) { // Verifica si hay suficientes campos
            Record record;
            record.id = stoi(vec[0]);
            record.Price = stoi(vec[1]);
            record.Year = stoi(vec[2]);
            strcpy(record.Vin, vec[6].c_str());
            strcpy(record.State, vec[5].c_str());
            strcpy(record.Make, vec[7].c_str());

            if (cont < i) {
                ext.insert_record(record);
                cout<<"El record numero: "<<cont<<endl;
                //record.showData();
                cout<<endl;
                cout<<endl;
                cont++;
            }
            else {
                return;
            }
        }
    }
}







void read_dataset_ALL(string filename, static_hash &ext) {


    fstream file(filename, ios::in | ios::out | ios::binary);
    string line;

    getline(file, line); // Lee la primera línea para ignorar los encabezados

    while (getline(file, line)) {
        string str;
        stringstream ss(line);
        vector<string> vec;

        while (getline(ss, str, ',')) {
            vec.push_back(str);
        }

        if (vec.size() >= 9) { // Verifica si hay suficientes campos
            Record record;
            record.id = stoi(vec[0]);
            record.Price = stoi(vec[1]);
            record.Year = stoi(vec[2]);
            strcpy(record.Vin, vec[6].c_str());
            strcpy(record.State, vec[5].c_str());
            strcpy(record.Make, vec[7].c_str());

            ext.insert_record(record);

                //record.showData();
                cout<<endl;
                cout<<endl;
        }
    }
}
