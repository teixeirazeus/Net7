


void readData(){
  FILE *fp;
  char buff[255];

  fp = fopen("xor.dat", "r");

  while (fgets(buff, 255, (FILE*)fp) != NULL){
   printf(">>: %s\n", buff );
  }

  fclose(fp);
}
