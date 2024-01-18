#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definiciones.h"

/////////////////////REPETIMOS LA DEFINICIÓN DE LOS TIPOS DE DATOS
//abin es un puntero a struct celda
typedef struct celda *abin;
//contenido de cada nodo del árbol

//es un árbol de componentes lexicos
typedef struct{
    int valor;
    char *lexema;
} tipoelem;

/////////////////////ESTRUCTURAS DE DATOS
struct celda{
	tipoelem info;
	struct celda *izq, *der;
};

///////////////////////FUNCIONES

//Crea el árbol vacío
void crear(abin *A){
	*A=NULL;
}

//Destruye el árbol recursivamente
//Al elegir la opción b) de implementación, necesito los prototipos
//para poder llamar a estas funciones (con la opción a)#include abb.h, ya
//estarían definidos estos prototipos)
void supizq(abin *A);
void supder(abin *A);
unsigned esVacio(abin A);
void destruir(abin *A){
	abin aux;
	aux=*A;
	if(!esVacio(aux)){
	  supizq(&aux);
	  supder(&aux);
	  free(aux);
	  *A=NULL;
	}	
}

//Comprueba si el árbol está vacío
unsigned esVacio(abin A){
	return (A==NULL);
}

//Devuelve el subárbol izquierdo de A
abin izq(abin A){
	return A->izq;
}

//Devuelve el subárbol derecho de A
abin der(abin A){
	return A->der;
}

//Recupera la informacion de la celda de la raiz del arbol
void leer(abin A, tipoelem *E){
	*E=A->info;
}

//Suprime el subárbol a la izquierda de A
void supizq(abin *A){
	abin aux;
	aux=izq(*A);
	if(!esVacio(aux)){
	  supizq(&aux);
	  supder(&aux);	
	  (*A)->izq=NULL;
	  free(aux);
	}	
}

//Suprime el subárbol a la derecha de A
void supder(abin *A){
	abin aux;
	aux=der(*A);
	if(!esVacio(aux)){
	  supizq(&aux);
	  supder(&aux);	
	  (*A)->der=NULL;
	  free(aux);
	}	
}

//funcion que inserta un nuevo componente lexico en el arbol
void insertar(abin *A, int valor, char *lexema) {
    if (esVacio(*A)) {
        *A = (abin)malloc(sizeof(struct celda));
        (*A)->info.lexema = (char*) malloc(strlen(lexema) + 1 * sizeof(char));
        strcpy((*A)->info.lexema, lexema);
        (*A)->info.lexema[strlen(lexema)] = '\0';
        (*A)->info.valor = valor;
        (*A)->izq = NULL;
        (*A)->der = NULL;
        return;
    }

    int comparacion = strcmp(lexema, (*A)->info.lexema);

    if (comparacion > 0) {
        insertar(&(*A)->der, valor, lexema);
    } else {
        insertar(&(*A)->izq, valor, lexema);
    }

}

//funcion que busca un lexema y devuelve su valor; si no esta en el arbol lo inserta
int buscar_lexema(abin *A, char *lexema) {

    if (esVacio(*A)) {
        insertar(A, ID, lexema);
        return ID;
    }

    int comp = strcmp(lexema, (*A)->info.lexema);
    if (comp == 0) {
        return (*A)->info.valor;
    } else if (comp < 0) {
        return (buscar_lexema(&(*A)->izq, lexema));
    } else {
        return (buscar_lexema(&(*A)->der, lexema));
    }

}

void imprimir(abin *A) {
    if (!esVacio(*A)) {
        if (&(*A)->izq != NULL) {
            imprimir(&(*A)->izq);
            printf("Lexema: %10s", (*A)->info.lexema);
        }

        printf("%10s: %-5d\n", "Valor", (*A)->info.valor);

        if (&(*A)->der != NULL) {
            imprimir(&(*A)->der);
        }
    }
}
