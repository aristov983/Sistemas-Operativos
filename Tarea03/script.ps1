function CrearGrupo ([string]$nombreGrupo, [string]$descripcion) {
    Write-Host "Creando grupo '$nombreGrupo'..."
    if (-not (Get-LocalGroup -Name $nombreGrupo -ErrorAction SilentlyContinue)) {
        New-LocalGroup -Name $nombreGrupo -Description $descripcion
        Write-Host "El grupo '$nombreGrupo' ha sido creado con éxito." -ForegroundColor Green
    } else {
        Write-Host "El grupo '$nombreGrupo' ya existe. No se realizaron cambios." -ForegroundColor Yellow
    }
}

#Crea un usuario (sin contraseña), obviamente verifica que exista el usuario antes no soy bobo.
function CrearUsuario([string]$nombreUsuario, [string]$descripcion) {
    Write-Host "Creando usuario '$nombreUsuario'..."
    if (-not (Get-LocalUser -Name $nombreUsuario -ErrorAction SilentlyContinue)){
        New-LocalUser -Name $nombreUsuario -Description $descripcion -NoPassword
                Write-Host "El usuario '$nombreUsuario' ha sido creado con éxito." -ForegroundColor Green

    } else {
        Write-Host "El usuario '$nombreUsuario' ya existe. No se realizaron cambios." -ForegroundColor Yellow
    }
}

function CrearUsuarioAgregarGrupo([string]$nombreUsuario, [string]$descripcion, [string]$nombreGrupo) {
    Write-Host "Creando usuario '$nombreUsuario'..."
    if (-not (Get-LocalUser -Name $nombreUsuario -ErrorAction SilentlyContinue)){
        New-LocalUser -Name $nombreUsuario -Description $descripcion -NoPassword
                Write-Host "El usuario '$nombreUsuario' ha sido creado con éxito." -ForegroundColor Green
                AgregarUsuarioGrupo $nombreUsuario $nombreGrupo
    } else {
        Write-Host "El usuario '$nombreUsuario' ya existe. Se intentará agregar al grupo '$nombreGrupo'" -ForegroundColor Yellow
        AgregarUsuarioGrupo $nombreUsuario $nombreGrupo
    }
}

#Verifica si éxiste el grupo, el usuario y si ambos existen lo agrega.
function AgregarUsuarioGrupo ([string]$nombreUsuario, [string]$nombreGrupo) {
    Write-Host "Intentando agregar '$nombreUsuario' al grupo '$nombreGrupo'..."
    if (Get-LocalGroup -Name $nombreGrupo -ErrorAction SilentlyContinue){
        if (Get-LocalUser -Name $nombreUsuario -ErrorAction SilentlyContinue){
            if (-not (Get-LocalGroupMember -Group $nombreGrupo | Where-Object {$_.Name -like "*$nombreUsuario"})){
                Add-LocalGroupMember -Group $nombreGrupo -Member $nombreUsuario -ErrorAction Stop
                Write-Host "'$nombreUsuario' fue agregado con éxito al grupo '$nombreGrupo'" -ForegroundColor Green
            }
            else {
                Write-Host "El usuario '$nombreUsuario' ya pertenece al grupo. No se realizaron cambios." -ForegroundColor Yellow
            }
        }
        else{
            Write-Host "El usuario '$nombreUsuario' no existe. No se realizaron cambios." -ForegroundColor Yellow
        }
    } else {
        Write-Host "El grupo '$nombreGrupo' no existe. No se realizaron cambios." -ForegroundColor Yellow
    }
}


function CrearCarpeta ([string]$rutaCarpeta){
    if(!(Test-Path -Path $rutaCarpeta)){
        Write-Host "Creando la carpeta '$rutaCarpeta'..."
        try {
            New-Item -Path $rutaCarpeta -ItemType Directory
            Write-Host "Carpeta creada con éxito." -ForegroundColor Green
        }
        catch {
            Write-Host "Error desconocido, no se pudo crear la carpeta" -ForegroundColor Red
        }
    }
    else {
        Write-Host "La carpeta en la ruta '$rutaCarpeta' ya existe. No se realizaron cambios." -ForegroundColor Yellow
    }
}

