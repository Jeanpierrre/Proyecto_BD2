#include <algorithm>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <string>
#include <vector>
#include<windows.h>
#include <iostream>
#include <chrono>
using namespace std;

class Sequential_Record {
    int id;
    int price;
    int year;
    char state[2];
    char VIN[20]; // Primary Key
    char make[10];

    long next = -1;
    char file = 'd';

public:
    Sequential_Record() {
        this->id = 0;
        this->price = 0;
        this->year = 0;
        string s = "n";
        string v = "null";
        string m = "null";
        strcpy(this->state, s.c_str());
        strcpy(this->VIN, v.c_str());
        strcpy(this->make, m.c_str());
    }
    Sequential_Record(int _id, int _price, int _year, string _state, string _VIN, string _make) { 
        this->id = _id;
        this->price = _price;
        this->year = _year;
        strcpy(this->state, _state.c_str());
        strcpy(this->VIN, _VIN.c_str());
        strcpy(this->make, _make.c_str());

        this->next = -1;
        char file = 'd';
    }

    bool operator<(const Sequential_Record &a) { return string(this->VIN) < string(a.VIN); }

    void setData() {
        int _id;
        int _price;
        int _year;
        string _state;
        string _VIN;
        string _make;

        cout << "Id: ";
        cin >> _id;
        cout << "Price: ";
        cin >> _price;
        cout << "Year: ";
        cin >> _year;
        cout << "State: ";
        cin >> _state;
        cout << "VIN: ";
        cin >> _VIN;
        cout << "Make: ";
        cin >> _make;

        this->id = _id;
        this->price = _price;
        this->year = _year;
        strcpy(this->state, _state.c_str());
        strcpy(this->VIN, _VIN.c_str());
        strcpy(this->make, _make.c_str());

        this->next = -1;
        char file = 'd';
    }

    void showData() {
        cout << "Id: " << id << endl;
        cout << "Price: " << price << endl;
        cout << "Year: " << year << endl;
        cout << "State: " << string(state, sizeof(state)) << endl;
        cout << "VIN: " << string(VIN, sizeof(VIN)) << endl;
        cout << "Make: " << string(make, sizeof(make)) << endl;
        cout << endl;
    }

    void setNext(long n, char f) {
        next = n;
        file = f;
    }
    long getNext()
    {
        return next;
    }
    char getFile()
    {
        return file;
    }
    string getKey() {
        return string(VIN);
    }

    void showRecord(int cont){
        if(cont == 1) {
            cout << "Header" << endl;
        }
        cout << setw(5) << id << setw(10) << price << setw(8) << year << setw(8) << state << setw(12) << VIN << setw(10) << make << endl;
    }
};

template <typename T>
bool comparator(T &a, T &b) { return a < b; }

// ------------------------------------------------------------------------------------

class Sequential {
private:
    string data_file_name;
    string aux_file_name;
    long sizeData = 0;
    long sizeAux = 0;

    const long sizeRecord = sizeof(Sequential_Record);

    long fileSize (string name) {
        fstream temp(name, ios::binary | ios::in | ios::out);
        temp.seekg(0, ios::beg);
        auto start = temp.tellg();
        temp.seekg(0, ios::end);
        auto end = temp.tellg();
        temp.close();
        return (long)(end - start) / sizeof(Sequential_Record);
    }

    void rebuild() {
        string newName = "temp.bin";

        fstream data(data_file_name, ios::binary | ios::in | ios::out);
        if (!data.is_open()) cerr << "Error - rebuild" << endl;
        else {
            fstream aux_data(aux_file_name, ios::binary | ios::in | ios::out);
            if (!aux_data.is_open()) cerr << "Error - rebuild" << endl;
            else {
                ofstream temp(newName, ofstream::out | ofstream::trunc);
                temp.close();

                fstream temp_data(newName, ios::binary | ios::in | ios::out);
                if (!temp_data.is_open()) cerr << "Error - rebuild" << endl;
                else {
                    Sequential_Record rx;
                    data.seekg(0, ios::beg);
                    data.read((char *)&rx, sizeof(Sequential_Record));

                    int next = 0;
                    char file = 'd';
                    int fn_pos = 0;

                    sizeAux = 0;
                    sizeData = 0;
                    do {
                        data.seekg(next, ios::beg);
                        data.read((char *)&rx, sizeof(Sequential_Record));
                        next = rx.getNext();
                        file = rx.getFile();

                        rx.setNext(fn_pos + sizeRecord, 'd');
                        temp_data.seekp(fn_pos, ios::beg);
                        temp_data.write((char *)&rx, sizeof(Sequential_Record));
                        sizeData++;
                        fn_pos += sizeRecord;

                        if (file == 'a') {
                            vector<Sequential_Record> in_aux;
                            do {
                                temp_data.seekg(next, ios::beg);
                                temp_data.read((char *)&rx, sizeof(Sequential_Record));
                                in_aux.push_back(rx);
                                next = rx.getNext();
                                file = rx.getFile();
                            } while (file == 'a' && next != -1);
                            sort(in_aux.begin(), in_aux.end(), comparator<Sequential_Record>);
                            for (Sequential_Record r : in_aux) {
                                r.setNext(fn_pos + sizeRecord, 'd');
                                temp_data.seekp(fn_pos, ios::beg);
                                temp_data.write((char *)&r, sizeof(Sequential_Record));
                                sizeData++;
                                fn_pos += sizeRecord;
                            }
                        }
                    } while (next != -1);

                    temp_data.seekg(sizeRecord * (sizeData - 1), ios::beg);
                    temp_data.read((char *)&rx, sizeof(Sequential_Record));
                    rx.setNext(-1, 'd');

                    temp_data.seekp(sizeRecord * (sizeData - 1), ios::beg);
                    temp_data.write((char *)&rx, sizeof(Sequential_Record));
                    temp_data.close();
                }
                aux_data.close();
            }
            data.close();
        }

        ofstream aux(aux_file_name, ofstream::out | ofstream::trunc);
        aux.close();

        char fileNameArr[data_file_name.size() + 1];
        strcpy(fileNameArr, data_file_name.c_str());

        char newNameArr[newName.size() + 1];
        strcpy(newNameArr, newName.c_str());
    }

