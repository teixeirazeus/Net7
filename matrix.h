//
// Created by Thiago da Silva Teixeira on 21/11/2019.
//
/*
This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NEURAL_MATRIX_H
#define NEURAL_MATRIX_H

#endif //NEURAL_MATRIX_H


typedef struct matrix{
    int row, column;
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
            m->values[i][j] = (float)rand()/(float)(RAND_MAX/1);
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

float matrix_sum_abs(matrix *m){
    float sum=0;
    for (size_t i = 0; i < m->row; i++) {
        for (size_t j = 0; j < m->column; j++) {
            sum += abs(m->values[i][j]);
        }
    }
    return sum;
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
  if(m1->row != m2->row || m1->column != m2->column){
    printf("Hadmard erro m'okay!\n");
  }
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
        printf("Primeiro %i X %i\n", m1->row, m1->column);
        printf("Segundo %i X %i\n", m2->row, m2->column);
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
