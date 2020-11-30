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
void arranque(){
    // inicializamos la cache
    for(int i = 0; i < CANTIDAD_LINEAS; i++){
        cahce[i].ETQ = 0XFF;
        for(int j = 0; j < TAMANIO_LINEA; j++)
            cahce[i].Datos[j]=0;
    }
    cargarRamEnRam();
    FILE *accesomem;
    char linea[12];
    char *resultado;
    int memoria;
    char etiqueta;
    char *ptr;
    accesomem = fopen("accesos_memoria.txt","rt");
    if (accesomem == NULL){
        //exit(-1)
        printf("fallo al abrir el fichero");
    }
    else{
        int a;
        while(a!=13){
            for(int b=0;b<=12;b++){
                printf("Hex:%s",linea);//Los tres printf de este for son orientativos para ver si se esta haciendo bien
                memoria=strtol(linea,&ptr,16);
                printf("Dec y entero:%d\n",memoria);
                printf("Etiqueta:%d\n",decimal_a_binario(memoria));
                fgets(linea, 12,accesomem);
                a++;
            }    
        }
        fclose(accesomem);
    }
}
int main(){
    tiempoglobal = numerofallos = 0;
    arranque();
    printf("Test RAM: %d", RAM[1]);
    return 0;
}
