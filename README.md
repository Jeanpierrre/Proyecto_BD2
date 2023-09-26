
# Proyecto 1 - Base de Datos 2

## Integrantes 
- Angel Mucha
- Jean Piere Sotomayor
- Juan Manuel Torres

## Introduccion

### Objetivos del proyecto
El proyecto consiste en crear tres estructuras de datos (Sequential-File, AVL-File y Extendible Hashing) junto con sus operaciones de inserción, búsqueda y eliminación. Luego, evaluar la eficiencia de cada operación utilizando gráficos que representen el tiempo de ejecución en función de la cantidad de registros.

### Descripcion del dataset
Se emplearon conjuntos de datos de Kaggle (https://www.kaggle.com/datasets/brentpafford/true-car-listings-2017-project) descargados en formato CSV. Los datos se movieron a nuevos archivos con el fin de asegurar el correcto funcionamiento de los algoritmos encargados de administrar archivos físicos.
La base de datos contiene información sobre vehículos usados, con detalles como ID, precio, año, millaje, ciudad, estado, VIN, marca y modelo. Sirve para identificar los vehiculos usados en EE.UU.

### Resultados esperados
Funcionamiento adecuado de las estructuras de manejo de archivos y sus métodos asociados.

# Tecnicas utilizadas

## Extendible hashing 

<div align="center">
  <img src="https://media.geeksforgeeks.org/wp-content/uploads/20190803103835/hash11.png" alt="Extendible Hash" style="border-radius: 15px;">
</div>

El extendible hash es una estructura de almacenamiento dinámico. Consta de 3 elementos importantes, registros, buckets y el hash table. Los registros son un conjunto de información relacionada los cuales se encuentran en un inicio en el archivo dataset y posteriormente en buckets, estructuras de almacenamiento de registros. Así mismo, el hash_table es un conjunto de asociaciones key-value en donde el key es la llave a través del cual se puede llegar al value, siendo este el valor en memoria que referencia a dicho bucket, valor en específico.  La estructura extendible hash emula el funcionamiento de una base de datos, la organización, accesibilidad y perdurabilidad de los datos. Los archivos index.bin y data.bin me permiten manejar la perdurabilidad de los registros. Dentro de index.bin se almacenan las asociaciones key-value del hash_table mientras que en data.bin buckets de registros.

### Insercion
Para llevar a cabo la inserción de un registro, es necesario determinar a qué bucket pertenecerá en función de su llave. Esto se logra a través de la búsqueda del índice correspondiente para el registro. Una vez obtenido el índice, se procede a realizar la inserción en el bucket apropiado. Sin embargo, antes de insertar el registro, se realizan comprobaciones adicionales para garantizar la integridad de la base de datos. Esto incluye verificar que el tamaño del bucket esté dentro de límites aceptables.

En caso de que el tamaño del bucket exceda un cierto factor de bloque, se evalúa si es necesario crear nuevos buckets y redistribuir los registros de acuerdo con la función hash. Todo este proceso se realiza con el objetivo de mantener la eficiencia y la organización de la base de datos al tiempo que se asegura la unicidad de las llaves primarias.
```cpp
void extendible_hash::insert_record(Record record) {
    int number_key = Vin_to_number(record.Vin);
    string key_record = (BinKeyMod(number_key)); // 
    int pos = search_pos_data(key_record);
    Bucket bucket = read_bucket(pos);
    if (bucket.count < fb) {
        add_Record(pos, bucket, record);
    }
    else {
        string vin = pos_key(key_record);
        Index_hash in = Index_hash(vin.c_str(), hash_table_unorder[vin].first);
        overflow(in, bucket, record);
    }
}
```



### Búsqueda
Primero se busca el bucket asociado a través de la clave (key) en el bucket principal. Si no se encuentra en el bucket principal, se itera en los buckets siguientes (bucket.nextbucket) hasta encontrar el registro deseado.
```cpp
bool extendible_hash::search_record(string vin_key) {
    Record record;
    int number_key= Vin_to_number(vin_key);
    string key=(BinKeyMod(number_key));//funcion hash
    int pos=search_pos_data(key);
    Bucket bucket= read_bucket(pos);
    cout<<"FIN"<<endl;
    for (int i = 0; i < bucket.count; i++){   
        record = bucket.records[i];
        if (record.Vin == vin_key){
            bucket.records[i].showData();
            return true;
            }
    }
    while(bucket.nextBucket!=-1){
        bucket = read_bucket(bucket.nextBucket);
        for (int i = 0; i < bucket.capacity; i++){
            record = bucket.records[i];
            if (record.Vin == vin_key)
                return true;}
    }
    return false;
}
```
### Eliminación

La eliminación de registros implica ubicar el registro deseado en el bucket correspondiente y reemplazarlo por el último registro del mismo, actualizando el tamaño del bucket.
```cpp
bool extendible_hash::delete_Record(string key_record) {
    int number_key= Vin_to_number(key_record);//convertir string a numero
    string key_record_Convert=(BinKeyMod(number_key));//convertir numero a binario con D bits caracteristicos
    string vin= pos_key(key_record_Convert);//Busca en el unordered map la referencia al indice en el archivo
    Index_hash in=Index_hash(vin.c_str(),hash_table_unorder[vin].first);//con esto armo el indice
    Bucket buc= read_bucket(in.pos_Data);
    Record temp;
    for(int i=0;i<buc.count;i++){
        temp=buc.records[i];
        temp.showData();
        if(temp.Vin==key_record){
            for (int j = i + 1; j < buc.count; j++){
                buc.records[j - 1] = buc.records[j];
            }
            buc.count--;
            write_bucket(buc,in.pos_Data);
            return true;
        }
    }
    while(buc.nextBucket!=1){
        int pos_data_temp=buc.nextBucket;
        buc= read_bucket(pos_data_temp);
        for(int i=0;i<buc.count;i++){
            temp=buc.records[i];
            if(temp.Vin==key_record){
                for (int j = i + 1; j < buc.count; j++){
                    buc.records[j - 1] = buc.records[j];
                }
                buc.count--;
                write_bucket(buc,pos_data_temp);
                return true;
            }
        }
    }
    return false;
}
```
## Static Hashing
<p align="center">
  <img src="/img/Static.png">
</p>

Static Hashing es una técnica de organización de datos que utiliza un esquema fijo de división de registros en buckets predefinidos.

### Inserción

La inserción implica asignar un registro a un bucket específico según su clave, utilizando una función de hash. Si el bucket está lleno, se pueden tomar medidas como reorganizar la estructura o implementar técnicas de resolución de colisiones.
```cpp
void static_hash::insert_record(Record record) {

    if(!search_record(record.Vin)){
        int number_key= Vin_to_number(record.Vin);//convertir a entero
        int key_record=(BinKeyMod(number_key));//funcion hash
        Bucket buc= read_bucket(key_record);
        if(buc.count<fb){
            add_Record(key_record,buc,record);
        }
        else{
            overflow(key_record,buc,record);
        };
    }
    else{
        cout<<"Car registrado, no se agrega"<<endl;
    }

}
```
### Tiempo de insertar los registros
<p align="center">
  <img src="/img/hash_ti.png">
</p>

### Búsqueda

La búsqueda implica localizar el bucket asociado a una clave mediante la función de hash y buscar el registro dentro de ese bucket. En caso no estar ahi validar el anexado a este.
```cpp
bool static_hash::search_record(string vin_key) {
    Record record;
    int number_key= Vin_to_number(vin_key);
    int key_pos =(BinKeyMod(number_key));
    Bucket bucket= read_bucket(key_pos);
    for (int i = 0; i < bucket.count; i++){
        record = bucket.records[i];
        if (record.Vin == vin_key){
            record.showData();
            return true;
        }
    }
    //CASO CUANDO ESTA ENLAZADO A OTRO BUCKET
    while(bucket.nextBucket!=-1){
        bucket = read_bucket_pos(bucket.nextBucket);
        for (int i = 0; i < bucket.count; i++){
            record = bucket.records[i];
            if (record.Vin == vin_key){
                record.showData();
                return true;}
                }
    }
    return false;
}
```
### Eliminación

La eliminación consiste en encontrar el registro deseado en su bucket correspondiente y eliminarlo, actualizando si es necesario la organización del bucket.

Este enfoque está diseñado para manejar una cantidad fija de datos y no se ajusta de forma dinámica como en otros métodos como Extendible Hashing.
```cpp
bool static_hash::delete_Record(string key_record) {
    int number_key = Vin_to_number(key_record); 
    int key_pos = BinKeyMod(number_key); 
    Bucket buc = read_bucket(key_pos); 
    for (int i = 0; i < buc.count; i++) {
        if (buc.records[i].Vin == key_record) {
            // Se encontró el registro, proceder a eliminarlo
            for (int j = i + 1; j < buc.count; j++) {
                buc.records[j - 1] = buc.records[j];
            }
            buc.count--;
            // Actualizar el bucket en el archivo
            write_bucket(buc, key_pos);
            return true; // Registro eliminado con éxito
        }
    }
    while (buc.nextBucket != -1) {
        int next_bucket_pos = buc.nextBucket;
        buc = read_bucket_pos(next_bucket_pos);

        for (int i = 0; i < buc.count; i++) {
            if (buc.records[i].Vin == key_record) {
                for (int j = i + 1; j < buc.count; j++) {
                    buc.records[j - 1] = buc.records[j];
                }
                buc.count--;
                write_in_pos(buc, next_bucket_pos);
                return true; // Registro eliminado con éxito}
        }
    }
    return false;
}

```

## Avl File
<div align="center">
  <img src="https://cdn.educba.com/academy/wp-content/uploads/2021/07/AVL-Tree-Rotation-1-1.jpg" alt="Avl FILE" style="border-radius: 15px;">
</div>
El AVL-File es una estructura de datos basada en árboles AVL que se utiliza para almacenar y gestionar registros. Esta estructura combina las propiedades de los árboles AVL (árboles binarios de búsqueda balanceados) con archivos binarios para lograr una organización eficiente de los registros y operaciones de búsqueda, inserción y eliminación.

### Inserción

La inserción de registros en el AVL-File se realiza de manera similar a la inserción en un árbol AVL convencional. Cada registro se inserta en el árbol según un criterio de orden, que generalmente está basado en una clave única, como un valor hash o un identificador único. Durante la inserción, el árbol se reorganiza automáticamente para mantener su propiedad de equilibrio (balance). Si la inserción desequilibra el árbol, se realizan rotaciones para restaurar el equilibrio.

```cpp
void insert(long &pos_node, RecordAvl record,fstream &file){
        if(pos_node==-1){
            file.seekg(0,ios::end);
            pos_node=file.tellg();
            file.write((char*)&record,sizeof(record));
            updateheight(pos_node, file);
        }
        else{
            file.seekg(pos_node,ios::beg);
            RecordAvl parent;
            file.read((char*) &parent,sizeof(RecordAvl));

            if(hash_function(record.Vin)<hash_function(parent.Vin)){
                insert(parent.left,record,file);
            }
            else if(hash_function(record.Vin)>hash_function(parent.Vin)){
                insert(parent.right,record,file);
            }
            else{
                throw "Record already exists";
            }
            write_file(pos_node, parent);
            updateheight(pos_node, file);
        }
        balance(pos_node, file); ser feliz
    }
```
### Tiempo de insertar los registros

<p align="center">
  <img src="/img/time_avl.jpg">
</p>

### Búsqueda

La búsqueda de registros en el AVL-File se basa en el mismo principio que la búsqueda en un árbol AVL estándar. Se busca el registro deseado siguiendo el criterio de orden de la clave y realizando comparaciones. La ventaja del AVL-File radica en su capacidad para realizar búsquedas eficientes gracias a la estructura balanceada del árbol.
```cpp
RecordAvl find(long pos_node, long long key, fstream &file){
        if(pos_node == -1) throw "Record not found";

		file.seekg(pos_node, ios::beg);
		RecordAvl record;
		file.read((char*)&record, sizeof(RecordAvl));
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
```
### Búsqueda por rango
La función "Range_search" se utiliza para buscar registros en un rango específico de claves dentro del árbol AVL y recopilar los registros que satisfacen ese criterio en un vector.
#### Descripción de las funciones en el contexto de "búsqueda por rango":

1. public vector<RecordAvl> Range_search(long long key_begin, long long key_end): Esta es la función principal que se llama desde fuera de la clase para realizar la búsqueda por rango. Toma dos argumentos de clave (key_begin y key_end) que definen el rango que se desea buscar. Abre el archivo binario, llama a la función privada Range_search y luego cierra el archivo. Devuelve un vector de registros (RecordAvl) que caen dentro del rango especificado.

2. private vector<RecordAvl> Range_search(long pos_node, long long key_begin, long long key_end, vector<RecordAvl> &result, fstream &file): Esta es la función recursiva privada que realiza la búsqueda por rango en el árbol AVL. Toma varios argumentos:

	pos_node: La posición actual en el archivo binario que representa el nodo en el árbol AVL.
	key_begin y key_end: Los límites del rango de búsqueda.

	result: Un vector donde se almacenan los registros que caen dentro del rango.
	file: Un objeto de flujo de archivo que se utiliza para leer los registros del archivo binario.

```cpp
 vector<RecordAvl> Range_search(long pos_node, long long key_begin, long long key_end, vector<RecordAvl> &result,fstream &file){
        if(pos_node != -1){
            RecordAvl record;
            file.seekg(pos_node, ios::beg);
            file.read(reinterpret_cast<char*>(&record), sizeof(RecordAvl));
            this->llamadasamemoria=llamadasamemoria+1;

            Range_search(record.left, key_begin, key_end, result, file);
            if(hash_function(record.Vin) >= key_begin && hash_function(record.Vin) <= key_end){
                result.push_back(record);
                cout<<record.Vin<<endl;
            }
            Range_search(record.right, key_begin, key_end, result, file);
        }
        return result;
    }
```

### Eliminacion

La eliminación de registros en el AVL-File se realiza también siguiendo el principio de un árbol AVL. Se busca el registro a eliminar, se realiza la eliminación y se restaura el equilibrio del árbol mediante rotaciones si es necesario.

## Sequential File
<p align="center">
  <img src="/img/seq_main.jpeg">
</p>
Se trata de un método simple utilizado para el almacenamiento, organización y gestión de datos mediante dos archivos primarios conectados con variables de apunte. Los datos se almacenan en forma secuencial en formato binario, de tal forma que, el registro de datos siempre será continuo y ordenado de menor a mayor según lo programado. 


### Inserción
La inserción para el sequential file y sus dos archivos presenta múltiples casos que deben ser analizados con anterioridad. Se recibe un registro el cual deberá posicionarse donde corresponda según su valor Key. En caso este valor sea menor que los valores ya insertados en el archivo principal, deberá utilizarse un archivo auxiliar con tamaño limitado, esto sin perder el registro anterior y el siguiente al insertado. Cuando el tamaño del archivo auxiliar exceda, se realizará una actualización en el achivo principal con todos los registros ya ordenados secuencialmente.

```cpp
void add (Sequential_Record record) {
	// Si el archivo principal esta vacío, se inserta
	// Caso contrario, hay data existente, se requiere comparar
		// Es menor que el previo y el siguiente es el último, se inserta
		// Caso contrario, se manda a Aux
	
		// Actualizar valores si es el último
		// Caso de sobre-escritura en record eliminado
}
```

### Búsqueda
La función de busqueda nos permite encontrar si es que existe algún registro que coincidan con el valor dado o este no se encuentra en la data actual. 

```cpp
Sequential_Record search (string key) {
    // Buscar pos
    // Abrir archivo principal
        // Leer en posición
        // Varificar registro eliminado o no existe

        // Abrir archivo secundario
            // Recorrer todos los next en archivo "a"
            // Si el key coincide, retorna
}
```

### Eliminación
Este método nos permite eliminar un registro de forma representativa ya que realmente se hace un cambio de estado. Esto permite saltar el registro ya que ya no habría ningun registro que apunte al eliminado como siguiente. 

### Tiempo de operaciones por registros

<p align="center">
  <img src="/img/seq.jpeg">
</p>


# Parser

El mini parser funciona para las consulas dadas por el profesor. Esta habilitado para las siguientes consultas.
```cpp
    string sqlStatement2 = "delete from "ESTRUCTURA" where VIN = "VALOR";// ac
    string sqlStatement3 = "select * from "ESTRUCTURA" where VIN = "VALOR"";
    string sqlStatement4 = "insert into "ESTRUCTURA" values "Cantidad";
```
Siendo la Estructura, las diferentes tecnicas como static_hash, avl y secuencial. El valor el Vin asociado a las operaciones de eliminacion e insercion. Y para la cantidad se refiere a la cantidad de regristros para insertar de la base de datos.
El parser sirve para poder insertar desde la base de datos una cierta cantidad, eliminar y buscar.

# Observaciones
Los archivos funcionales son los que estan en las carpetas, debido a que al correr todo junto, solo funciona el secuencial y avl( ya que estos fueron hechos con VisualStudio) con el menu, ya que StaticHash y Extendiblehash fueron hechos en Clion y tiene error al correrelo con los demas.
Las estructuras tienen la variable llamada acceso a memoria secundaria("llamadas_mem_secundaria"), que cuenta las veces que hacemos una lectura o escritura en el archivo. 
