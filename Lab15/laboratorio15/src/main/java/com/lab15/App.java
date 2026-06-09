package com.lab15;

import java.io.IOException;
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Scanner;

import com.lab15.estructuras.Archivo;
import com.lab15.estructuras.MemoriaManager;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args ) throws IOException
    {
        Path carpeta = Path.of("laboratorio15/archivos");
        ArrayList<Archivo> archivosCarpeta = new ArrayList<>();

        Scanner sc = new Scanner(System.in);
        MemoriaManager mm = new MemoriaManager(100*MemoriaManager.KB); //En bytes
        while (true) { 

            //RECARGO ARCHIVOS DE CARPETA POR SI SE CREO ALGUNO EN EJECUCIÓN. 
            try (DirectoryStream<Path> archivos = Files.newDirectoryStream(carpeta)) {
                for (Path archivoPath : archivos) {

                    String nombre = archivoPath.getFileName().toString();
                    long tamanio = Files.size(archivoPath);
                    boolean existe = false;

                    for (Archivo a : archivosCarpeta) {
                        if (a.getNombre().equals(nombre)) {
                            existe = true;
                            break;
                        }
                    }

                    if (!existe) {
                        archivosCarpeta.add(new Archivo(nombre, tamanio));
                    }
                }
            }
            

            imprimirMenu();
            int opcion = sc.nextInt();
            sc.nextLine();

            switch (opcion) {
                case 1:
                    listarArchivosDisco(archivosCarpeta);
                    break;
                case 2:
                    System.out.println("Digite el nombre del archivo a cargar en memoria: ");
                    String nombreArchivo = sc.nextLine();
                    Archivo archivoNuevo = obtenerArchivoDiscoPorNombre(archivosCarpeta, nombreArchivo);
                    mm.cargarEnMemoria(archivoNuevo);
                    break;
                case 3:
                    System.out.println("Digite el nombre del archivo a eliminar de memoria: ");
                    String archivoNombre = sc.nextLine();
                    Archivo archivoEliminar = mm.obtenerArchivoCargadoPorNombre(archivoNombre);
                    mm.liberarDeMemoria(archivoEliminar);
                    break;
                case 4:
                    mm.listarArchivosCargados();
                    break;
                case 5:
                    return;
                default:
                    System.out.println("ERROR: OPCION NO RECONOCIDA. INTENTE NUEVAMENTE.");
            }
        }
    }

    public static void imprimirMenu(){
    System.out.println("===== SIMULADOR DE MEMORIA Y ARCHIVOS =====");
    System.out.println("\n1. Listar archivos disponibles en disco.");
    System.out.println("\n2. Cargar archivo en memoria.");
    System.out.println("\n3. Liberar archivo de memoria.");
    System.out.println("\n4. Ver estado de la memoria.");
    System.out.println("\n5. Salir");
    }

    public static void listarArchivosDisco(ArrayList<Archivo> archivosCargados){
        System.out.println("--- ARCHIVOS EN DISCO ---");
        for (Archivo a: archivosCargados){
            System.out.println(a.getNombre()+" | "+a.getTamanio()+" Bytes");
        }
        System.out.println("--- FIN DEL LISTADO ---");
    }

    public static Archivo obtenerArchivoDiscoPorNombre(ArrayList<Archivo> archivosDisco, String nombre){
        for (Archivo a: archivosDisco){
            if (a.getNombre().equals(nombre)){
                return a;
            }
        }
        return null;
    }
}
