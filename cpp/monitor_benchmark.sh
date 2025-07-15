#!/bin/bash

# Script para monitorear el progreso del benchmark
# Autores: Carolina Bencosme-10148929 y Manuel Rodríguez-10150681

echo "======================================================================"
echo "Monitor de Progreso del Benchmark"
echo "======================================================================"

# Verificar si el benchmark está corriendo
if pgrep -f "./benchmark" > /dev/null; then
    echo "✅ Benchmark está ejecutándose"
    PID=$(pgrep -f "./benchmark")
    echo "PID: $PID"
    
    # Mostrar el tiempo que lleva corriendo
    if [ -f benchmark_output.log ]; then
        echo ""
        echo "📊 Último progreso registrado:"
        tail -n 20 benchmark_output.log | grep -E "(Progress|Testing|Timeout|Skipping)" | tail -n 10
        
        echo ""
        echo "📈 Estadísticas del archivo de log:"
        echo "Líneas totales: $(wc -l < benchmark_output.log)"
        echo "Algoritmos procesados: $(grep -c "Testing algorithm" benchmark_output.log)"
        echo "Timeouts ocurridos: $(grep -c "Timeout:" benchmark_output.log)"
        echo "Algoritmos omitidos: $(grep -c "Skipping" benchmark_output.log)"
        
        # Mostrar progreso más reciente
        LATEST_PROGRESS=$(grep "Progress:" benchmark_output.log | tail -n 1)
        if [ ! -z "$LATEST_PROGRESS" ]; then
            echo ""
            echo "🔄 Progreso actual: $LATEST_PROGRESS"
        fi
    else
        echo "❌ No se encontró el archivo de log benchmark_output.log"
    fi
    
    echo ""
    echo "📱 Comandos útiles:"
    echo "  Ver progreso en tiempo real: tail -f benchmark_output.log"
    echo "  Filtrar solo progreso: tail -f benchmark_output.log | grep Progress"
    echo "  Detener benchmark: kill $PID"
    
else
    echo "❌ El benchmark no está ejecutándose"
    
    # Verificar si hay resultados previos
    if [ -f cpp_benchmark_results.json ]; then
        echo ""
        echo "📁 Se encontraron resultados previos:"
        echo "Archivo: cpp_benchmark_results.json"
        echo "Tamaño: $(du -h cpp_benchmark_results.json | cut -f1)"
        echo "Modificado: $(date -r cpp_benchmark_results.json)"
        
        # Mostrar estadísticas básicas del JSON
        if command -v jq &> /dev/null; then
            echo "Resultados totales: $(jq 'length' cpp_benchmark_results.json)"
            echo "Algoritmos únicos: $(jq '[.[].algorithm] | unique | length' cpp_benchmark_results.json)"
        else
            echo "Instala 'jq' para ver estadísticas detalladas del JSON"
        fi
    fi
    
    if [ -f benchmark_output.log ]; then
        echo ""
        echo "📋 Último log disponible:"
        echo "Tamaño: $(du -h benchmark_output.log | cut -f1)"
        echo "Últimas líneas:"
        tail -n 10 benchmark_output.log
    fi
fi

echo ""
echo "======================================================================"
