#ifndef COMPILADORES_PRACTICA1_SISTEMA_ENTRADA_H
#define COMPILADORES_PRACTICA1_SISTEMA_ENTRADA_H

#include "analizador_lexico.h"

char sig_caracter();

//las siguientes funciones son del sistema de entrada basico

void iniciar_sistema(char *archivo);

char devolverCaracter();

void aceptarLexema();

void retroceder();

void conseguirLexema(componente *comp_lexico);

#endif //COMPILADORES_PRACTICA1_SISTEMA_ENTRADA_H
