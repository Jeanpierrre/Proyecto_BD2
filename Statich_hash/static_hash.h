//
// Created by sotom on 24/09/2023.
//

#ifndef STATIC_HASH_STATIC_HASH_H
#define STATIC_HASH_STATIC_HASH_H

#include <fstream>
#include <cstdint>
using namespace std;
#include "Bucket.h"
#include <bitset>
#include <unordered_map>
class static_hash {
private:
    string data_file;
    int M;
    void initialize_hash_structure(int M);
    int size_data();

    void add_Record(int pos, Bucket & bucket, Record record);
    void write_bucket(Bucket &bucket, int pos);

public:
    Bucket read_bucket(int pos);
    static_hash(string dataFile, int m);
    void insert_record(Record record);
    bool search_record(string vin_key);

    unsigned long long Vin_to_number(string key) {

        // Convierte el string en un número largo
        unsigned long long key_as_long = 0;

        for (char c : key) {
            key_as_long = key_as_long * 256 + static_cast<unsigned long long>(c);
        }

        return (key_as_long);  // Convierte a int antes de devolverlo

    }

    int BinKeyMod(int key) {
        long long result = key % M;
        return result;
    }

    void overflow(int pos,Bucket bucket, Record record);

    void resize(Bucket &bucket, Bucket &bucket1, int pos);

    void write_in_pos(Bucket bucket, int pos);

    Bucket read_bucket_pos(int pos);

    bool delete_Record(string key_record);

    [[maybe_unused]] void read_Buckets();
};


#endif //STATIC_HASH_STATIC_HASH_H
