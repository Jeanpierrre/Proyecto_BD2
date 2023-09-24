#include <algorithm>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct Record_Sequential {
  int id;
  int price;
  int year;
  char state[2];
  char VIN[20]; // Primary Key
  char make[10];

  int next = -1;
  char file = 'd';

  Record_Sequential() {}
  Record_Sequential(int _id, int _price, int _year, string _state, string _VIN,
                    string _make) {
    this->id = _id;
    this->price = _price;
    this->year = _year;
    strcpy(this->state, _state.c_str());
    strcpy(this->VIN, _VIN.c_str());
    strcpy(this->make, _make.c_str());

    this->next = -1;
    this->file = 'd';
  }

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

  void setNext(int n, char f) {
    next = n;
    file = f;
  }

  // Convierte el string en un número largo
  unsigned long long hash_function(const std::string &key) {
    unsigned long long key_as_long = 0;
    for (char c : key) {
      key_as_long = key_as_long * 256 + static_cast<unsigned long long>(c);
    }
    // cout << "->" << key_as_long << "<-" << endl;
    return key_as_long;
  }

  long long getKey() { return hash_function(this->VIN); }
};

class Sequential_File {
  string data_file_name;
  string aux_file_name;
  int dataSize = 0;
  int auxSize = 0;

  const int recordSize = sizeof(Record_Sequential);

  struct comparator {
    inline bool operator()(const Record_Sequential &registro1,
                           const Record_Sequential &registro2) {
      if (registro1.id < registro2.id)
        return true;
      return false;
    }
  };

  // Private functions

  int sizeFile(string name) {
    fstream temp(name, ios::binary | ios::in | ios::out);
    temp.seekg(0, ios::beg);
    auto start = temp.tellg();
    temp.seekg(0, ios::end);
    auto end = temp.tellg();
    temp.close();
    return (end - start) / sizeof(Record_Sequential);
  }

  int binarySearch(long long key) {
    int out = 0;
    fstream file(this->data_file_name, ios::binary | ios::in | ios::out);
    if (file.is_open()) {
      Record_Sequential temp;
      file.seekg(0, ios::beg);
      file.read((char *)&temp, sizeof(Record_Sequential));
      if (key <= temp.getKey()) {
        file.close();
        return 0;
      }

      file.seekg((dataSize - 1) * recordSize, ios::beg);
      file.read((char *)&temp, sizeof(Record_Sequential));
      if (key >= temp.getKey()) {
        file.close();
        return (dataSize - 1) * recordSize;
      }

      int un = 0;
      int top = dataSize - 1;
      int mid;
      while (un <= top) {
        mid = (top + un) / 2;
        if (mid == un)
          break;

        file.seekg(mid * recordSize, ios::beg);
        file.read((char *)&temp, sizeof(Record_Sequential));

        if (key == temp.getKey())
          break;
        else if (key < temp.getKey())
          top = mid;
        else
          un = mid;
      }
      file.seekg(mid * recordSize, ios::beg);
      file.read((char *)&temp, sizeof(Record_Sequential));
      while (temp.next == -2) {
        mid--;
        file.seekg(mid * recordSize, ios::beg);
        file.read((char *)&temp, sizeof(Record_Sequential));
      }
      file.close();
      return mid * recordSize;
    } else
      cout << "Error - binary search" << endl;
    return out;
  }

