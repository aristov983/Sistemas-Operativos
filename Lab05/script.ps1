while ($true){
    Clear-Host
    Write-Host "***** UCU-Campus Salto || SO 2026 *****"
    Write-Host "---------- Menu Principal ----------"
    Write-Host "1. Ordenar lista."
    Write-Host "2. Repetir la palabra N veces."
    Write-Host "3. Listar usuarios locales del equipo."
    Write-Host "4. Salir"
    Write-Host "5. Secreto"
    Write-Host "------------------------------------"
    $option = Read-Host "Elegi una opcion"
    switch ($option){
        "1"{
            $lista = @()
            for ($i = 0; $i -lt 5; $i++) {
                $lista += [int](Read-Host "Ingrese un numero: ")
            }
            $ordenada = $lista | Sort-Object -Descending
            Write-Host "Lista ordenada: "
            Write-Host $ordenada
            Pause
        }
        "2"{
            $palabra = Read-Host "Ingresa una palabra: "
            do {
                $iteraciones = [int](Read-Host "Ingresa la cantidad de repeticiones (mayor a 0 menor a 100): ")
            } while ($iteraciones -le 0 -or $iteraciones -ge 100 )
            
            for ($i = 0; $i -lt $iteraciones; $i++) {
                Write-Host $palabra
            }
            Pause
        }
        "3"{
            Write-Host "Mostrando usuarios..."
            $usuarios = Get-LocalUser
            $usuarios | Out-Host
            Pause
        }
        "4"{
            Write-Host "Adios"
            return
        }
        "5"{
            Write-Host "Se instalo el virus de minado de bitcoin, iniciado minado..."
            Pause
        }
    }
}