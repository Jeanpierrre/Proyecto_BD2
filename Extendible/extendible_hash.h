//
// Created by sotom on 6/09/2023.
//

#ifndef EXTENDIBLE_HASHING_EXTENDIBLE_HASH_H
#define EXTENDIBLE_HASHING_EXTENDIBLE_HASH_H

#include <fstream>
#include <cstdint>
#include "Index_hash.h"
#include "Bucket.h"
#include <bitset>
#include <unordered_map>
using namespace std;
class extendible_hash {
private:
    string zero_initial="0";
    string one_initial="1";
    Bucket bloque;

    string File_hash_index;
    string File_data;
    int D;//profundidad global
    int can_index=0;
    int DD;
    void initialize_hash_structure(string key, int pos);//crear indice

    Index_hash create_index(string key);
    void modificar_index( string new_key,string old_key,Index_hash &old_index);

    void Iniciar_Buckets_iniciales();
    void Index_count();

    int size_index_hash();

    void SaveIndex(Index_hash indice);
    void change_index(int pos, string old_key, int pos_data,Index_hash &old_index);
    void insert_bucket(Bucket & buc);

    void read_indices();
    Index_hash search_key_to_Index();
    Bucket read_bucket(int pos);

    void add_Record(int pos, Bucket &bucket,Record record);

    string pos_key(string key);

    unsigned long long Vin_to_number(string key);
    std::unordered_map<std::string, std::pair<int, int>> hash_table_unorder;
    //std::unordered_map<string, int> hash_table_unorder;//cargar los indices a tabla
    std::unordered_map<string, int> table_cabezas;//cargar los indices a tabla
    //void read_Buckets();
    int search_pos_data(string key);

    void write_bucket(Bucket &bucket, int pos);

    //Index_hash search_index(string key);

    void cargar_indices_a_table();
    Index_hash read_index(int pos);
    void cargar_cabezas_a_table();
    string verr(string key);
    void overflow(Index_hash &index, Bucket &bucket, Record &record);
    void indice_profundidad(string key, int d);
    void change_cabezas(Index_hash indice);
public:
    int size_data();
    extendible_hash(int D,string index, string data);
    bool fileExists(const std::string &fileName) {
        std::ifstream file(fileName);
        return file.good();
    }
    void read_Buckets();
    void resize(Index_hash& index_old,Bucket &bucket_old, Bucket &new_bucket, Index_hash &index_new);
    void insert_record(Record record);
    bool search_record(string vin );
    int hash_function(int key);
    Index_hash search_index(string key);
    std::string BinKeyMod(int key) {
        long long result = key % (1 << D);
        int numBits = 0;

        // Determine how many bits are needed to represent result
        while (result > 0) {
            numBits++;
            result >>= 1; // Shift right to check the next bit
        }

        if (numBits == 0) {
            numBits = 1; // At least one bit is needed to represent 0
        }

        // Create a dynamically allocated array to store the binary representation
        // of key with the necessary number of bits.
        bool* binaryArray = new bool[numBits];

        // Convert key to binary and store it in the array
        result = key % (1 << D);
        for (int i = numBits - 1; i >= 0; i--) {
            binaryArray[i] = result & 1;
            result >>= 1;
        }

        // Convert the binary array to a string
        std::string binaryString;
        for (int i = 0; i < numBits; i++) {
            binaryString += binaryArray[i] ? '1' : '0';
        }

        // Free the dynamically allocated memory
        delete[] binaryArray;

        return binaryString;
    }


    bool delete_Record(string key_record);
};


#endif //EXTENDIBLE_HASHING_EXTENDIBLE_HASH_H