  void rebuild() {
    string newName = "temp.bin";

    fstream data(data_file_name, ios::binary | ios::in | ios::out);
    if (data.is_open()) {
      fstream aux_data(aux_file_name, ios::binary | ios::in | ios::out);
      if (aux_data.is_open()) {
        ofstream temp(newName, ofstream::out | ofstream::trunc);
        temp.close();

        fstream temp_data(newName, ios::binary | ios::in | ios::out);
        if (temp_data.is_open()) {
          Record_Sequential rx;
          data.seekg(0, ios::beg);
          data.read((char *)&rx, sizeof(Record_Sequential));

          int next = 0;
          char file = 'd';
          int fn_pos = 0;

          auxSize = 0;
          dataSize = 0;
          do {
            data.seekg(next, ios::beg);
            data.read((char *)&rx, sizeof(Record_Sequential));
            next = rx.next;
            file = rx.file;

            rx.setNext(fn_pos + recordSize, 'd');
            temp_data.seekp(fn_pos, ios::beg);
            temp_data.write((char *)&rx, sizeof(Record_Sequential));
            dataSize++;
            fn_pos += recordSize;

            if (file == 'a') {
              vector<Record_Sequential> in_aux;
              do {
                temp_data.seekg(next, ios::beg);
                temp_data.read((char *)&rx, sizeof(Record_Sequential));
                in_aux.push_back(rx);
                next = rx.next;
                file = rx.file;
              } while (file == 'a' && next != -1);
              sort(in_aux.begin(), in_aux.end(), comparator());
              for (Record_Sequential r : in_aux) {
                r.setNext(fn_pos + recordSize, 'd');
                temp_data.seekp(fn_pos, ios::beg);
                temp_data.write((char *)&r, sizeof(Record_Sequential));
                dataSize++;
                fn_pos += recordSize;
              }
            }
          } while (next != -1);

          temp_data.seekg(recordSize * (dataSize - 1), ios::beg);
          temp_data.read((char *)&rx, sizeof(Record_Sequential));
          rx.setNext(-1, 'd');

          temp_data.seekp(recordSize * (dataSize - 1), ios::beg);
          temp_data.write((char *)&rx, sizeof(Record_Sequential));
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

    if (remove(fileNameArr) != 0) perror("Error - ref");
    if (rename(newNameArr, fileNameArr) != 0) perror("Error - ref 2");
  }

public:
  Sequential_File(string name) {
    this->data_file_name = name;
    this->aux_file_name = "aux_" + name;

    ofstream data(data_file_name, ios::binary);
    data.close();
    ofstream a_data(aux_file_name, ios::binary);
    a_data.close();

    dataSize = sizeFile(data_file_name);
    auxSize = sizeFile(aux_file_name);
  }

  void showAll() {
    fstream data(this->data_file_name, ios::binary | ios::in | ios::out);
    if (data.is_open()) {
      fstream a_data(this->aux_file_name, ios::binary | ios::in | ios::out);
      if (a_data.is_open()) {
        Record_Sequential temp;
        data.seekg(0, ios::beg);
        data.read((char *)&temp, sizeof(Record_Sequential));

        int next = temp.next;
        char file = temp.file;

        cout << "data: ";
        temp.showData();

        while (next != -1) {
          if (file == 'd') {
            data.seekg(next, ios::beg);
            data.read((char *)&temp, sizeof(Record_Sequential));
            cout << "data: ";
          } else if (file == 'a') {
            a_data.seekg(next, ios::beg);
            a_data.read((char *)&temp, sizeof(Record_Sequential));
            cout << "aux: ";
          } else {
            cout << "Algo fallo" << endl;
            data.close();
            a_data.close();
            return;
          }
          temp.showData();
          next = temp.next;
          file = temp.file;
        }
        a_data.close();
        cout << "\n\n----- File Size -----\n\n";
        cout << "SizeData: " << dataSize << "\n";
        cout << "SizeAux: " << auxSize << "\n";
      } else {
        cout << "Error - x";
        data.close();
      }
    } else {
      cout << "Error - xx";
    }
  }

  void add(Record_Sequential record) {
    record.setNext(-1, 'd');
    if (dataSize == 0) {
      fstream data(this->data_file_name, ios::binary | ios::in | ios::out);
      if (data.is_open()) {
        data.seekp(0, ios::beg);
        data.write((char *)&record, sizeof(Record_Sequential));
        dataSize++;
        data.close();
        return;
      } else
        cout << "SequentialError al abrir archivo data en add, sizeData = 0\n";
    }

    long pos = binarySearch(record.getKey());
    fstream data(this->data_file_name, ios::binary | ios::in | ios::out);
    if (data.is_open()) {
      Record_Sequential tmp;
      data.seekg(pos);
      data.read((char *)&tmp, sizeof(Record_Sequential));

      if (pos == 0 && record.getKey() < tmp.getKey()) {
        if (tmp.next == -1) {
          long finalPos = recordSize;
          data.seekp(finalPos, ios::end);
          record.setNext(finalPos, 'd');
          data.write((char *)&tmp, sizeof(Record_Sequential));
          data.seekp(pos, ios::beg);
          data.write((char *)&record, sizeof(Record_Sequential));
          dataSize++;
        } else {
          fstream a_data(this->aux_file_name, ios::binary | ios::in | ios::out);
          if (a_data.is_open()) {
            a_data.seekp(0, ios::end);
            auto finalPosition = a_data.tellp();
            record.setNext(finalPosition, 'a');
            a_data.write((char *)&tmp, sizeof(Record_Sequential));

            data.seekp(pos, ios::beg);
            data.write((char *)&record, sizeof(Record_Sequential));

            auxSize++;
            a_data.close();

            if (auxSize == 10) {
              data.close();
              rebuild();
            }
          } else
            cout << "SequentialError al abrir archivo aux en add record < "
                    "pos0\n";
        }
      } else {
        if (tmp.next == -1) {
          data.seekp(0, ios::end);
          auto finalPosition = data.tellp();
          data.write((char *)&record, sizeof(Record_Sequential));

          tmp.setNext(finalPosition, 'd');
          data.seekp(pos);
          data.write((char *)&tmp, sizeof(Record_Sequential));

          dataSize++;
        } else {
          Record_Sequential tmpNext;
          data.seekg(pos + dataSize);
          data.write((char *)&tmpNext, sizeof(Record_Sequential));
          if (tmpNext.next == -2) {
            record.setNext(tmp.next, tmp.next);
            tmp.setNext(pos + dataSize, 'd');
            data.seekp(pos);
            data.write((char *)&tmp, sizeof(Record_Sequential));
            data.seekp(pos + dataSize);
            data.write((char *)&record, sizeof(Record_Sequential));
            dataSize++;
          } else {
            fstream a_data(this->aux_file_name,
                           ios::binary | ios::in | ios::out);
            if (a_data.is_open()) {
              a_data.seekp(0, ios::end);
              auto finalPosition = a_data.tellp();
              record.setNext(tmp.next, tmp.next);
              tmp.setNext(finalPosition, 'a');
              a_data.write((char *)&record, sizeof(Record_Sequential));
              data.seekp(pos, ios::beg);
              data.write((char *)&tmp, sizeof(Record_Sequential));
              auxSize++;
              a_data.close();

              if (auxSize == 10) {
                data.close();
                rebuild();
              }
            } else
              cout << "SequentialError al abrir archivo aux en add\n";
          }
        }
      }
      data.close();
    } else
      cout << "SequentialError al abrir archivo data en add\n";
  }

  Record_Sequential search(int key) {
    Record_Sequential empty;
    Record_Sequential out;
    int pos = binarySearch(key);
    fstream data(data_file_name, ios::binary | ios::in | ios::out);
    if (data.is_open()) {
      data.seekg(pos, ios::beg);
      data.write((char *)&out, sizeof(Record_Sequential));
      if (out.getKey() == key) {
        data.close();
        if (out.getKey() == -2) {
          return empty;
        } else
          return out;
      }
      if (out.next == -1) {
        data.close();
        return empty;
      }

      fstream aux_data(aux_file_name, ios::binary | ios::in | ios::out);
      if (aux_data.is_open()) {
        int a_pos = out.next;
        char a_file = out.file;
        while (a_file == 'a') {
          aux_data.seekg(a_pos, ios::beg);
          aux_data.write((char *)&out, sizeof(Record_Sequential));
          if (out.getKey() == key) {
            data.close();
            aux_data.close();
            if (out.next == -2) {
              return empty;
            } else
              return out;
          }
          a_pos = out.next;
          a_file = out.file;
        }
        aux_data.close();
      }
      data.close();
    }
    return empty;
  }

  vector<Record_Sequential> rangeSearch(int begin_key, int end_key) {
    vector<Record_Sequential> out;
    return out;
  }

  bool remove_record(Record_Sequential record) {
    // ToDo - Update
    return true;
  }
};

void read_dataset_count(string filename, int i, Sequential_File &ext) {
  int cont = 0;
  fstream fio;

  fio.open(filename, ios::in | ios::out | ios::binary);
  string line;

  // Lee la primera línea para ignorar los encabezados
  getline(fio, line);

  while (getline(fio, line)) {
    // cout<<"count: "<<cont<<endl;
    string str;
    stringstream ss(line);
    vector<string> vec;

    while (getline(ss, str, ',')) {
      vec.push_back(str);
    }

    // Verifica si hay suficientes campos
    if (vec.size() >= 9) {
      Record_Sequential record;
      record.id = stoi(vec[0]);
      record.price = stoi(vec[1]);
      record.year = stoi(vec[2]);
      strcpy(record.VIN, vec[6].c_str());
      strcpy(record.state, vec[5].c_str());
      strcpy(record.make, vec[7].c_str());

      if (cont < i) {
        ext.add(record);
        // record.showData();
        cont++;
      } else {
        return;
      }
    }
  }
}

int Menu_SequentialFile() {
  int op;
  while (true) {
    cout << endl << "///// Menu ///// " << endl;
    cout << "1. Load .csv" << endl;
    cout << "2. Add record" << endl;
    cout << "3. Remove record" << endl;
    cout << "4. Search record" << endl;
    cout << "5. Search by range" << endl;
    cout << "6. Show all records" << endl;
    cout << "0. Exit" << endl;

    cout << "--> ";
    cin >> op;

    if (op == 0 || op == 1 || op == 2 || op == 3 || op == 4 || op == 5 ||
        op == 6) {
      return op;
      break;
    } else {
      cout << "Not valid option" << endl;
      system("clear");
    }
  }
}
