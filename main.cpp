#include <iostream>
#include "Record.h"
#include "Bucket.h"
#include "extendible_hash.h"
using namespace std;
#include <chrono>
#include "llamadas.h"

void Interactive_Menu_ExtendibleHash()
{
    extendible_hash prueba(8,"index_prue.bin","data_prue.bin");
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
                    read_dataset_count("prueba.csv",n,prueba);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> duration = end - start;
                    //cout << "\nSecondary memory accesses are: " << ext_hash.memory_accesses << "\n";
                    cout << "ADD: Elapsed time: " << duration.count() << " segundos"
                         << "\n";
                }
                else
                {
                    auto start = std::chrono::high_resolution_clock::now();
                    read_dataset_ALL("prueba.csv",prueba);
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
                //ingrese_0_para_salir(opcion);
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
                //cout << prueba.delete_Record((key)) << endl;
                if (prueba.delete_Record((key))){
                    cout<<"Eliminado con exito"<<endl;
                }
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                //cout << "\nSecondary memory accesses are: " << ext_hash.memory_accesses << "\n";
                cout << "DELETE: Elapsed time:  " << duration.count() << " seconds"
                     << "\n";
               // ingrese_0_para_salir(opcion);
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
               // ingrese_0_para_salir(opcion);
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


int main() {

    Interactive_Menu_ExtendibleHash();
    //read_dataset_count("prueba.csv",4);
/*
    extendible_hash prueba(3,"index_prue.bin","data_prue.bin");

    Bucket b=Bucket(4);
    prueba.read_Buckets();
    cout<<prueba.delete_Record("KL4CJBSB8FB068543");
    prueba.read_Buckets();

    cout<<"CANTIDAD DE BUCKETS: "<<endl;
    cout<<prueba.size_data()/b.sizeofBucket()<<endl;
    prueba.read_Buckets();
    */
    /*for(int i=0;i<5;i++){
        Record record;
        record.set_data();
        record.showData();
        prueba.insert_record(record);

    }
*/

/*
    if(prueba.search_record("KL4CJBSB8FB068543")){
        cout<<"Record encontrado"<<endl;
    }
    else{
        cout<<"Record no existente"<<endl;
    }
*/


    //BUSQUEDA FUNCIONAL
/*
    extendible_hash prueba(3,"index_prue.bin","data_prue.bin");

    Index_hash pe=prueba.search_index("1");
    cout<<"Indice encontrado: "<<endl;
    pe.index_cout();
*/
//END BUSQUEDA FUNCIONAL

/*
    Record record;
    record.set_data();
    record.showData();
    prueba.insert_record(record);
*/
    //cout<<"El resultado de la busqueda es: "<<prueba.search_record("KL4CJBSB8FB068543");
/*
    if(prueba.search_record("KL4CJBSB8FB068543")){
        cout<<"Record encontrado"<<endl;
    }
    else{
        cout<<"Record no existente"<<endl;
    }
    */
/*
        cout << "Peso record: " << sizeof(Record) << endl;

        Bucket pepe(4);
        cout << "Peso bucket " << pepe.sizeofBucket() << endl;
        pepe.count++;
        cout << "a meter" << endl;
        Record pipi;
        pipi.set_data();
        pepe.records[0]=pipi;
        pepe.allcout();

        cout << "Sacar" << endl;

        // Escribir el objeto en un archivo binario (reemplazando cualquier contenido anterior)
        ofstream file("sas.bin", ios::binary | ios::out);
        file.write(reinterpret_cast<char*>(&pepe), pepe.sizeofBucket());
        file.close();
        cout << "pepe size " << pepe.sizeofBucket() << endl;

        // Leer el objeto desde el archivo
        Bucket pepelen(4);
        ifstream file2("sas.bin", ios::binary);
        file2.seekg(0,ios::beg);
        file2.read(reinterpret_cast<char*>(&pepelen), pepelen.sizeofBucket());
        file2.close();
        cout << "el metido" << endl;
        pepelen.allcout();
        cout<<"Alumno"<<endl;
        pepelen.records[0].showData();
        cout << "Fin" << endl;
        cout<<"pepelensda "<<pepelen.sizeofBucket()<<endl;
        return 0;

*/
}
