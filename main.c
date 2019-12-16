/*

Thiago da Silva Teixeira teixeira.zeus@gmail.com

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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "matrix.h"
#include "network.h"
#include "rwdata.h"


int main(int argc, char const *argv[]) {

  char name[] = "xor.dat";
  data* d = readData(name, ',');
  matrix_print(d->atributes);
  matrix_print(d->target);



  int size = 5;
  network* net;
  net = network_create(3,20,1,5);

  //matrix_random(target);
  net->print_epoch = 10;
  while(1)
    network_train(net, d->atributes, d->target, 0.01, 1000);

  matrix_free(d->atributes);
  matrix_free(d->target);
  free(d);
  network_free(net);

  return 0;
}