function CopiarCarpeta ([string]$rutaOriginal, [string]$rutaDestino) {
    Write-Host "Copiando '$rutaOriginal' en '$rutaDestino'..."

    if (!(Test-Path $rutaDestino)) {
        Write-Host "La carpeta destino no existe, se creará." -ForegroundColor DarkYellow
        CrearCarpeta $rutaDestino
    }

    $nombreCarpeta = Split-Path $rutaOriginal -Leaf
    $rutaRespaldo = Join-Path $rutaDestino $nombreCarpeta

    if (Test-Path $rutaRespaldo) {
        Write-Host "Ya existe respaldo previo, eliminando..." -ForegroundColor DarkYellow
        Remove-Item $rutaRespaldo -Recurse -Force
    }

    Copy-Item $rutaOriginal -Destination $rutaDestino -Recurse
    Write-Host "Respaldo realizado correctamente." -ForegroundColor Green
}

if (-not ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole(
    [Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Write-Host "Debe ejecutar este script como administrador." -ForegroundColor Red
    exit
}

while ($true){
    Clear-Host
    Write-Host "***** ANDA A SABER - TAREA 03 *****"
    Write-Host "---------- Menu Principal ----------"
    Write-Host "1. Correr script."
    Write-Host "2. Instrucciones de uso."
    Write-Host "3. Salir."
    Write-Host "------------------------------------"
    $option = Read-Host "Elige una opción: "
    switch ($option){
        "1"{

            #CREACION DE GRUPOS
            Write-Host " "
            Write-Host "====== CREACION DE GRUPOS LOCALES ======" -ForegroundColor Cyan
            Write-Host " "
            Write-Host "Creando grupo local 'Administracion'..."
            try {
                CrearGrupo "Administracion" "Grupo para el equipo de administracion"
            }
            catch {
                Write-Host "Error desconocido, algo salió mal." -ForegroundColor Red
            }

            Write-Host " "
            Write-Host "Creando grupo local 'Soporte'..."
            try {
                CrearGrupo "Soporte" "Grupo para el equipo muy trabajador de soporte"
            }
            catch {
                Write-Host "Error desconocido, algo salió mal." -ForegroundColor Red
            }

            Write-Host " "
            Write-Host "Creando grupo local 'Docentes'..."
            try {
                CrearGrupo "Docentes" "Grupo para el equipo de docentes"
            }
            catch {
                Write-Host "Error desconocido, algo salió mal." -ForegroundColor Red
            }
        
            #CREACION DE USUARIOS
            Write-Host " "
            Write-Host "====== CREACION DE USUARIOS Y ASIGNACION A GRUPOS ======" -ForegroundColor Cyan
            Write-Host " "
            try {
                CrearUsuarioAgregarGrupo "ana.admin" "Funcionaria ana de administración" "Administracion"
            }
            catch {
                Write-Host "Error desconocido, algo salió mal." -ForegroundColor Red
            }
            try {
                CrearUsuarioAgregarGrupo "bruno.soporte" "Funcionario bruno de soporte" "Soporte"
            }
            catch {
                Write-Host "Error desconocido, algo salió mal." -ForegroundColor Red
            }
            try {
                CrearUsuarioAgregarGrupo "carla.docente" "Funcionaria carla de docencia" "Docentes"
            }
            catch {
                Write-Host "Error desconocido, algo salió mal." -ForegroundColor Red
            }

            #CREACION DE ESTRUCTURA DE CARPETAS
            Write-Host " "
            Write-Host "====== CREACION DE RUTAS DE CARPETAS ======" -ForegroundColor Cyan
            Write-Host " "
            CrearCarpeta "C:\Empresa"
            CrearCarpeta "C:\Empresa\Administracion"
            CrearCarpeta "C:\Empresa\Soporte"
            CrearCarpeta "C:\Empresa\Docentes"
            CrearCarpeta "C:\Empresa\Compartida"

            #Copia de respaldo
            Write-Host " "
            Write-Host "====== CREACION DE RESPALDO / GENERACION DE NUEVO RESPALDO ======" -ForegroundColor Cyan
            Write-Host " "
            CrearCarpeta "C:\Respaldos"
            CopiarCarpeta "C:\Empresa" "C:\Respaldos"

            Write-Host " "
            Write-Host "====== ¡EJECUCION FINALIZADA! ======" -ForegroundColor Cyan
            Pause
        }
        "2"{
            Write-Host "====== GUIA DE USO ======" -ForegroundColor Cyan
            Write-Host " "
            Write-Host "1. Ejecutar PowerShell como Administrador."
            Write-Host "2. Habilitar scripts si es necesario:"
            Write-Host "   Set-ExecutionPolicy RemoteSigned"
            Write-Host "3. Ejecutar el archivo .ps1"
            Write-Host "4. Elegir la opción 1 para correr la configuración automática."
            Pause
        }
        "3"{
            Write-Host " "
            Write-Host "====== ADIOS ======"
            return
        }
        default{
            Write-Host "Opcion no reconocida..."
            Pause
        }
    }
}