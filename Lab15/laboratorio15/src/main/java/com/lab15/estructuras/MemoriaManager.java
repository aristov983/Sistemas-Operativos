package com.lab15.estructuras;

import java.util.ArrayList;

public class MemoriaManager {

    public static final int KB = 1024;
    public static final int MB = 1024 * KB;
    public static final int GB = 1024 * MB;


    private long memoriaUso;
    private long capacidad;
    private ArrayList<Archivo> archivosMemoria;

    public MemoriaManager(long capacidad){
        this.capacidad = capacidad;
        this.memoriaUso = 0;
        this.archivosMemoria = new ArrayList<>();
    }

    public long getCapacidad(){
        return this.capacidad;
    }

    public long getMemoriaUso(){
        return this.memoriaUso;
    }

    public double getPorcentajeUso(){
        return this.capacidad == 0 ? 0.0 : (this.memoriaUso * 100.0) / this.capacidad;
    }

    public void cargarEnMemoria(Archivo archivo){
        
        if (!this.archivosMemoria.contains(archivo)){
            if (archivo != null  && archivo.getTamanio() <= (this.capacidad - this.memoriaUso)){
                this.memoriaUso += archivo.getTamanio();
                this.archivosMemoria.add(archivo);
            } else System.out.println("\nEL ARCHIVO NO EXISTE O NO HAY SUFICIENTE MEMORIA EN EL SISTEMA.");
        } else System.out.println("\nERROR AL CARGAR EN MEMORIA: EL ARCHIVO YA ESTA CARGADO EN MEMORIA.");
    }

    public void liberarDeMemoria(Archivo archivo){
        if (archivo != null && this.archivosMemoria.contains(archivo)){
            this.memoriaUso -= archivo.getTamanio();
            this.archivosMemoria.remove(archivo);
        } else System.out.println("\nERROR AL LIBERAR DE MEMORIA: EL ARCHIVO NO EXISTE O NO ESTÁ CARGADO EN MEMORIA.");
    }

    public void listarArchivosCargados(){
        System.out.println("--- RESUMEN DE MEMORIA ---");
        System.out.println(" | Capacidad: "+this.capacidad+" bytes | En uso: "+this.memoriaUso+" bytes |");
        System.out.println("--- ARCHIVOS CARGADOS ---");
        for (Archivo a: this.archivosMemoria){
            double porcentaje = this.capacidad == 0 ? 0.0 : (a.getTamanio() * 100.0) / this.capacidad;
            System.out.println("ARCHIVO ["+a.getNombre()+"] | MEMORIA: "+a.getTamanio() + " | USO DE MEMORIA: "+porcentaje+"%");
        }
        System.out.println("--- FIN DE LISTADO ---\n");
    }

    public Archivo obtenerArchivoCargadoPorNombre(String nombre){
        for (Archivo a: archivosMemoria){
            if (a.getNombre().equals(nombre)){
                return a;
            }
        }
        return null;
    }
}
