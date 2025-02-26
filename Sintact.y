%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "semantico.h"
#include "lex.yy.c"

int yylex();
void yyerror(char const* msg);

%}

%define parse.error verbose
//%error-verbose

// Declaraciones de tokens
%token PRINCIPAL
%token SI
%token SINO
%token MIENTRAS
%token SELECCIONADOR
%token CASO
%token DOSPUNTOS
%token PARA
%token LEER
%token IMPRIME
%token DEVFUN
%token LLAVEIZQ
%token LLAVEDER
%token CONSTN
%token CONSTB
%token CONSTCAD
%token CONSTCAR
%token COMA
%token PUNTO
%token PUNTOCOMA
%token OP1
%token OP6
%token DEVAR
%token PARIZQ
%token PARDER
%token CORIZQ
%token CORDER
%token TVAR
%token ID

//Tiene mayor precedencia la mult y div que suma y resta
%left OP5
%left NEGACION
%left OP4
%left OP2  
%left OP3
%right OP1

// Reglas de la gramática
%%

programa : { generarFichero();} Cabecera_programa {fputs("void main()\n", files[n_file]);} Bloque {cerrarFichero(1); cerrarFichero(0);}
         ;

Cabecera_programa : PRINCIPAL PARIZQ PARDER
                  ;

Bloque : LLAVEIZQ { tsInsertaMarca(); fputs("{\n", files[n_file]);} Declar_Variables Declar_subprogs Sentencias LLAVEDER { tsVaciarEntradas(); fputs("}\n", files[n_file]);} 
       ;

Declar_Variables : DEVAR Variables_locales DEVAR
                 | 
                 ;
                        
Declar_subprogs : Declar_subprogs Subprograma
                |
                ;

Sentencias : Sentencias Sentencia 
           | Sentencia
           ;

Variables_locales : Variables_locales Cuerpo_declar_variable 
                  | Cuerpo_declar_variable
                  ;

Cuerpo_declar_variable : TVAR {tsAsignaTipoGlobal($1.tipo); fputs(tipoDeDato($1.tipo), files[n_file]); } Lista_variables PUNTOCOMA { fputs(";\n", files[n_file]);}
                       | error
                       ;

Lista_variables : Lista_variables COMA {fputs(", ", files[n_file]);} Variable 
                | Variable
                ;

Variable : ID {tsInserta($1); fputs($1.lexema, files[n_file]);}
         | ID CORIZQ CONSTN CORDER {$1.dimensiones = 1; $1.TamDimen1 = atoi($3.lexema); tsInserta($1); fputs($1.lexema, files[n_file]); fputs("[", files[n_file]); fputs($3.lexema,files[n_file]); fputs("]", files[n_file]);}
         | ID CORIZQ CONSTN CORDER CORIZQ CONSTN CORDER {$1.dimensiones = 2; $1.TamDimen1 = atoi($3.lexema); $1.TamDimen2 = atoi($6.lexema); tsInserta($1); fputs($1.lexema, files[n_file]); fputs("[", files[n_file]); fputs($3.lexema, files[n_file]); fputs("]", files[n_file]); fputs("[", files[n_file]); fputs($6.lexema, files[n_file]); fputs("]", files[n_file]);} 
         | error
         ;

Subprograma : {n_subprogramas_anidados++; n_file = 1;} Cabecera_Subprograma { subProg = 1; subBloque = 1;} Bloque {subBloque = 0; n_subprogramas_anidados--; if (n_subprogramas_anidados == 0) {n_file = 0;}}
            ;

Cabecera_Subprograma : TVAR {tsAsignaTipoGlobal($1.tipo);} ID {tsInsertaSubprog($3);} PARIZQ {fputs(tipoDeDato($1.tipo), files[n_file]); fputs($3.lexema,files[n_file]); fputs("(",files[n_file]);} {lista_declaracion = 1;} lista_declaracion {lista_declaracion = 0;} PARDER {tsActualizaNumParam(); funcionActual = -1; numParam = 0; fputs(")", files[n_file]);} 
                     | error
                     ;

lista_declaracion : TVAR {tsAsignaTipoGlobal($1.tipo); fputs(tipoDeDato($1.tipo), files[n_file]);} Variable
                  | lista_declaracion COMA {fputs(", ", files[n_file]); } TVAR {tsAsignaTipoGlobal($4.tipo); fputs(tipoDeDato($4.tipo), files[n_file]);} Variable
                  ;

