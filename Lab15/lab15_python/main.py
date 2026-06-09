from pathlib import Path


class Archivo:
    def __init__(self, nombre, tamanio):
        self.nombre = nombre
        self.tamanio = tamanio

    def __eq__(self, other):
        return isinstance(other, Archivo) and self.nombre == other.nombre

    def __hash__(self):
        return hash(self.nombre)


class MemoriaManager:
    KB = 1024
    MEMORIA_TOTAL = 100 * KB

    def __init__(self):
        self.capacidad = self.MEMORIA_TOTAL
        self.memoria_uso = 0
        self.archivos_memoria = []

    def memoria_libre(self):
        return self.capacidad - self.memoria_uso

    def porcentaje_uso(self):
        return (self.memoria_uso * 100) / self.capacidad

    def cargar_en_memoria(self, archivo):
        if archivo in self.archivos_memoria:
            print("\nERROR: EL ARCHIVO YA ESTA CARGADO EN MEMORIA.")
            return

        if archivo.tamanio > self.memoria_libre():
            print("\nERROR: NO HAY MEMORIA SUFICIENTE.")
            print(f"Memoria disponible: {self.memoria_libre()} bytes")
            print(f"Tamaño del archivo: {archivo.tamanio} bytes")
            return

        self.archivos_memoria.append(archivo)
        self.memoria_uso += archivo.tamanio

        print("\nArchivo cargado correctamente.")

    def liberar_de_memoria(self, nombre_archivo):
        for archivo in self.archivos_memoria:
            if archivo.nombre == nombre_archivo:
                self.archivos_memoria.remove(archivo)
                self.memoria_uso -= archivo.tamanio
                print("\nArchivo liberado correctamente.")
                return

        print("\nERROR: EL ARCHIVO NO ESTA CARGADO EN MEMORIA.")

    def mostrar_estado(self):
        print("\n===== ESTADO DE MEMORIA =====")
        print(f"Memoria total: {self.capacidad} bytes")
        print(f"Memoria utilizada: {self.memoria_uso} bytes")
        print(f"Memoria libre: {self.memoria_libre()} bytes")
        print(f"Uso de memoria: {self.porcentaje_uso():.2f}%")

        print("\nArchivos cargados:")
        if not self.archivos_memoria:
            print("(ninguno)")
        else:
            for archivo in self.archivos_memoria:
                print(f"- {archivo.nombre} | {archivo.tamanio} bytes")


def obtener_archivos_disco():
    carpeta = Path("lab15_python/archivos")
    archivos = []

    if not carpeta.exists():
        print("La carpeta 'archivos' no existe.")
        return archivos

    for ruta in carpeta.iterdir():
        if ruta.is_file():
            archivos.append(
                Archivo(
                    ruta.name,
                    ruta.stat().st_size
                )
            )

    return archivos


def listar_archivos_disco():
    archivos = obtener_archivos_disco()

    print("\n===== ARCHIVOS DISPONIBLES =====")

    if not archivos:
        print("No se encontraron archivos.")
        return

    for archivo in archivos:
        kb = archivo.tamanio / 1024
        print(
            f"- {archivo.nombre} | "
            f"{archivo.tamanio} bytes | "
            f"{kb:.2f} KB"
        )


def buscar_archivo(nombre):
    for archivo in obtener_archivos_disco():
        if archivo.nombre == nombre:
            return archivo
    return None


def imprimir_menu():
    print("\n===== SIMULADOR DE MEMORIA Y ARCHIVOS =====")
    print("1. Listar archivos disponibles en disco")
    print("2. Cargar archivo en memoria")
    print("3. Liberar archivo de memoria")
    print("4. Ver estado de la memoria")
    print("5. Salir")


def main():
    memoria = MemoriaManager()

    while True:
        imprimir_menu()

        opcion = input("\nSeleccione una opcion: ")

        match opcion:
            case "1":
                listar_archivos_disco()

            case "2":
                nombre = input(
                    "Ingrese nombre del archivo a cargar: "
                )

                archivo = buscar_archivo(nombre)

                if archivo is None:
                    print("\nERROR: EL ARCHIVO NO EXISTE EN DISCO.")
                else:
                    memoria.cargar_en_memoria(archivo)

            case "3":
                nombre = input(
                    "Ingrese nombre del archivo a liberar: "
                )

                memoria.liberar_de_memoria(nombre)

            case "4":
                memoria.mostrar_estado()

            case "5":
                print("\nFinalizando programa...")
                break

            case _:
                print("\nERROR: OPCION NO VALIDA.")


if __name__ == "__main__":
    main()