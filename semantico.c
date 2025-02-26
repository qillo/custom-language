#include "semantico.h"

entradaTS TS[MAX_TS];
unsigned int TOPE = 0;
unsigned int subProg = 0;
dTipo tipoGlobal = desconocido;
int lineaActual = 1;
int funcionActual = -1;
int numParam = 0;
int subBloque = 0;
int lista_declaracion = 0;

/**
 * Almacena en la variable global tipo el tipo de la variable
 */
int tsAsignaTipoGlobal(dTipo tipo) {
	tipoGlobal = tipo;
}

// ------------  Funciones de manejo de la Tabla de Simbolos  ------------

/**
  * Inserta una entrada a la tabla de simbolos 
**/
int tsInsertaEntrada(tipoEntrada entrada, char *nombre, dTipo tipo, unsigned int parametros,
						unsigned int dimensiones, int TamDimen1, int TamDimen2) {
	
	if (TOPE < MAX_TS) {
		TS[TOPE].entrada = entrada;
		TS[TOPE].nombre = nombre;
		TS[TOPE].tipoDato = tipo;
		TS[TOPE].parametros = parametros;
		TS[TOPE].dimensiones = dimensiones;
		TS[TOPE].TamDimen1 = TamDimen1;
		TS[TOPE].TamDimen2 = TamDimen2;

		TOPE++;

		// tsImprimeTabla();
		return 1;
	} else {
		printf("ERROR PILA LLENA");
		return 0;
	}
};

/**
  * Elimina el elemento TOPE de tabla de simbolos 
**/
int tsDelEntrada() {
	// Si la pila no está vacía. 
	if(TOPE > 0){
		TOPE--;
		return 1;
	}else{
		printf("ERROR: PILA VACIA");
		return 0;
	}
};

/**
 * Elimina de la tabla de simbolos todas las entradas hasta la ultima marca de inicio de bloque, tambien incluida
 */
void tsVaciarEntradas() {
	while (TS[TOPE-1].entrada != marca && TOPE > 0) { 
		TOPE--;
	} 

	if (TS[TOPE-1].entrada == marca) {
		TOPE--; 
	}
}

/**
 * Busca un identificador en la tabla de simbolos
 */
int tsBuscarIdent(char *ident, tipoEntrada ambito) {
	int i = TOPE - 1;
	bool encontrado = false;
	char *lim;

	if (ambito == marca){
		while (i >= 0 && strcmp(TS[i].nombre,"{") != 0 && !encontrado) {
			if(TS[i].entrada != parametro_formal)
				encontrado = strcmp(ident, TS[i].nombre) == 0;
			if (!encontrado)
				i--;
		}
	}
	else{
		while (i >= 0 && TS[i].entrada != funcion && !encontrado) {
			encontrado = strcmp(ident, TS[i].nombre) == 0;
			/*ToDo...*/
			if (!encontrado)
				i--;
		}
	}

	if (encontrado) {
		return i;
	} else {
		return -1;
	}
}

/**
 * Inserta una marca de comienzo de un bloque
 */
void tsInsertaMarca(){
	char *nombreMarca = strdup("{");
	if (subBloque == 1){
		nombreMarca = strdup("{{");
	}

	// Se inserta una marca de comienzo de bloque
	tsInsertaEntrada(marca, nombreMarca, desconocido, 0, 0, 0, 0);

	//Se insertan los parametros formales de la funcion como variables locales a esta
	if(subProg == 1){
		int j = TOPE - 2;

		while(j > 0 && TS[j].entrada == parametro_formal){	
			tsInsertaEntrada(variable, TS[j].nombre, TS[j].tipoDato,
						 TS[j].parametros, TS[j].dimensiones, TS[j].TamDimen1, TS[j].TamDimen2);		
			j--;
		}
		subProg = 0;
	}
}

/**
 * Inserta un nuevo identificador en la tabla de simbolos
 */
