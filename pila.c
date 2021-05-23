#include "pila.h"
#include <stdlib.h>
#define REDIMENSIONAR 2
#define CAPACIDAD 10
#define DISMINUIR_CANTIDAD 4

struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

pila_t *pila_crear(void) {
    pila_t *pila = malloc(sizeof(pila_t));
    if (pila == NULL) return NULL;
    pila->cantidad = 0;
    pila->capacidad = CAPACIDAD;
    pila->datos = malloc(pila->capacidad * sizeof(void*));
    if (!pila->datos) {
        free(pila);
        return NULL;
    }
    return pila;
}

void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
    return pila->cantidad == 0;
}

bool pila_redimensionar(pila_t *pila, size_t nueva_capacidad) {
    void *nuevos_datos = realloc(pila->datos, nueva_capacidad * sizeof(void*));
    if (!nuevos_datos) return false;
    pila->datos = nuevos_datos;
    pila->capacidad = nueva_capacidad;
    return true;
}

bool pila_apilar(pila_t *pila, void *valor) {
    if (pila->cantidad == pila->capacidad) {
        size_t nueva_capacidad = REDIMENSIONAR * pila->capacidad;
        bool redimension = pila_redimensionar(pila, nueva_capacidad);
        if (!redimension) return false;
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    return true;
}

void *pila_ver_tope(const pila_t *pila) {
    if (pila_esta_vacia(pila)) return NULL;
    return pila->datos[pila->cantidad - 1];
}

void *pila_desapilar(pila_t *pila) {
    if (pila_esta_vacia(pila)) return NULL;
    if (pila->capacidad > CAPACIDAD && pila->cantidad * DISMINUIR_CANTIDAD <= pila->capacidad) {
        size_t nueva_capacidad = pila->capacidad / REDIMENSIONAR;
        pila_redimensionar(pila, nueva_capacidad);
    }
    pila->cantidad--;
    return pila->datos[pila->cantidad];
}