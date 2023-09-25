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



void read_dataset_count_extendible(string filename, int i, extendible_hash &ext) {
    int cont = 0;
    fstream fio;

    fio.open(filename, ios::in | ios::out | ios::binary);
    string line;

    getline(fio, line); // Lee la primera línea para ignorar los encabezados

    while (getline(fio, line)) {
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
               // cout<<"El record numero: "<<cont<<endl;
                //record.showData();
               // cout<<endl;
               // cout<<endl;
                cont++;
            }
            else {
                return;
            }
        }
    }
    cout<<"count: "<<cont<<endl;

}


void read_dataset_ALL_extendible(string filename, extendible_hash &ext) {


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

void Interactive_Menu_ExtendibleHash()
{
    extendible_hash prueba(8,"index_prueba.bin","data_prueba.bin");
    int opcion;
    bool repetir = true;

    do
    {
        cout << "\n\n        Menú " << endl;
        cout << "--------------------" << endl;
        cout << "1. Cargar .csv" << endl;
        cout << "2. Agregar registro" << endl;
        cout << "3. Eliminar registro" << endl;
        cout << "4. Buscar registro" << endl;
        cout << "5. Mostrar todos los registros" << endl;
        cout << "0. Salir" << endl;

        cout << "\nIngrese una opción: ";

        cin >> opcion;

        while (opcion != 0 and opcion != 1 and opcion != 2 and opcion != 3 and opcion != 4 and opcion != 5)
        {
            cerr << "\nIngrese una opción válida";
            cout << "\nIngrese una opción: ";
            cin >> opcion;

        }
        switch (opcion)
        {
            case 1:
            {
                //ext_hash.memory_accesses = 0;
                int n;
                cout << "¿Cuántos registros desea cargar? (0 si desea cargar todos)\n";
                cin>>n;
                if (n != 0)
                {
                    auto start = std::chrono::high_resolution_clock::now();
                    read_dataset_count_extendible("prueba.csv",n,prueba);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> duration = end - start;
                    //cout << "\nSecondary memory accesses are: " << ext_hash.memory_accesses << "\n";
                    cout << "AGREGAR: Tiempo transcurrido: " << duration.count() << " segundos"<<endl;

                }
                else
                {
                    auto start = std::chrono::high_resolution_clock::now();
                    read_dataset_ALL_extendible("prueba.csv",prueba);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> duration = end - start;
                    // cout << "\nSecondary memory accesses are: " << ext_hash.memory_accesses << "\n";
                    cout << "AGREGAR: Tiempo transcurrido: " << duration.count() << " segundos"<<endl;

                }
            }
                break;
            case 2:
            {
                Record record;
                record.set_data();
                auto start = std::chrono::high_resolution_clock::now();
                prueba.insert_record(record);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                cout << "AGREGAR: Tiempo transcurrido: " << duration.count() << " segundos"
                     << endl;

            }
                break;
            case 3:
            {
                string key;
                cout << "Ingrese la clave a eliminar: ";

                cin.ignore();
                getline(cin, key);
                auto start = std::chrono::high_resolution_clock::now();
                if (prueba.delete_Record((key))){
                    cout<<"Eliminado con exito"<<endl;
                }
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                cout << "ELIMINAR: Tiempo transcurrido: " << duration.count() << " segundos"<<endl;

            }
                break;
            case 4:
            {
                string key;
                cout << "Insertar clave para buscar:"<<endl;
                cin.ignore();
                getline(cin, key);

                auto start = std::chrono::high_resolution_clock::now();
                if (prueba.search_record(key))
                {
                    cout<<"Encontrado con exito "<<endl;

                }
                else{
                    cout<<"Car no encontrado "<<endl;
                }
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;

                cout << "BÚSQUEDA: Tiempo transcurrido:  " << duration.count() << " segundos"<<endl;

            }
                break;
            case 5:
            {
                cout<<"Imprimir todos los record "<<endl;
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
                cout<<endl;
                cout << "Opción inválida.";
                cout<<endl;


            }
                break;
        }
    } while (repetir);
}


