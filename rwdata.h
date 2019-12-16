int file_lines(FILE *fp){
  int count=0;
  char c;
  for (c = getc(fp); c != EOF; c = getc(fp)){
    if (c == '\n') count++;
  }
  fseek(fp, 0, SEEK_SET);
  return count;
}

int str_len(char str[]){
  int count = 0;
  while (str[count] != '\0') {
    count++;
  }
  return count;
}

typedef struct data{
  matrix *atributes;
  matrix *target;
}data;

float *line2array(char line[], int size){
  float *array = malloc(sizeof(float)*size);
  char cut[40]; int cut_i = 0;
  int array_i = 0, line_i = -1;
  while (array_i != size) {
    line_i++;
    if(line[line_i] == ',' || line[line_i] == '\n' || line[line_i] == '\0'){
      cut[cut_i] = '\0';
      array[array_i++] = atof(cut);
      cut_i = 0;
    }else{
      cut[cut_i++] = line[line_i];
    }
  }
  return array;
}

data *readData(){
  data *d = malloc(sizeof(data));
  int size_train = 2, size_target = 1;
  FILE *fp;
  char buff[255];

  fp = fopen("xor.dat", "r");
  int file_size = file_lines(fp);
  float *a;

  matrix* train = matrix_create(file_size, size_train+1);
  matrix* target = matrix_create(file_size, size_target);

  int line_count = 0;
  while (fgets(buff, 255, (FILE*)fp) != NULL){
    a = line2array(buff, size_train+size_target);

    //train
    int i;
    for (i = 0; i < size_train; i++) {
      train->values[line_count][i] = a[i];
    }
    train->values[line_count][size_train] = 1.0;//bias

    //target
    for (i = 0; i < size_target; i++) {
      target->values[line_count][i] = a[i+size_train];
    }

    line_count++;

    free(a);
  }
  fclose(fp);

  d->atributes = train;
  d->target = target;
  return d;
}
