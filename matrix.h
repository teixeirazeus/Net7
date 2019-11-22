//
// Created by Thiago da Silva Teixeira on 21/11/2019.
//

#ifndef NEURAL_MATRIX_H
#define NEURAL_MATRIX_H

#endif //NEURAL_MATRIX_H


typedef struct matrix{
    size_t row, column;
    float **values;
}matrix;


matrix *matrix_create(int row, int column){
    matrix *m = malloc(sizeof(matrix));
    m->row = row; m->column = column;
    m->values = (float**)malloc(sizeof(float*)*row);
    for (size_t i = 0; i < row; i++){
        m->values[i] = (float*) malloc(sizeof(float)*column);
    }
    return m;
}

void matrix_random(matrix *m){
    for (size_t i = 0; i < m->row; i++) {
        for (size_t j = 0; j < m->column; j++) {
            m->values[i][j] = (float)rand()/(float)(RAND_MAX/3);
            if (rand() & 1){
                m->values[i][j] = -m->values[i][j];
            }
        }
    }
}

void matrix_print(matrix *m){
    for (size_t i = 0; i < m->row; i++) {
        for (size_t j = 0; j < m->column; j++) {
            printf("%f ", m->values[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void matrix_free(matrix *m){
    for (size_t i = 0; i < m->row; i++) {
        free(m->values[i]);
    }
    free(m->values);
    free(m);
}

matrix *matrix_add(matrix *m1, matrix *m2){
    matrix *result = matrix_create(m1->row,m1->column);
    for (size_t i = 0; i < result->row; i++) {
        for (size_t j = 0; j < result->column; j++) {
            result->values[i][j] = m1->values[i][j] + m2->values[i][j];
        }
    }
    return result;
}

void matrix_add_s(matrix *m1, matrix *m2){
    for (size_t i = 0; i < m1->row; i++) {
        for (size_t j = 0; j < m1->column; j++) {
            m1->values[i][j] += m2->values[i][j];
        }
    }
}

matrix *matrix_sub(matrix *m1, matrix *m2){
    matrix *result = matrix_create(m1->row,m1->column);
    for (size_t i = 0; i < result->row; i++) {
        for (size_t j = 0; j < result->column; j++) {
            result->values[i][j] = m1->values[i][j] - m2->values[i][j];
        }
    }
    return result;
}


void matrix_mult(matrix *m, float scalar){
    for (size_t i = 0; i < m->row; i++) {
        for (size_t j = 0; j < m->column; j++) {
            m->values[i][j] *= scalar;
        }
    }
}

matrix *matrix_copy(matrix *m){
    matrix *copy = matrix_create(m->row, m->column);
    for (size_t i = 0; i < m->row; i++) {
        for (size_t j = 0; j < m->column; j++) {
            copy->values[i][j] = m->values[i][j];
        }
    }
    return copy;
}

matrix *matrix_hadamard(matrix *m1, matrix *m2){
    matrix *result = matrix_create(m1->row,m1->column);
    for (size_t i = 0; i < result->row; i++) {
        for (size_t j = 0; j < result->column; j++) {
            result->values[i][j] = m1->values[i][j] * m2->values[i][j];
        }
    }
    return result;
}


void matrix_hadamard_s(matrix *m1, matrix *m2){
    for (size_t i = 0; i < m1->row; i++) {
        for (size_t j = 0; j < m1->column; j++) {
            m1->values[i][j] *= m2->values[i][j];
        }
    }
}


matrix *matrix_transpose(matrix *m){
    matrix *result = matrix_create(m->column, m->row);
    for (size_t i = 0; i < result->row; i++) {
        for (size_t j = 0; j < result->column; j++) {
            result->values[i][j] = m->values[j][i];
        }
    }
    return result;
}

void matrix_map(matrix *m, float (*fun)(float)){
    for (size_t i = 0; i < m->row; i++) {
        for (size_t j = 0; j < m->column; j++) {
            m->values[i][j] = fun(m->values[i][j]);
        }
    }
}



matrix *matrix_dot(matrix *m1, matrix *m2){
    if (m1->column != m2->row){
        printf("Erro de dimensões\n");
    }
    matrix *result = matrix_create(m1->row, m2->column);
    for (size_t i = 0; i < m1->row; i++) {
        for (size_t j = 0; j < m2->column; j++) {
            result->values[i][j] = 0;
            for (size_t x = 0; x < m1->column; x++) {
                result->values[i][j] += m1->values[i][x]*m2->values[x][j];
            }
        }
    }
    return result;
}
