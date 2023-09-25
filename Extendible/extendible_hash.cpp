//
// Created by sotom on 6/09/2023.
//

#include "extendible_hash.h"
#include "Index_hash.h"
#include "Bucket.h"
#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
void extendible_hash::insert_bucket(Bucket &buc) {
    fstream fileData(this->File_data, ios::app | ios::binary | std::ios::out);
    //fileData.seekp(  (sizeof(Bucket)), ios::beg);
    fileData.write((char *)&buc, bloque.sizeofBucket());
}


int extendible_hash::size_index_hash() {
    fstream fileIndex(this->File_hash_index, ios::app | ios::binary );
    if (!fileIndex.is_open()) {
     //   cout << "Error al abrir el archivo." << endl;
        return -1;
    }
    fileIndex.seekg(0, ios::end);
  //  cout<<"tamaño del archivo index"<<fileIndex.tellg()<<endl;
    return fileIndex.tellg();
}


int extendible_hash::size_data() {
    fstream fileData(this->File_data, ios::app | ios::binary);
    fileData.seekg(0, ios::end);
    return fileData.tellg();
}

void extendible_hash::initialize_hash_structure(string key, int pos) {
  //  cout<<"Indice : "<<key<<endl;
    //cantidad de registros
   // cout<<"Bucket enlazado: ";
    //cout<<"size data en key: "<<key<<" - "<<size_data()<<endl;
    int pos_Data=size_data();
   // cout<<pos_Data<<endl;
    Index_hash indice= Index_hash((key.c_str()),pos_Data);//guardar indice ejem 0 con la pos 0 del archivo

    hash_table_unorder[key]=make_pair(pos_Data,can_index);


    SaveIndex(indice);

    fstream fileIndexRead(this->File_hash_index, ios::in | ios::binary);

    // Modificación aquí: Leer el índice recién guardado

    Index_hash prueba("DSAD", 77);
    fileIndexRead.seekg((sizeof(int) * 2) + (sizeof(Index_hash) *pos), ios::beg);
    fileIndexRead.read((char*)&prueba, sizeof(Index_hash));
    fileIndexRead.close();



  //  cout << "SACADO BIEN" << endl;
    //prueba.index_cout();

    //cout << "Sacado Fin" << endl;

    Bucket bucket=Bucket(fb);
    //cout<<"Initialice: "<<bucket.sizeofBucket()<<endl;
    //<<"PESO BLOQUE:"<<bucket.sizeofBucket()<<endl;
    insert_bucket(bucket);




}



extendible_hash::extendible_hash(int D, string index, string data) {

    this->File_data=data;
    this->File_hash_index=index;
    this-> D=D;

    if(size_index_hash()==0){
        //cout<<"Archivo vacio"<<endl;
        Index_count();
        //inicializar indices
        initialize_hash_structure("0",0);
        initialize_hash_structure("1",1);
       // cout<<"INDICES GUARDADOS: "<<endl;
        //cargar_indices_a_table();
    }
    else{
        //cout<<"Cargar indices"<<endl;
        cargar_indices_a_table();
        cargar_cabezas_a_table();
       // cout<<"Indices cargados"<<endl;

        cout<<"Cabezas: "<<endl;
        for (const auto& pair : table_cabezas){
            cout<<pair.first<<" - "<<pair.second<<endl;
        }
        cout<<"indices: "<<endl;
        for (const auto& pair : hash_table_unorder){
            const std::string& clave = pair.first;
            const std::pair<int, int>& valor = pair.second;

            std::cout << "Clave: " << clave << ", Valor: (" << valor.first << ", " << valor.second << ")" << std::endl;
            //cout<<pair.first<<" - "<<pair.second<<endl;
        }
       //cout<<"FIn"<<endl;

    }

    //index hash

    //EN no existir, entonces inicializamos 2 buckets
    //uno con 0 y otro con 1
    //idependientemente de la profundidad
    //Index_count();

    //inicializar indices

    //initialize_hash_structure("0",0);
    //initialize_hash_structure("1",1);

    /*hash_table_unorder["0"]=0;
    hash_table_unorder["1"]=24;
    table_cabezas["0's"]=1;
    table_cabezas["1's"]=1;*/

    // initialize_hash_structure("1");
/*
/*
        cout<<"INDEX FILE"<<endl;
        read_indices();
        cout<<"DASDA"<<endl;




        cout<<"READ BIN"<<endl;
        read_Buckets();

*/
}

