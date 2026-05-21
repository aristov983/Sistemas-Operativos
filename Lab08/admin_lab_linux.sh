#!/bin/bash
# ──────────────────────────────────
# Nombre: script.sh
# Autor: Emmanuel Aristov
# Fecha: $(date '+%d-%m-%Y')
# Descripción: Script Lab08
# ──────────────────────────────────

#funciones
crear_carpeta(){
    echo "intentando crear carpeta '$1'"
    
    if [ -d "$1" ]; then
        echo "la carpeta '$1' ya existe"
    else
        mkdir -p "$1"
        echo "Carpeta '$1' creada exitosamente"
    fi
}


#codigo

echo 'Laboratorio 08      | Universidad Católica del Uruguay'
echo 'Sistemas Operativos | Profesor: Jorge Martinez        '
echo 'Estudiante: Emmanuel Aristov                          '   

echo 'Parte 1: Estructura de directorios...'

if [ "$PWD" != "$HOME" ]; then
    cd "$HOME" || {
        echo "no se puede acceder al directorio home"
        exit 1
    }
fi

crear_carpeta "laboratorio_linux"
cd "laboratorio_linux" || {
    echo "no se puede acceder al directorio laboratorio_linux"
    exit 1
    }

for carpeta in documentos respaldos logs sistema; do
    crear_carpeta "$carpeta"
done

cd "logs" || {
    echo "no se puede acceder al directorio logs"
    exit 1
}

echo "creando ejecucion.log en logs..."
touch ejecucion.log
echo "La fecha es: " > ejecucion.log
(date) >> ejecucion.log
cd ..

echo "Parte 2: Archivos y navegación..."
cd "documentos" || {
    echo "no se puede acceder a la ruta documentos"
    exit 1
}
echo "La ruta actual es: $(pwd)"

touch informe.txt
echo "=== LAB 08 | INFORME ===" > informe.txt
echo "Autor: Emmanuel Aristov" >> informe.txt
echo "Profesor: Jorge Martinez" >> informe.txt
echo -e "Contenido:\nAtención!\nGracias por su atención" >> informe.txt

touch notas.txt
echo "=== LAB 08 | NOTAS ===" > notas.txt
echo "1. Estudiar." >> notas.txt  
echo "2. Salvar la materia." >> notas.txt
echo "3. Ser multimillonario." >> notas.txt

touch comandos.txt
echo "=== LAB 08 | COMANDOS ===" > comandos.txt
echo "cd sirve para cambiar directorios" >> comandos.txt
echo "ls sirve para ver directorios en una ruta" >> comandos.txt
echo "hay mas comandos!" >> comandos.txt

echo "=== LAB 08 | Parte 3 y 4 ==="
echo "La ruta actual es: $(pwd)"
echo "=== Listando contenido... ==="
ls -l

echo "Parte 3: Usuarios, grupos y permisos"
echo "creando grupo 'estudiantes_linux'..."
if getent group estudiantes_linux > /dev/null; then
    echo "el grupo ya existe"
else
    sudo groupadd estudiantes_linux
    echo "grupo creado correctamente!"
fi

echo "cambiando propietario de informe.txt"
sudo chgrp estudiantes_linux informe.txt
echo "asignando permisos 640..."
chmod 640 informe.txt

echo "Parte 4: Manipulación de consultas"
cat /proc/cpuinfo > ../sistema/cpuinfo.txt
cat /proc/meminfo > ../sistema/meminfo.txt
echo "Archivos de sistema guardados correctamente"

echo "Si lees esto te debo un alfajor" > /dev/null

echo "Parte 5: Respaldo y reporte final"
cp *.txt ../respaldos/
echo "Archivos respaldados en carpeta respaldos"

reporte="../respaldos/reporte_final.txt"
echo "=== REPORTE FINAL ===" > "$reporte"
echo "Usuario: $(whoami)" >> "$reporte"
echo "Ruta actual: $(pwd)" >> "$reporte"
echo "Fecha: $(date)" >> "$reporte"

echo "Archivos creados:" >> "$reporte"
ls *.txt >> "$reporte"

echo "Permisos de informe.txt:" >> "$reporte"
ls -l informe.txt >> "$reporte"

echo "Confirmación acceso /proc: Si" >> "$reporte"
echo "Confirmación uso /dev/null: Si" >> "$reporte"

echo "Reporte final creado en respaldos/reporte_final.txt"

#Ta obviamente no es el script más prolijo, intente ir de lo más sencillo a ir probando instrucciones más lindas