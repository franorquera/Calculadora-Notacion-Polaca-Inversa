#include "calc_helper.h"
#include "cola.h"
#include "strutil.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stdio.h"

bool agregar_elemento(struct calc_token* tok, struct calc_token* tope, pila_t* pila_oper, cola_t* cola_nums) {
    if ((*tok).oper.asociatividad != ASSOC_LEFT && (*tok).oper.precedencia >= (*tope).oper.precedencia) return false;
    
    else if ((*tok).oper.asociatividad != ASSOC_RIGHT && (*tok).oper.precedencia > (*tope).oper.precedencia) return false;

    cola_encolar(cola_nums, pila_desapilar(pila_oper));
    return true;
}

void obtener_valor_token(struct calc_token* tope, char* tope_parse, pila_t* pila_oper) {
    tope_parse =  pila_ver_tope(pila_oper);
    calc_parse(tope_parse, tope);
}

void tok_parentesis(struct calc_token* tok, struct calc_token* tope, pila_t* pila_oper, cola_t* cola_nums, char* tope_parse) {
    while (!pila_esta_vacia(pila_oper)) {
        obtener_valor_token(tope, tope_parse, pila_oper);
                   
        if ((*tope).type == TOK_LPAREN) return;

        if (!agregar_elemento(tok, tope, pila_oper, cola_nums)) return;
    }
}

void tok_parentesis_derecho(struct calc_token* tope, pila_t* pila_oper, cola_t* cola_nums, char* tope_parse) {
    obtener_valor_token(tope, tope_parse, pila_oper);

    while ((*tope).type != TOK_LPAREN) {
        cola_encolar(cola_nums, pila_desapilar(pila_oper));
        obtener_valor_token(tope, tope_parse, pila_oper);
    }
    pila_desapilar(pila_oper);
}

void destruir_estructuras_auxiliares(pila_t* pila_oper, cola_t* cola_nums, char** arreglo) {
    pila_destruir(pila_oper);
    cola_destruir(cola_nums, NULL);
    free_strv(arreglo);
}

void mostrar_infix(pila_t* pila_oper, cola_t* cola_nums) {
    while (!pila_esta_vacia(pila_oper)) cola_encolar(cola_nums, pila_desapilar(pila_oper));
    while (!cola_esta_vacia(cola_nums)) printf("%s ", (char*) cola_desencolar(cola_nums));
    printf("\n");
}

void conversion_postfijo(struct calc_token* tok, struct calc_token* tope, pila_t* pila_oper, cola_t* cola_nums, char** arreglo, char* tope_parse) {
    for (int i = 0; arreglo[i] != NULL; i++) {
            if (!calc_parse(arreglo[i], tok)) continue;

            else if((*tok).type == TOK_NUM) cola_encolar(cola_nums, arreglo[i]);
           
            else if ((*tok).type == TOK_OPER)  {
                tok_parentesis(tok, tope, pila_oper, cola_nums, tope_parse);
                pila_apilar(pila_oper, arreglo[i]);
            }

           else if ((*tok).type == TOK_LPAREN) pila_apilar(pila_oper, arreglo[i]);
           
           else if ((*tok).type == TOK_RPAREN) tok_parentesis_derecho(tope, pila_oper, cola_nums, tope_parse);
        }
}


int main (void) {
    char* linea = NULL;
    size_t tam = 0;
    
    while((getline(&linea, &tam, stdin)) != EOF) {
        struct calc_token tok;
        struct calc_token tope;
        pila_t* pila_oper = pila_crear();
        cola_t* cola_nums = cola_crear();
        char** arreglo = infix_split(linea);
        char* tope_parse = NULL;
        conversion_postfijo(&tok, &tope, pila_oper, cola_nums, arreglo, tope_parse);
        mostrar_infix(pila_oper, cola_nums);
        destruir_estructuras_auxiliares(pila_oper, cola_nums, arreglo);
    }
    free(linea);
    return 0;
}