    long binarySearch(string key) {
        long res = 0;
        fstream data (data_file_name, ios::in | ios::out | ios::binary);
        if (!data.is_open()) cerr << "Error - binarySearch" << endl;
        else {
            Sequential_Record tmp;
            data.seekg(0, ios::beg);
            data.read((char*) &tmp, sizeof(Sequential_Record));
            if (key <= tmp.getKey())
            {
                data.close();
                return 0;
            }
            data.seekg((sizeData - 1) * sizeRecord, ios::beg);
            data.read((char*) &tmp, sizeof(Sequential_Record));
            if (key >= tmp.getKey())
            {
                data.close();
                return (sizeData - 1) * sizeRecord;
            }

            int lo = 0, hi = sizeData - 1, mi;
            while (1)
            {
                mi = (lo + hi) / 2;
                if (mi == lo)
                    break;
                data.seekg(mi * sizeRecord, ios::beg);
                data.read((char*) &tmp, sizeof(Sequential_Record));
                if (key == tmp.getKey())
                    break;
                else if (key < tmp.getKey())
                    hi = mi;
                else
                    lo = mi;
            }
            data.seekg(mi * sizeRecord, ios::beg);
            data.read((char*) &tmp, sizeof(Sequential_Record));
            while (tmp.getNext() == -2)
            {
                mi--;
                data.seekg(mi * sizeRecord, ios::beg);
                data.read((char*) &tmp, sizeof(Sequential_Record));
            }
            data.close();
            return mi * sizeRecord;
        }
        return res;
    }

public:
    Sequential(string name) {
        this->data_file_name = name;
        this->aux_file_name = "aux_" + name;

        ofstream ofs (data_file_name, std::ofstream::out | std::ofstream::trunc);
        ofs.close();
        ofstream a_ofs (aux_file_name, std::ofstream::out | std::ofstream::trunc);
        a_ofs.close();

        sizeData = fileSize(data_file_name);
        sizeAux = fileSize(aux_file_name);
    }

