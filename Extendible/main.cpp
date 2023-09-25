#include <iostream>
#include <chrono>
#include "Record.h"
#include "Bucket.h"
#include "extendible_hash.h"
#include "llamadas.h"
using namespace std;



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
