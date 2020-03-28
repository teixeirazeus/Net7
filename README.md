Net7 - Multilayer Perceptron  library
==================
<br>
<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/9/9e/Complete_graph_K7.svg/800px-Complete_graph_K7.svg.png" width="200">

## Basic Usage
It creates a neural network with 3 neurons in the input layer, 1 neuron in the output layer and 5 hidden layers with 20 neurons each.
```
network* net = network_create(3,20,1,5);
```
Load train data named xor.dat with column separeted by ','.
```
char name[] = "xor.dat";
data* d = readData(name, ',');
```
Print erro every 10 epochs.

Train until erro < 0.01 or 1000 epochs.
```
net->print_epoch = 10;
network_train(net, d->atributes, d->target, 0.01, 1000);
```
Save network weights.
```
network_save(net, "brain.net7");
```
Load network weights.
```
network* net = network_load("brain.net7");
```
Free train data and network.
```
matrix_free(d->atributes);
matrix_free(d->target);
free(d);

network_free(net);

```
