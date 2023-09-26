#include <iostream>
using namespace std;
#include "parser.h"

void parser_initial(){

    string sqlStatement1 = "create table Customer from file 'data.csv' using index hash(VIN)";
    string sqlStatement2 = "delete from avi_file where VIN = KL4CJASB1HB027202";
    string sqlStatement3 = "select * from avi_file where VIN = KL4CJASB1HB027202";
    string sqlStatement4 = "insert into avi_file values 40";
    //parser capaz de cargar indices eliminar y buscar
    Parser parser(sqlStatement3);



    cout << "Tipo = " << parser.tipo << ", tabla = " << parser.tabla << ", valor = " << parser.valor << endl;
    parser.operacion();
};

int main() {
    parser_initial();
}