    void add(Sequential_Record &record) {
        record.setNext(-1, 'd');
        if (sizeData == 0) {
            fstream data(this->data_file_name, ios::binary | ios::in | ios::out);
            if (!data.is_open()) cerr << "Error - Add.1" << endl;
            else {
                data.seekp(0, ios::beg);
                data.write((char *)&record, sizeof(Sequential_Record));
                sizeData++;
                data.close();
                return;
            }
        }

        long long pos = binarySearch(record.getKey());
        
        fstream data(this->data_file_name, ios::binary | ios::in | ios::out);
        if (!data.is_open()) cerr << "Error - Add.2" << endl;
        else {
            Sequential_Record tmp;
            data.seekg(pos);
            data.read((char *)&tmp, sizeof(Sequential_Record));

            if (pos == 0 && record.getKey() < tmp.getKey()) {
                if (tmp.getNext() == -1) {
                    long finalPos = sizeRecord;
                    data.seekp(finalPos, ios::end);
                    record.setNext(finalPos, 'd');
                    data.write((char *)&tmp, sizeof(Sequential_Record));
                    data.seekp(pos, ios::beg);
                    data.write((char *)&record, sizeof(Sequential_Record));
                    
                    sizeData++;
                } else {
                    fstream a_data(this->aux_file_name, ios::binary | ios::in | ios::out);
                    if (!a_data.is_open()) cerr << "Error - Add.3" << endl;
                    else {
                        a_data.seekp(0, ios::end);
                        auto finalPosition = a_data.tellp();
                        record.setNext(finalPosition, 'a');

                        a_data.write((char *)&tmp, sizeof(Sequential_Record));

                        data.seekp(pos, ios::beg);
                        data.write((char *)&record, sizeof(Sequential_Record));

                        sizeAux++;
                        a_data.close();

                        if (sizeAux == 10) {
                            data.close();
                            rebuild();
                        }
                    }
                }
            } else {
                if (tmp.getNext() == -1) {
                    data.seekp(0, ios::end);
                    auto finalPosition = data.tellp();
                    data.write((char *)&record, sizeof(Sequential_Record));

                    tmp.setNext(finalPosition, 'd');
                    data.seekp(pos);
                    data.write((char *)&tmp, sizeof(Sequential_Record));

                    tmp.showData();

                    sizeData++;
                } else {
                    Sequential_Record tmpNext;
                    data.seekg(pos + sizeData);
                    data.write((char *)&tmpNext, sizeof(Sequential_Record));
                    if (tmpNext.getNext() == -2) {
                        record.setNext(tmp.getNext(), tmp.getNext());
                        tmp.setNext(pos + sizeData, 'd');
                        data.seekp(pos);
                        data.write((char *)&tmp, sizeof(Sequential_Record));
                        data.seekp(pos + sizeData);
                        data.write((char *)&record, sizeof(Sequential_Record));
                        sizeData++;
                    } else {
                        fstream a_data(this->aux_file_name, ios::binary | ios::in | ios::out);
                        if (!a_data.is_open()) cerr << "Add - Error .4" << endl;
                        else {
                            a_data.seekp(0, ios::end);
                            auto finalPosition = a_data.tellp();
                            record.setNext(tmp.getNext(), tmp.getNext());
                            tmp.setNext(finalPosition, 'a');
                            a_data.write((char *)&record, sizeof(Sequential_Record));
                            data.seekp(pos, ios::beg);
                            data.write((char *)&tmp, sizeof(Sequential_Record));
                            sizeAux++;
                            a_data.close();

                            if (sizeAux == 10) {
                                data.close();
                                rebuild();
                            }
                        }
                    }
                }
            }
            data.close();
        }
    }

    Sequential_Record search(string key) {
        Sequential_Record empty;
    
        return empty;
    }

    vector<Sequential_Record> search(string begin_key, string end_key) {
        vector<Sequential_Record> result;
        
        return result;
    }

    bool erase(string key) {
        return false;
    }
    
    void showRecords() {
        int cont = 1;
        fstream data(data_file_name, ios::binary | ios::in | ios::out);
        if (!data.is_open()) cerr << "Error - showRecord" << endl;
        else {
            fstream a_data(aux_file_name, ios::binary | ios::in | ios::out);
            if (!a_data.is_open()) cerr << "Error - showRecord" << endl;
            else {
                Sequential_Record tmp;
                data.seekg(0, ios::beg);
                data.read((char*) &tmp, sizeof(Sequential_Record));
                long next = tmp.getNext();
                char file = tmp.getFile();
                
                tmp.showRecord(cont++);

                while (next != -1) {
                    if (file == 'd') {
                        data.seekg(next, ios::beg);
                        data.read((char*) &tmp, sizeof(Sequential_Record)); 
                    }
                    else if (file == 'a') {
                        a_data.seekg(next, ios::beg);
                        a_data.read((char*) &tmp, sizeof(Sequential_Record));
                    }
                    else {
                        data.close(); a_data.close();
                        return;
                    }
                    tmp.showRecord(cont++);
                    next = tmp.getNext();
                    file = tmp.getFile();
                }
                a_data.close();
            }
            data.close();
        }
    }
};

// --------------------------------------------------------------------------------------------

void read_dataset_count(string filename, Sequential &ext) {
    fstream fio (filename, ios::in | ios::out | ios::binary);
    if (!fio.is_open()) {
        cout << "Error en archivo CSV" << endl;
        exit(-1);
    }
    
    string line;
    char delimitador = ',';

    // Lee la primera línea para ignorar los encabezados
    getline(fio, line);
    vector<Sequential_Record> out;

    while (getline(fio, line)) {
        int id, price, year = 0;

        stringstream stream(line);
        string ID, PRICE, YEAR, MILAGE, CITY, STATE, VIN, MAKE, MODEL;
        getline(stream, ID, delimitador);
        getline(stream, PRICE, delimitador);
        getline(stream, YEAR, delimitador);
        getline(stream, MILAGE, delimitador);
        getline(stream, CITY, delimitador);
        getline(stream, STATE, delimitador);
        getline(stream, VIN, delimitador);
        getline(stream, MAKE, delimitador);
        getline(stream, MODEL, delimitador);

        stringstream _id(ID);
        stringstream _price(PRICE);
        stringstream _year(YEAR);

        _id >> id;
        _price >> price;
        _year >> year;

        Sequential_Record record(id, price, year, STATE, VIN, MAKE);
        out.push_back(record);
    }
    sort(out.begin(), out.end(), comparator<Sequential_Record>);
    for (Sequential_Record record : out) {
        ext.add(record);
    }
}