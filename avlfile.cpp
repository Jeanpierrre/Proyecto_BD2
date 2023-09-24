#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;
unsigned long long hash_function(const std::string& key) {
    // Convierte el string en un número largo
    unsigned long long key_as_long = 0;

    for (char c : key) {
        key_as_long = key_as_long * 256 + static_cast<unsigned long long>(c);
    }

    return key_as_long;
}
struct Record
{
    int id;
    int Price;
    int Year;
    char Mileage[20];
    char city[20];
    char State[20];
    char Vin[18];
    char Make[50];
    char model[50];

    long left=-1, right=-1;
    int height=-1;

    void showData() {
        cout<<id<<" "<<Price<<" "<<Year<<" "<<Mileage<<" "<<city<<" "<<State<<" "<<Vin<<" "<<Make<<" "<<model<<" i: "<<left<<" d: "<<right<<" height: "<<height<<endl;
    }
    
};

class AVLFile
{
private:
    string filename;
    long pos_root;
public:
    bool fileExists(const std::string &fileName) {
        std::ifstream file(fileName);
        return file.good();
    }
    AVLFile(string filename){
        this->filename = filename;
        if(!fileExists(filename)){
            cout<<"Archivo creado"<<endl;
            this->pos_root = -1;
            cout<<"Creada"<<endl;
        }
        else{
            cout<<"Archivo existente "<<endl; //Si el archivo existe la raiz siempre sera 0
            this->pos_root = 0;
        }
    }
    
    Record find(int key){
        fstream file(filename,ios::binary | ios::app | ios::in | ios::out);
        string line;
        getline(file, line);
        Record record = find(pos_root, key, file);
        file.close();
        return record;
    }
    void insert(Record record){
        fstream file(filename, ios::binary | ios::app | ios::in | ios::out);
        //if(!file.is_open()) throw ("No se pudo abrir el archivo");
        
    
        insert(pos_root, record, file);
        file.close();
    }

    vector<Record> inorder(){
        //cout<<"entra"<<endl;
        fstream file(filename, ios::binary | ios::app | ios::in | ios::out);
        string line;
        getline(file, line);
        vector<Record> result;
        inorder(pos_root, result,file);
        //cout<<"SALE"<<endl;
        file.close();
        return result;
    }

    int height(){
        fstream file(filename, ios::binary | ios::app | ios::in | ios::out);
        int result = height(pos_root, file);
        file.close();
        return result;
    }
    
private:
    Record find(long pos_node, int key, fstream &file){
        if(pos_node == -1) throw "Record not found";

		file.seekg(pos_node, ios::beg);
		Record record;
		file.read((char*)&record, sizeof(Record));
        //record.showData();
        if(hash_function(record.Vin) == key){
			return record;}
		else if(hash_function(record.Vin) > key){
			return find(record.left, key, file);
		}
		else if(hash_function(record.Vin) < key){
			return find(record.right, key, file);
		}
    }
    void insert(long &pos_node, Record record,fstream &file){
        // cout<<"POS NODEEEE"<<pos_node<<endl;
        if(pos_node==-1){
            file.seekg(0,ios::end);//posiciona el puntero al final
            pos_node=file.tellg();//dice todo el tamaño del archivo
            //record.height = 0;
            file.write((char*)&record,sizeof(record));
            /*balance(pos_node, file);*/
            updateheight(pos_node, file);
            balance(pos_node, file);
        }
        else{
            file.seekg(pos_node,ios::beg);

            Record parent;
            file.read((char*) &parent,sizeof(Record));

            if(hash_function(record.Vin)<hash_function(parent.Vin)){
                //long new_pos = parent.left;
                insert(parent.left,record,file);
                //parent.left = new_pos; // Actualizar el puntero left del padre

            }
            else if(hash_function(record.Vin)>hash_function(parent.Vin)){
                //long new_pos = parent.right;
                insert(parent.right,record,file);
                //parent.right = new_pos; // Actualizar el puntero left del padre

            }
            else{
                throw "Record already exists";
            }
            ofstream file_W(filename, std::ios::binary |  std::ios::in | std::ios::out);//abrir en binario y al cambiar algo no elimina lo demas
            if(!file_W.is_open()) throw ("Error al abrir");//no hago nada 
            parent.height = 1 + max(height(parent.left, file), height(parent.right, file));
            
            file_W.seekp(pos_node);//mover puntero al inicio
            file_W.write(reinterpret_cast<char*>(&parent), sizeof(Record));//sobreescribir cabeza anterior
            //updateheight(pos_node, file);
            file_W.close();
            balance(pos_node, file);

        }
    }

    int height(long pos_node, fstream &file){
        if(pos_node == -1) return -1;
        file.seekg(pos_node, ios::beg);
        Record record;
        file.read((char*)&record, sizeof(Record));
        return record.height;
    }

    int balancingfactor(long pos_node, fstream &file){
        if(pos_node == -1) return 0;
        file.seekg(pos_node, ios::beg);
        Record record;
        file.read((char*)&record, sizeof(Record));
        return height(record.left, file) - height(record.right, file);
    }

    void updateheight(long pos_node, fstream &file) {
        if (pos_node == -1) return;
        
        file.seekg(pos_node, ios::beg);
        Record record;
        file.read((char*)&record, sizeof(Record));
        
        // Actualizar altura del nodo actual
        record.height = 1 + max(height(record.left, file), height(record.right, file));

        ofstream file_W(filename, ios::binary | ios::in | ios::out);
        if (!file_W.is_open()) throw ("Error al abrir");
        file_W.seekp(pos_node);
        file_W.write(reinterpret_cast<char*>(&record), sizeof(Record));
        file_W.close();

}

