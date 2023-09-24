//
// Created by sotom on 24/09/2023.
//

#include "static_hash.h"

static_hash::static_hash( string dataFile, int m)  {
    this->data_file=dataFile;
    this->M=m;
    if(size_data()==0){
        cout<<"Archivo vacio"<<endl;
        initialize_hash_structure(m);
        cout<<"INDICES GUARDADOS: "<<endl;
        //cargar_indices_a_table();
    }



}
int static_hash::size_data() {
    cout<<"size_data"<<endl;

    fstream fileData(this->data_file, ios::app | ios::binary);
    fileData.seekg(0, ios::end);
    return fileData.tellg();
}
void static_hash::initialize_hash_structure(int M) {
    cout<<"initialize"<<endl;
    ofstream file(this->data_file,  std::ios::binary | std::ios::in | std::ios::out);//abrir en binario y al cambiar algo no elimina lo demas
    if(!file.is_open()) throw ("El archivo no existe");//no hago nada
    Bucket bucket=Bucket(fb);
    for(int i=0;i<M;i++){
        cout<<"aa: "<<i<<endl;
        file.write(reinterpret_cast<char*>(&bucket), bucket.sizeofBucket());//sobreescribir bucket
    }

    file.close();

    cout<<"Mostrar: "<<endl;
    int cant = size_data() / bucket.sizeofBucket();
    cout<<"SIZE BUCKET"<<bucket.sizeofBucket()<<endl;
    cout<<"CANTIDAD DE BUCKET: "<<cant<<endl;
    ifstream file2(this->data_file, ios::binary);
    if(!file2.is_open()) throw ("No se pudo abrir el archivo");
    Bucket pepelen(fb);
    for(int i=0;i<M;i++){
        cout<<"en el i: "<<i<<endl;

        file2.read(reinterpret_cast<char*>(&pepelen), pepelen.sizeofBucket());
        cout<<"mostrar bucket leido: "<<endl;
        pepelen.allcout();
        cout<<endl;
        cout<<endl;
    }
    file2.close();


}

void static_hash::insert_record(Record record) {
    int number_key= Vin_to_number(record.Vin);//convertir a entero
    int key_record=(BinKeyMod(number_key));//funcion hash
    cout<<"valor: "<<key_record<<endl;
    cout<<"Leer bucket: "<<endl;
    Bucket buc= read_bucket(key_record);
    if(buc.count<fb){
        add_Record(key_record,buc,record);
        cout<<"Agregado"<<endl;
        cout<<"VIN: "<<buc.records[0].Vin<<endl;
    }
    else{
        cout<<"OVERFLOW CAUSA"<<endl;
        overflow(key_record,buc,record);
    };
}

Bucket static_hash::read_bucket(int pos) {
    cout<<"Read_bucket"<<endl;
    ifstream file(this->data_file, ios::binary);
    if(!file.is_open()) throw ("No se pudo abrir el archivo");
    Bucket pepelen(fb);
    file.seekg(pos*pepelen.sizeofBucket(), ios::beg);//fixed length record
    file.read(reinterpret_cast<char*>(&pepelen), pepelen.sizeofBucket());
   // cout<<"mostrar bucket leido: "<<endl;
    //pepelen.allcout();
    file.close();

    return pepelen;
}

void static_hash::add_Record(int pos, Bucket &bucket, Record record) {
    bucket.records[bucket.count] = record;
    cout<<"COUNT ANTES"<<bucket.count<<endl;
    bucket.count++;
    cout<<"COUNT DESPUES"<<bucket.count<<endl;

    write_bucket(bucket, pos);
}

void static_hash::write_bucket(Bucket &bucket, int pos) {
    ofstream file(this->data_file,  std::ios::binary | std::ios::in | std::ios::out);//abrir en binario y al cambiar algo no elimina lo demas
    if(!file.is_open()) throw ("El archivo no existe");//no hago nada

    file.seekp(pos*bucket.sizeofBucket());//mover puntero al inicio
    file.write(reinterpret_cast<char*>(&bucket), bucket.sizeofBucket());//sobreescribir bucket
    file.close();
    cout<<"write en key: "<<pos<<endl;

}

bool static_hash::search_record(string vin_key) {

    Record record;
    int number_key= Vin_to_number(vin_key);
    int key_pos =(BinKeyMod(number_key));//funcion hash
    cout<<"key pos in search"<<key_pos<<endl;
    Bucket bucket= read_bucket(key_pos);
    cout<<"BUCKET SACADO search"<<endl;
    bucket.allcout();
    //cout<<"Primer record"<<endl;
    //bucket.records[0].showData();

    for (int i = 0; i < bucket.count; i++)
    {

        record = bucket.records[i];
        //cout<<"For "<<i<<" "<<record.Vin<<endl;
        if (record.Vin == vin_key){
            cout<<"For "<<i<<" "<<record.Vin<<endl;
            cout<<"record for"<<record.Vin<<endl;
            cout<<"VIN IF "<<vin_key<<endl;
            cout<<"DATOS DEL RECORD"<<endl;
            bucket.records[i].showData();
            return true;
        }

    }
    //CASO CUANDO ESTA ENLAZADO A OTRO BUCKET
    while(bucket.nextBucket!=-1){
        bucket = read_bucket(bucket.nextBucket);
        for (int i = 0; i < bucket.capacity; i++){
            record = bucket.records[i];
            if (record.Vin == vin_key)
                return true;
        }
    }
    return false;
}