void tsInsertaIdent(atributos elem){
	int i = tsBuscarIdent(elem.lexema, funcion);

	tipoEntrada entrada = desconocido;
	if (i > 0){
		entrada = TS[i].entrada;
	}

	if (i == -1 || entrada == parametro_formal) {
		tsInsertaEntrada(variable, elem.lexema, tipoGlobal, 0, elem.dimensiones, elem.TamDimen1, elem.TamDimen2);
	}
	else {
		printf("Error linea %d. Identificador ya declarado: %s\n", lineaActual, elem.lexema);
	}
};

/**
 * Inserta una entrada de subprograma en la tabla de simbolos
 */
void tsInsertaSubprog(atributos elem) {
	int i = tsBuscarIdent(elem.lexema, marca);
	tipoEntrada entrada = desconocido;
	if (i > 0){
		entrada = TS[i].entrada;
	}

	if (i == -1 || entrada == parametro_formal) {
		tsInsertaEntrada(funcion, elem.lexema, tipoGlobal, 0, 0, 0, 0);
	}
	else {
		printf("Error linea %d. Identificador ya declarado: %s\n", lineaActual, elem.lexema);
	}
	
	funcionActual = TOPE - 1;
}

/*
 * Inserta una entrada en la tabla de simbolos en funcion de si
 * se esta declarando una variable o un parametro formal. 
*/
void tsInserta(atributos elem){
	if (lista_declaracion == 1)
		tsInsertaParamFormal(elem);
	else
		tsInsertaIdent(elem);
}

/**
 * Inserta una entrada de parametro formal de un subprograma en la tabla de simbolos
 */
void tsInsertaParamFormal(atributos elem) {
	int i = tsBuscarIdent(elem.lexema, funcion);

	if (i == -1) {
		tsInsertaEntrada(parametro_formal, elem.lexema, tipoGlobal, 0, elem.dimensiones,
				 elem.TamDimen1, elem.TamDimen2);
	}
	else {
		printf("Error linea %d. Parametro ya declarado: %s\n", lineaActual, elem.lexema);
	}

	numParam++;	
}

/**
 * Actualiza el número de parámetros de la función actual
 */
void tsActualizaNumParam(){
	TS[funcionActual].parametros = numParam;
	funcionActual = -1;
	numParam = 0;
}

/**
 * Imprime la tabla de simbolos
 */
void tsImprimeTabla(){
	int j = 0; 

	printf("----------\n"); 
	while(j <= TOPE-1) {
		printf("-Lexema: %s, %d, %d\n", TS[j].nombre, TS[j].tipoDato, TS[j].entrada);
		j++; 
	} 
	printf("----------\n\n"); 
}

// ------------  Funciones para las comprobaciones semanticas ------------

/**
 * Copia los valores de un identificador de la tabla de simbolos a los atributos
*/
void tsGetIdent(atributos ident, atributos* valores) {
	int i = tsBuscarIdent(ident.lexema, marca);

	if (TS[i].entrada != parametro_formal && i != -1) {
		valores->lexema = strdup(TS[i].nombre);
		valores->tipo = TS[i].tipoDato;
		valores->dimensiones = TS[i].dimensiones;
		valores->TamDimen1 = TS[i].TamDimen1;
		valores->TamDimen2 = TS[i].TamDimen2;
	}
	else {
		if (TS[i].entrada == parametro_formal){
			printf("Error linea %d. No se puede usar un parametro formal como variable\n", lineaActual);
		}
		else{
			printf("Error linea %d. Identificador no declarado: %s\n", lineaActual, ident.lexema);
		}
	}
}

/**
 * Comprueba que los tipos de los operandos de una operacion son compatibles
 */
void tsComprobarTipos(dTipo tipoElem1, dTipo tipoElem2){
	if (tipoElem1 != tipoElem2) {
		printf("Error linea %d. Los tipos de datos no son compatibles\n", lineaActual);
	}
}

/**
 * Comprueba que los tipos de los operandos de una operacion son compatibles
 */
void tsComprobarDimensiones(atributos elem1, atributos elem2){
	if (elem1.dimensiones != elem2.dimensiones) {
		printf("Error linea %d. Las dimensiones no son compatibles\n", lineaActual);
	}
	else if (elem1.dimensiones > 0){
		if (elem1.TamDimen1 != elem2.TamDimen1){
			printf("Error linea %d. La primera dimension no son compatibles\n", lineaActual);
		}
		else if(elem1.TamDimen2 != elem2.TamDimen2){
			printf("Error linea %d. La segunda dimension no son compatibles\n", lineaActual);
		}
	}
}