Sentencia : {subBloque = 1;} Bloque {subBloque = 0;}
          | sentencia_asignacion
          | sentencia_if
          | sentencia_while
          | sentencia_Case_Switch
          | sentencia_entrada
          | sentencia_salida
          | sentencia_return
          ;

sentencia_asignacion : ID OP6 Expresion PUNTOCOMA { 
                                                       tsComprobarAsignacion($1.lexema, $3, 0); 
                                                       if ($3.dimensiones == 0) {
                                                            fputs($1.lexema,files[n_file]); 
                                                            fputs(" = ",files[n_file]); 
                                                            fputs($3.lexema,files[n_file]);  
                                                            fputs(";\n", files[n_file]); 
                                                           
                                                       }
                                                       else if ($3.dimensiones == 1) {
                                                            fputs("asignar_vectores_",files[n_file]); 
                                                            fputs(tipoDeDato($$.tipo),files[n_file]);
                                                            fputs("(",files[n_file]); 
                                                            fputs($1.lexema,files[n_file]); 
                                                            fputs(",",files[n_file]); 
                                                            fputs($3.lexema,files[n_file]); 
                                                            fputs(",",files[n_file]); 
                                                            char* tam_dimen1_str = (char*) malloc(20);
                                                            sprintf(tam_dimen1_str, "%d", $3.TamDimen1);
                                                            fputs(tam_dimen1_str,files[n_file]);                                                            
                                                            fputs(");\n",files[n_file]);                                                  
                                                           
                                                       }
                                                       else if ($3.dimensiones == 2) {
                                                            fputs("asignar_matrices_",files[n_file]); 
                                                            fputs(tipoDeDato($$.tipo),files[n_file]);
                                                            fputs("(",files[n_file]); 
                                                            
                                                            char* tam_dimen1_str = (char*) malloc(20);
                                                            sprintf(tam_dimen1_str, "%d", $3.TamDimen1);
                                                            fputs(tam_dimen1_str,files[n_file]);
                                                            fputs(",",files[n_file]);
                                                            char* tam_dimen2_str = (char*) malloc(20);
                                                            sprintf(tam_dimen2_str, "%d", $3.TamDimen2);
                                                            fputs(tam_dimen2_str,files[n_file]);
                                                            fputs(",",files[n_file]);  
                                                            fputs($1.lexema,files[n_file]); 
                                                            fputs(",",files[n_file]); 
                                                            fputs($3.lexema,files[n_file]);                                              
                                                            fputs(");\n",files[n_file]);                                                           
                                                       }
                                                  }
                     | ID CORIZQ indice CORDER OP6 Expresion PUNTOCOMA {fputs($1.lexema, files[n_file]); fputs("[", files[n_file]); fputs($3.lexema,files[n_file]); fputs("]", files[n_file]); fputs("=", files[n_file]); fputs($6.lexema,files[n_file]); fputs(";\n",files[n_file]); $1.dimensiones = $1.dimensiones - 1; tsComprobarAsignacion($1.lexema, $6, 1);}
                     | ID CORIZQ indice CORDER CORIZQ indice CORDER OP6 Expresion PUNTOCOMA{fputs($1.lexema, files[n_file]); fputs("[", files[n_file]); fputs($3.lexema,files[n_file]); fputs("]", files[n_file]); fputs("[", files[n_file]); fputs($6.lexema,files[n_file]); fputs("]", files[n_file]); fputs("=", files[n_file]); fputs($9.lexema,files[n_file]); fputs(";\n",files[n_file]); $1.dimensiones = $1.dimensiones - 2; tsComprobarAsignacion($1.lexema, $9, 2); }
                     ;

aux : {   etiquetaFlujo *aux = malloc(sizeof(etiquetaFlujo));
          aux->EtiquetaElse = generarEtiqueta();
          aux->EtiquetaSalida = generarEtiqueta();
          insertarFlujo(*aux);
          fputs("if (!", files[n_file]);
          fputs($-1.lexema,files[n_file]);
          fputs(") goto ", files[n_file]);
          fputs(TF[TOPEFLUJO-1].EtiquetaElse, files[n_file]);
          fputs(";\n",files[n_file]);} 

