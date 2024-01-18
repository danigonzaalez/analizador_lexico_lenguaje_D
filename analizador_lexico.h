#ifndef COMPILADORES_PRACTICA1_ANALIZADOR_LEXICO_H
#define COMPILADORES_PRACTICA1_ANALIZADOR_LEXICO_H

//estructura de los componentes lexicos <numero,"lexema">
typedef struct {
    int valor;
    char *lexema;
}componente;

componente sig_comp_lexico();

#endif //COMPILADORES_PRACTICA1_ANALIZADOR_LEXICO_H
