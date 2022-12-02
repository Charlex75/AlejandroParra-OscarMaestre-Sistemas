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

	//Abrir los dos ficheros y comprobar si se an abierto correctamente
    FILE *contenido_ram;
    contenido_ram = fopen("CONTENTS_RAM.bin", "rb");
	if(contenido_ram  == NULL){
		printf("Error, no se puede abrir CONTENTS_RAM.bin\n");
		return -1;
    	}
	FILE *accesos_memoria;
	accesos_memoria = fopen("accesos_memoria.txt", "r");
	if(accesos_memoria == NULL){
		printf("Error, no se puede abrir accesos_memoria.txt\n");
        	return -1;
    	}

	//Crear el fichero y comprobar si se a creado correctamente
	FILE *contenido_cache;
	contenido_cache = fopen("CONTENTS_CACHE.bin", "wb");
	if(contenido_cache == NULL){
                printf("Error, no se puede crear CONTENTS_CACHE.bin\n");
        	return -1;
	}

 //Copio los contenidos de contenido_ram en Simul_RAM y limpio el posible contenido de la cache
    fread(Simul_RAM, 1, TAM_RAM, contenido_ram);
    LimpiarCACHE(cache);

    //Bucle principal que no se detiene hasta que llege al final de accesos_memoria
    while(fscanf(accesos_memoria, "%X", &direccion)!=EOF){

        //Divido direccion en etiqueta palabra y linea, tambien calcule en que bloque le correspondia
        ParsearDireccion(direccion, etiqueta, palabra, linea, &bloque);

        //Paso de binario a hexadecimal
        etiqueta_int=binario_decimal(etiqueta,5);
        linea_int=binario_decimal(linea,3);
        palabra_int=binario_decimal(palabra,4);

        //Compruebo si la etiqueta de la linea es igual a la etiqueta de la direccion y en consecuencia hacer lo pedido por el enunciado
        if(cache[linea_int].ETQ==etiqueta_int){
            positivo_cache(globaltime, direccion, etiqueta_int, linea_int, palabra_int, cache, frases, &alfa);
        }else{
            numfallos++;
            printf("T: %d, Fallo de CACHE %d, ADDR %04X Label %X linea %02X palabra %02X bloque %02X\n", globaltime, numfallos, direccion, etiqueta_int, linea_int, palabra_int, bloque);
            globaltime=globaltime+10;
            TratarFallo(cache, Simul_RAM, etiqueta_int, linea_int, bloque);
            printf("Cargando el bloque %02X en la linea %02X\n", bloque, linea_int);
            positivo_cache(globaltime, direccion, etiqueta_int, linea_int, palabra_int, cache, frases, &alfa);
        }

        //Volcar la cache por pantalla, aumentar en 1 el numero de accesos y hacer una pausa de 1 segundo
        VolcarCACHE(cache);
        numaccesos++;
        sleep(1);
    }
    //Se calcula el tiempo promedio transcurrido y se imprime por pantalla los datos establecidos por el programa y el texto leido
	tiempomedio=(float)globaltime/(float)numaccesos;
	printf("Accesos totales: %d; fallos: %d; Tiempo medio: %f\n", numaccesos, numfallos, tiempomedio);
	printf("Texto leido: %s\n", frases);

	//Pasar los datos de la cache en el fichero previamente creado
	for(int e=0; e<NUM_FILAS; e++){
                fwrite(cache[e].Data, TAM_LINEA, 1, contenido_cache);
        }

	//Cerrar los tres ficheros y devolver 0 para acabar el programa
	fclose(contenido_ram);
        fclose(accesos_memoria);
        fclose(contenido_cache);
        return 0;

}

//Alex hizo esta funcion
//Una funcion que se dedica a pasar valores hexadecimales a binarios cada vez que se invoca
void hexadecimal_binario(int direccion, int *bin){
	div_t dividir;
        for(int a=0; a<TAM_BUS; a++){
                dividir=div(direccion, 2);
                direccion=dividir.quot;
                bin[11-a]=dividir.rem;
        }
}

//Alex hizo esta funcion
//Una funcion que se dedica a pasar valores binarios a decimales cada vez que se invoca
int binario_decimal(int *binario, int num){
	int respuesta=0;
	
	if(num==3){
		respuesta=binario[0]*4+binario[1]*2+binario[2]*1;
	}
	if(num==4){
                respuesta=binario[0]*8+binario[1]*4+binario[2]*2+binario[3]*1;
        }
	if(num==5){
                respuesta=binario[0]*16+binario[1]*8+binario[2]*4+binario[3]*2+binario[4]*1;
        }


	return respuesta;
} 


