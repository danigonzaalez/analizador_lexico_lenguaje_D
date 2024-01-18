#include <stdio.h>
#include <ctype.h>
#include "analizador_lexico.h"
#include "sistema_entrada.h"
#include "tabla_simbolos.h"
#include "definiciones.h"

int aceptado, error;
//variables para saber si se acepta un lexema o si hay algun error
char c;
//caracter actual que se esta leyendo del programa
componente comp_lexico;
//variable para almacenar los componentes lexicos

//funcion que retrocede si es necesario y actualiza el componente lexico con su valor, accediendo a la tabla si se requiere
void _recuperarLexema(int valor, int retroceso) {
    if (retroceso) { //si tenemos que retroceder
        retroceder(); //volvemos al ultimo caracter del lexema
    }
    conseguirLexema(&comp_lexico); //obtenemos la cadena comprendida entre inicio y delantero
    if (valor == ID) { //si es un ID miramos la tabla de simbolos
        comp_lexico.valor = buscar_insertar_tabla(comp_lexico.lexema); //consultamos tabla y devolvemos el codigo
    } else { //si no es un ID
        comp_lexico.valor = valor;
    }
    aceptarLexema(); //actualizo puntero inicio para leer siguiente lexema
}

//automata que acepta alfanumericos
void _alphanumerico() {
    do {
        c = devolverCaracter();
    } while (isalpha(c) || isdigit(c) || c == '_');
    _recuperarLexema(ID, 1); //conseguimos codigo del componente lexico
    aceptado = 1; //actualizamos bandera aceptado
}

//automata que acepta strings (con comillas dobles o simples)
void _strings() {
    int escapado = 0;
    if (c == '"') { //si es un string entre comillas dobles
        do {
            if (c == '\\') { //si se lee una '\'
                escapado = 1;
            } else if (c != '"') {
                escapado = 0;
            }
                c = devolverCaracter(); //cogemos siguiente caracter
        } while (c != '"' || escapado == 1); //mientras no se lea la comilla doble (o si se leyo si se ha escapado)
        _recuperarLexema(STRING, 0);
        aceptado = 1;
    } else { //si es un string entre comillas alternativas
        do {
            if (c == '\\') { //si se lee una '\'
                escapado = 1;
            } else if (c != '`'){
                escapado = 0;
            }
            c = devolverCaracter();
        } while (c != '`' || escapado == 1);
        _recuperarLexema(STRING, 0);
        aceptado = 1;
    }
}

//automata comentario de linea (//----------)
void _comentario_linea() {
    do {
        c = devolverCaracter();
    } while (c != '\n');
    aceptarLexema(); //actualizamos inicio
}

//automata comentario de bloque (/*----*/)
void _comentario_bloque() {
    int estado = 0;
    int fin = 0;
    do {
        switch (estado) {
            case 0: //estado inicial: buscamos el '*' del final del comentario
                do {
                    c = devolverCaracter();
                } while (c != '*');
                estado = 1;
                break;
            case 1: //comprobamos que despues del '*' va '/'
                c = devolverCaracter();
                if (c == '/') { //final del comentario
                    aceptarLexema(); //actualizamos inicio
                    fin = 1;
                } else { //si aun no se llego al final del comentario
                    estado = 0; //volvemos al inicio del automata
                }
                break;
        }
    } while (!fin); //mientras no se cierra el comentario
}

//automata para comentarios anidados
void _comentario_anidado() {
    int estado = 0; //control del switch
    int tipo = -1; //si se prevee un inicio de comentario o un final
    int abiertos = 1; //numero de comentarios anidados abiertos
    do {
        switch (estado) {
            case 0: //estado inicial: buscamos el '+' del final del comentario o el '/' del siguiente anidado
                do {
                    c = devolverCaracter();
                } while (c != '+' && c != '/');
                if (c == '+') {
                    tipo = 0; //si se encontro '+'
                } else {
                    tipo = 1; //si se encontro '/'
                }
                estado = 1; //pasamos a buscar el siguiente caracter
                break;
            case 1:
                c = devolverCaracter();
                if (c == '/' && tipo == 0) { //si se cerro un anidado
                    abiertos--; //decrementamos contador de anidados
                    estado = 0; //volvemos al estado anterior (si no se cerraron todos los anidados aun)
                } else if (c == '+' && tipo == 1) { //si se abrio un anidado
                    abiertos++; //incremento contador de anidados
                    estado = 0; //volvemos al estado anterior
                } else { //si no era ni '+/' ni '/+', volvemos al estado anterior
                    estado = 0; //volvemos al inicio del automata
                }
                break;
        }
    } while (abiertos); //mientras no se cierran todos los anidados
    aceptarLexema(); //una vez que leimos el comentario, actualizamos inicio para saltarlo
}

