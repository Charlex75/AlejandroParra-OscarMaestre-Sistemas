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

//Oscar se encargaba de hacer el main y Alex hizo los comandos relacionados con el uso de los ficheros y los mensages que se printan al final
int main(){
        //Variables inizializadas
        int globaltime=0;
        int numfallos=0;
        int numaccesos=0;
        float tiempomedio=0;

        int etiqueta[DIREC_ETQ]={0};
        int linea[DIREC_LINEA]={0};
        int palabra[DIREC_PALABRA]={0};

        int etiqueta_int=0;
        int linea_int=0;
        int palabra_int=0;

        int alfa=0;
        int bloque=0;
        unsigned int direccion=0;
        int hexa[TAM_BUS]={0};
        unsigned char Simul_RAM[TAM_RAM], frases[100];


        //Estructura de cache inizializada
        T_CACHE_LINE cache[NUM_FILAS];
