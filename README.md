# "Compilador" de lenguaje personalizado

Este es proyecto de la asignatura Procesadores de Lenguajes de la Universidad de Granada. Se trata un **"compilador"** para un lenguaje de programación personalizado basado en funcionalidades básicas de C. Dicho lenguaje, consiste en una "traducción simplificada al español de C".

El lenguaje implementado en este compilador es un subconjunto de un lenguaje de programación estructurado, inspirado en **C** y con palabras reservadas en **castellano**. 

## 💻 Características principales del lenguaje

**LAS PALABRAS RESERVADAS SERÁN EN CASTELLANO**

1. **Tipos de datos**: 
   - Entero
   - Real
   - Carácter
   - Booleano
   - ***Arrays***: Soporte para arrays 1D y 2D con operaciones básicas como suma, resta y producto.

2. **Operaciones soportadas**:
   - Aritméticas: suma, resta, producto, división, operaciones de relación (para enteros y reales).
   - Lógicas: `and`, `or`, `not`, `xor` (para booleanos).

3. **Sentencias soportadas**:
   - Asignación
   - Expresiones aritméticas y lógicas
   - Entrada y salida (lectura y escritura de variables y expresiones)
   - Estructuras de control:
     - `if-then-else`
     - `while`
     - `case/switch`

4. **Declaraciones**:
   - Los identificadores deben ser declarados antes de ser usados, dentro de bloques y entre una marca de inicio y otra de final de las declaraciones.
   - Se permiten estructuras de control anidadas y bloques.

5. **Subprogramas**:
   - **SÓLO FUNCIONES** con paso de parámetros por valor.
   - Su estructura sintáctica consistirá en: <Declar_subprog>::=<Cabecera_subprograma><bloque>

El objetivo del "compilador" es traducir (con todas las fases oportunas) nuestro lenguaje, a la versión C correspondiente, para que pueda ser compilado por herramientas tradicionales, y realice las tareas para las que había sido programado en el lenguaje original personalizado. Incluye las siguientes fases y tecnologías:

## ➰ Fases del compilador

### 🔎 1. **Análisis Léxico**

- **Tecnología**: Usa **Flex** para el análisis léxico.
- **Proceso**: Se extraen los tokens del código fuente mediante expresiones regulares definidas en el archivo [lex](lex). Los tokens generados incluyen palabras clave (`si`, `entero`, `principal`), operadores (`+`, `-`, `==`, etc.), identificadores (`ID`), y literales (números, cadenas, booleanos).

### 🔎 2. **Análisis Sintáctico**

- **Tecnología**: Usa **Bison** para el análisis sintáctico.
- **Proceso**: **Bison** toma los tokens generados por el analizador léxico y los analiza según una gramática definida en el archivo [Sintact.y](Sintact.y). Esto genera el archivo `Sintact.tab.c`, que contiene el código para el análisis sintáctico.

### 🔎 3. **Análisis Semántico y Generación de Código**

- **Tecnología**: Implementado en **C** en el archivo [semantico.c](semantico.c).
- **Proceso**: Esta fase no solo asegura que el programa sigue las reglas semánticas del lenguaje, sino que también genera el **código compilable** correspondiente en C. Las tareas clave incluyen:
  - **Verificación de Tipos**: Se asegura de que las operaciones sean válidas para los tipos de datos dados (por ejemplo, evitar sumar un `int` con un `real`).
  - **Verificación de Ámbito**: Asegura que las variables estén declaradas antes de ser usadas.
  - **Verificación de Estructuras de Control**: Se asegura de que las estructuras de control como `if`, `while`, `for` se usen correctamente.
  - **Generación de Código**: El analizador semántico genera código en C que luego puede ser compilado. El código generado sigue la estructura del código fuente, pero ahora usa bibliotecas estándar como `stdio.h` para operaciones de entrada/salida.

## ✨ Ejemplo de Transformación

Por ejemplo, el siguiente código fuente [Prueba.c](Prueba.c) en nuestro lenguaje, realizaría la descomposición en factores primos de un número ingresado por el usuario, imprimiendo cada factor primo a medida que lo encuentra:

```plaintext
principal ()
{
    @
        entero n, curr;
    @

    imprime("introduce un numero : ");
    leer(n);
    imprime(n, " == " );
    curr = 2;

    mientras(curr <= n){
        @
            entero d;
        @
        d = n / curr;

        si( d*curr == n)
        {
            imprime("* ", curr);
            n = n / curr;
        }
        sino
            curr = curr + 1;
    }
    imprime("\n");
}

```

👉 Gracias al compilador, se transforma en el siguiente código bajo-nivel en C, que ya si es compilable por herramientas tradicionales como **gcc**:

```c
#include "dec_fun.h" 
void main()
{
    int n, curr;
    printf("%s", "introduce un numero : ");
    scanf("%d", &n);
    printf("%d", n);
    printf("%s", " == ");
    curr = 2;
    etiqueta0:
    bool temp0;
    temp0 = curr<=n;
    if (!temp0) goto etiqueta1;
    {
        int d;
        int temp1;
        temp1 = n/curr;
        d = temp1;
        int temp2;
        temp2 = d*curr;
        bool temp3;
        temp3 = temp2==n;
        if (!temp3) goto etiqueta2;
        {
            printf("%s", "* ");
            printf("%d", curr);
            int temp4;
            temp4 = n/curr;
            n = temp4;
        }
        goto etiqueta3;
    etiqueta2:
        int temp5;
        temp5 = curr+1;
        curr = temp5;
    etiqueta3:
    }
    goto etiqueta0;
    etiqueta1:
    printf("%s", "\n");
}

```

## 🛠️ Guía para ejecutar el "compilador" 

### Requisitos previos

Asegúrate de tener instalados los siguientes programas:

- **GCC**: Para compilar el código C.
- **Bison**: Para generar el analizador sintáctico.
- **Flex**: Para generar el analizador léxico.
- **Make**: Para automatizar la compilación.

En sistemas basados en Unix, puedes instalarlo con:

```bash
sudo apt-get install gcc bison flex make
```

### Pasos para ejecutar el "compilador"
#### Compilar el proyecto:

El archivo [Makefile](makefile) contiene toda la información necesaria. Ejecuta el siguiente comando en la terminal en el directorio del proyecto:

```bash
make
```
Esto generará el **compilador** `Generador`.

```bash
./Generador archivo_con_el_codigo_personalizdo
````
Esto generará un archivo .c con el código compilable.

Compilar el código generado:

```bash
gcc -o programa codigo.c
```

Ejecutar el programa:

```bash
./programa
```

## 👷 Autores