void extendible_hash::read_Buckets() {
    ifstream file(this->File_data, ios::binary);
    cout<<"Cantidad de registros-read: "<<endl;
    int pos_Data=size_data()/ sizeof(Bucket);
    cout<<pos_Data<<endl;
    //Bucket buc;
    Bucket buc(fb);
    int contador=0;
    for(int i=0;i<pos_Data;i++){
        //cout<<"Read_bucket"<<endl;
        cout<<"En el bucket i: "<<i<<endl;
        file.seekg(i*bloque.sizeofBucket(), ios::beg);//fixed length record
        file.read(reinterpret_cast<char*>(&buc), bloque.sizeofBucket());
        buc.allcout();
        cout<<endl;
        cout<<"Registros: "<<endl;
        for(int i=0;i<buc.count;i++){
            Record record=buc.records[i];
            contador++;
            record.showData();
            cout<<endl;
        }

    }
    cout<<"Contador"<<contador<<endl;

}

void extendible_hash::read_indices() {

}

unsigned long long extendible_hash::Vin_to_number(string key) {

        // Convierte el string en un número largo
        unsigned long long key_as_long = 0;

        for (char c : key) {
            key_as_long = key_as_long * 256 + static_cast<unsigned long long>(c);
        }

        return (key_as_long);  // Convierte a int antes de devolverlo





}




void extendible_hash::Index_count() {
    //ofstream fileData(this->File_data, ios::app | ios::binary);
    fstream fileIndex(this->File_hash_index, ios::app | ios::binary | ios::out);
    int cabeza_0 = 1;
    int cabeza_1 = 1;
    table_cabezas["0's"]=cabeza_0;
    table_cabezas["1's"]=cabeza_1;

    // Se escriben uno después del otro.
    fileIndex.write((char*)&cabeza_0, sizeof(int));
    fileIndex.write((char*)&cabeza_1, sizeof(int));
    fileIndex.close();
/*
    // Reabrir el archivo en modo lectura
    fstream fileIndexs(this->File_hash_index,  ios::in |ios::binary);

    int prueba;
    fileIndexs.read((char*)&prueba, sizeof(int));
   // cout << "prueba 1: " << prueba << endl;
    fileIndexs.read((char*)&prueba, sizeof(int));
    //cout << "prueba 2: " << prueba << endl;
    fileIndexs.close();*/
}

void extendible_hash::SaveIndex(Index_hash indice) {
    fstream fileIndex(this->File_hash_index, ios::app | ios::binary | ios::out);
    fileIndex.write(reinterpret_cast<char*>(&indice), sizeof(Index_hash));
    fileIndex.close();
    can_index=can_index+1;

}



