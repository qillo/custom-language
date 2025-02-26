#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin;
extern int yylineno;

int yyparse(void);

void yyerror(char const *s){
    fprintf(stderr, "[Line %2d] *** %s\n", yylineno, s);
}

FILE *abrir_entrada (int argc, char *argv[])
{
    FILE *f = NULL;
    if (argc != 2){
        fprintf(stderr, "Uso: %s <fichero>\n", argv[0]);
        exit(1);
    }
    f = fopen(argv[1], "r");
    if (f == NULL){
        fprintf(stderr, "No se puede abrir el fichero %s\n", argv[1]);
        exit(2);
    }
    return f;
}

int main(int argc, char *argv[]){
    yyin = abrir_entrada(argc, argv);
    return yyparse();
}
