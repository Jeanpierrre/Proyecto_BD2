//
// Created by sotom on 6/09/2023.
//

#ifndef EXTENDIBLE_HASHING_INDEX_HASH_H
#define EXTENDIBLE_HASHING_INDEX_HASH_H
#include <cstring>
using namespace std;
#include <iostream>
class Index_hash {
public:
    char key[8];
    int pos_Data;
    int pos_file_index;

    Index_hash(const char* key, int pos_Data){
        strncpy(this->key, key, sizeof(this->key));
        this->key[sizeof(this->key) - 1] = '\0'; // Ensure null-terminated string
        this->pos_Data = pos_Data;
    }
    Index_hash()=default;

    void index_cout(){
        cout<<"Key - "<<key<<" pos_Data - "<< pos_Data<<endl;
    }


};


#endif //EXTENDIBLE_HASHING_INDEX_HASH_H
