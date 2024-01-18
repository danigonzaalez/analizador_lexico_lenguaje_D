#ifndef ABIN_H
#define ABIN_H
//Interfaz del TAD abin

////////////////////////////////////////////////////////////////////
//Tipo de dato abstracto para arbol binario y elemento de celda
////////////////////////////////////////////////////////////////////

//TIPO OPACO, no sabemos como esta construido
//https://es.wikipedia.org/wiki/Puntero_opaco
//Dos opciones:
//Opción a:
//      -Importar abin.h en abin.c: #include abin.h
//      -Esto evita repetir la sentencia typedef int tipoelem;
//       en abin.c (sólo se pondría en abin.h)
//      -También evita definir en abin.c el tipo opaco como un puntero a celda:
//       typedef struct celda *abin; (sólo se pondría en abin.h)
//Opción b: (es la que usaremos en esta ocasión)
//      -Podemos lograr mas opacidad usando en abin.h:
//       typedef void *abin;
//      -Y teniendo en abin.c la definicion typedef struct celda *abin;
//      -Pero tenemos que repetir la sentencia typedef int tipoelem;
//       en ambos ficheros (abin.h y abin.c)
// Tenemos que decidir que preferimos: mayor opacidad (opcion b) o menos
// codigo repetido (opcion a).

//En este caso hemos optado por lo primero (opcion b), por lo que:
//-Debemos repetir la definición de tipoelem en ambos ficheros (abin.h y abin.c)
//-Definimos abin como un tipo totalmente opaco en abin.h
//-Definimos abin como un puntero a struct celda en abin.c

//contenido de cada nodo del árbol
//es un árbol de componentes lexicos
typedef struct{
    int valor;
    char *lexema;
} tipoelem;

//definición del tipo opaco
typedef void *abin;

/////////////////////////////// FUNCIONES
/*
 * Crea el arbol vacio.
 * @param A Puntero al arbol. Debe estar inicializado.
 */
void crear(abin *A);

/*
 * Destruye el arbol recursivamente
 * @param A El arbol que queremos destruir
 */
void destruir(abin *A);

/**
 * Comprueba si el arbol esta vacio
 * @param A El arbol binario
 */
unsigned esVacio(abin A);

/**
 * Recupera la informacion de la celda de la raiz del arbol
 * @param A
 * @param E Puntero al nodo que apuntara a la informacion
 */
void leer(abin A, tipoelem *E);

//funcion que inserta un nuevo componente lexico en el arbol
void insertar(abin *A, int valor, char *lexema);

//funcion que busca un lexema y devuelve su valor; si no esta en el arbol lo inserta
int buscar_lexema(abin *A, char *lexema);

//funcion que imprime el arbol
void imprimir(abin *A);

#endif	// ABIN_H