sentencia_if : SI PARIZQ Expresion PARDER aux Sentencia  
                                        SINO {
                                             fputs("goto ", files[n_file]);
                                             fputs(TF[TOPEFLUJO-1].EtiquetaSalida, files[n_file]);
                                             fputs(";\n",files[n_file]);
                                             fputs(TF[TOPEFLUJO-1].EtiquetaElse, files[n_file]);
                                             fputs(":\n",files[n_file]);} 
                                        Sentencia {
                                             fputs(TF[TOPEFLUJO-1].EtiquetaSalida, files[n_file]);
                                             fputs(":\n",files[n_file]);
                                             sacarTF();
                                        }
             | SI PARIZQ Expresion PARDER aux Sentencia {
                                        fputs(TF[TOPEFLUJO-1].EtiquetaElse, files[n_file]);
                                        fputs(":\n",files[n_file]);
                                        sacarTF();}
             ;

sentencia_while : MIENTRAS PARIZQ {   etiquetaFlujo *aux = malloc(sizeof(etiquetaFlujo));
                                                       aux->EtiquetaEntrada = generarEtiqueta();
                                                       aux->EtiquetaSalida = generarEtiqueta();
                                                       insertarFlujo(*aux);
                                                       fputs(TF[TOPEFLUJO-1].EtiquetaEntrada, files[n_file]);
                                                       fputs(":\n", files[n_file]);
                                                       } 
                                                  Expresion {
                                                       fputs("if (!", files[n_file]);
                                                       fputs($4.lexema,files[n_file]);
                                                       fputs(") goto ", files[n_file]);
                                                       fputs(TF[TOPEFLUJO-1].EtiquetaSalida, files[n_file]);
                                                       fputs(";\n",files[n_file]);
                                                       tsComprobarBooleano($4.tipo);
                                                  } PARDER Sentencia 
                                                  {
                                                       fputs("goto ", files[n_file]);
                                                       fputs(TF[TOPEFLUJO-1].EtiquetaEntrada, files[n_file]);
                                                       fputs(";\n",files[n_file]);
                                                       fputs(TF[TOPEFLUJO-1].EtiquetaSalida, files[n_file]);
                                                       fputs(":\n",files[n_file]);
                                                       sacarTF();
                                                  }
                ;

sentencia_Case_Switch : SELECCIONADOR PARIZQ Expresion PARDER LLAVEIZQ { 
                                                                           etiquetaFlujo *aux = malloc(sizeof(etiquetaFlujo));
                                                                           aux->EtiquetaEntrada = generarEtiqueta();
                                                                           aux->EtiquetaSalida = generarEtiqueta();
                                                                           aux->NombreVarControl = $3.lexema;
                                                                           insertarFlujo(*aux);
                                                                      } 
                                                                      Casos LLAVEDER { 
                                                                           fputs(TF[TOPEFLUJO-1].EtiquetaSalida, files[n_file]);
                                                                           fputs(":\n",files[n_file]);
                                                                           sacarTF();
                                                                           tsComprobarCasos($3.tipo, $7.tipo, &$$); 
                                                                      }
                      ;
                      
Casos : Casos Caso { tsComprobarCasos($1.tipo, $2.tipo, &$$); }
      | Caso {$$.tipo = $1.tipo; $$.tipo = $1.tipo;}
      ;

