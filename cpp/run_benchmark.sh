#!/bin/bash

# Script para compilar y ejecutar el programa interactivo de ordenamiento

echo "====================================================================="
echo "Compilando y Ejecutando el Benchmark Interactivo de Ordenamiento (C++)"
echo "====================================================================="

# Compilar el programa
echo "Compilando el programa..."
g++ -std=c++17 -O2 main.cpp -o interactive_sorter

if [ $? -ne 0 ]; then
    echo "Error en la compilación. Abortando."
    exit 1
fi

# Ejecutar el programa interactivo
echo "Iniciando programa interactivo..."
./interactive_sorter

echo "
Programa finalizado."