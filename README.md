
# Proyecto 1 - Base de Datos 2

## Integrantes 
- Angel Mucha
- Jean Piere Sotomayor
- Juan Manuel Torres

## Introduccion

### Objetivos del proyecto
El proyecto consiste en crear tres estructuras de datos (Sequential-File, AVL-File y Extendible Hashing) junto con sus operaciones de inserción, búsqueda y eliminación. Luego, evaluar la eficiencia de cada operación utilizando gráficos que representen el tiempo de ejecución en función de la cantidad de registros.

### Descripcion del dataset
Se emplearon conjuntos de datos de Kaggle (https://www.kaggle.com/datasets) descargados en formato CSV. Los datos se movieron a nuevos archivos con el fin de asegurar el correcto funcionamiento de los algoritmos encargados de administrar archivos físicos.

### Resultados esperados
Funcionamiento adecuado de las estructuras de manejo de archivos y sus métodos asociados.

## Tecnicas utilizadas

## Extendible hashing
El extendible hash es una estructura de almacenamiento dinámico. Consta de 3 elementos importantes, registros, buckets y el hash table. Los registros son un conjunto de información relacionada los cuales se encuentran en un inicio en el archivo dataset y posteriormente en buckets, estructuras de almacenamiento de registros. Así mismo, el hash_table es un conjunto de asociaciones key-value en donde el key es la llave a través del cual se puede llegar al value, siendo este el valor en memoria que referencia a dicho bucket, valor en específico.  La estructura extendible hash emula el funcionamiento de una base de datos, la organización, accesibilidad y perdurabilidad de los datos. Los archivos index.bin y data.bin me permiten manejar la perdurabilidad de los registros. Dentro de index.bin se almacenan las asociaciones key-value del hash_table mientras que en data.bin buckets de registros.

### Insercion
Para llevar a cabo la inserción de un registro, es necesario determinar a qué bucket pertenecerá en función de su llave. Esto se logra a través de la búsqueda del índice correspondiente para el registro. Una vez obtenido el índice, se procede a realizar la inserción en el bucket apropiado. Sin embargo, antes de insertar el registro, se realizan comprobaciones adicionales para garantizar la integridad de la base de datos. Esto incluye verificar que el tamaño del bucket esté dentro de límites aceptables.

En caso de que el tamaño del bucket exceda un cierto factor de bloque, se evalúa si es necesario crear nuevos buckets y redistribuir los registros de acuerdo con la función hash. Todo este proceso se realiza con el objetivo de mantener la eficiencia y la organización de la base de datos al tiempo que se asegura la unicidad de las llaves primarias.

### Búsqueda
Primero se busca el bucket asociado a través de la clave (key) en el bucket principal. Si no se encuentra en el bucket principal, se itera en los buckets siguientes (bucket.nextbucket) hasta encontrar el registro deseado.

### Eliminación

La eliminación de registros implica ubicar el registro deseado en el bucket correspondiente y reemplazarlo por el último registro del mismo, actualizando el tamaño del bucket.

## Avl File
El AVL-File es una estructura de datos basada en árboles AVL que se utiliza para almacenar y gestionar registros. Esta estructura combina las propiedades de los árboles AVL (árboles binarios de búsqueda balanceados) con archivos binarios para lograr una organización eficiente de los registros y operaciones de búsqueda, inserción y eliminación.

### Inserción

La inserción de registros en el AVL-File se realiza de manera similar a la inserción en un árbol AVL convencional. Cada registro se inserta en el árbol según un criterio de orden, que generalmente está basado en una clave única, como un valor hash o un identificador único. Durante la inserción, el árbol se reorganiza automáticamente para mantener su propiedad de equilibrio (balance). Si la inserción desequilibra el árbol, se realizan rotaciones para restaurar el equilibrio.

### Búsqueda

La búsqueda de registros en el AVL-File se basa en el mismo principio que la búsqueda en un árbol AVL estándar. Se busca el registro deseado siguiendo el criterio de orden de la clave y realizando comparaciones. La ventaja del AVL-File radica en su capacidad para realizar búsquedas eficientes gracias a la estructura balanceada del árbol.

### Eliminacion

La eliminación de registros en el AVL-File se realiza también siguiendo el principio de un árbol AVL. Se busca el registro a eliminar, se realiza la eliminación y se restaura el equilibrio del árbol mediante rotaciones si es necesario.

## Sequential File
Se trata de un método simple utilizado para el almacenamiento, organización y gestión de datos mediante dos archivos primarios conectados con variables de apunte. Los datos se almacenan en forma secuencial en formato binario, de tal forma que, el registro de datos siempre será continuo y ordenado de menor a mayor según lo programado. 

### Inserción
La inserción para el sequential file y sus dos archivos presenta múltiples casos que deben ser analizados con anterioridad. Se recibe un registro el cual deberá posicionarse donde corresponda según su valor Key. En caso este valor sea menor que los valores ya insertados en el archivo principal, deberá utilizarse un archivo auxiliar con tamaño limitado, esto sin perder el registro anterior y el siguiente al insertado. Cuando el tamaño del archivo auxiliar exceda, se realizará una actualización en el achivo principal con todos los registros ya ordenados secuencialmente.

### Búsqueda
La función de busqueda nos permite encontrar si es que existe algún registro que coincidan con el valor dado o este no se encuentra en la data actual. 

### Eliminación
Este método nos permite eliminar un registro de forma representativa ya que realmente se hace un cambio de estado. Esto permite saltar el registro ya que ya no habría ningun registro que apunte al eliminado como siguiente. 
