#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <chrono>
using namespace std;
unsigned long long hash_function(const std::string& key) {
    // Convierte el string en un número largo
    unsigned long long key_as_long = 0;

    for (char c : key) {
        key_as_long = key_as_long * 256 + static_cast<unsigned long long>(c);
    }

    return key_as_long;
}
struct RecordAvl
{
    int id;//
    int Price;//
    int Year;//
    char State[20];//
    char Vin[18];//
    char Make[50];//

    long left=-1, right=-1;
    int height=-1;

    void showData() {
        cout<<id<<" "<<Price<<" "<<Year<<" "<<" "<<State<<" "<<Vin<<" "<<Make<<" "<<" i: "<<left<<" d: "<<right<<" height: "<<height<<endl;
    }
    void setData() {
        cout << "id:";
        cin >> id;
        cout << "Price: ";
        cin >> Price;
        cout << "Year: ";
        cin >> Year;
        cout << "State: ";
        cin >> State;
        cout << "Vin: ";
        cin >> Vin;
        cout << "Make: ";
        cin >> Make;
    }
    
};

class AVLFile
{
private:
    string filename;
    long pos_root;
    /*ifstream readFile;
    ofstream writeFile;*/
public:
    bool fileExists(const std::string &fileName) {
        std::ifstream file(fileName);
        return file.good();
    }
    AVLFile(string filename){
        this->filename = filename;
        /*this->writeFile.open(filename, std::ios::binary |  std::ios::in | std::ios::out);//abrir en binario y al cambiar algo no elimina lo demas
        this->readFile.open(filename, std::ios::binary |  std::ios::in | std::ios::out);//abrir en binario y al cambiar algo no elimina lo demas*/
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
    
    RecordAvl find(long long key){
        fstream file(filename,ios::binary | ios::app | ios::in | ios::out);
        RecordAvl record = find(pos_root, key, file);
        file.close();
        return record;
    }
    void insert(RecordAvl record){
        fstream file(filename, ios::binary | ios::app | ios::in | ios::out);
        //if(!file.is_open()) throw ("No se pudo abrir el archivo");
        
    
        insert(pos_root, record, file);
        file.close();
    }

    vector<RecordAvl> Range_search(long long key_begin, long long key_end){
        fstream file(filename, ios::binary | ios::app | ios::in | ios::out);
        vector<RecordAvl> result;
        Range_search(pos_root, key_begin, key_end,result,file);
        file.close();
        return result;
    }

    vector<RecordAvl> inorder(){
        //cout<<"entra"<<endl;
        fstream file(filename, ios::binary | ios::app | ios::in | ios::out);
        string line;
        getline(file, line);
        vector<RecordAvl> result;
        inorder(pos_root, result,file);
        //cout<<"SALE"<<endl;
        file.close();
        return result;
    }

    void remove_record(long long key){
        fstream file(filename, ios::binary | ios::app | ios::in | ios::out);
        remove_record(pos_root, key, file);
        file.close();
    }

    int height(){
        fstream file(filename, ios::binary | ios::app | ios::in | ios::out);
        int result = height(pos_root, file);
        file.close();
        return result;
    }
    
private:
    void write_file(long pos_node,RecordAvl record){
        ofstream file_W(filename, std::ios::binary |  std::ios::in | std::ios::out);//abrir en binario y al cambiar algo no elimina lo demas
        if(!file_W.is_open()) throw ("Error al abrir");//no hago nada     
        file_W.seekp(pos_node);//mover puntero al inicio
        file_W.write(reinterpret_cast<char*>(&record), sizeof(RecordAvl));//sobreescribir cabeza anterior
        file_W.close();
    }
    RecordAvl find(long pos_node, long long key, fstream &file){
        if(pos_node == -1) throw "Record not found";

		file.seekg(pos_node, ios::beg);
		RecordAvl record;
		file.read((char*)&record, sizeof(RecordAvl));
        //record.showData();
        if(hash_function(record.Vin) == key){
            record.showData();
			return record;
            }
		else if(hash_function(record.Vin) > key){
			return find(record.left, key, file);
		}
		else if(hash_function(record.Vin) < key){
			return find(record.right, key, file);
		}
    }

    RecordAvl getRecord(long pos_node, fstream &file){
        if(pos_node == -1) throw "Record not found";
        file.seekg(pos_node, ios::beg);
        RecordAvl record;
        file.read((char*)&record, sizeof(RecordAvl));
        return record;
    }

    void insert(long &pos_node, RecordAvl record,fstream &file){

        if(pos_node==-1){
            file.seekg(0,ios::end);//posiciona el puntero al final
            pos_node=file.tellg();//dice todo el tamaño del archivo
            //record.height = 0;
            file.write((char*)&record,sizeof(record));
            /*balance(pos_node, file);*/
            updateheight(pos_node, file);
            
            
        }
        else{
            file.seekg(pos_node,ios::beg);

            RecordAvl parent;
            file.read((char*) &parent,sizeof(RecordAvl));

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
            write_file(pos_node, parent);
            updateheight(pos_node, file);
            
            

        }
        //balance(pos_node, file); ser feliz
    }

    int height(long pos_node, fstream &file){
        if(pos_node == -1) return -1;
        file.seekg(pos_node, ios::beg);
        RecordAvl record;
        file.read((char*)&record, sizeof(RecordAvl));
        return record.height;
    }

    int balancingfactor(long pos_node, fstream &file){
        if(pos_node == -1) return 0;
        file.seekg(pos_node, ios::beg);
        RecordAvl record;
        file.read((char*)&record, sizeof(RecordAvl));
        return height(record.left, file) - height(record.right, file);
    }

    void updateheight(long pos_node, fstream &file) {
        if (pos_node == -1) return;
        
        file.seekg(pos_node, ios::beg);
        RecordAvl record;
        file.read((char*)&record, sizeof(RecordAvl));
        
        // Actualizar altura del nodo actual
        record.height = 1 + max(height(record.left, file), height(record.right, file));

        write_file(pos_node, record);

}

    void balance(long &pos_node,fstream &file){
        if(pos_node == -1) return;
        file.seekg(pos_node, ios::beg);
        RecordAvl record;
        file.read((char*)&record, sizeof(RecordAvl));
        int bf = balancingfactor(pos_node, file);
        if(bf >= 2){
            if(balancingfactor(record.left, file) <=1){
                cout<<"ANTIGUO RECORD.LEFT: "<<getRecord(record.left, file).id<<endl;
                left_rota(record.left, file);
                cout<<"NUEVO RECORD.LEFT: "<<getRecord(record.left, file).id<<endl;
            }
            write_file(pos_node, record);
            //cout<<"NUEVO RECOD.LEFT DEL POS_NODE: "<<getRecord(getRecord(pos_node, file).left, file).id<<endl;
            right_rota(pos_node, file);
        }
        else if(bf <= -2){
            if(balancingfactor(record.right, file) >= -1){
                right_rota(record.right, file);
            }
            write_file(pos_node, record);
            left_rota(pos_node, file);
        }
    }

    void left_rota(long &pos_node, fstream &file){
        //cout<<"Rotacion IZQUIERDAA"<<endl;
        file.seekg(pos_node, ios::beg);
        RecordAvl padre;
        file.read((char*)&padre, sizeof(RecordAvl));
        //cout<<"record_padre: "<<"------>"<<padre.id<<" "<<padre.height<<endl;
        long hijo = padre.right;
        file.seekg(hijo, ios::beg);
        RecordAvl hijo_record;
        file.read((char*)&hijo_record, sizeof(RecordAvl));
        //cout<<"record_hijo: "<<"------->"<<hijo_record.id<<" "<<hijo_record.height<<endl;

        padre.right = hijo_record.left;
        //cout<<"nuevo hijo del padre: "<<padre.right<<endl;
        hijo_record.left = pos_node;
        //cout<<"nuevo padre del hijo: "<<getRecord(hijo_record.left,file).id<<endl;

        padre.height = 1 + max(height(padre.left, file), height(padre.right, file));
        //cout<<"ESTE ES EL POS_NODE: "<<pos_node<<endl;
        //cout<<"ESTE ES EL HIJO_RECORD.LEFT: "<<hijo_record.left<<endl;
        hijo_record.height = 1 + max(padre.height, height(hijo_record.right, file));
        //cout<<"record_padre: "<<padre.id<<" "<<padre.height<<endl;
        //cout<<"record_hijo: "<<hijo_record.id<<" "<<hijo_record.height<<endl;

        write_file(pos_node, padre);

        write_file(hijo, hijo_record);

        pos_node = hijo;
        
    }

    void right_rota(long &pos_node, fstream &file){
        cout<<"Rotacion DEREECHAAAAA"<<endl;
        file.seekg(pos_node, ios::beg);
        RecordAvl padre;
        file.read((char*)&padre, sizeof(RecordAvl));
        cout<<"record_padre: "<<"------>"<<padre.id<<" "<<padre.height<<endl;
        long hijo = padre.left;
        file.seekg(hijo, ios::beg);
        RecordAvl hijo_record;
        file.read((char*)&hijo_record, sizeof(RecordAvl));
        cout<<"record_hijo: "<<"------->"<<hijo_record.id<<" "<<hijo_record.height<<endl;

        padre.left = hijo_record.right;
        hijo_record.right = pos_node;

        padre.height = 1 + max(height(padre.left, file), height(padre.right, file));
        hijo_record.height = 1 + max(height(hijo_record.left, file), padre.height);

        cout<<"record_padre: "<<padre.id<<"  "<<padre.height<<endl;
        cout<<"record_hijo: "<<hijo_record.id<<" "<<hijo_record.height<<endl;

        write_file(pos_node, padre);

        write_file(hijo, hijo_record);
        //cout<<"RECORD DEL HIJO: "<<getRecord(hijo, file).id<<endl;
        pos_node = hijo;
        //cout<<"RECORD DEL POS_NODE: "<<getRecord(pos_node, file).id<<endl;
    }

    vector<RecordAvl> inorder(long &pos_node, vector<RecordAvl> &result, fstream &file){
        if(pos_node != -1){
            RecordAvl record;
            file.seekg(pos_node, ios::beg);
            file.read(reinterpret_cast<char*>(&record), sizeof(RecordAvl));

            inorder(record.left, result, file);
            result.push_back(record);
            inorder(record.right, result, file);
        }
        return result;
    }
    vector<RecordAvl> Range_search(long pos_node, long long key_begin, long long key_end, vector<RecordAvl> &result,fstream &file){
        if(pos_node != -1){
            RecordAvl record;
            file.seekg(pos_node, ios::beg);
            file.read(reinterpret_cast<char*>(&record), sizeof(RecordAvl));

            Range_search(record.left, key_begin, key_end, result, file);
            if(hash_function(record.Vin) >= key_begin && hash_function(record.Vin) <= key_end){
                result.push_back(record);
                cout<<record.Vin<<endl;
            }
            Range_search(record.right, key_begin, key_end, result, file);
        }
        return result;
    }
    void remove_record(long &pos_node, long long key, fstream &file){
        if(pos_node == -1) throw "Record not found";

        file.seekg(pos_node, ios::beg);
        RecordAvl record;
        file.read((char*)&record, sizeof(RecordAvl));

        if(hash_function(record.Vin) > key){
            remove_record(record.left, key, file);
        }
        else if(hash_function(record.Vin) < key){
            remove_record(record.right, key, file);
        }
        else{
            if(record.left == -1 && record.right == -1){
                pos_node = -1;
            }
            else if(record.left == -1){
                pos_node = record.right;
            }
            else if(record.right == -1){
                pos_node = record.left;
            }
            else{
                long &pos_node_aux = record.left;
                while(pos_node_aux != -1){
                    file.seekg(pos_node_aux, ios::beg);
                    file.read((char*)&record, sizeof(RecordAvl));
                    pos_node_aux = record.right;
                }
                pos_node_aux = record.left;
                record.left = pos_node;
                record.right = pos_node;
                pos_node = record.left;
            }
        }
        write_file(pos_node, record);
        updateheight(pos_node, file);
        //balance(pos_node, file);
    }
    
};
void writeFile(string filename, int i){
    try {
        int cont = 0;
        AVLFile fileavl("filenameavl.bin");
        RecordAvl record;
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
            RecordAvl registro;

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
            
            strncpy(registro.State, State_str.c_str(), sizeof(registro.State));
            strncpy(registro.Vin, Vin_str.c_str(), sizeof(registro.Vin));
            strncpy(registro.Make, Make_str.c_str(), sizeof(registro.Make));
            
            if (cont < i) {
                fileavl.insert(registro);
                if(cont%100==0){
                    cout<<"Insertando: "<<cont<<endl;
                }
                //record.showData();
                cont++;
            }

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
    vector<RecordAvl> result = file.inorder();
    for(RecordAvl re : result){
        re.showData();
    }
    }
    catch (const char* e) {
        cout << "Exception: " << e << endl;
    }
}

void Buscar_alumno_codigo(string filename){
    cout<<"---------------Buscar---------------"<<endl;
    cout<<"Escribe el VIM del carro que deseas buscar"<<endl;
    string key; 
    cin>>key;
    long long key_hash = hash_function(key);
    
    AVLFile file(filename);
    //Record record;
    RecordAvl alumno= file.find(key_hash);
    alumno.showData();
    cout<<endl;
    cout<<"Fin de la busqueda"<<endl;
    
}

void busqueda_rango(string filename){
    cout<<"---------------Rango---------------"<<endl;
    cout<<"Escribe el VIM inicial del carro que deseas buscar"<<endl;
    string key_begin;
    cin>>key_begin;
    cout<<"Escribe el VIM final del carro que deseas buscar"<<endl;
    string key_end;
    cin>>key_end;
    long long key_begin_hash = hash_function(key_begin);
    long long key_end_hash = hash_function(key_end);
    AVLFile file(filename);
    vector<RecordAvl> result = file.Range_search(key_begin_hash, key_end_hash);
    for(RecordAvl re : result){
        re.showData();
    }
    cout<<endl;
    cout<<"Fin de la busqueda"<<endl;
}

void ingresar_registro(string filename){
    AVLFile file(filename);
    RecordAvl record;
    record.setData();
    file.insert(record);
}

void remove_record(string filename){
    AVLFile file(filename);
    cout<<"---------------Eliminar---------------"<<endl;
    cout<<"Escribe el VIM del carro que deseas eliminar"<<endl;
    string key; 
    cin>>key;
    long long key_hash = hash_function(key);
    file.remove_record(key_hash);
    cout<<"Fin de la eliminacion"<<endl;
}