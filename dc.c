#include "calc_helper.h"
#include "strutil.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stdio.h"

void raiz_cuadrada(pilanum_t *pila_nums, struct calc_token* tok, calc_num* resultado, bool* error) {
    calc_num num1;
    
    desapilar_num(pila_nums, &num1);
                   
    if (num1 >= 0 && (*tok).oper.op == OP_RAIZ) {
        double num_sq = (double) num1;
        calc_num sq = sqrt(num_sq);
        *resultado = sq;
    }
    else *resultado = 0, *error = true;
}

void potencia(calc_num* num1, calc_num* num2, calc_num* resultado, bool* error) {
    if (*num1 < 0) *resultado = 0, *error = true;
                       
    else {
        double pow_num1 = (double) *num1; double pow_num2 = (double) *num2;
        calc_num pw = pow(pow_num2, pow_num1);
        *resultado = pw;
    }
}

void logaritmo(calc_num* num1, calc_num* num2, calc_num* resultado, bool* error) {
    if (*num1 < 2) *resultado = 0, *error = true;
    
    else *resultado = (calc_num) (log((double) *num2) / log((double) *num1));
}

bool pila_llena(pilanum_t *pila_nums, calc_num* resultado, bool* error) {
    if (pila_esta_vacia(pila_nums)) {
        *resultado = 0;
        *error = true;
        return false;
    }
    return true;
}

bool guardar_numero(struct calc_token* tok, pilanum_t *pila_nums, calc_num* resultado) {
    apilar_num(pila_nums, (*tok).value);
    *resultado = (*tok).value;
    return true;
}

bool calcular_resultado_un_operador(struct calc_token* tok, pilanum_t *pila_nums, calc_num* resultado, bool* error) {
    if (!pila_llena(pila_nums, resultado, error)) return false;
    else raiz_cuadrada(pila_nums, tok, resultado, error);
    return true;
}

bool calcular_resultado_dos_operadores(struct calc_token* tok, pilanum_t *pila_nums, calc_num* resultado, bool* error) {
    calc_num num1; calc_num num2;
    int opeador = (*tok).oper.op;
                   
    desapilar_num(pila_nums, &num1);
                   
    if (!pila_llena(pila_nums, resultado, error)) return false;
                   
    desapilar_num(pila_nums, &num2);
                   
    if (opeador == OP_ADD) *resultado = num2 + num1;
                   
    else if (opeador == OP_SUB) *resultado = num2 - num1;
                   
    else if (opeador == OP_MUL) *resultado = num2 * num1;
                   
    else if (opeador == OP_DIV) {
        if (num1 == 0) *resultado = 0, *error = true;          
        else *resultado = num2 / num1;
    }
                   
    else if (opeador == OP_POW) potencia(&num1, &num2, resultado, error);  
                   
    else if (opeador == OP_LOG) logaritmo(&num1, &num2, resultado, error);

    return true;
}

void resultado_oper_ternario(pilanum_t *pila_nums, struct calc_token* tok, calc_num* resultado, bool* error) {
    calc_num num1; calc_num num2; calc_num num3;
    
    bool des1 = desapilar_num(pila_nums, &num1); bool des2 = desapilar_num(pila_nums, &num2); bool des3 = desapilar_num(pila_nums, &num3);
    
    if (!des1 || !des2 || !des3) *resultado = 0, *error = true;
                   
    else if ((*tok).oper.op == OP_TERN) *resultado = (num3 == 0) ? num1 : num2;
}

void mostrar_dc(pilanum_t *pila_nums, calc_num* resultado, bool* error) {
    desapilar_num(pila_nums, resultado);
        
    if (desapilar_num(pila_nums, resultado)) *error = true;
        
    if (*error) printf("ERROR\n");
        
    else printf("%ld\n", *resultado);
}
 
void destruir_dc(pilanum_t *pila_nums, char** arreglo) {
    pilanum_destruir(pila_nums);
    free_strv(arreglo);
}

void calcular_resultado(struct calc_token* tok, pilanum_t *pila_nums, char** arreglo, calc_num* resultado, bool* error) {
    for (int i = 0; arreglo[i] != NULL; i++) {
           if (!calc_parse(arreglo[i], tok)) {
               *error = true;
               continue;
           }
           
           else if((*tok).type == TOK_NUM) { if (guardar_numero(tok, pila_nums, resultado)) continue; }
           
           else if ((*tok).type == TOK_OPER) {
               if ((*tok).oper.num_operandos == 1) { if (!calcular_resultado_un_operador(tok, pila_nums, resultado, error)) continue; }
               
               else if ((*tok).oper.num_operandos == 2) { if (!calcular_resultado_dos_operadores(tok, pila_nums, resultado, error)) continue; }
               
               else if ((*tok).oper.num_operandos == 3) resultado_oper_ternario(pila_nums, tok, resultado, error);
           }
           apilar_num(pila_nums, *resultado);
        }
}


int main (void) {
    char* linea = NULL;
    size_t tam = 0;
    
    while((getline(&linea, &tam, stdin)) != EOF) {
        char** arreglo = dc_split(linea);
        struct calc_token tok;
        pilanum_t *pila_nums = pilanum_crear();
        calc_num resultado = 0;
        bool error = false; 
        calcular_resultado(&tok, pila_nums, arreglo, &resultado, &error);
        mostrar_dc(pila_nums, &resultado, &error);
        destruir_dc(pila_nums, arreglo);
    }
    free(linea);
    return 0;
}