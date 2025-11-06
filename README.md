# Ejercicios de Feedback 1 – Sistemas Operativos (UAX)
Autor: <NPAlumno>

## 1. Descripción
Este repositorio contiene la solución a los Ejercicios 1 y 2. El Ejercicio 3 (planificación y cronogramas) se documenta en la memoria aparte. Objetivos:
- Practicar C (estructuras, arrays, funciones).
- Usar primitivas de sistema: procesos (`fork`, `waitpid`) y hebras (`pthread`).
- Preparar análisis de planificación (RR multinivel vs FCFS por cola).

## 2. Archivos
| Archivo | Contenido |
|---------|----------|
| `ej1.c` | Estructura `arrayLength_t` y funciones (init, print, add, get, set, reset) con demostración en `main`. |
| `ej2.c` | Estructura `potencia_t`, cálculo de potencias con procesos e hilos. |
| `README.md` | Guía ampliada. |
| `Memoria_Ejercicio_Feedback.txt` | Plantilla para la memoria (se genera por separado). |

## 3. Entorno recomendado
Linux / WSL para soporte POSIX completo. En Windows puro, preferible WSL. MSYS2/MinGW puede compilar `pthread`, pero `fork` está limitado.

Instalación rápida (Ubuntu/WSL):
```bash
sudo apt update
sudo apt install -y build-essential
```

## 4. Compilación
### Linux / WSL
```bash
gcc -std=c11 -Wall -Wextra -O2 ej1.c -o ej1
gcc -std=c11 -Wall -Wextra -O2 ej2.c -o ej2 -lpthread
./ej1
./ej2
```

### Windows (MSYS2 MinGW64)
```bash
pacman -Syu
pacman -S --needed base-devel mingw-w64-x86_64-toolchain
gcc -std=c11 -Wall -Wextra -O2 ej1.c -o ej1.exe
gcc -std=c11 -Wall -Wextra -O2 ej2.c -o ej2.exe -lpthread
./ej1.exe
./ej2.exe
```

## 5. Ejercicio 1 – Diseño
- Valor libre: -1. Válidos: >= 0.
- Inserción secuencial sin huecos intermedios.
- `arrSize` y `arrAdd` mantenidos incrementalmente (O(1) consulta).
- `recomputeSum()` disponible para auditoría.
- `setElement` sólo modifica posiciones ocupadas (no cambia arrSize).

Salida de ejemplo:
```
{[0, 10, 20, 30, 40, 50, 60, 70, 80, 90], 10, 450}
{[0, 1, 20, 3, 40, 5, 60, 7, 80, 9], 10, 225}
```

## 6. Ejercicio 2 – Diseño
- Estructura `{ base, exp, potencia }`.
- Cálculo iterativo (sin `pow`).
- Procesos: cada hijo calcula potencia y la devuelve en `exit` (valor <= 255).
- Hilos: cálculo directo sobre el array compartido.
- Posible mejora: pipes/memoria compartida para resultados mayores.

Fragmento:
```
arr1[0]: base: 0 exp: 2 potencia 0
arr1[9]: base: 9 exp: 2 potencia 81
```

## 7. Ejercicio 3 – Resumen (ver memoria)
- RR multinivel (q1=2, q2=3, q3=4) con democión y bloqueo de PD.
- Variante prioridades + FCFS por cola.
- Cálculo de retorno y espera para PA, PB, PC, PD, PE.

## 8. Mejoras futuras
- Eliminación y compactación en Ejercicio 1.
- Comunicación más robusta entre procesos (pipes, shm).
- Makefile y pruebas unitarias.
- Medición de tiempos (gettimeofday) para hebras y procesos.

## 9. Troubleshooting
- `gcc: command not found` → Instalar build-essential / MSYS2 / WSL.
- `undefined reference to pthread_create` → Añadir `-lpthread` y compilar en entorno compatible.
- Sumas incoherentes → Ejecutar `recomputeSum()` (activar en código).

## 10. Licencia / Uso
Uso académico interno. Sustituye `<NPAlumno>` por tu identificador antes de entregar.

## 11. Próximos pasos
1. Añadir script de compilación (Makefile).
2. Exportar memoria con capturas de ejecución.
3. Extender Ejercicio 2 con manejo de errores detallado y logging.

---
Para la memoria PDF, referenciar secciones 5–7 y capturas de las ejecuciones.
# UAX Sistemas Operativos – Ejercicio de feedback 1

Este repositorio contiene dos archivos de código fuente requeridos para la entrega:

- `ej1.c`: Solución del Ejercicio 1 (arrays y funciones auxiliares).
- `ej2.c`: Solución del Ejercicio 2 (estructuras de potencias, procesos con `fork()` y hebras con `pthread`).

Los archivos incluyen comentarios internos indicando los apartados del enunciado.

## Compilación y ejecución

Recomendado: Linux o WSL (Windows Subsystem for Linux), ya que `ej2.c` utiliza POSIX (`fork`, `wait`, `pthread`). En Windows nativo, puede usar MSYS2/MinGW con soporte POSIX.

### En Linux/WSL

Compilar `ej1.c`:

```bash
gcc -std=c11 -Wall -Wextra -O2 ej1.c -o ej1
./ej1
```

Compilar `ej2.c` (requiere `pthread`):

```bash
gcc -std=c11 -Wall -Wextra -O2 ej2.c -o ej2 -lpthread
./ej2
```

### En Windows (PowerShell) con MSYS2/MinGW

- Instale MSYS2 y el toolchain `mingw-w64-x86_64-gcc` y `mingw-w64-x86_64-pthreads`.
- Abra la consola de MSYS2 MinGW64 y ejecute los mismos comandos que arriba.

## Notas de diseño

- En `ej1.c` se considera válido cualquier entero no negativo (`>= 0`) como dato. El valor `-1` indica posición libre.
- Las funciones gestionan `arrSize` y `arrAdd` de forma consistente al añadir o actualizar elementos. `setElement` solo permite actualizar posiciones ya ocupadas.
- En `ej2.c`, `getPotencia` implementa la potencia de forma iterativa sin usar `pow()`.
- Para la parte de procesos, cada hijo calcula `base^exp` y comunica el resultado al padre mediante `exit` (caben en un byte para los casos del enunciado). Para casos más grandes, se recomendaría tuberías o memoria compartida.
- En la parte de hebras, cada hebra escribe directamente el resultado en su estructura.

## Troubleshooting

- Si obtiene un error de enlazado relacionado con `pthread`, asegúrese de enlazar con `-lpthread` y usar un entorno con soporte POSIX.
- Si el compilador `gcc` no se encuentra en Windows, use WSL o MSYS2.
