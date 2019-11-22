#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "matrix.h"

typedef struct network{
    matrix *weights_ih,*weights_ho,
            *bias_h, *bias_o;
    float learning_rate;
}network;

void network_print(network *net){
    puts("===Rede===\n");
    puts("weights_ih\n");
    matrix_print(net->weights_ih);
    matrix_print(net->bias_h);
    puts("weights_ho\n");
    matrix_print(net->weights_ho);
    matrix_print(net->bias_o);
    printf("Lr: %f\n", net->learning_rate);
    puts("=========\n");
}

/*
float sigmoid(float x){
    return x / (1 + fabs(x));
}
*/

float sigmoid(float x) {
    return (float) (1.0/(1.0 + expf(-x)));
}

float dsigmoid(float x) {
    return x * (1 - x);
}

network *network_create(int in, int hidden, int out, float lr){
    network *net = malloc(sizeof(network));

    net->weights_ih = matrix_create(in, hidden);
    matrix_random(net->weights_ih);

    net->weights_ho = matrix_create(hidden, out);
    matrix_random(net->weights_ho);

    net->bias_h = matrix_create(1, hidden);
    matrix_random(net->bias_h);

    net->bias_o = matrix_create(1, out);
    matrix_random(net->bias_o);

    net->learning_rate = lr;

    return net;
}

matrix *array_toMatrix(float *a, int size){
    matrix *m = matrix_create(1, size);
    for (size_t i = 0; i < size; i++) {
        m->values[0][i] = a[i];
    }
    return m;
}

void network_free(network *n){
    matrix_free(n->weights_ih);
    matrix_free(n->weights_ho);
    matrix_free(n->bias_h);
    matrix_free(n->bias_o);
    free(n);
}

float alt(float in){
    return -in;
}

matrix *feedForward(
        network *net,float *input_array){
    matrix *input = array_toMatrix(input_array, net->weights_ih->row);

    matrix *hidden_in = matrix_dot(input, net->weights_ih);
    hidden_in = matrix_add(hidden_in, net->bias_h);
    matrix_map(hidden_in, sigmoid);

    matrix *hidden_out = matrix_dot(hidden_in, net->weights_ho);
    hidden_out = matrix_add(hidden_out, net->bias_o);
    matrix_map(hidden_out, sigmoid);

    matrix_free(input);
    matrix_free(hidden_in);
    return hidden_out;
}

matrix *network_train(network *net,
                      float *input_array, float *target_array){

    // Converte array para matrix
    matrix *input = array_toMatrix(input_array, net->weights_ih->row);

    // Camada ih
    matrix *hidden_in = matrix_dot(input, net->weights_ih);
    hidden_in = matrix_add(hidden_in, net->bias_h);
    matrix_map(hidden_in, sigmoid);

    // Camada ho
    matrix *hidden_out = matrix_dot(hidden_in, net->weights_ho);
    hidden_out = matrix_add(hidden_out, net->bias_o);
    matrix_map(hidden_out, sigmoid);

    // Calculo de erro
    matrix *target = array_toMatrix(target_array, hidden_out->column);
    matrix *erro = matrix_sub(target, hidden_out);

    // Gradiente
    matrix *gradient = matrix_copy(hidden_out);
    matrix_map(gradient, dsigmoid);
    matrix *tmp = gradient;
    gradient = matrix_hadamard(gradient, erro);
    free(tmp);
    matrix_mult(gradient, net->learning_rate);

    // Calculo de delta
    matrix *hidden_in_t = matrix_transpose(hidden_in);
    matrix *delta_ho = matrix_dot(hidden_in_t, gradient);

    // Ajuste de peso
    matrix_add_s(net->weights_ho, delta_ho);
    matrix_add_s(net->bias_o, gradient);

    // Calculo de erro do layer
    matrix *weights_ho_t = matrix_transpose(net->weights_ho);
    matrix *hidden_erro = matrix_dot(erro, weights_ho_t);

    // Calculo de gradiente h
    matrix *gradient_h = matrix_copy(hidden_in);
    matrix_map(gradient_h, dsigmoid);
    tmp = gradient_h;
    gradient_h = matrix_hadamard(gradient_h, erro);
    free(tmp);
    matrix_mult(gradient_h, net->learning_rate);

    // Calculo de delta ih
    matrix *input_t = matrix_transpose(input);
    matrix *delta_ih = matrix_dot(input_t, gradient_h);

    // Ajuste de peso
    matrix_add_s(net->weights_ih, delta_ih);
    matrix_add_s(net->bias_h, gradient_h);

    matrix_free(input);
    matrix_free(input_t);
    matrix_free(target);
    matrix_free(hidden_in);
    matrix_free(hidden_in_t);
    matrix_free(delta_ho);
    matrix_free(delta_ih);
    matrix_free(weights_ho_t);
    matrix_free(hidden_erro);
    matrix_free(hidden_out);
    matrix_free(gradient);
    matrix_free(gradient_h);

    return erro;
}

int toInt(float n){
    return (int) n;
}


int main(int argc, char const *argv[]) {
    srand((unsigned) time(NULL));

    network *net = network_create(2,4,1,0.1);

    matrix *result;
    float cases_input[4][2] = {{0,0},{0,1},{1,0},{1,1}};
    float cases_target[4] = {0,1,1,0};
    float input[1], target[0];
    int c;
    float erro_total = 0.0;
    for (int i = 0; i < 5000*10; ++i) {
        c = rand() % 5;
        input[0] = cases_input[c][0];
        input[1] = cases_input[c][1];
        target[0] = cases_target[c];
        result = network_train(net, input, target);
        //matrix_print(result);
        //network_print(net);
        matrix_free(result);
    }

    printf("Teste de entrada\n");

    for (size_t c = 0; c < 4; c++) {
        input[0] = cases_input[c][0];
        input[1] = cases_input[c][1];
        target[0] = cases_target[c];
        printf("Entra %f %f > %f\n", input[0], input[1], target[0]);
        result = feedForward(net, input);
        matrix_print(result);
        matrix_free(result);
    }

    //matrix_free(result);
    network_free(net);

    return 0;
}
