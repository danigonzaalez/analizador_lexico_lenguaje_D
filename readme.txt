INFORMACION GENERAL:
	- Esta es la versión 0 del análisis léxico
	- Funciona con un sistema de entrada simple, usando un buffer de tamaño 10000. Por lo tanto
		* Las funciones del sistema de entrada que se usan, son las que estan a partir del comentario //---PRUEBAS---//, pues se probó a hacer una implementación con doble buffer que aún no 		está operativa
	- No obstante, el análisis léxico está finalizado con todos sus autómatas

PARA COMPILAR Y EJECUTAR:
	- Desde el directorio donde está el código fuente, usar:
		* gcc -Wall *.c -o analizadorLexico
		* ./analizadorLexico fichero
		
PORBADO EN UNA MÁQUINA Ubuntu 20.04.5 LTS CON:
	- Versión de GCC: gcc (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0
