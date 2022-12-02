#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define TAM_BUS 12
#define NUM_FILAS 8
#define TAM_LINEA 16
#define DIREC_PALABRA 4
#define DIREC_LINEA 3
#define DIREC_ETQ 5
#define TAM_RAM 4096

//Estructura de la cache
typedef struct {
        unsigned char ETQ;
        unsigned char Data[TAM_LINEA];
}T_CACHE_LINE;

//Funciones utilizadas
void hexadecimal_binario(int direccion, int *bin);
void ParsearDireccion(unsigned int addr, int *ETQ, int *palabra, int *linea, int *bloque);
int binario_decimal(int *binario, int num);
void positivo_cache(int time, int direccion, int etiqueta, int linea, int palabra, T_CACHE_LINE *tbl, char *frases, int *alfa);
void VolcarCACHE(T_CACHE_LINE *tbl); 
void TratarFallo(T_CACHE_LINE *tbl, char *MRAM, int ETQ, int linea, int bloque); 
void LimpiarCACHE(T_CACHE_LINE tbl[NUM_FILAS]); 

