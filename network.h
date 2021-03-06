
typedef struct network{
  int input, hidden, output, hidden_size;
  matrix** synapse; // n_input+n_hidden
  matrix** layers; // n_input+n_hidden +1
  matrix** deltas; // n_input+n_hidden
  int print_epoch;
}network;

float sigmoid(float x){
   return x / (1 + abs(x));
}

float sigmoid_dv(float x){
  return x*(1-x);
}

network* network_create(int input, int hidden, int output, int hidden_size){
  network* net = malloc(sizeof(network));
  net->input = input;
  net->hidden = hidden;
  net->output = output;
  net->hidden_size = hidden_size;

  // synapse
  net->synapse = malloc(sizeof(matrix*)*(hidden_size+2));

  net->synapse[0] = matrix_create(input, hidden);
  matrix_random(net->synapse[0]);
  for (size_t i = 1; i < hidden_size+1; i++) {
    net->synapse[i] = matrix_create(hidden, hidden);
    matrix_random(net->synapse[i]);
  }
  net->synapse[hidden_size+1] = matrix_create(hidden, output);
  matrix_random(net->synapse[hidden_size+1]);

  // layer
  // reserva layer[0] para input
  net->layers = malloc(sizeof(matrix*)*(hidden_size+3));
  for (size_t i = 1; i < hidden_size+3; i++) {
    net->layers[i] = matrix_create(1,1);
  }

  //deltas
  net->deltas = malloc(sizeof(matrix*)*(hidden_size+2));
  for (size_t i = 0; i < hidden_size+2; i++) {
    net->deltas[i] = matrix_create(1,1);
  }

  net->print_epoch = 10;

  return net;
}

void network_free(network* net){
  for (size_t i = 0; i < net->hidden_size +2; i++) {
    matrix_free(net->synapse[i]);
  }
  free(net->synapse);

  // layer[0] input
  for (size_t i = 1; i < net->hidden_size +3; i++) {
    matrix_free(net->layers[i]);
  }
  free(net->layers);

  for (size_t i = 0; i < net->hidden_size +2; i++) {
    matrix_free(net->deltas[i]);
  }
  free(net->deltas);

  free(net);
}

void network_forward(network* net, matrix* input){
  //matrix_free(net->layers[0]);
  net->layers[0] = input;

  for (int i = 1; i < net->hidden_size +3; i++) {
    matrix_free(net->layers[i]);
    net->layers[i] = matrix_dot(net->layers[i-1], net->synapse[i-1]);
    matrix_map(net->layers[i], sigmoid);
  }
}

float network_train(network* net, matrix* input, matrix* target, float lr, int n_epoch){
  float epoch_erro, sum;
  for (int e = 0; e < n_epoch; e++) {

    // forward
    network_forward(net, input);
    // delta
    matrix* erro;

    erro = matrix_sub(target, net->layers[net->hidden_size+2]);
    epoch_erro = matrix_sum_abs(erro)/erro->row;

    matrix_map(net->layers[net->hidden_size+2], sigmoid_dv);
    matrix_free(net->deltas[net->hidden_size+1]);

    net->deltas[net->hidden_size+1] = matrix_hadamard(erro, net->layers[net->hidden_size+2]);

    matrix* sy_t;
    for (int i = net->hidden_size+1; i > 0; i--) {
      // erro = self.deltas[i].dot(self.synapse[i].T)
      matrix_free(erro);
      sy_t = matrix_transpose(net->synapse[i]);
      erro = matrix_dot(net->deltas[i], sy_t);
      matrix_free(sy_t);

      matrix_map(net->layers[i], sigmoid_dv);
      matrix_free(net->deltas[i-1]);
      net->deltas[i-1] = matrix_hadamard(erro, net->layers[i]);

    }
    matrix_free(erro);

    // update weights
    //for i in reversed(range(len(self.synapse))):
    //    self.synapse[i] += (self.layers[i].T.dot(self.deltas[i]))*lr
    matrix *layer_t, *result;
    for (int i = net->hidden_size+1; i >= 0; i--) {
      layer_t = matrix_transpose(net->layers[i]);
      result = matrix_dot(layer_t, net->deltas[i]);
      matrix_mult(result, lr);
      matrix_add_s(net->synapse[i], result);
      matrix_free(layer_t);
      matrix_free(result);
    }

    if((e)%net->print_epoch == 0){
      printf("Epoch %i erro: %f\n", e, epoch_erro);
    }

  }
  return epoch_erro;
}

void matrix_write(matrix *m, FILE *file){
  fprintf(file, "M\n");
  fprintf(file, "%i %i\n", m->row, m->column);
  for (int line = 0; line < m->row; line++) {
    for (int c = 0; c <  m->column; c++) {
      if(c == m->column-1){
        fprintf(file, "%f\n", m->values[line][c]);
      }else{
        fprintf(file, "%f ", m->values[line][c]);
      }
    }
  }
}

void network_save(network *net, char filename[]){
  FILE *fp;
  fp = fopen(filename, "w+");
  fprintf(fp, "%i %i %i %i\n", net->input, net->hidden, net->output, net->hidden_size);
  for (int i = 0; i < net->hidden_size+2; i++) {
    matrix_write(net->synapse[i], fp);
  }
  fclose(fp);
}

network* network_load(char filename[]){
  FILE *fp;
  char buff[255];
  fp = fopen(filename, "r");

  int netArg[4], netai=0;
  for (int i = 0; i < 4; i++) {
    fscanf(fp, "%s", buff);
    netArg[netai++] = atoi(buff);
  }
  network* net;
  net = network_create(netArg[0],netArg[1],netArg[2],netArg[3]);


  //printf("final >> %s\n", buff);

  for (int isy = 0; isy < netArg[3]+2; isy++) {
    fscanf(fp, "%s", buff);
    if(buff[0] != 'M'){break;}
    fscanf(fp, "%s", buff);
    fscanf(fp, "%s", buff);

    for (int i = 0; i < net->synapse[isy]->row; i++) {
        for (int j = 0; j < net->synapse[isy]->column; j++) {
            fscanf(fp, "%s", buff);
            net->synapse[isy]->values[i][j] = atof(buff);
            printf("%f\n", net->synapse[isy]->values[i][j]);
        }
    }

  }

  fclose(fp);

  //network* net = malloc(sizeof(network));

  return net;
}