Bucket static_hash::read_bucket_pos(int pos) {

    cout<<"Read_bucket"<<endl;
    ifstream file(this->data_file, ios::binary);
    if(!file.is_open()) throw ("No se pudo abrir el archivo");
    Bucket pepelen(fb);
    file.seekg(pos, ios::beg);//fixed length record
    file.read(reinterpret_cast<char*>(&pepelen), pepelen.sizeofBucket());
    cout<<"mostrar bucket leido: "<<endl;
    pepelen.allcout();
    file.close();

    return pepelen;
}

void static_hash::overflow(int pos,Bucket buc, Record record) {
    Bucket new_bucket=Bucket(fb);

    resize(new_bucket,buc, pos);
    if(new_bucket.count<fb){
        cout<<"overflow if"<<endl;
        add_Record(pos, new_bucket, record);
        cout<<"fin: "<<endl;
    }
    else{
        cout<<"Else"<<endl;
        overflow(pos, new_bucket, record);
    }

}


void static_hash::resize(Bucket &bucket, Bucket &bucket1, int pos) {
    int cant=size_data();
    cout<<"cant: "<<cant<<endl;
    write_in_pos( bucket1, cant);
    bucket.nextBucket=cant;
    //write_bucket(bucket,pos);

}

void static_hash::write_in_pos(Bucket bucket, int pos) {
    ofstream file(this->data_file,  std::ios::binary | std::ios::in | std::ios::out);//abrir en binario y al cambiar algo no elimina lo demas
    if(!file.is_open()) throw ("El archivo no existe");//no hago nada

    file.seekp(pos);//mover puntero al inicio
    file.write(reinterpret_cast<char*>(&bucket), bucket.sizeofBucket());//sobreescribir bucket
    file.close();

    cout<<"write in pos "<<pos<<endl;
    Bucket aa= read_bucket_pos(pos);

}
bool static_hash::delete_Record(string key_record) {
    int number_key = Vin_to_number(key_record); // Convertir string a número
    int key_pos = BinKeyMod(number_key); // Calcular la posición en la tabla hash

    Bucket buc = read_bucket(key_pos); // Leer el bucket correspondiente

    // Buscar el registro en el bucket actual
    for (int i = 0; i < buc.count; i++) {
        if (buc.records[i].Vin == key_record) {
            cout<<"ENCONTRADO"<<endl;
            cout<<"RECORD[I] "<<buc.records[i].Vin<<" key "<<key_record<<endl;
            // Se encontró el registro, proceder a eliminarlo
            for (int j = i + 1; j < buc.count; j++) {
                buc.records[j - 1] = buc.records[j];
            }
            buc.count--;

            // Actualizar el bucket en el archivo
            write_bucket(buc, key_pos);

            return true; // Registro eliminado con éxito
        }
    }

    // El registro no se encontró en el bucket actual, verificar buckets enlazados
    while (buc.nextBucket != -1) {
        int next_bucket_pos = buc.nextBucket;
        buc = read_bucket_pos(next_bucket_pos);

        for (int i = 0; i < buc.count; i++) {
            if (buc.records[i].Vin == key_record) {
                // Se encontró el registro, proceder a eliminarlo
                for (int j = i + 1; j < buc.count; j++) {
                    buc.records[j - 1] = buc.records[j];
                }
                buc.count--;

                // Actualizar el bucket en el archivo
                write_in_pos(buc, next_bucket_pos);

                return true; // Registro eliminado con éxito
            }
        }

    }

    // El registro no se encontró en ningún bucket
    return false;
}

 void static_hash::read_Buckets() {
    ifstream file(this->data_file, ios::binary);
    cout<<"Cantidad de registros-read: "<<endl;
    Bucket buc(fb);
    int pos_Data=size_data()/buc.sizeofBucket();
    cout<<pos_Data<<endl;
    //Bucket buc;
    int contador=0;
    for(int i=0;i<pos_Data;i++){
        cout<<"Read_bucket"<<endl;
        cout<<"I"<<i<<endl;
        file.seekg(i*buc.sizeofBucket(), ios::beg);//fixed length record
        file.read(reinterpret_cast<char*>(&buc), buc.sizeofBucket());
        buc.allcout();

        for(int i=0;i<buc.count;i++){
            Record record=buc.records[i];
            contador++;
            record.showData();
        }

    }
    cout<<"Contador"<<contador<<endl;

}