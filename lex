%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "Sintact.tab.h"
%}

letras  [a-zA-Z]
numero  [0-9]

%option yylineno
%option noyywrap

%%

\n				{ ++lineaActual; }
\t				{ ; }
[ \t]			{ ; }
\r			    { ; }
principal       {return PRINCIPAL;}
entero          {yylval.tipo = entero; return TVAR;}
real            {yylval.tipo = real; return TVAR;}
caracter        {yylval.tipo = caracter; return TVAR;}
booleano        {yylval.tipo = booleano; return TVAR;}
si              {return SI;}
sino            {return SINO;}
mientras        {return MIENTRAS;}
seleccionador   {return SELECCIONADOR;}
caso            {return CASO;}
y               {return OP4;}
o               {return OP4;}
\:              {return DOSPUNTOS;}
para            {return PARA;}
leer            {return LEER;}
imprime         {return IMPRIME;}
devolver        {return DEVFUN;}
\{              {return LLAVEIZQ;}
\}              {return LLAVEDER;}
{numero}+       {yylval.lexema = strdup(yytext); yylval.dimensiones = 0; yylval.tipo = entero; return CONSTN;}
\"[^/\"]*\"     {yylval.lexema = strdup(yytext); yylval.dimensiones = 1; yylval.TamDimen1 = strlen(strdup(yytext)); yylval.tipo = array; return CONSTCAD;}
\'[^/\']\'      {yylval.lexema = strdup(yytext); yylval.dimensiones = 0; yylval.tipo = caracter; return CONSTCAR;}
verdadero       {yylval.atrib = 1; yylval.tipo = booleano; yylval.dimensiones = 0; return CONSTB;}
falso           {yylval.atrib = 0; yylval.tipo = booleano; yylval.dimensiones = 0; return CONSTB;}
\=              {return OP6;}
\,              {return COMA;}
\.              {return PUNTO;}
\;              {return PUNTOCOMA;}
\+\+            {yylval.atrib = 0; yylval.lexema = strdup(yytext); return OP1;}
\-\-            {yylval.atrib = 1; yylval.lexema = strdup(yytext); return OP1;}
\+              {yylval.atrib = 0; yylval.lexema = strdup(yytext); return OP2;}
\-              {yylval.atrib = 1; yylval.lexema = strdup(yytext); return OP2;}
\*              {yylval.atrib = 0; yylval.lexema = strdup(yytext); return OP3;}
\/              {yylval.atrib = 1; yylval.lexema = strdup(yytext); return OP3;}
\%              {yylval.atrib = 2; yylval.lexema = strdup(yytext); return OP3;}
\=\=            {yylval.atrib = 0; yylval.lexema = strdup(yytext); return OP5;}
\!\=            {yylval.atrib = 1; yylval.lexema = strdup(yytext); return OP5;}
\<              {yylval.atrib = 2; yylval.lexema = strdup(yytext); return OP5;}
\<\=            {yylval.atrib = 3; yylval.lexema = strdup(yytext); return OP5;}
\>              {yylval.atrib = 4; yylval.lexema = strdup(yytext); return OP5;}
\>\=            {yylval.atrib = 5; yylval.lexema = strdup(yytext); return OP5;}
\!              {return NEGACION; yylval.lexema = strdup(yytext);}
\@              {return DEVAR;}
\(              {return PARIZQ;}
\)              {return PARDER;}
\[              {return CORIZQ; yylval.lexema = strdup(yytext);}
\]              {return CORDER; yylval.lexema = strdup(yytext);}
{letras}({letras}|{numero}|_)* {yylval.lexema = strdup(yytext); return ID;}

.   { printf( "[Line %d] Error lexico en %s\n", yylineno,yytext);}

%%

