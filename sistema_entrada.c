#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema_entrada.h"

#define N 10000 //CAMBIAR
//tamanho de los dos buffer centinela

typedef struct {
    char A[N + 1]; //buffer A
    char B[N + 1]; //buffer B
    int inicio; //puntero inicio que señala el inicio de un lexema
    int delantero; //punto delantero que va avanzando por los buffers recorriendo los lexemas
    int control; //variable que indica el buffer que esta activo, sobre el que se esta trabajando
    int cargar; //variable para saber si se debe cargar el buffer
} buffer_centinela;

FILE *fp; //puntero de fichero
buffer_centinela buffer; //buffer
int longitud_lexema = 0; //variable para controlar si el tamanho del lexema excede el tamanho del bloque

//inicializamos la estructura del buffer_centinela
void _inicializar_buffer() {
    buffer.inicio = 0;
    buffer.delantero = 0;
    buffer.control = 1; //ponemos como activo el buffer B para que se empiece cargando el A
    buffer.cargar = 1; //indicamos inicialmente cargar a 1 para el momento de carga
}

//funcion que carga el fichero en el buffer centinela que no esta activo
void _cargar_buffer() {
    int n_bytes;
    /*variable que guarda el numero de caracteres leidos del archivo, para que al cargar el
     * ultimo bloque se pueda poner el EOF justo a continuación, evitando espacios en blanco*/
    if (buffer.control == 1) { //si esta activo el buffer B, cargamos en A y ponemos EOF al final
        n_bytes = fread(buffer.A, 1, N, fp);
        buffer.A[n_bytes] = EOF;
    } else { //cargamos en buffer B y ponemos EOF al final
        n_bytes = fread(buffer.B, 1, N, fp);
        buffer.A[n_bytes] = EOF;
    }
}

//funcion que cambia la variable de control para que cuando se haya cargado un buffer se cambie al otro
void _alternar_buffer() {
    if (buffer.control == 0) { //si estaba activo el buffer A
        buffer.control = 1; //ponemos como activo el B
    } else { //si estaba activo el buffer B
        buffer.control = 0; //ponemos como activo el A
        buffer.delantero = 0; //ponemos delantero a 0
    }
}

//funcion que arranca el sistema de entrada y cargando los primeros N-1 caracteres
void iniciar_sistema_entrada(char *archivo) {
    fp = fopen(archivo, "r");

    if (!fp) { //si no se abrio correctamente
        printf("Error al abrir el fichero con el codigo fuente\n");
        exit(1);
    }

    _inicializar_buffer(); //inicializamos estructura buffer_centinela
    _cargar_buffer(); //cargamos el buffer correspondiente
    _alternar_buffer(); //modificamos el control de la estructura para que le toque al otro buffer que se cargue
}

//funcion que obtiene el siguiente caracter del buffer
char sig_caracter() {
    char c;
    if (buffer.control == 0) { //estamos en el buffer A
        c = buffer.A[buffer.delantero]; //cogemos el caracter apuntado por delantero
        if (c == EOF) { //si leimos un EOF
            //comprobamos si es el EOF de final de buffer o si es el EOF de fin de fichero
            if (!feof(fp)) { //si no se llego al EOF del fichero
                if (buffer.cargar == 1) { //si se necesita cargar el bloque no activo lo cargamos
                    _cargar_buffer();
                } else { //si no se necesitaba cargar aun
                    buffer.cargar = 1; //lo indicamos para la proxima iteracion
                }
                _alternar_buffer(); //cambiamos el buffer de trabajo
                c = sig_caracter(); //se habia leido un EOF y queremos el siguiente caracter
            } else {
                buffer.delantero++; //movemos el delantero
                longitud_lexema++; //incrementamos longitud del lexema
            }
        } else { //si no se leyo un EOF, actualizamos delantero y la longitud del lexema a leer
            buffer.delantero++;
            longitud_lexema++;
        }
    } else { //estamos en el buffer B
        c = buffer.B[buffer.delantero - N]; //cogemos el siguiente caracter del buffer B
        if (c == EOF) { //si se leyo un EOF
            if (!feof(fp)) {
                if (buffer.cargar == 1) {
                    _cargar_buffer();
                } else {
                    buffer.cargar = 1;
                }
                _alternar_buffer();
                c = sig_caracter();
            } else {
                buffer.delantero++;
                longitud_lexema++;
            }
        } else { //si no se leyo un EOF, actualizamos delantero y la longitud del lexema
            buffer.delantero++;
            longitud_lexema++;
        }
    }
    return c;
}

//------------PRUEBAS--------------//
void iniciar_sistema(char *archivo) {
    fp = fopen(archivo, "r");

    if (!fp) { //si no se abrio correctamente
        printf("Error al abrir el fichero con el codigo fuente\n");
        exit(1);
    }

    int n_elementos = fread(buffer.A, 1, N, fp); //cargo el buffer
    buffer.A[n_elementos] = EOF; //meto el EOF en la ultima posicion

    buffer.inicio = 0;
    buffer.delantero = 0;
}

char devolverCaracter() { //devuelve el caracter apuntado por delantero y lo actualiza
    char c = buffer.A[buffer.delantero];
    buffer.delantero++;
    return c;
}

void aceptarLexema() { //actualiza inicio para ponerlo al nivel de delantero
    buffer.inicio = buffer.delantero;
}

void retroceder() {
    buffer.delantero--;
}

void conseguirLexema(componente *comp_lexico) { //guarda el lexema comprendido entre inicio y delantero
    comp_lexico->lexema = (char *)malloc(buffer.delantero-buffer.inicio);
    strncpy(comp_lexico->lexema, buffer.A + buffer.inicio, buffer.delantero - buffer.inicio);
}
