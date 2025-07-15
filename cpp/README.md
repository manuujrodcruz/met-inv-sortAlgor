# Benchmark de Algoritmos de Ordenamiento - C++

**Autores:** Carolina Bencosme-10148929 y Manuel Rodríguez-10150681  
**Curso:** ICC-Metodología de la Investigación, PUCMM 2024-2025

## Descripción

Este proyecto implementa un benchmark comprehensivo de algoritmos de ordenamiento en C++, comparando el rendimiento de diferentes algoritmos con diversos tamaños de arrays y distribuciones de datos.

## Algoritmos Implementados

1. **BubbleSort** - O(n²) - Timeouts adaptativos hasta 1 hora
2. **MergeSort** - O(n log n) - Timeout 10 minutos
3. **QuickSort** - O(n log n) promedio - Timeout 10 minutos
4. **HeapSort** - O(n log n) - Timeout 10 minutos
5. **CountingSort** - O(n + k) - Timeout 10 minutos, rangos optimizados

## Tamaños de Arrays

- 1,000 elementos
- 10,000 elementos
- 50,000 elementos
- 100,000 elementos
- 500,000 elementos
- 1,000,000 elementos

## Distribuciones de Datos

1. **HighRepetition** - Valores con alta repetición (1-5)
2. **UniqueValues** - Valores únicos generados aleatoriamente
3. **PartiallyOrdered** - Arrays parcialmente ordenados (80% ordenado)
4. **PositiveNegative** - Mix de valores positivos y negativos
5. **RandomPure** - Valores completamente aleatorios

## Configuración del Benchmark

- **10 repeticiones** por combinación algoritmo-tamaño-distribución
- **Timeouts adaptativos** según algoritmo y tamaño
- **Sin omisiones** - Todos los algoritmos se ejecutan para todos los tamaños
- **Seed fija (42)** para reproducibilidad
- **Estimación de tiempo**: 8-12 horas para completarse

## Timeouts Configurados

### BubbleSort (Adaptativos)
- Arrays 1K-50K: 5 minutos
- Arrays 50K-100K: 10 minutos  
- Arrays 100K-500K: 30 minutos
- Arrays 500K-1M: 1 hora

### Otros Algoritmos
- MergeSort, QuickSort, HeapSort: 10 minutos
- CountingSort: 10 minutos (rangos optimizados)

## Archivos del Proyecto

- `main.cpp` - Implementación principal del benchmark
- `simple_json.h` - Serialización JSON simple
- `benchmark_config.h` - Configuración de timeouts y límites
- `run_benchmark.sh` - Script para ejecutar el benchmark completo
- `monitor_benchmark.sh` - Script para monitorear el progreso
- `README.md` - Este archivo

## Compilación y Ejecución

### Opción 1: Ejecución Automática (Recomendada)
```bash
./run_benchmark.sh
```

### Opción 2: Ejecución Manual
```bash
g++ -std=c++17 -O2 -pthread main.cpp -o benchmark
./benchmark
```

### Opción 3: Monitoreo del Progreso
```bash
./monitor_benchmark.sh
```

## Comportamiento Esperado

### Tiempos de Ejecución Aproximados
- **Arrays pequeños (1K-10K)**: Segundos a minutos
- **Arrays medianos (50K-100K)**: Minutos a horas
- **Arrays grandes (500K-1M)**: Puede tomar varias horas

### ⚠️ ADVERTENCIA: Tiempo Total Estimado
El benchmark completo puede tardar **8-12 horas** en completarse, con BubbleSort tomando la mayor parte del tiempo para arrays grandes.

### Algoritmos y Comportamiento
- ✅ **Todos los algoritmos se ejecutan para todos los tamaños**
- ✅ **No hay omisiones** - Solo timeouts como protección
- ✅ **BubbleSort**: Timeouts adaptativos hasta 1 hora para 1M elementos
- ✅ **CountingSort**: Rangos optimizados para evitar problemas de memoria
- ⚠️ **Timeouts**: Se registran pero no impiden el benchmark completo

## Notas de Rendimiento

1. **BubbleSort** es extremadamente lento para arrays grandes
2. **CountingSort** puede consumir mucha memoria con rangos grandes
3. **QuickSort** usa implementación iterativa para evitar stack overflow
4. **MergeSort** y **HeapSort** son más consistentes en tiempo

## Resultados Esperados

El benchmark debería completarse en tiempo razonable (horas, no días) y proporcionar datos científicamente válidos para la comparación de algoritmos de ordenamiento.

## Solución de Problemas

### Si el programa se cuelga:
- Verificar que los timeouts estén funcionando
- Revisar el uso de memoria del sistema
- Considerar reducir el tamaño máximo de arrays

### Si hay errores de compilación:
- Verificar que tienes C++17 o superior
- Instalar pthread si es necesario
- Verificar que todos los headers estén disponibles

### Si los resultados son inconsistentes:
- Verificar que el sistema no esté bajo carga pesada
- Considerar ejecutar multiple veces y promediar
- Revisar la configuración de optimización del compilador