Caso : CASO CONSTN DOSPUNTOS  {                                   
                                   fputs("if (", files[n_file]);
                                   fputs($2.lexema,files[n_file]);
                                   fputs(" != ", files[n_file]);
                                   fputs(TF[TOPEFLUJO-1].NombreVarControl,files[n_file]);
                                   fputs(") goto ", files[n_file]);
                                   TF[TOPEFLUJO-1].EtiquetaEntrada = generarEtiqueta();
                                   fputs(TF[TOPEFLUJO-1].EtiquetaEntrada, files[n_file]);
                                   fputs(";\n",files[n_file]);
     } Sentencia PARA PUNTOCOMA { 
                                   fputs("goto ", files[n_file]);
                                   fputs(TF[TOPEFLUJO-1].EtiquetaSalida, files[n_file]);
                                   fputs(";\n",files[n_file]);
                                   fputs(TF[TOPEFLUJO-1].EtiquetaEntrada, files[n_file]);
                                   fputs(":\n",files[n_file]);
                                   $$.tipo = entero;
     }

     | CASO CONSTCAR DOSPUNTOS {
                                   fputs("if (", files[n_file]);
                                   fputs($2.lexema,files[n_file]);
                                   fputs(" != ", files[n_file]);
                                   fputs(TF[TOPEFLUJO-1].NombreVarControl,files[n_file]);
                                   fputs(") goto ", files[n_file]);
                                   TF[TOPEFLUJO-1].EtiquetaEntrada = generarEtiqueta();
                                   fputs(TF[TOPEFLUJO-1].EtiquetaEntrada, files[n_file]);
                                   fputs(";\n",files[n_file]);   
     } Sentencia PARA PUNTOCOMA { 
                                   fputs("goto ", files[n_file]);
                                   fputs(TF[TOPEFLUJO-1].EtiquetaSalida, files[n_file]);
                                   fputs(";\n",files[n_file]);
                                   fputs(TF[TOPEFLUJO-1].EtiquetaEntrada, files[n_file]);
                                   fputs(":\n",files[n_file]);
                                   $$.tipo = entero;
     }
     | CASO CONSTB DOSPUNTOS {
                                   fputs("if (", files[n_file]);
                                   fputs($2.lexema,files[n_file]);
                                   fputs(" != ", files[n_file]);
                                   fputs(TF[TOPEFLUJO-1].NombreVarControl,files[n_file]);
                                   fputs(") goto ", files[n_file]);
                                   TF[TOPEFLUJO-1].EtiquetaEntrada = generarEtiqueta();
                                   fputs(TF[TOPEFLUJO-1].EtiquetaEntrada, files[n_file]);
                                   fputs(";\n",files[n_file]);   
     } Sentencia PARA PUNTOCOMA { 
                                   fputs("goto ", files[n_file]);
                                   fputs(TF[TOPEFLUJO-1].EtiquetaSalida, files[n_file]);
                                   fputs(";\n",files[n_file]);
                                   fputs(TF[TOPEFLUJO-1].EtiquetaEntrada, files[n_file]);
                                   fputs(":\n",files[n_file]);
                                   $$.tipo = entero;
     }
     ;

sentencia_entrada : LEER PARIZQ lista_variables_entrada PARDER PUNTOCOMA
                  ;

variable_entrada : ID { tsComprobarIdentExiste($1.lexema); escribirEntrada($1);}
               | Valor_vect_1d {escribirEntrada($1);}
               | Valor_vect_2d {escribirEntrada($1);}
               ;

lista_variables_entrada : variable_entrada
                        | lista_variables_entrada COMA variable_entrada
                        ;

sentencia_salida : IMPRIME PARIZQ cadena_salida PARDER PUNTOCOMA  
                 ;

cadena_salida : Expresion {escribirSalida($1);}
              | cadena_salida COMA Expresion {escribirSalida($3);}
              ;

sentencia_return : DEVFUN Expresion PUNTOCOMA {fputs("return ", files[n_file]); fputs($2.lexema, files[n_file]); fputs(";\n", files[n_file]); tsComprobarDevolver($2.tipo);}
                 ;

