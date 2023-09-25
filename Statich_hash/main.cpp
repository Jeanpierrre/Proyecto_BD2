#include <iostream>
#include "static_hash.h"
using namespace std;
#include "llamadas.h"
/*#include "parser.h"

void parser_initial(){

    string sqlStatement1 = "create table Customer from file 'data.csv' using index hash(VIN)";
    string sqlStatement2 = "delete from static_hash where VIN = KL4CJCSB0FB264921";
    string sqlStatement3 = "select * from static_hash where VIN = KL4CJCSB0FB264921";
    string sqlStatement4 = "insert into static_hash values 20";
    //parser capaz de cargar indices eliminar y buscar
    Parser parser(sqlStatement3);



    //parser.parse(sqlStatement1);
    cout << "Tipo = " << parser.tipo << ", tabla = " << parser.tabla << ", valor = " << parser.valor << endl;
    parser.operacion();
};
*/
int main() {
    Interactive_Menu_Hash();
   // parser_initial();
}
