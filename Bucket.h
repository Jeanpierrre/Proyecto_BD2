//
// Created by sotom on 5/09/2023.
//

#ifndef EXTENDIBLE_HASHING_BUCKET_H
#define EXTENDIBLE_HASHING_BUCKET_H
#include "Record.h"
const int fb = 8;
class Bucket {

public:
    Record records[fb];
    int capacity;
    int nextBucket;
    int count;
    int d;//profundidad local

    Bucket() = default;
    Bucket(int fbs){
        this->capacity = fbs;//cantidad que lleva
        this->nextBucket = -1;//si ya no puedo agregar mas y apunta a otro
        this->count = 0;//validar la profundidad
        //this->records = new Record[fb];
        this->d=1;//al iniciar
    }

    int sizeofBucket(){ //igual oara todos los bloques aunque este vacio
        return sizeof(int)*4+sizeof(Record)*fb;
    }

    void allcout(){
        //cout<<records[0]<<endl;
        cout << "Capacity: " << this->capacity << endl;
        cout << "Next Bucket: " << this->nextBucket << endl;
        cout << "Count: " << this->count << endl;
        cout << "Profundidad Local (d): " << this->d << endl;

    }


};


#endif //EXTENDIBLE_HASHING_BUCKET_H