/**
 * Asigna el tipo de la función actual
 */
void tsComprobarDevolver(dTipo tipoElem){
	int i = TOPE - 1;
	bool encontrado = false;
	while (i > 0 && !encontrado) {
		if (TS[i].entrada == funcion) {
			encontrado = true;
		} else {
			i--;
		}
	}
	
	if(encontrado) {
		tsComprobarTipos(tipoElem, TS[i].tipoDato);
	} else {
		printf("Error linea %d. Sentencia devolver no corresponde a ningun subprog\n", lineaActual);
	}
}

/**
 * Comprueba que los tipos de datos en un case switch son compatibles
 */
void tsComprobarCasos(dTipo tipoElem1, dTipo tipoElem2, atributos *valores){
	if (tipoElem1 != tipoElem2) {
		printf("Error linea %d. El tipo de caso es incompatible con el resto de los casos.\n", lineaActual);
		valores->tipo = desconocido;
	}
	else{
		valores->tipo = tipoElem1;
	}
}

/**
 * Comprueba que el tipo de dato de una expresion es booleano
 */
void tsComprobarBooleano(dTipo tipoElem){
	if (tipoElem != booleano) {
		printf("Error linea %d. Se esperaba un dato de tipo booleano\n", lineaActual);
	}
}

/**
 * Comprueba que el tipo de dato de una expresion es entero
 */
void tsComprobarEntero(dTipo tipoElem){
	if (tipoElem != entero) {
		printf("Error linea %d. Se esperaba un entero\n", lineaActual);
	}
}

/**
 * Comprueba la compatibilidad de una asignacion
*/
void tsComprobarAsignacion(char *ident, atributos elem2, int dimension){
	int i = tsComprobarIdentExiste(ident), dimensionAux;
	if (i == -1) {
		return;
	}
	else{
		dimensionAux = TS[i].dimensiones - dimension;
	}

	if (TS[i].tipoDato != elem2.tipo) {
		printf("Error linea %d. El tipo de dato no es compatible con el esperado\n", lineaActual);
	}
	else{
		if (dimensionAux != elem2.dimensiones) {
			printf("Error linea %d. El numero de dimensiones es incompatible\n", lineaActual);
		}
		else{
			if (dimensionAux == 1 && (TS[i].TamDimen1 != elem2.TamDimen1)) {
				printf("Error linea %d. El tamanyo de la dimension 1 no es compatible con el esperado\n", lineaActual);
			}
			
			if (dimensionAux == 2 && (TS[i].TamDimen1 != elem2.TamDimen1 || TS[i].TamDimen2 != elem2.TamDimen2 )) {
				printf("Error linea %d. El tamanyo de la dimension 1 o 2 no es compatible con el esperado\n", lineaActual);
			}
			
		}
	}
}

/**
 * Comprueba que el tipo de parámetro pasado se corresponde con el de la función
*/
void tsComprobarParametro(int numP, atributos elem){
	int paramActual = funcionActual + numP;

	if (TS[paramActual].entrada != parametro_formal	){
		printf("Error linea %d: El numero de parametros no coincide con el de la funcion\n", lineaActual);
	}
	else{
		if (TS[paramActual].tipoDato != elem.tipo){
			printf("Error linea %d: El tipo de los parametros no coincide con el de la funcion\n", lineaActual);
		}
	}
}

/**
 * Comprueba que el identificador pasado existe en la tabla de simbolos
*/
int tsComprobarIdentExiste(char *ident){
	int i = tsBuscarIdent(ident,marca);
	if (i == -1) { //---
		printf("Error linea %d. Identificador no declarado: %s\n", lineaActual, ident);
		return -1;
	}
	else{
		return i;
	}
}

