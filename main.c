#include <stdio.h>
#include "tabla_simbolos.h"
#include "analizador_sintactico.h"
#include "sistema_entrada.h"

void main(int args, char* argv[]) {
    //--------------inicializacion------------
    inicializar_tabla(); //inicializo la tabla y meto palabras reservadas
    iniciar_sistema(argv[1]); //inicializo el sistema de entrada

    //-----------------ejecucion-----------------
    printf("\n----- TABLA DE SIMBOLOS INICIAL -----\n");
    imprimir_tabla();
    printf("\n----- IDENTIFICACION COMPONENTES LEXICOS -----\n");
    ejecutar_analizador(); //llamo a sintactico ejecutando la fase productor-consumidor
    printf("\n----- TABLA DE SIMBOLOS FINAL -----\n");
    imprimir_tabla();

    //----------------liberacion-----------------
    destruir_tabla();
}
