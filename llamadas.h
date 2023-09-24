//
// Created by sotom on 23/09/2023.
//
#include "Record.h"
#include <string>
#include <cstring>
#include <sstream>
#include "extendible_hash.h"
#include <fstream>
#include <vector>
using namespace std;
void read_dataset_count(string filename, int i, extendible_hash &ext) {
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


void read_dataset_ALL(string filename, extendible_hash &ext) {


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
                //hash_structure.insert_record(record);
                /*record.showData();
                cout<<endl;
                cout<<endl;
*/
        }
    }
}
