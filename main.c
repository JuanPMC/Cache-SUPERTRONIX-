#include <stdio.h>
#include <stdlib.h>

#define TAMANIO_LINEA 8
#define CANTIDAD_LINEAS 4
#define TAMANIO_RAM 1024

typedef struct {    // una linea de chache
    short int ETQ;
    short int Datos[TAMANIO_LINEA];
} T_LINEA_CACHE;

T_LINEA_CACHE cahce[CANTIDAD_LINEAS];

unsigned char RAM[TAMANIO_RAM];

int tiempoglobal, numerofallos;

void cargarRamEnRam(){
    FILE* ramBinary;
    ramBinary = fopen("RAM.bin","rb");  // r para leer y b de binario
    fread(RAM,TAMANIO_RAM,1,ramBinary); // leer el fichero y cargarlo en la ram
    fclose(ramBinary);
}

void arranque(){
    // inicializamos la cache
    for(int i = 0; i < CANTIDAD_LINEAS; i++){
        cahce[i].ETQ = 0XFF;
        for(int j = 0; j < TAMANIO_LINEA; j++)
            cahce[i].Datos[j];
    }
    cargarRamEnRam();
}

int main(){
    tiempoglobal = numerofallos = 0;
    arranque();
    printf("Test RAM: %d", RAM[0X02E4]);
    return 0;
}