    void balance(long &pos_node, fstream &file){
        file.seekg(pos_node, ios::beg);
        Record record;
        file.read((char*)&record, sizeof(Record));
        int hb = balancingfactor(pos_node, file);
        if(hb >= 2){
            if(balancingfactor(record.left, file) < 0)
                left_rota(record.left, file);
            right_rota(pos_node, file);
        }
        else if(hb <= -2){
            if(balancingfactor(record.right, file) > 0)
                right_rota(record.right, file);
            left_rota(pos_node, file);
        }
    }

    void left_rota(long &pos_node, fstream &file){
        file.seekg(pos_node, ios::beg);
        Record record;
        file.read((char*)&record, sizeof(Record));

        long new_root = record.right;
        file.seekg(new_root, ios::beg);
        Record new_root_record;
        file.read((char*)&new_root_record, sizeof(Record));

        record.right = new_root_record.left;
        new_root_record.left = pos_node;

        record.height = 1 + max(height(record.left, file), height(record.right, file));
        new_root_record.height = 1 + max(height(new_root_record.left, file), height(new_root_record.right, file));


        file.seekp(pos_node, ios::beg);
        file.write(reinterpret_cast<char*>(&record), sizeof(Record));

        file.seekp(new_root, ios::beg);
        file.write(reinterpret_cast<char*>(&new_root_record), sizeof(Record));

        pos_node = new_root;
    }

    void right_rota(long &pos_node, fstream &file){
        file.seekg(pos_node, ios::beg);
        Record record;
        file.read((char*)&record, sizeof(Record));

        long new_root = record.left;
        file.seekg(new_root, ios::beg);
        Record new_root_record;
        file.read((char*)&new_root_record, sizeof(Record));

        record.left = new_root_record.right;
        new_root_record.right = pos_node;

        record.height = 1 + max(height(record.left, file), height(record.right, file));
        new_root_record.height = 1 + max(height(new_root_record.left, file), height(new_root_record.right, file));

        file.seekp(pos_node, ios::beg);
        file.write(reinterpret_cast<char*>(&record), sizeof(Record));

        pos_node = new_root;
    }

    vector<Record> inorder(long &pos_node, vector<Record> &result, fstream &file){
        if(pos_node != -1){
            Record record;
            file.seekg(pos_node, ios::beg);
            file.read(reinterpret_cast<char*>(&record), sizeof(Record));

            inorder(record.left, result, file);
            result.push_back(record);
            inorder(record.right, result, file);
        }
        return result;
    }
    
};
void writeFile(string filename){
    try {
        AVLFile fileavl("filenameavl.bin");
        Record record;
        //vector<Record> records;
        ifstream file(filename);

        if (!file.is_open())
        {
            cerr << "Error opening file." << endl;
        }

        // salto la primera linea (header)
        string line;
        getline(file, line);
        while (getline(file, line)){
            istringstream iss(line);
            Record registro;

            string id_str, Price_str, Year_str;
            string Mileage_str, city_str, State_str, Vin_str, Make_str, model_str;

            getline(iss, id_str, ',');
            getline(iss, Price_str, ',');
            getline(iss, Year_str, ',');
            getline(iss, Mileage_str, ',');
            getline(iss, city_str, ',');
            getline(iss, State_str, ',');
            getline(iss, Vin_str, ',');
            getline(iss, Make_str, ',');
            getline(iss, model_str, ',');

            // Convierte las cadenas a tipos correspondientes
            registro.id = stoi(id_str);
            registro.Price = stoi(Price_str);
            registro.Year = stoi(Year_str);

            // Copia los valores de las cadenas a los campos de caracteres de longitud fija
            strncpy(registro.Mileage, Mileage_str.c_str(), sizeof(registro.Mileage));
            strncpy(registro.city, city_str.c_str(), sizeof(registro.city));
            strncpy(registro.State, State_str.c_str(), sizeof(registro.State));
            strncpy(registro.Vin, Vin_str.c_str(), sizeof(registro.Vin));
            strncpy(registro.Make, Make_str.c_str(), sizeof(registro.Make));
            strncpy(registro.model, model_str.c_str(), sizeof(registro.model));

            fileavl.insert(registro);

        }
        file.close();


    }
    catch (const char* e) {
    cout << "Exception: " << e << endl;
    }
}

void readFile(string filename){
    try{
    AVLFile file(filename);
    cout<<"--------- show all sorted data -----------\n";
    vector<Record> result = file.inorder();
    for(Record re : result){
        re.showData();
    }
    }
    catch (const char* e) {
        cout << "Exception: " << e << endl;
    }
}

void Buscar_alumno_codigo(string filename){
    cout<<"---------------Buscar---------------"<<endl;
    cout<<"Escribe el codigo del alumno que deseas buscar"<<endl;
    int pos; cin>>pos;
    try{
        AVLFile file(filename);
        //Record record;
        Record alumno= file.find(pos);
        alumno.showData();
        cout<<endl;
        cout<<"Fin de la busqueda"<<endl;
    }
    catch (const char* e) {
        cout << "Exception: " << e << endl;
    }
}
int main() {
    //AVLFile file("texto.txt");
    //file.load();
    /*for(Record re : result){
        re.showData();
    }*/
    writeFile("texto.txt");
    readFile("filenameavl.bin");
    //Buscar_alumno_codigo("filenameavl.bin");
    return 0;
}