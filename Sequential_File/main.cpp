#include <iostream>
#include "sequential_file.h"
using namespace std;

// Parser class only for Sequential File
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
                // En el caso de "create", obtener el valor entre comillas simples o dobles
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
                size_t fromIndex = tokens.size();
                for (size_t i = 0; i < tokens.size(); ++i) {
                    if (tokens[i] == "from") {
                        fromIndex = i;
                        break;
                    }
                }
                if (fromIndex < tokens.size() - 1) {
                    // Si hay un espacio entre la columna y el operador, obtenemos la tabla
                    if (tokens[fromIndex + 1] != "=") {
                        tabla = tokens[fromIndex + 1];
                    } else if (fromIndex < tokens.size() - 2) {
                        // Si no, tomamos el siguiente token como la tabla
                        tabla = tokens[fromIndex + 2];
                    }
                }
                // El valor es el último token
                valor = tokens.back();
            } else {
                // Para otros tipos, la tabla es el siguiente token después de la palabra clave
                if (tokens.size() > 2) {
                    tabla = tokens[2];
                }
                // El valor es el último token
                valor = tokens.back();
            }
        }
    }

    void operacion() {
        // Parser for Sequential File
        if (tabla == "sequential_file") {
            string temp = tabla + "parser";
            Sequential_File file("temp");
            if (tipo == "create") {
                cout << "CREATE" << endl;
                read_dataset_count("tc20171021.csv", 1000, file);
                cout << "Se inserto la data correspondiente" << endl << endl;
            }
            else if (tipo == "select") {
                cout << "SELECT" << endl;
                if (file.search(valor).getKey() != "null") {
                    cout << endl << "IN DATA" << endl;
                }
                else {
                    cout << endl << "NOT FOUND" << endl;
                }
            }
            else if (tipo == "insert") {
                cout << "INSERT" << endl;

                read_dataset_count("tc20171021.csv", stoi(valor), file);
            }
            else if (tipo == "delete") {
                cout << "DELETE" << endl;
            }
        }
    }
};

void titulo () {
    cout << "  ____                          ____  " << endl;
    cout << " / ___|_ __ _   _ _ __   ___   | ___| " << endl;
    cout << "| |  _| '__| | | | '_ \\ / _ \\  |___ \\ " << endl;
    cout << "| |_| | |  | |_| | |_) | (_) |  ___) |" << endl;
    cout << " \\____|_|   \\__,_| .__/ \\___/  |____/ " << endl;
    cout << "                 |_|                  " << endl;
}

int Menu_SequentialFile() {
    int op;
    while (true) {
        cout << endl << "///// Menu ///// " << endl;
        cout << "1. Load .csv" << endl;
        cout << "2. Add record" << endl;
        cout << "3. Remove record - x" << endl;
        cout << "4. Search record" << endl;
        cout << "5. Search by range - x" << endl;
        cout << "6. Show all records" << endl;
        cout << "0. Exit" << endl;
        cout << endl;
        cout << "--> ";
        cin >> op;

        if (op == 0 || op == 1 || op == 2 || op == 3 || op == 4 || op == 5 || op == 6) {
            return op;
            break;
        } else {
            cout << "Not valid option" << endl;
            Sleep(2000);
            system("clear");
        }
    }
}

int main () {
    titulo();
    Sequential_File seq("data.dat");
    int data_to_insert = 0;

    while (true) {
        int op = Menu_SequentialFile();

        if (op == 1) {
            cout << "Cargando data from .cvs ... " << endl << endl;
            auto start = std::chrono::high_resolution_clock::now(); 

            read_dataset_count("tc20171021.csv", 6000, seq);

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            cout << endl;
            cout << "ADD FROM CSV TIME: " << duration.count() << "s";
            cout << endl;

        } else if (op == 2) {
            cout << "Add record ... " << endl << endl;
            Sequential_Record rx;
            auto start = std::chrono::high_resolution_clock::now();
            
            rx.setData();
            seq.add(rx);

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            cout << endl;
            cout << "Data añadida a base de datos ..." << endl;
            cout << "SINGLE ADD TIME: " << duration.count() << "s";
            cout << endl;

        } else if (op == 3) {
            cout << "Remove record from data ... " << endl << endl;
            cout << "Not implemented" << endl;

        } else if (op == 4) {
            cout << "Search a record" << endl << endl;
            string sh;
            cout << "Inser key of record: "; cin >> sh;
            auto start = std::chrono::high_resolution_clock::now();

            seq.search(sh).showData();

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            cout << endl;
            cout << "TIME TO SEARCH A RECORD: " << duration.count() << "s";

            cout << endl;

        } else if (op == 5) {
            cout << "Search by range" << endl << endl;
            cout << "Not implemented" << endl;

        } else if (op == 6) {
            cout << endl;
            cout << "Show all record in data" << endl << endl;
            auto start = std::chrono::high_resolution_clock::now();

            seq.showRecords();

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            cout << "SHOW RECORDS TIME: " << duration.count() << "s";
            cout << endl;

        } else {
            break;
        }
    }
    cout << endl << "Gracias! - Fin menu interactivo" << endl;

    Sleep(5000);
    printf("\033[H\033[J");

    cout << endl << "///// SEQUENTIAL FILE - PARSER ///// " << endl << endl;

    string sqlStatement1 = "create table sequential_file from file 'tc20171021.csv' using index hash(VIN)";
    string sqlStatement2 = "select * from sequential_file where VIN = KL4CJCSB0FB264921";
    string sqlStatement3 = "insert into sequential_file values 40";

    while (true) {
        cout << "1. " << sqlStatement1 << endl;
        cout << "2. " << sqlStatement2 << endl;
        cout << "3. " << sqlStatement3 << endl;
        cout << "4. " << "Salir" << endl << endl;
        int op = 0;
        cout << "-> "; cin >> op;
        cout << endl;

        if (op == 1) {
            Parser parser(sqlStatement1);
            cout << "Tipo = " << parser.tipo << ", tabla = " << parser.tabla << ", valor = " << parser.valor << endl;
            parser.operacion();

            Sleep(3000);
            printf("\033[H\033[J");
        }
        else if (op == 2) {
            Parser parser(sqlStatement2);
            cout << "Tipo = " << parser.tipo << ", tabla = " << parser.tabla << ", valor = " << parser.valor << endl;
            parser.operacion();

            Sleep(3000);
            printf("\033[H\033[J");
        }
        else if (op == 3) {
            Parser parser(sqlStatement3);
            cout << "Tipo = " << parser.tipo << ", tabla = " << parser.tabla << ", valor = " << parser.valor << endl;
            parser.operacion();

            Sleep(3000);
            printf("\033[H\033[J");
        }
        else if (op == 4) {
            break;
        }
        else {
            cout << endl << "Try again!" << endl;
        }
    }
    cout << "Gracias! - Fin de programa" << endl;
}