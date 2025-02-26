#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;

void operar_vectores_int(char *operacion, int resultado[], int vector1[], int vector2[], int longitud) {
    switch(operacion[0]){
        case '+':
            for (int i = 0; i < longitud; i++) {
                resultado[i] = vector1[i] + vector2[i];
                }
            break;
        case '-':
            for (int i = 0; i < longitud; i++) {
                resultado[i] = vector1[i] - vector2[i];
            }
            break;
        case '*':
            for (int i = 0; i < longitud; i++) {
                resultado[i] = vector1[i] * vector2[i];
            }
            break;
        case '/':
            for (int i = 0; i < longitud; i++) {
                resultado[i] = vector1[i] / vector2[i];
            }
            break;
        default:
            printf("Opcion no valida");
            break;
    }   
}


void operar_matrices_int(char* operacion, int filas, int columnas, int resultado[][columnas], int matriz1[][columnas], int matriz2[][columnas]) {
    switch(operacion[0]){
        case '+':
            for (int i = 0; i < filas; i++) {
                for (int j = 0; j < columnas; j++) {
                    resultado[i][j] = matriz1[i][j] + matriz2[i][j];
                }
            }
            break;
        case '-':
            for (int i = 0; i < filas; i++) {
                for (int j = 0; j < columnas; j++) {
                    resultado[i][j] = matriz1[i][j] - matriz2[i][j];
                }
            }
            break;
        case '*':
            for (int i = 0; i < filas; i++) {
                for (int j = 0; j < columnas; j++) {
                    resultado[i][j] = matriz1[i][j] * matriz2[i][j];
                }
            }
            break;
        case '/':
            for (int i = 0; i < filas; i++) {
                for (int j = 0; j < columnas; j++) {
                    resultado[i][j] = matriz1[i][j] / matriz2[i][j];
                }
            }
            break;
        default:
            printf("Opcion no valida");
            break;
    }   
}

void operar_vectores_float(char *operacion, float resultado[], float vector1[], float vector2[], int longitud) {
    switch(operacion[0]){
        case '+':
            for (int i = 0; i < longitud; i++) {
                resultado[i] = vector1[i] + vector2[i];
                }
            break;
        case '-':
            for (int i = 0; i < longitud; i++) {
                resultado[i] = vector1[i] - vector2[i];
            }
            break;
        case '*':
            for (int i = 0; i < longitud; i++) {
                resultado[i] = vector1[i] * vector2[i];
            }
            break;
        case '/':
            for (int i = 0; i < longitud; i++) {
                resultado[i] = vector1[i] / vector2[i];
            }
            break;
        default:
            printf("Opcion no valida");
            break;
    }   
}


void operar_matrices_float(char* operacion, int filas, int columnas, float resultado[][columnas], float matriz1[][columnas], float matriz2[][columnas]) {
    switch(operacion[0]){
        case '+':
            for (int i = 0; i < filas; i++) {
                for (int j = 0; j < columnas; j++) {
                    resultado[i][j] = matriz1[i][j] + matriz2[i][j];
                }
            }
            break;
        case '-':
            for (int i = 0; i < filas; i++) {
                for (int j = 0; j < columnas; j++) {
                    resultado[i][j] = matriz1[i][j] - matriz2[i][j];
                }
            }
            break;
        case '*':
            for (int i = 0; i < filas; i++) {
                for (int j = 0; j < columnas; j++) {
                    resultado[i][j] = matriz1[i][j] * matriz2[i][j];
                }
            }
            break;
        case '/':
            for (int i = 0; i < filas; i++) {
                for (int j = 0; j < columnas; j++) {
                    resultado[i][j] = matriz1[i][j] / matriz2[i][j];
                }
            }
            break;
        default:
            printf("Opcion no valida");
            break;
    }   
}

void operar_vectores_char(char *operacion, char resultado[], char vector1[], char vector2[], int longitud) {
    switch(operacion[0]){
        case '+':
            for (int i = 0; i < longitud; i++) {
                resultado[i] = vector1[i] + vector2[i];
                }
            break;
        case '-':
            for (int i = 0; i < longitud; i++) {
                resultado[i] = vector1[i] - vector2[i];
            }
            break;
        case '*':
            for (int i = 0; i < longitud; i++) {
                resultado[i] = vector1[i] * vector2[i];
            }
            break;
        case '/':
            for (int i = 0; i < longitud; i++) {
                resultado[i] = vector1[i] / vector2[i];
            }
            break;
        default:
            printf("Opcion no valida");
            break;
    }   
}


void operar_matrices_char(char* operacion, int filas, int columnas, char resultado[][columnas], char matriz1[][columnas], char matriz2[][columnas]) {
    switch(operacion[0]){
        case '+':
            for (int i = 0; i < filas; i++) {
                for (int j = 0; j < columnas; j++) {
                    resultado[i][j] = matriz1[i][j] + matriz2[i][j];
                }
            }
            break;
        case '-':
            for (int i = 0; i < filas; i++) {
                for (int j = 0; j < columnas; j++) {
                    resultado[i][j] = matriz1[i][j] - matriz2[i][j];
                }
            }
            break;
        case '*':
            for (int i = 0; i < filas; i++) {
                for (int j = 0; j < columnas; j++) {
                    resultado[i][j] = matriz1[i][j] * matriz2[i][j];
                }
            }
            break;
        case '/':
            for (int i = 0; i < filas; i++) {
                for (int j = 0; j < columnas; j++) {
                    resultado[i][j] = matriz1[i][j] / matriz2[i][j];
                }
            }
            break;
        default:
            printf("Opcion no valida");
            break;
    }   
}

void asignar_vectores_int( int resultado[], int vector1[], int longitud){
    for (int i = 0; i < longitud; i++) {
        resultado[i] = vector1[i];
    }
}

void asignar_matrices_int( int filas, int columnas, int resultado[][columnas], int matriz1[][columnas]){
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++){
            resultado[i][j] = matriz1[i][j];
        }
    }
}

void asignar_vectores_float( float resultado[], float vector1[], int longitud){
    for (int i = 0; i < longitud; i++) {
        resultado[i] = vector1[i];
    }
}

void asignar_matrices_float( int filas, int columnas, float resultado[][columnas], float matriz1[][columnas]){
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++){
            resultado[i][j] = matriz1[i][j];
        }
    }
}

void asignar_vectores_char( char resultado[], char vector1[], int longitud){
    for (int i = 0; i < longitud; i++) {
        resultado[i] = vector1[i];
    }
}

void asignar_matrices_char( int filas, int columnas, char resultado[][columnas], char matriz1[][columnas]){
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++){
            resultado[i][j] = matriz1[i][j];
        }
    }
}