// ej1.c - Ejercicio 1 (Feedback) - Sistemas Operativos (UAX)
// Autor: <NPAlumno>
//
// Descripción general:
// Este archivo implementa la solución del Ejercicio 1 (Unidades Didácticas 1-3).
// Se define una estructura para gestionar un array de hasta 10 enteros no negativos
// con operaciones de inicialización, inserción, consulta, modificación y reinicio.
// El valor -1 marca posiciones libres. Se almacena el número de elementos válidos (arrSize)
// y la suma de todos los elementos ocupados (arrAdd) para acceso O(1).
//
// Decisiones de diseño principales:
// - Se permite el valor 0 como elemento válido (el enunciado habla de enteros positivos, pero
//   se requiere añadir 0, 10, 20... por lo que se interpreta "positivo" como "no negativo".
// - Las inserciones se realizan de forma contigua desde el índice 0; no se hace compactación.
// - La actualización de un elemento verifica que la posición esté ocupada para mantener arrSize.
// - Se ofrece una función auxiliar para recomputar arrAdd (por robustez) aunque normalmente
//   se mantiene incrementalmente.
// - Se añaden mensajes de error más consistentes para facilitar depuración.
//
// Uso: compilar con gcc -std=c11 -Wall -Wextra -O2 ej1.c -o ej1
//
// -----------------------------
// Apartado 2.1: Definición de estructura y typedef
// -----------------------------
#include <stdio.h>
#include <stdlib.h>

// Estructura y typedef
typedef struct arrayLength {
    int arrInt[10];     // Array de enteros no negativos; -1 indica libre
    int arrSize;        // Número de elementos añadidos
    int arrAdd;         // Suma de los elementos añadidos
} arrayLength_t;

// Constante de capacidad para evitar literales mágicos
#define ARRAY_CAP 10

// Función interna para validar puntero
static int validatePtr(const arrayLength_t *al) { return al != NULL; }

// Recalcula la suma desde cero (para auditoría). Coste O(n).
static int recomputeSum(arrayLength_t *al) {
    if (!al) return -1;
    int s = 0; int count = 0;
    for (int i = 0; i < ARRAY_CAP; ++i) {
        if (al->arrInt[i] >= 0) { s += al->arrInt[i]; count++; }
    }
    al->arrAdd = s;
    al->arrSize = count;
    return 0;
}

// -----------------------------
// Apartado 2.2
// -----------------------------
// initArray: inicializa la estructura
int initArray(arrayLength_t *al) {
    if (!al) return -1;
    for (int i = 0; i < ARRAY_CAP; ++i) al->arrInt[i] = -1;
    al->arrSize = 0;
    al->arrAdd = 0;
    return 0;
}

// printArr: imprime la estructura en el formato requerido
void printArr(const arrayLength_t *al) {
    if (!validatePtr(al)) { printf("{[], -1, -1}\n"); return; }
    printf("{[");
    for (int i = 0; i < ARRAY_CAP; ++i) {
        printf("%d", al->arrInt[i]);
        if (i != ARRAY_CAP - 1) printf(", ");
    }
    printf("], %d, %d}\n", al->arrSize, al->arrAdd);
}

// addElement: añade un valor no negativo al final (posición siguiente a la última ocupada)
int addElement(arrayLength_t *al, int value) {
    if (!al) return -1;
    if (value < 0) return -1; // -1 reservado, negativos inválidos
    if (al->arrSize >= ARRAY_CAP) return -1; // sin hueco
    int idx = al->arrSize; // siguiente posición contigua libre
    al->arrInt[idx] = value;
    al->arrSize++;
    al->arrAdd += value;
    return 0;
}

// -----------------------------
// Apartado 2.3
// -----------------------------
int getArrSize(const arrayLength_t *al) { return validatePtr(al) ? al->arrSize : -1; }
int getArrAdd(const arrayLength_t *al) { return validatePtr(al) ? al->arrAdd : -1; }
int getElement(const arrayLength_t *al, int pos) {
    if (!validatePtr(al)) return -1;
    if (pos < 0 || pos >= ARRAY_CAP) return -1;
    if (al->arrInt[pos] < 0) return -1; // libre
    return al->arrInt[pos];
}

// -----------------------------
// Apartado 2.4
// -----------------------------
int setElement(arrayLength_t *al, int pos, int value) {
    if (!al) return -1;
    if (pos < 0 || pos >= ARRAY_CAP) return -1;
    if (value < 0) return -1;
    if (al->arrInt[pos] < 0) return -1; // no ocupada
    al->arrAdd -= al->arrInt[pos];
    al->arrInt[pos] = value;
    al->arrAdd += value;
    return 0;
}

// -----------------------------
// Apartado 2.5
// -----------------------------
int resetArr(arrayLength_t *al) { return initArray(al); }

// -----------------------------
// Apartado 2.6 - main de prueba
// -----------------------------
int main(void) {
    // 1) Declarar dos estructuras
    arrayLength_t al1, al2;

    // Inicializar
    if (initArray(&al1) != 0) {
        printf("Error en inicialización al1\n");
        return EXIT_FAILURE;
    }
    if (initArray(&al2) != 0) {
        printf("Error en inicialización al2\n");
        return EXIT_FAILURE;
    }

    // 2) al1 almacene 0, 10, 20, ..., 90
    for (int i = 0; i < ARRAY_CAP; ++i) {
        if (addElement(&al1, i * 10) != 0) {
            printf("Error al añadir %d a al1\n", i * 10);
        }
    }

    // 3) Mostrar al1
    printArr(&al1);

    // 4) Actualizar posiciones impares a 1,3,5,7,9
    int v = 1;
    for (int pos = 1; pos < ARRAY_CAP; pos += 2) {
        if (setElement(&al1, pos, v) != 0) {
            printf("Error al actualizar al1[%d]\n", pos);
        }
        v += 2;
    }

    // 5) Mostrar de nuevo al1
    printArr(&al1);

    // 6) Añadir a al2 las posiciones pares de al1 de forma consecutiva
    // pares: 0,2,4,6,8
    for (int pos = 0; pos < ARRAY_CAP; pos += 2) {
        int val = getElement(&al1, pos);
        if (val >= 0) {
            if (addElement(&al2, val) != 0) {
                printf("Error al añadir al2 (de al1[%d])\n", pos);
            }
        }
    }

    // 7) Actualizar las posiciones finales de al2 para almacenar los valores 0..4
    // Interpretación: añadir al final 0,1,2,3,4 (usando addElement)
    for (int k = 0; k <= 4; ++k) {
        if (addElement(&al2, k) != 0) {
            printf("Error al añadir %d a al2\n", k);
        }
    }

    // 8) Mostrar al2
    printArr(&al2);

    // Auditoría opcional: recomputar suma y tamaño para verificar integridad
    recomputeSum(&al1);
    recomputeSum(&al2);
    // printArr(&al1);
    // printArr(&al2);

    // Salidas adicionales de utilidad
    // (Opcional) Mostrar tamaño y suma de al1 y al2
    // printf("al1 -> size: %d, add: %d\n", getArrSize(&al1), getArrAdd(&al1));
    // printf("al2 -> size: %d, add: %d\n", getArrSize(&al2), getArrAdd(&al2));

    return EXIT_SUCCESS;
}
