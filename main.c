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
    int bin = 0;
    while(dec > 0){
        rem = dec%2;
        dec = dec/2;
        bin = bin + (c * rem);
        c = c * 10;
    }
    return bin;
}
char* toCadena(int bin){
    static char snum[11];
    sprintf(snum,"%d", bin);
    return snum;
}
int sacarPalabra(int bin){
    int palabra= (bin-((sacarEtiqueta(bin)*100000)+(sacarLinea(bin)*1000)));
    return palabra;
}

int* peticiones_de_lectura(){
    FILE *accesomem;
    char linea[6];
    static int resultado[100];
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

int sacarLinea(int bin){
    int linea= (bin - sacarEtiqueta(bin) * 100000)/1000;
    return linea;
}

int sacarEtiqueta(int bin){
    return (int) (bin / 100000);
}
int pow(int num, int exp){
    if (!exp) return 1;
    return pow(num , exp- 1 ) * num;
}
int aDecimal(int bin){
    int decim = 0;
    int i = 0;
    while (bin){
        decim = bin%10 * pow(2,i);
        bin /= 10;
        i++;
    }
    return decim;
}
void cargarDeRam(int bin){
    int linea = aDecimal(sacarLinea(bin));
    int pos = aDecimal(sacarLinea(bin)*1000 + sacarEtiqueta(bin)*100000);
    printf("Cargado bloque: %04X en Linea: %02X\n", pos,linea);
    cahce[linea].ETQ = aDecimal(sacarEtiqueta(bin));
    for (int i = 0; i < TAMANIO_LINEA; i++)
        cahce[linea].Datos[TAMANIO_LINEA - 1 - i] = RAM[pos + i];
}
void mostrarCache(){
    for(int j = 0; j < CANTIDAD_LINEAS; j++){
        printf("ETQ:%X Datos " , cahce[j].ETQ);
        for(int i = 0; i < TAMANIO_LINEA; i++)
            printf(" %02X ", cahce[j].Datos[i]);
        printf("\n");
    }
}

int main(){
    int *accesosAmemoria;
    tiempoglobal = numerofallos = 0;

    arranque();
    accesosAmemoria = peticiones_de_lectura();

    int ACESOA = 10;

    printf("Test RAM: %X, Test Lectura: %s", RAM[1], toCadena(accesosAmemoria[ACESOA]));
    printf("Line: %02X Etiqueta: %02X \n", sacarLinea(accesosAmemoria[ACESOA]) , sacarEtiqueta(accesosAmemoria[ACESOA]));

    mostrarCache();
    cargarDeRam(accesosAmemoria[ACESOA]);
    mostrarCache();

    return 0;
}
