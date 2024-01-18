
#include "tabla_simbolos.h"
#include "definiciones.h"
#include "abin.h"

abin tabla; //tabla de simbolos global

//funcion que crea la tabla e introduce las palabras reservadas
void inicializar_tabla() {
    //creo la tabla
    crear(&tabla);
    //inserto las palabras reservadas
    insertar(&tabla, IMPORT, "import");
    insertar(&tabla, DOUBLE, "double");
    insertar(&tabla, INT, "int");
    insertar(&tabla, WHILE, "while");
    insertar(&tabla, RETURN, "return");
    insertar(&tabla, VOID, "void");
    insertar(&tabla, FOREACH, "foreach");
}

//funcion que busca el componente lexico en la tabla de simbolos y si no esta lo inserta
int buscar_insertar_tabla(char *lexema) {
    int valor = buscar_lexema(&tabla, lexema);
    return valor;
}

void imprimir_tabla() {
    imprimir(&tabla);
}

//funcion que destruye la tabla de simbolos
void destruir_tabla() {
    destruir(&tabla);
}