Expresion : ID { tsGetIdent($1, &$$); }
          | CONSTCAR {$$ = $1;}
          | CONSTCAD {$$ = $1;}
          | llamada_funcion {$$ = $1;}
          | Valor_vect_1d {$$ = $1;}
          | Valor_vect_2d {$$ = $1;}
          | CONSTB {$$ = $1;}
          | Expresion OP5 Expresion {tsComprobarTipos($1.tipo, $3.tipo); $$ = $1; $$.tipo = booleano; fputs(generarTemp($$), files[n_file]); char* aux = (char*) malloc(20); fputs($1.lexema, files[n_file]); fputs($2.lexema, files[n_file]); fputs($3.lexema, files[n_file]); fputs(";\n", files[n_file]); sprintf(aux, "temp%d", temp); sprintf($$.lexema, "%s", aux);} 
          | Expresion OP4 Expresion {tsComprobarTipos($1.tipo, $3.tipo); $$ = $1; fputs(generarTemp($$), files[n_file]); char* aux = (char*) malloc(20); fputs($1.lexema, files[n_file]); fputs($2.lexema, files[n_file]); fputs($3.lexema, files[n_file]); fputs(";\n", files[n_file]); sprintf(aux, "temp%d", temp); sprintf($$.lexema, "%s", aux);}
          | NEGACION Expresion {$$ = $2; char* aux = (char*) malloc(20); fputs(generarTemp($$), files[n_file]); fputs($1.lexema, files[n_file]); fputs($2.lexema, files[n_file]); fputs(";\n", files[n_file]); sprintf(aux, "temp%d", temp); sprintf($$.lexema, "%s", aux);}
          | CONSTN { $$ = $1;}
          | real {$$ = $1;}
          | Expresion OP2 Expresion {   tsComprobarTipos($1.tipo, $3.tipo);
                                        tsComprobarDimensiones($1, $3); 
                                        $$ = $1; 
                                        
                                        if($$.dimensiones == 0){
                                             fputs(generarTemp($$), files[n_file]); 
                                             char* aux = (char*) malloc(20); 
                                             fputs($1.lexema, files[n_file]); fputs($2.lexema, files[n_file]); 
                                             fputs($3.lexema, files[n_file]); fputs(";\n", files[n_file]); 
                                             sprintf(aux, "temp%d", temp); 
                                             sprintf($$.lexema, "%s", aux);
                                        }
                                        if($$.dimensiones == 1){
                                             
                                             fputs(generarTemp($$), files[n_file]); 
                                             char* aux = (char*) malloc(20); 
                                             
                                             fputs("operar_vectores_",files[n_file]);
                                             fputs(tipoDeDato($$.tipo),files[n_file]);
                                             fputs("(\"",files[n_file]);
                                             fputs($2.lexema,files[n_file]);
                                             fputs("\", ",files[n_file]);
                                             sprintf(aux, "temp%d", temp);
                                             fputs(aux,files[n_file]);
                                             fputs(", ",files[n_file]);
                                             fputs($1.lexema,files[n_file]);
                                             fputs(", ",files[n_file]);
                                             fputs($3.lexema,files[n_file]);
                                             fputs(", ",files[n_file]);
                                             char* tam_dimen1_str = (char*) malloc(20); 
                                             sprintf(tam_dimen1_str, "%d", $$.TamDimen1);
                                             fputs(tam_dimen1_str,files[n_file]);
                                             fputs(");\n",files[n_file]); 

                                             sprintf($$.lexema, "%s", aux);
                                        }
                                        if($$.dimensiones == 2){
                                             fputs(generarTemp($$), files[n_file]); 
                                             char* aux = (char*) malloc(20); 
                                             
                                             fputs("operar_matrices_",files[n_file]);
                                             fputs(tipoDeDato($$.tipo),files[n_file]);
                                             fputs("(\"",files[n_file]);
                                             fputs($2.lexema,files[n_file]);
                                             fputs("\", ",files[n_file]);
                                             char* tam_dimen1_str = (char*) malloc(20);
                                             sprintf(tam_dimen1_str, "%d", $$.TamDimen1);
                                             fputs(tam_dimen1_str,files[n_file]);
                                             fputs(", ",files[n_file]);
                                             char* tam_dimen2_str = (char*) malloc(20);
                                             sprintf(tam_dimen2_str, "%d", $$.TamDimen2);
                                             fputs(tam_dimen2_str,files[n_file]);
                                             fputs(", ",files[n_file]);
                                             
                                             sprintf(aux, "temp%d", temp);
                                             fputs(aux,files[n_file]);
                                             fputs(", ",files[n_file]);
                                             fputs($1.lexema,files[n_file]);
                                             fputs(", ",files[n_file]);
                                             fputs($3.lexema,files[n_file]);
                                             
                                             fputs(");\n",files[n_file]); 

                                             sprintf($$.lexema, "%s", aux);
                                        }

                                   }                                     
          | Expresion OP3 Expresion {   tsComprobarTipos($1.tipo, $3.tipo);
                                        tsComprobarDimensiones($1, $3); 
                                        $$ = $1; 
                                        
                                        if($$.dimensiones == 0){
                                             fputs(generarTemp($$), files[n_file]); 
                                             char* aux = (char*) malloc(20); 
                                             fputs($1.lexema, files[n_file]); fputs($2.lexema, files[n_file]); 
                                             fputs($3.lexema, files[n_file]); fputs(";\n", files[n_file]); 
                                             sprintf(aux, "temp%d", temp); 
                                             sprintf($$.lexema, "%s", aux);
                                        }
                                        if($$.dimensiones == 1){
                                             
                                             fputs(generarTemp($$), files[n_file]); 
                                             char* aux = (char*) malloc(20); 
                                             
                                             fputs("operar_vectores_",files[n_file]);
                                             fputs(tipoDeDato($$.tipo),files[n_file]);
                                             fputs("(\"",files[n_file]);
                                             fputs($2.lexema,files[n_file]);
                                             fputs("\", ",files[n_file]);
                                             sprintf(aux, "temp%d", temp);
                                             fputs(aux,files[n_file]);
                                             fputs(", ",files[n_file]);
                                             fputs($1.lexema,files[n_file]);
                                             fputs(", ",files[n_file]);
                                             fputs($3.lexema,files[n_file]);
                                             fputs(", ",files[n_file]);
                                             char* tam_dimen1_str = (char*) malloc(20); 
                                             sprintf(tam_dimen1_str, "%d", $$.TamDimen1);
                                             fputs(tam_dimen1_str,files[n_file]);
                                             fputs(");\n",files[n_file]); 

                                             sprintf($$.lexema, "%s", aux);
                                        }
                                        if($$.dimensiones == 2){
                                             fputs(generarTemp($$), files[n_file]); 
                                             char* aux = (char*) malloc(20); 
                                             
                                             fputs("operar_matrices_",files[n_file]);
                                             fputs(tipoDeDato($$.tipo),files[n_file]);
                                             fputs("(\"",files[n_file]);
                                             fputs($2.lexema,files[n_file]);
                                             fputs("\", ",files[n_file]);
                                             char* tam_dimen1_str = (char*) malloc(20);
                                             sprintf(tam_dimen1_str, "%d", $$.TamDimen1);
                                             fputs(tam_dimen1_str,files[n_file]);
                                             fputs(", ",files[n_file]);
                                             char* tam_dimen2_str = (char*) malloc(20);
                                             sprintf(tam_dimen2_str, "%d", $$.TamDimen2);
                                             fputs(tam_dimen2_str,files[n_file]);
                                             fputs(", ",files[n_file]);
                                             
                                             sprintf(aux, "temp%d", temp);
                                             fputs(aux,files[n_file]);
                                             fputs(", ",files[n_file]);
                                             fputs($1.lexema,files[n_file]);
                                             fputs(", ",files[n_file]);
                                             fputs($3.lexema,files[n_file]);
                                             
                                             fputs(");\n",files[n_file]); 

                                             sprintf($$.lexema, "%s", aux);
                                        }

                                   }
          | PARIZQ Expresion PARDER {$$ = $3;}
          | Expresion OP1 { $$ = $1; fputs(generarTemp($$), files[n_file]); char* aux = (char*) malloc(20);fputs($1.lexema, files[n_file]); fputs($2.lexema, files[n_file]); fputs(";\n", files[n_file]); sprintf(aux, "temp%d", temp); sprintf($$.lexema, "%s", aux);}
          | error
          ;