//automata que acepta operadores y considera comentarios. Puede poner a 0 el estado de sig_comp_lexico
void _operadores(int *state) {
    switch (c) {
        case '+':
            c = devolverCaracter();
            switch (c) {
                case '+':
                    _recuperarLexema(OPERADOR_MAS_MAS, 0);
                    aceptado = 1;
                    break;
                case '=':
                    _recuperarLexema(OPERADOR_MAS_IGUAL, 0);
                    aceptado = 1;
                    break;
                default: //si se encontro un elemento que no forma parte del operador
                    _recuperarLexema('+', 1);
                    aceptado = 1;
                    break;
            }
            break;
        case '-':
            c = devolverCaracter();
            switch (c) {
                case '-':
                    _recuperarLexema(OPERADOR_MENOS_MENOS, 0);
                    aceptado = 1;
                    break;
                case '=':
                    _recuperarLexema(OPERADOR_MENOS_IGUAL, 0);
                    aceptado = 1;
                    break;
                default: //si se encontro un elemento que no forma parte del operador
                    _recuperarLexema('-', 1);
                    aceptado = 1;
                    break;
            }
            break;
        case '=':
            c = devolverCaracter();
            if (c == '=') {
                _recuperarLexema(OPERADOR_IGUAL_IGUAL, 0);
                aceptado = 1;
            } else { //si se encontro un elemento que no forma parte del operador
                _recuperarLexema('=', 1);
                aceptado = 1;
            }
            break;
        case '*':
            c = devolverCaracter();
            if (c == '=') {
                _recuperarLexema(OPERADOR_POR_IGUAL, 0);
                aceptado = 1;
            } else { //si se encontro un elemento que no forma parte del operador
                _recuperarLexema('*', 1);
                aceptado = 1;
            }
            break;
        case '<':
            _recuperarLexema('<', 0);
            aceptado = 1;
            break;
        case '>':
            _recuperarLexema('>', 0);
            aceptado = 1;
            break;
        case '/':
            c = devolverCaracter();
            switch (c) {
                case '=':
                    _recuperarLexema(OPERADOR_ENTRE_IGUAL, 0);
                    aceptado = 1;
                    break;
                case '/': //comentario de linea
                    _comentario_linea();
                    *state = 0; //ponemos de nuevo el estado a 0
                    break;
                case '*': //comentario de bloque
                    _comentario_bloque();
                    *state = 0; //ponemos de nuevo el estado a 0
                    break;
                case '+': //comentario anidado
                    _comentario_anidado();
                    *state = 0; //ponemos de nuevo el estado a 0
                    break;
                default: //si se encontro un elemento que no forma parte del operador
                    _recuperarLexema('/', 1);
                    aceptado = 1;
            }
    }
}

//automata que detecta operadores tales como parentesis, corchetes y llaves
void _operadores_simples() {
    _recuperarLexema(c, 0);
    aceptado = 1;
}

//automata que acepta numeros binarios
void _numeros_binarios() {
    do {
        c = devolverCaracter();
    } while (c == '0' || c == '1');
    _recuperarLexema(INTEGER_LITERAL, 1); //retrocedemos el ultimo caracter y aceptamos
    aceptado = 1;
}