/**
 * Comprueba que el numero de parametros de una funcion es correcto
*/
void tsComprobarNumeroParam(){
	if (TS[funcionActual].parametros != numParam){
		printf("Error linea %d. El numero de parametros no coincide con el de la funcion\n", lineaActual);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//								GENERACION DE CODIGO

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FILE* files[2];
int temp = -1;
int etiqueta = -1;
etiquetaFlujo TF[MAX_TF];
int TOPEFLUJO = 0;
int n_file = 0;
int n_subprogramas_anidados = 0;

void generarFichero() {
	files[0] = fopen("codigo.c", "w+");
	fputs("#include \"dec_fun.h\"\n", files[0]);
	// fputs("#include \"dec_dat.h\"\n\n", files[0]);
	generarFicheroFunciones();
}

void generarFicheroFunciones() {
	files[1] = fopen("dec_fun.h", "w+");
	fputs("#include \"dec_dat.h\"\n\n", files[1]);
}

void cerrarFichero(int n) {
	fclose(files[n]);
}

char* tipoDeDato (dTipo td) {
	if(td == entero)	return "int ";
	if(td == booleano)	return "bool ";
	if(td == real)		return "float ";
	if(td == caracter)	return "char ";
	return "Error de tipo";
}

char* tipoDeBool (int verdadero) {
	if(verdadero == 1)	return "1";
	if(verdadero == 0)	return "0";
	return "Error de tipo booleano";
}

char tipoAFormato(dTipo dato) {
	if(dato == desconocido)		return 's';
	else if(dato == real)		return 'f';
	else if(dato == entero)		return 'd';
	else if(dato == caracter)	return 'c';
	else if(dato == booleano)	return 'd';
	else if(dato == array)	 	return 's';
	return 's';
}

void escribirEntrada(atributos elem){
	
	int i = tsBuscarIdent(elem.lexema, marca);
	
	if (i == -1) {
		printf("Error linea %d. Identificador no declarado: %s\n", lineaActual, elem.lexema);
		return;
	}
	else{
		char* sent;
		sent = (char*) malloc(200);

		sprintf(sent, "scanf(\"%%%c\", &%s);\n", tipoAFormato(TS[i].tipoDato),TS[i].nombre);
		
		fputs(sent, files[n_file]);
		
		free(sent);
	}
}

void escribirSalida(atributos elem){
	char* sent;
		sent = (char*) malloc(200);

		sprintf(sent, "printf(\"%%%c\", %s);\n", tipoAFormato(elem.tipo),elem.lexema);
		
		fputs(sent, files[n_file]);
		
		free(sent);
}

char* generarTemp(atributos elem){
	char* cadena = (char*) malloc(30);
	++temp;
	if (elem.dimensiones == 0){
		sprintf(cadena, "%stemp%d;\ntemp%d = ", tipoDeDato(elem.tipo), temp, temp);
	}
	else if (elem.dimensiones == 1){
		sprintf(cadena, "%stemp%d[%d];\n", tipoDeDato(elem.tipo), temp, elem.TamDimen1);
	}
	else if (elem.dimensiones == 2){
		sprintf(cadena, "%stemp%d[%d][%d];\n", tipoDeDato(elem.tipo), temp, elem.TamDimen1, elem.TamDimen2);
	}

	return cadena;
}

char* generarEtiqueta() {
	char* cadena = (char*) malloc(20);
	++etiqueta;
	sprintf(cadena, "etiqueta%d", etiqueta);
	return cadena;
}

void sacarTF(){
   if (TOPEFLUJO > 0) {
      --TOPEFLUJO;
   }
}

void insertarFlujo (etiquetaFlujo s){
	if (TOPEFLUJO == MAX_TF) {
		printf("\nError: tamanio maximo alcanzado\n");
		exit(-1);
	} else {
		TF[TOPEFLUJO].EtiquetaEntrada=s.EtiquetaEntrada;
		TF[TOPEFLUJO].EtiquetaSalida=s.EtiquetaSalida;
		TF[TOPEFLUJO].EtiquetaElse=s.EtiquetaElse;
		TF[TOPEFLUJO].NombreVarControl=s.NombreVarControl;
		++TOPEFLUJO;
	}
}