llamada_funcion : ID PARIZQ {funcionActual = tsBuscarIdent($1.lexema, marca);} lista_llamada PARDER { tsComprobarNumeroParam();numParam = 0; tsGetIdent($1, &$$); strcat($$.lexema,"("); strcat($$.lexema,$4.lexema); strcat($$.lexema, ")"); }
                ;

lista_llamada : Expresion { numParam++; tsComprobarParametro(numParam, $1); $$.lexema = $1.lexema;}
              | lista_llamada COMA Expresion { numParam++; tsComprobarParametro(numParam, $3); strcat($$.lexema,", "); strcat($$.lexema,$3.lexema);}
              ;

real : CONSTN PUNTO CONSTN { $$.dimensiones = 0; $$.tipo = real; strcat(strcat($1.lexema,"."),$3.lexema); }
     ;

indice : Expresion {tsComprobarEntero($1.tipo);}
       ;

Valor_vect_1d : ID CORIZQ indice CORDER { tsGetIdent($1, &$$); $$.dimensiones = 0; strcat($1.lexema,"["); strcat($$.lexema,$3.lexema);  strcat($$.lexema,"]");}
              ;

Valor_vect_2d : ID CORIZQ indice CORDER CORIZQ indice CORDER { tsGetIdent($1, &$$); $$.dimensiones = 0;  strcat($$.lexema,"[");  strcat($$.lexema,$3.lexema); strcat($$.lexema,"]"); strcat($$.lexema,"["); strcat($$.lexema,$6.lexema); strcat($$.lexema,"]"); }
              ;

%%