//automata que acepta numeros exponenciales
void _numeros_exponenciales() {
    c = devolverCaracter();
    if (c == '+' || c == '-' || isdigit(c)) { //si a la 'e' o 'E' le sigue un '+', '-' o un digito
        do {
            c = devolverCaracter();
        } while (isdigit(c) || c == '_');
        _recuperarLexema(FLOATING_POINT_LITERAL, 1); //retrocedemos el ultimo caracter y aceptamos
        aceptado = 1;
    }
}

//automata que acepta numeros decimales
void _numeros_con_decimales() {
    do {
        c = devolverCaracter();
    } while (isdigit(c) || c == '_');

    if (c == 'e' || c == 'E') {
        _numeros_exponenciales();
    } else {
        _recuperarLexema(FLOATING_POINT_LITERAL, 1); //retrocedemos el ultimo caracter y aceptamos
        aceptado = 1;
    }
}

//automata que acepta numeros no binarios
void _numeros_no_binarios() {
    do {
        c = devolverCaracter();
    } while (isdigit(c) || c == '_');

    if (c == 'e' || c == 'E') { //si es un numero exponencial
        _numeros_exponenciales();
    } else if (c == '.') { //si es un numero con decimales
        _numeros_con_decimales();
    } else { //si se acabo el numero
        _recuperarLexema(INTEGER_LITERAL, 1); //retrocedemos y aceptamos
        aceptado = 1;
    }
}

//automata que reconoce numeros
void _numeros() {
    if (c == '0') { //miramos posibilidad de que sea un binario
        c = devolverCaracter();
        if (c == 'b' || c == 'B') { //numero binario
            _numeros_binarios(); //automata de numeros binarios
        } else if (isdigit(c) || c == 'e' || c == '_' || c == '.') { //si es el inicio de cualquier otro numero
            //puede ser un numero normal, un exponencial, uno con barra baja o un numero con decimales
            _numeros_no_binarios();
        } else { //el numero '0' se acepta porque el siguiente caracter no es la continuacion del numero
            _recuperarLexema(INTEGER_LITERAL, 1); //cogemos el lexema y retrocedemos el ultimo caracter
            aceptado = 1;
        }
    } else { //puede ser un numero normal, un exponencial, uno con barra baja o un numero con decimales
        _numeros_no_binarios();
    }
}

//funcion que devuelve los componentes lexicos del codigo
componente sig_comp_lexico() {
    aceptado = 0;
    error = 0;
    int estado = 0;

    do {
        switch (estado) {
            case 0:
                c = devolverCaracter();
                if (isblank(c) || c == '\t' || c == '\n' || c == ';' || c == ',' ||
                    c == '.') { //si es uno de estos caracteres
                    if (c == ';' || c == ',' || c == '.') { //si encontramos un ';', una ',' o un '.'
                        _recuperarLexema(c, 0); //formamos componente lexico, pero no retrocedemos
                        aceptado = 1; //salimos del while para devolver el componente
                    }
                    aceptarLexema(); //actualizamos puntero de inicio por si no se entro en el if anterior
                } else if (isalpha(c) || c == '_') { //si empieza por un alfabético o '_'
                    estado = 1; //automata de cadenas alfanumericas
                } else if (c == '"' || c == '`') {
                    estado = 2; //automata de strings
                } else if (c == '/' || c == '*' || c == '+' || c == '-' || c == '=' || c == '<' || c == '>') {
                    estado = 3; //automata de operadores
                } else if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}') {
                    estado = 4; //automata de operadores simples (no aritmeticos, asignacion...)
                } else if (isdigit(c)) { //automata de numeros
                        estado = 5;
                } else if (c == EOF) { //si encontramos un EOF (esto funciona solo para el sistema de entrada simple)
                    comp_lexico.valor = EOF;
                    aceptado = 1;
                }
                break;
            case 1:
                _alphanumerico(); //para IDs y palabras reservadas
                break;
            case 2:
                _strings(); //para strings
                break;
            case 3:
                _operadores(&estado); //para operadores aritmeticos, de asignación...
                break;
            case 4:
                _operadores_simples(); //para operadores simples
                break;
            case 5:
                _numeros(); //automata de numeros*/
        }
    } while (!aceptado && !error);

    return comp_lexico;
}