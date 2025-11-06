// ej2.c - Ejercicio 2 (Feedback) - Sistemas Operativos (UAX)
// Autor: <NPAlumno>
//
// Descripción general:
// Este archivo implementa estructuras y funciones para calcular potencias (base^exp)
// y demuestra el uso de procesos (fork/exit/waitpid) y hebras (pthread).
//
// Requisitos de entorno:
// - Funciones POSIX: se recomienda compilar y ejecutar en Linux/WSL.
// - En Windows nativo, usar MSYS2 o adaptar la parte de procesos a CreateProcess.
//
// -----------------------------
// Apartado 3.1
// -----------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

// 1) Constante simbólica SIZE
#define SIZE 10

// 2) Estructura potencia_t
typedef struct potencia {
    int base;
    int exp;
    int potencia; // resultado base^exp
} potencia_t;

// 3) Tipo puntero a estructura potencia_t
typedef potencia_t* potenciaP_t;

// 4) setBaseExp: asigna base, exp y deja potencia = -1
void setBaseExp(potencia_t *p, int base, int exp) {
    if (!p) return;
    p->base = base;
    p->exp = exp;
    p->potencia = -1;
}

// 5) getPotencia: calcula base^exp sin usar pow
int getPotencia(int base, int exp) {
    if (exp < 0) {
        // No se requiere enunciado, pero dejamos 0 para exponentes negativos por simplicidad
        return 0;
    }
    if (exp == 0) return 1; // por convenio 0^0 -> 1 aquí
    long long res = 1;
    for (int i = 0; i < exp; ++i) {
        res *= base;
    }
    // Dado el rango en el ejercicio, cabe en int
    return (int)res;
}

// 6) setPotenciaEst: almacena en el campo potencia el resultado base^exp
void setPotenciaEst(potencia_t *p) {
    if (!p) return;
    p->potencia = getPotencia(p->base, p->exp);
}

// -----------------------------
// Apartado 3.2
// -----------------------------
// 1) initArrayEst: inicializa arr[i].base = i+1, exp=0, potencia=1
void initArrayEst(potencia_t arr[SIZE]) {
    for (int i = 0; i < SIZE; ++i) {
        arr[i].base = i + 1;
        arr[i].exp = 0;
        arr[i].potencia = 1;
    }
}

// 2) printArrayEst: muestra arr[i]: base: X exp: Y potencia Z
void printArrayEst(const potencia_t arr[SIZE], const char *name) {
    for (int i = 0; i < SIZE; ++i) {
        printf("%s[%d]: base: %d exp: %d potencia %d\n", name ? name : "arr", i, arr[i].base, arr[i].exp, arr[i].potencia);
    }
}

// 3) calcuPotHeb: función para hebras (threads)
void* calcuPotHeb(void *arg) {
    potencia_t *p = (potencia_t*)arg;
    if (p) {
        p->potencia = getPotencia(p->base, p->exp);
    }
    return NULL;
}

// -----------------------------
// Apartado 3.3 - main
// -----------------------------
int main(void) {
    // 1) Declarar arr1
    potencia_t arr1[SIZE];

    // 2) Inicializar arr1
    initArrayEst(arr1);

    // 3) Mostrar arr1
    printArrayEst(arr1, "arr1");

    // 4) setBaseExp -> base = índice, exp = 2; potencia = -1
    for (int i = 0; i < SIZE; ++i) {
        setBaseExp(&arr1[i], i, 2);
    }

    // 5) Mostrar arr1
    printArrayEst(arr1, "arr1");

    // 6) Crear SIZE hijos con fork() y que cada uno calcule su potencia, comunicar por exit
    for (int i = 0; i < SIZE; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return EXIT_FAILURE;
        } else if (pid == 0) {
            // Proceso hijo: calcula y devuelve por exit (0..255)
            int val = getPotencia(arr1[i].base, arr1[i].exp);
            // Asegurar rango 0..255 para exit (en este caso 0..81), ok
            _exit(val & 0xFF);
        } else {
            // Proceso padre: espera a este hijo y recoge el valor
            int status = 0;
            if (waitpid(pid, &status, 0) < 0) {
                perror("waitpid");
                return EXIT_FAILURE;
            }
            if (WIFEXITED(status)) {
                int val = WEXITSTATUS(status);
                arr1[i].potencia = val;
            } else {
                fprintf(stderr, "Hijo %d no terminó con exit normal\n", i);
            }
        }
    }

    // 7) Mostrar de nuevo arr1
    printArrayEst(arr1, "arr1");

    // 8) Declarar arr2
    potencia_t arr2[SIZE];

    // 9) Inicializar arr2
    initArrayEst(arr2);

    // 10) Mostrar arr2
    printArrayEst(arr2, "arr2");

    // 11) setBaseExp base = índice, exp = 2 en arr2
    for (int i = 0; i < SIZE; ++i) {
        setBaseExp(&arr2[i], i, 2);
    }

    // 12) Mostrar arr2
    printArrayEst(arr2, "arr2");

    // 13) Crear SIZE hebras para calcular y asignar potencia
    pthread_t tids[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        int rc = pthread_create(&tids[i], NULL, calcuPotHeb, (void*)&arr2[i]);
        if (rc != 0) {
            fprintf(stderr, "Error pthread_create en i=%d (rc=%d)\n", i, rc);
            return EXIT_FAILURE;
        }
    }
    for (int i = 0; i < SIZE; ++i) {
        pthread_join(tids[i], NULL);
    }

    // 14) Mostrar de nuevo arr2
    printArrayEst(arr2, "arr2");

    return EXIT_SUCCESS;
}