bool extendible_hash::search_record(string vin_key) {
    cout<<"VIN-KEY IN SEARCH: "<<vin_key<<endl;
    //int val=Vin_to_number(vin);
    //string key =BinKeyMod(val);
//primero transformar el vin y sacar su key
    Record record;
    int number_key= Vin_to_number(vin_key);
    string key=(BinKeyMod(number_key));//funcion hash
    //ahora tocara validar el indice donde va el key
    cout<<"LA KEY ES:"<<key<<endl;

    int pos=search_pos_data(key);//paso el binario
    cout<<"LA POS ES:"<<pos<<endl;
    //return false;
    //validar bucket

    Bucket bucket= read_bucket(pos);
    //return false;

    cout<<"BUCKET SACADO"<<endl;
    bucket.allcout();
    cout<<"Primer record"<<endl;
    bucket.records[0].showData();

    cout<<"FIN"<<endl;
    for (int i = 0; i < bucket.count; i++)
    {
        //cout<<"For "<<i<<" "<<record.Vin<<endl;
        record = bucket.records[i];
        //cout<<"For "<<i<<" "<<record.Vin<<endl;
        if (record.Vin == vin_key){
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

void extendible_hash::insert_record(Record record) {
    //primero transformar el vin y sacar su key
    //cout<<"Antes del error"<<record.Vin<<endl;
    int number_key= Vin_to_number(record.Vin);
    string key_record=(BinKeyMod(number_key));//funcion hash
    //ahora tocara validar el indice donde va el key


    int pos=search_pos_data(key_record);//paso el binario
   // cout<<"Insert - pos"<<pos<<endl;
    //validar bucket
    Bucket bucket= read_bucket(pos);
    //cout<<"Bucket sacado"<<endl;
    //cout<<"VIN: "<<bucket.records[0].Vin<<endl;
    if(bucket.count<fb){
        add_Record(pos,bucket,record);
       // cout<<"Agregado"<<endl;
        //cout<<"VIN: "<<bucket.records[0].Vin<<endl;
    }

    else{
        //cout<<"overflow"<<endl;

        string vin= pos_key(key_record);
        Index_hash in=Index_hash(vin.c_str(),hash_table_unorder[vin].first);
       // cout<<"Vin - over: "<<vin<<endl;
       // in.index_cout();
        overflow(in,bucket,record);
    }


}

int extendible_hash::search_pos_data(string key) {
   // cout<<"Search_pos_data"<<endl;
   // cout<<"Binario: "<<key<<endl;
    //validar termino del key
    string val_in_index= pos_key(key);
    int pos=hash_table_unorder[val_in_index].first;
    return pos;
    /*
    //sacar si va a los bucket 0 o los bucket 1
    char last_char = key[key.length() - 1];
    string key_val = std::string(1, last_char) + "'s";
    cout<<"Ultimo digito"<<endl;
    cout<<"Key_val: "<<key_val<<endl;

    int bits_leer=table_cabezas[key_val];
    cout<<"bits_leer: "<<bits_leer<<endl;
    //Index_hash indi;

    //INDICE  1
    //KEY 101
    if(bits_leer<key.length()){
        cout<<"key mayor"<<endl;
        string index_val=key.substr(key.length() - bits_leer);
        cout<<"INDEX VAL "<<index_val<<endl;

        //indice, con posicion en memoria del archivo
        //Index_hash pepe(index_val.c_str(),hash_table_unorder[index_val]);
        return hash_table_unorder[index_val];
    }
        //1
        //1
    else if(bits_leer==key.length()){
        cout<<"key igual"<<endl;

        return hash_table_unorder[key];
    }
        //INDICE 10111
        //KEY 11
    else{
        cout<<"key menor"<<endl;

        //cuando es mayor
        for (const auto& pair : hash_table_unorder){
            //key = pair.first
            string key_pair=pair.first;
            cout<<endl;
            cout<<"Key_pair: "<<key_pair<<endl;
            string validar=key_pair.substr(key_pair.length() - key.length());
            cout<<"Validar: "<<validar<<endl;
            if(validar==key){
                <<"Key -validar: "<<key<<endl;
                return hash_table_unorder[pair.first];
            }
        }
    }
*/
}

Bucket extendible_hash::read_bucket(int pos) {

   // cout<<"Read_bucket"<<endl;
    ifstream file(this->File_data, ios::binary);
    if(!file.is_open()) throw ("No se pudo abrir el archivo");
    Bucket pepelen(fb);
    file.seekg(pos, ios::beg);//fixed length record
    file.read(reinterpret_cast<char*>(&pepelen), bloque.sizeofBucket());
    //cout<<"mostrar bucket leido: "<<endl;
   // pepelen.allcout();
    file.close();

    return pepelen;

/*
    // Ahora que tienes la información del Bucket, puedes usarla para leer los registros
    ifstream fileRecords(this->File_data, ios::binary);
    fileRecords.seekg(pos, ios::beg);

    for (int i = 0; i < bucket.count; i++) {
        Record record;
        fileRecords.read(reinterpret_cast<char*>(&record), sizeof(Record));
        // Ahora 'record' contiene un registro válido que puedes utilizar
        record.showData();
    }

    fileRecords.close();
    */
}

void extendible_hash::add_Record(int pos, Bucket &bucket, Record record) {
    bucket.records[bucket.count] = record;
   // cout<<"COUNT ANTES"<<bucket.count<<endl;
    bucket.count++;
    //cout<<"COUNT DESPUES"<<bucket.count<<endl;

    write_bucket(bucket, pos);
}

void extendible_hash::write_bucket(Bucket &bucket, int pos) {
    ofstream file(this->File_data,  std::ios::binary | std::ios::in | std::ios::out);//abrir en binario y al cambiar algo no elimina lo demas
    if(!file.is_open()) throw ("El archivo no existe");//no hago nada

    file.seekp(pos);//mover puntero al inicio
    file.write(reinterpret_cast<char*>(&bucket), bloque.sizeofBucket());//sobreescribir bucket
    file.close();
/*
    cout<<"Mostrar si el buckets se agrego"<<endl;
    ifstream fileIndexs(this->File_data,  ios::binary);
    Bucket record= read_bucket(pos);
    cout<<"PRUEBA DEL RECORD VIN: "<<record.records[0].Vin<<endl;*/
/*
    Bucket record;
    //cout<<"Pos en el archivo binario"<<endl;
    //cout<< sizeof(Alumno)<<" , "<< sizeof(pos)<<endl;
    fileIndexs.seekg(pos * bloque.sizeofBucket(), ios::beg);//fixed length record
    fileIndexs.read((char*) &record, bloque.sizeofBucket());
    fileIndexs.close();
    cout<<"Bucket metido"<<endl;
    record.allcout();
    cout<<"El metido"<<endl;
    record.records[0].showData();
    cout<<"fin"<<endl;
    cout<<"Cantidad de buckets: "<<endl;
    */

    //cout<<size_data()/ bloque.sizeofBucket()<<endl;

}

void extendible_hash::cargar_indices_a_table() {
    Index_hash index;
    int cantidad_solo_indices=(size_index_hash())-(sizeof(int) * 2);
    //cout<<"solo indices"<<cantidad_solo_indices<<endl;
    int indices_number = cantidad_solo_indices /sizeof(Index_hash);
    //cout<<"SIZEOF INDEX: "<< sizeof(Index_hash)<<endl;
    cout<<"Number indices: "<<indices_number<<endl;
    for(int i=0;i<indices_number;i++){
        index= read_index(i);

        hash_table_unorder[index.key]= make_pair(index.pos_Data,can_index);
        can_index=can_index+1;
    }

}

Index_hash extendible_hash::read_index(int pos) {
    //cout<<"Read_ index"<<endl;
    ifstream file(this->File_hash_index,ios::in | ios::binary);
    if(!file.is_open()) throw ("No se pudo abrir el archivo");
    Index_hash index;
    file.seekg(pos*sizeof(Index_hash)+(sizeof(int) * 2), ios::beg);
    file.read((char *)&index, sizeof(Index_hash));
    file.close();
   // cout<<"Indice leido"<<endl;
   // cout<<"Salir read"<<endl;
    return index;

}

void extendible_hash::cargar_cabezas_a_table() {
    //cout<<"CABEZAS"<<endl;
    fstream fileIndexs(this->File_hash_index,  ios::in |ios::binary);

    int prueba;
    fileIndexs.read((char*)&prueba, sizeof(int));
   // cout << "Cabeza 0: " << prueba << endl;
    table_cabezas["0's"]=prueba;
    fileIndexs.read((char*)&prueba, sizeof(int));
    //cout << "Cabeza 1: " << prueba << endl;
    table_cabezas["1's"]=prueba;
    fileIndexs.close();


}
string extendible_hash::pos_key(string key) {

    //sacar si va a los bucket 0 o los bucket 1
    char last_char = key[key.length() - 1];
    string key_val = std::string(1, last_char) + "'s";
  //  cout<<"Ultimo digito"<<endl;
   // cout<<"Key_val: "<<key<<endl;
   // cout<<"key_ len "<<key.length()<<endl;
    int bits_leer=table_cabezas[key_val];
    //cout<<"bits_leer: "<<bits_leer<<endl;
    //Index_hash indi;

    //INDICE  1
    //KEY 101
    if(bits_leer<=key.length()){
       // cout<<"key mayor"<<endl;
        string index_val=key.substr(key.length() - bits_leer);
       // cout<<"INDEX VAL "<<index_val<<endl;

        //indice, con posicion en memoria del archivo
        //Index_hash pepe(index_val.c_str(),hash_table_unorder[index_val]);
        return index_val;
    }
        //1
        //1
    else if(bits_leer==key.length()){
       // cout<<"key igual"<<endl;

        return key;
    }
        //INDICE 10111
        //KEY 11

    else if(key.length()<bits_leer){
        //cout<<"key menor"<<endl;

        //cuando es mayor
        for (const auto& pair : hash_table_unorder){
            //key = pair.first
            string key_pair=pair.first;
            //cout<<endl;
          //  cout<<"Key_pair: "<<key_pair<<endl;
           // cout<<"Key_pair len"<<key_pair.length()<<endl;
            if(key_pair.length()>key.length()){
                string validar=key_pair.substr(key_pair.length() - key.length());
               // cout<<"Validar: "<<validar<<endl;
                if(validar==key){
                   // cout<<"Key -validar: "<<key<<endl;
                    return pair.first;
                }
            }

        }

    }
    else{
        for (const auto& pair : hash_table_unorder) {

            if(pair.first[pair.first.length()-1]==last_char){
                return pair.first;
            }
        }
    }

}
/*
Index_hash extendible_hash::search_index(string key) {
    cout<<"key en bruto: "<<key<<endl;
    string lugar_index=pos_key(key);//con esto se a que indice va esta key
    cout<<"Key - en index: "<<lugar_index<<endl;
    //Index_hash index;
    if (hash_table_unorder.find(lugar_index) != hash_table_unorder.end())
    {
        cout<<"Esta"<<endl;
        Index_hash index(lugar_index.c_str(),hash_table_unorder[lugar_index].first);

        return index;
    }
        // Index not found
    else
    {
        cout<<"NO Esta"<<endl;

        Index_hash indice("-1",-1);
        // index.pos_log = -1;
        return indice;
    }


}
*/
void extendible_hash::overflow(Index_hash &index, Bucket &bucket, Record &record) {
    if (bucket.d < D) {//en este caso, entonces tocara enlazar nomas :)
        /// Y se modifica el indice,

        string new_index_key=zero_initial+index.key; //can_index=pos index
        string new_index_one=one_initial+index.key;
       // cout<<"zero_initial "<<new_index_key<<endl;
       // cout<<"one_initial "<<new_index_one<<endl;

        cout<<endl;
        cout<<endl;
        cout<<endl;

      //  cout<<"#indices Initial: "<<(size_index_hash()- (sizeof(int)*2))/ sizeof(Index_hash)<<endl;
       // cout<<"can index"<<can_index<<endl;

        //cout<<"Modificar"<<endl;
        modificar_index(new_index_key,index.key,index);
       // cout<<"#indices inital change: "<<(size_index_hash()- (sizeof(int)*2))/ sizeof(Index_hash)<<endl;
        //cout<<"can index"<<can_index<<endl;


        cout<<endl;
        cout<<endl;
        cout<<endl;
        cout<<endl;


        //cout<<"Create new"<<endl;
        Index_hash new_index= create_index(new_index_one);

      //  cout<<"#indices + new: "<<(size_index_hash()- (sizeof(int)*2))/ sizeof(Index_hash)<<endl;
      //  cout<<"can index: "<<can_index<<endl;


       // cout<<"NUEVO INDEX"<<endl;
        //new_index.index_cout();
        //cout<<"FIN"<<endl;
        if(table_cabezas["0's"] < D && table_cabezas["1's"] <D){
            change_cabezas(index);

        }
        //cout<<"New bucket?? "<<endl;
        //Bucket new_bucket = read_bucket(new_index.pos_Data);
        Bucket new_bucket = Bucket(fb);
        //int pos=search_pos_data(index.key);
        //cout<<"Resize"<<endl;
        resize(index,bucket,new_bucket,new_index);
        //cout<<"Resize"<<endl;

        int number_key= Vin_to_number(record.Vin);

        string key_record=(BinKeyMod(number_key));
        string vin= pos_key(key_record);
        if(vin==index.key){
            //cout<<"OLD BUCKET - KEY: "<<index.key<<endl;
            if (bucket.count < fb)
                add_Record(index.pos_Data, bucket, record);
            else
                overflow(index, bucket, record);

        }
        else{
            //cout<<"New BUCKET - KEY: "<<new_index.key<<endl;

            if (new_bucket.count < fb)
                add_Record(new_index.pos_Data, new_bucket, record);
            else
                overflow(new_index, new_bucket, record);
        }

    }
    else{
       // cout<<"Encadenaar ps"<<endl;
        while(bucket.nextBucket!=-1){
            bucket= read_bucket(bucket.nextBucket);
        }
        //la primera vez de todas vendra aqui
        if(bucket.nextBucket==-1){
            int pos_next=size_data();
            bucket.nextBucket=pos_next;//apunta al ultimo donde ahora voy agregar el bucket nuevo
            //sobre escribir el bucket con su next
            write_bucket(bucket, index.pos_Data);//actualizao el bucket en donde al principio queria llenar

            //poner nuevo bucket
            Bucket encadenado_bucket=Bucket(fb);
            encadenado_bucket.records[encadenado_bucket.count]=record;
            encadenado_bucket.count++;
            write_bucket(encadenado_bucket,pos_next);
        }

    }

}




void extendible_hash::modificar_index( string new_key,string old_key,Index_hash &old_index) {
    int pos_index=hash_table_unorder[old_key].second;//saco el numero del key metido, entonces ahora lo
    //cout<<"En archivo index: "<<pos_index<<endl;
    //sobreescribo con una nueva key pero apuntando al mismo bucket
    int pos_data=hash_table_unorder[old_key].first;
   // cout<<pos_index<<" "<<new_key<<" "<<pos_data<<" "<<endl;

    change_index(pos_index, new_key, pos_data, old_index);
    //toca hacer el cambio en el unordered_map
    hash_table_unorder[new_key]= make_pair(hash_table_unorder[old_key].first, hash_table_unorder[old_key].second);
    hash_table_unorder.erase(old_key);
    /// INDEX CAMBIADO EN EL UNORDERED_MAP

}

void extendible_hash::change_index(int pos, string new_key, int pos_data,Index_hash &old_index) {
        //cout<<"Change_index"<<endl;
        ofstream file(this->File_hash_index,std::ios::binary | std::ios::in | std::ios::out);
        if(!file.is_open()) throw ("No se pudo abrir el archivo");
        Index_hash index(new_key.c_str(), pos_data);
        //cout<<"El que voy a meter es: "<<endl;
        //index.index_cout();
        //cout<<"El anterior a es es: "<<endl;
        //old_index.index_cout();
        old_index=index;
        //cout<<"nuevo old: "<<endl;
        //old_index.index_cout();
        file.seekp(pos*sizeof(Index_hash)+(sizeof(int) * 2), ios::beg);
        file.write(reinterpret_cast<char*> (&index), sizeof(Index_hash));
        file.close();
        cout<<endl;
        cout<<endl;
        /*
        cout<<"VALIDAR INDICE CHANGE IN ARCHIVO: "<<endl;
        Index_hash temp=read_index(pos);
        temp.index_cout();
        cout<<"a"<<endl;*/
        //return index;


}

Index_hash extendible_hash::create_index(string key) {
  //  cout<<"Indice : "<<key<<endl;
    //cantidad de registros
   // cout<<"Bucket enlazado: ";
   // cout<<"size data en key: "<<key<<" - "<<size_data()<<endl;
    int pos_Data=size_data();
   // cout<<pos_Data<<endl;
    Index_hash indice= Index_hash((key.c_str()),pos_Data);//guardar indice ejem 0 con la pos 0 del archivo

    hash_table_unorder[key]=make_pair(pos_Data,can_index);

    SaveIndex(indice);
    return indice;
}

void extendible_hash::resize(Index_hash& index_old,Bucket &bucket_old, Bucket &new_bucket, Index_hash &index_new) {
    int bucket_size = 0, new_bucket_size = 0;
    Bucket buc_temp=Bucket(fb);

    for(int i=0;i<bucket_old.count;i++){
        Record record=bucket_old.records[i];
        string key= pos_key(index_old.key);
        //if(key.length()-1
        if(key==index_new.key){
           // cout<<"01"<<endl;
            //cout<<"key - "<<key<<" inde_new.key "<<index_new.key<<endl;
            buc_temp.records[bucket_size]=record;
            bucket_size++;
        }
        else{
           // cout<<"key - "<<key<<" inde_new.key "<<index_new.key<<endl;
           // cout<<"1s1"<<endl;
            new_bucket.records[new_bucket_size] = record;
            new_bucket_size++;
        }
    }
    buc_temp.d=buc_temp.d+1;
    new_bucket.d=buc_temp.d+1;
    buc_temp.count=bucket_size;
    new_bucket.count=new_bucket_size;
    bucket_old=buc_temp;
    write_bucket(bucket_old,index_old.pos_Data);
    write_bucket(new_bucket,index_new.pos_Data);
}

void extendible_hash::change_cabezas(Index_hash indice) {
    //auto valor=indice.key;
    //cout<<"VAL CAHR "<<valor<<endl;
    //cout<<"ANTES WEBADA: "<<to_string(indice.key)<<endl;
    std::string val(indice.key);
  //  cout<<"VAL EN STRING"<<val<<endl;
   // cout<<"ULTIMO "<<val[val.length()-1]<<endl;
    //string ultimo =val[val.length()-1];
    //string val = (indice.key[strlen(indice.key)]);
  //  cout<<"Indice.key"<<indice.key<<endl;
    ofstream file(this->File_hash_index,std::ios::binary | std::ios::in | std::ios::out);
    std::string val_key = val.substr(val.length() - 1) + "'s";
   // cout<<"val_key :"<<val_key<<"key: "<<table_cabezas[val_key]<<endl;
    int prueba=table_cabezas[val_key]+1;
    //cout<<"Ahora con : "<<prueba<<endl;
    if(val=="0"){
       // cout<<"cabeza_0"<<endl;
        file.seekp(0, ios::beg);
        file.write(reinterpret_cast<char*>(&prueba), sizeof(int));
        table_cabezas[val_key]= prueba;
        //table_cabezas.erase(val_key);

    }
    else{
        //cout<<"cabeza_1"<<endl;

        file.seekp(sizeof(int), ios::beg);
        file.write(reinterpret_cast<char*>(&prueba), sizeof(int));
        //table_cabezas.erase(val_key);
        table_cabezas[val_key]= prueba;

    }
    file.close();
   // cout<<"validar change: "<<table_cabezas[val_key]<<endl;
}

bool extendible_hash::delete_Record(string key_record) {

    int number_key= Vin_to_number(key_record);//convertir string a numero
    string key_record_Convert=(BinKeyMod(number_key));//convertir numero a binario con D bits caracteristicos
    string vin= pos_key(key_record_Convert);//Busca en el unordered map la referencia al indice en el archivo
    Index_hash in=Index_hash(vin.c_str(),hash_table_unorder[vin].first);//con esto armo el indice
    //Con el indice tengo acceso al bucket
    Bucket buc= read_bucket(in.pos_Data);
    Record temp;

    //en caso estar en el primer bucket a donde apunta su indice
    for(int i=0;i<buc.count;i++){
        //cout<<"Delete - i "<<i<<endl;
        temp=buc.records[i];
        temp.showData();
        if(temp.Vin==key_record){
            // 2
            //j=3  2 3
            //se borra el 2 pasa por el 3
            for (int j = i + 1; j < buc.count; j++)
            {
                buc.records[j - 1] = buc.records[j];
            }
            buc.count--;
            write_bucket(buc,in.pos_Data);
            return true;
        }
    }
    //en caso de estar enlazado
    while(buc.nextBucket!=1){
        int pos_data_temp=buc.nextBucket;
        buc= read_bucket(pos_data_temp);
        for(int i=0;i<buc.count;i++){
            temp=buc.records[i];
            if(temp.Vin==key_record){
                // 2
                //j=3  2 3
                //se borra el 2 pasa por el 3
                for (int j = i + 1; j < buc.count; j++)
                {
                    buc.records[j - 1] = buc.records[j];
                }
                buc.count--;
                write_bucket(buc,pos_data_temp);
                return true;
            }
        }

    }


    return false;
}

string extendible_hash::verr(string key) {
    string result;
    // Index found
    if (hash_table_unorder.find(key) != hash_table_unorder.end())
    {
        result = key; // Clave encontrada
    }
        // Index not found
    else
    {
        result = ""; // Clave no encontrada
    }
    return result;
}

