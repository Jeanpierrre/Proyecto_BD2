#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "static_hash.h"
using namespace std;
#include "llamadas.h"
class Parser {


public:
    string tipo;
    string tabla;
    string valor;

    Parser(string sqlStatement ) {
        tipo = "desconocido";
        tabla = "";
        valor = "";

        // Eliminar espacios en blanco adicionales y punto y coma al final
        string cleanedStatement = sqlStatement;
        while (!cleanedStatement.empty() && (cleanedStatement.back() == ' ' || cleanedStatement.back() == ';')) {
            cleanedStatement.pop_back();
        }

        istringstream iss(cleanedStatement);
        vector<string> tokens;
        string token;

        while (getline(iss, token, ' ')) {
            tokens.push_back(token);
        }

        if (!tokens.empty()) {
            tipo = tokens[0];

            if (tipo == "create") {
                //  obtener el valor entre comillas simples o dobles
                size_t firstQuote = cleanedStatement.find_first_of("\"'");
                size_t lastQuote = cleanedStatement.find_last_of("\"'");
                if (firstQuote != string::npos && lastQuote != string::npos && firstQuote < lastQuote) {
                    valor = cleanedStatement.substr(firstQuote + 1, lastQuote - firstQuote - 1);
                }
                // La tabla es el token después de "table"
                size_t tableIndex = cleanedStatement.find("table") + 5;
                while (tableIndex < cleanedStatement.size() && cleanedStatement[tableIndex] == ' ') {
                    tableIndex++;
                }
                size_t spaceIndex = cleanedStatement.find(" ", tableIndex);
                if (spaceIndex != string::npos) {
                    tabla = cleanedStatement.substr(tableIndex, spaceIndex - tableIndex);
                }
            } else if (tipo == "select") {
                // Para "select", la tabla está después de "from" o después de "*"
                //el * molesta asi que lo avanzo
                size_t fromIndex = tokens.size();
                for (size_t i = 0; i < tokens.size(); ++i) {
                    if (tokens[i] == "from") {
                        fromIndex = i;
                        break;
                    }
                }
                if (fromIndex < tokens.size() - 1) {

                    if (tokens[fromIndex + 1] != "=") {
                        tabla = tokens[fromIndex + 1];
                    } else if (fromIndex < tokens.size() - 2) {

                        tabla = tokens[fromIndex + 2];
                    }
                }
                // El valor es el último token casi siempre la verdad
                valor = tokens.back();
            } else {
                // Para otros tipos, la tabla es el siguiente token después de la palabra clave validar el =
                if (tokens.size() > 2) {
                    tabla = tokens[2];
                }
                // El valor es el último token despues del =
                valor = tokens.back();
            }
        }
    }
    void operacion(){
        if(tabla=="static_hash"){
            string temp=tabla+"paser"+".bin";
            static_hash prueba(temp,8);

            if(tipo=="create"){
                cout<<"create"<<endl;
                read_dataset_ALL("tc20171021.csv",prueba);
            }
            else if(tipo=="delete"){
                cout<<"delete"<<endl;
                if(prueba.delete_Record(valor)){
                    cout<<"Eliminado correctamente"<<endl;
                }
                else{
                    cout<<"Fallo al eliminar, no se encuentra"<<endl;
                }
            }
            else if(tipo=="insert"){
                cout<<"INSERT"<<endl;
                cout<<stoi(valor)+1<<endl;

                read_dataset_count("tc20171021.csv",stoi(valor),prueba);
            }
            else if(tipo=="select"){
                cout<<"Select"<<endl;
                if(prueba.search_record(valor)){
                 cout<<"Key presente"<<endl;
                }
                else{
                    cout<<"Key no presente"<<endl;
                }
            }


        }
    }

};

/*
int main() {

    string sqlStatement1 = "create table Customer from file 'data.csv' using index hash(VIN)";
    string sqlStatement2 = "delete from static_hash where VIN = KL4CJCSB0FB264921";
    string sqlStatement3 = "select * from static_hash where VIN = KL4CJCSB0FB264921";
    string sqlStatement4 = "insert into static_hash values 20";
    //parser capaz de cargar indices eliminar y buscar
    Parser parser(sqlStatement3);



    //parser.parse(sqlStatement1);
    cout << "Tipo = " << parser.tipo << ", tabla = " << parser.tabla << ", valor = " << parser.valor << endl;
    parser.operacion();

}
*/