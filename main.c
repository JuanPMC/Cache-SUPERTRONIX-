#include <stdio.h>
#include <stdlib.h>
#define TAMANIO_LINEA 8
#define CANTIDAD_LINEAS 4
#define TAMANIO_RAM 1024

//ETQ=5bits,linea=2bits,palabra=3bits
typedef struct {    // una linea de chache
    short int ETQ;
    short int Datos[TAMANIO_LINEA];
} T_LINEA_CACHE;

T_LINEA_CACHE cahce[CANTIDAD_LINEAS];
unsigned char RAM[TAMANIO_RAM];

int tiempoglobal, numerofallos;

void cargarRamEnRam(){
    FILE *ramBinary;
    ramBinary = fopen("RAM.bin","rb");  // r para leer y b de binario
    fread(RAM,TAMANIO_RAM,1,ramBinary); // leer el fichero y cargarlo en la ram
    fclose(ramBinary);
}

int decimal_a_binario(int dec){//Con esta funcion nos va a salir el binario empezando por el primer 1 de mayor orden
    int rem, c = 1;
    long int bin = 0;
    while(dec > 0){
        rem = dec%2;
        dec = dec/2;
        bin = bin + (c * rem);
        c = c * 10;
    }
    return bin;
}

long int* peticiones_de_lectura(){
    FILE *accesomem;
    char linea[6];
    static long int resultado[100];
    int memoria;
    int i;

    // abrir fichero
    accesomem = fopen("accesos_memoria.txt","rt");

    // gestion de fallos
    if (accesomem == NULL){
        return NULL;
        printf("fallo al abrir el fichero");
    }

    // operar con fichero
    for(i = 0; fgets(linea, 6,accesomem); i++){
        memoria=strtol(linea,NULL,16); // pasar a base 10 y a entero
        resultado[i] = decimal_a_binario(memoria);  // guardar en array de accesos
    }
    resultado[i] = -1; // senializar el final del buffer

    // cerrar fichero
    fclose(accesomem);

    return resultado; // retornar el array
}

void arranque(){
    // inicializamos la cache
    for(int i = 0; i < CANTIDAD_LINEAS; i++){
        cahce[i].ETQ = 0XFF;
        for(int j = 0; j < TAMANIO_LINEA; j++)
            cahce[i].Datos[j]=0;
    }
    cargarRamEnRam();
}

int main(){
    long int *accesosAmemoria;
    tiempoglobal = numerofallos = 0;

    arranque();
    accesosAmemoria = peticiones_de_lectura();

    printf("Test RAM: %d, Test Lectura: %d", RAM[1], accesosAmemoria[1]);

    return 0;
}
