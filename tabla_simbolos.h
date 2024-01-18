
#ifndef COMPILADORES_PRACTICA1_TABLA_SIMBOLOS_H
#define COMPILADORES_PRACTICA1_TABLA_SIMBOLOS_H

//funcion que crea la tabla de simbolos y va insertando todas las palabras reservadas en el
void inicializar_tabla();

//funcion que busca el componente lexico en la tabla de simbolos y si no esta lo inserta
int buscar_insertar_tabla(char *lexema);

//funcion que imprime la tabla de simbolos
void imprimir_tabla();

//funcion que destruye la tabla de simbolos
void destruir_tabla();

#endif //COMPILADORES_PRACTICA1_TABLA_SIMBOLOS_H
