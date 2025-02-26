#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
	marca, 				// Marca de inicio de bloque
	funcion, 			// Funcion
	variable, 			// Variable
	parametro_formal	// Parametro formal
} tipoEntrada;

typedef enum {
	entero,
	real,
	caracter,
	booleano,
	array,
	desconocido
} dTipo;

typedef struct {
	tipoEntrada entrada;
	char *nombre;
	dTipo tipoDato;
	unsigned int parametros;
	unsigned int dimensiones;
	int TamDimen1;				// Dimension 1
	int TamDimen2;				// Dimension 2
} entradaTS;

typedef struct {
	int atrib;
	char *lexema; 
	dTipo tipo;

	unsigned int dimensiones;
	int TamDimen1;				// Dimension 1
	int TamDimen2;				// Dimension 2
} atributos;

#define MAX_TS 500
#define MAX_TF 500
#define YYSTYPE atributos

extern unsigned int TOPE;
extern unsigned int subProg;
extern dTipo tipoGlobal;
extern int lineaActual;
extern int funcionActual;
extern int numParam;
extern int subBloque;
extern int lista_declaracion;

extern entradaTS TS[MAX_TS];

/**
 * Almacena en la variable global tipo el tipo de la variable
 */
int tsAsignaTipoGlobal(dTipo tipo);

// ------------  Funciones de manejo de la Tabla de Simbolos  ------------

/**
  * Inserta una entrada a la tabla de simbolos e incrementa el TOPE en 1
**/
int tsInsertaEntrada(tipoEntrada entrada, char *nombre, dTipo tipo, unsigned int parametros,
						unsigned int dimensiones, int TamDimen1, int TamDimen2);
/**
  * Elimina el elemento TOPE de tabla de simbolos y decrementa el TOPE en 1
**/
int tsDelEntrada();

/**
 * Elimina de la tabla de simbolos todas las entradas hasta la ultima marca de inicio de bloque, tambien incluida
 */
void tsVaciarEntradas();

/**
 * Busca un identificador en la tabla de simbolos
 */
int tsBuscarIdent(char *ident, tipoEntrada ambito);

/**
 * Inserta una marca de comienzo de un bloque
 */
void tsInsertaMarca();

/**
 * Inserta un nuevo identificador en la tabla de simbolos
 */
void tsInsertaIdent(atributos elem);

/*
 * Inserta una entrada en la tabla de simbolos en funcion de si
 * se esta declarando una variable o un parametro formal. 
*/
void tsInserta(atributos elem);

/**
 * Inserta una entrada de subprograma en la tabla de simbolos
*/
void tsInsertaSubprog(atributos elem);

/**
 * Inserta una entrada de parametro formal de un subprograma en la tabla de simbolos
 */
void tsInsertaParamFormal(atributos elem);

/**
 * Actualiza el número de parámetros de la función actual
 */
void tsActualizaNumParam();

/**
 * Asigna el tipo de variable a devolver
 */
void tsAsignaTipoDevolver(atributos* devolver);

/**
 * Impime la tabla de simbolos
*/
void tsImprimeTabla();

/* Fin de funciones y procedimientos para manejo de la TS */

/**
 * Copia los valores de un identificador de la tabla de simbolos a los atributos
*/
void tsGetIdent(atributos e1, atributos *e2);

/**
 * Comprueba que los tipos de los operandos de una operacion son compatibles
 */
void tsComprobarTipos(dTipo tipoElem1, dTipo tipoElem2);

/**
 * Comprueba que los tipos de los operandos de una operacion son compatibles
 */
void tsComprobarDimensiones(atributos elem1, atributos elem2);

/**
 * Asigna el tipo de la función actual
 */
void tsComprobarDevolver(dTipo tipoElem);

/**
 * Comprueba que los tipos de datos en un case switch son compatibles
 */
void tsComprobarCasos(dTipo tipoElem1, dTipo tipoElem2, atributos *valores);

/**
 * Comprueba que el tipo de dato de una expresion es booleano
 */
void tsComprobarBooleano(dTipo tipoElem);

/**
 * Comprueba que el tipo de dato de una expresion es entero
 */
void tsComprobarEntero(dTipo tipoElem);

/**
 * Comprueba la compatibilidad de una asignacion
*/
void tsComprobarAsignacion(char *ident, atributos elem2, int dimension);

/**
 * Comprueba que el tipo de parámetro pasado se corresponde con el de la función
*/
void tsComprobarParametro(int numP, atributos elem);

/**
 * Comprueba que el identificador pasado existe en la tabla de simbolos
*/
int tsComprobarIdentExiste(char *ident);

/**
 * Comprueba que el numero de parametros de una funcion es correcto
*/
void tsComprobarNumeroParam();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//								GENERACION DE CODIGO

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern FILE* files[2];

typedef struct {
char* EtiquetaEntrada ;
char* EtiquetaSalida ;
char* EtiquetaElse ;
char* NombreVarControl ;
} etiquetaFlujo ;

extern int TOPEFLUJO;
extern etiquetaFlujo TF[MAX_TF];
extern int temp;
extern int etiqueta;
extern int n_file;
extern int n_subprogramas_anidados;

void generarFichero();

void generarFicheroFunciones();

void cerrarFichero(int n_file);

void cerrarFicheroFunciones();

char* tipoDeDato (dTipo td);

char* tipoDeBool (int verdadero);

char tipoAFormato(dTipo dato);

void escribirEntrada(atributos elem);

void escribirSalida(atributos elem);

// void introducirTipo(dTipo dt);

char* generarTemp(atributos elem);

char* generarEtiqueta();

void copiarEF(etiquetaFlujo *dest, etiquetaFlujo *source);

void insertarFlujo (etiquetaFlujo s);

void sacarTF();