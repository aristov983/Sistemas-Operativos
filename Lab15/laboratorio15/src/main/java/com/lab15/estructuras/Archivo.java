package com.lab15.estructuras;

public class Archivo {
    private final String nombre;
    private long tamanio;

    public Archivo(String nombre, long tamanio){
        this.nombre = nombre;
        this.tamanio = tamanio;
    }

    public String getNombre(){
        return this.nombre;
    }

    public long getTamanio(){
        return this.tamanio;
    }
}
