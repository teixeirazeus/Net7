
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
