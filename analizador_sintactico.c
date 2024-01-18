#include <stdio.h>
#include "analizador_sintactico.h"
#include "analizador_lexico.h"

void ejecutar_analizador() {
    componente comp;

    while ((comp = sig_comp_lexico()).valor != EOF) { //bucle while hasta encontrar EOF
            printf("< %d, %s >\n", comp.valor, comp.lexema);
    }

}
