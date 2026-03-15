#define _CRT_SERCURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Laptop{
  int id;
  int RAM;
  char* producator;
  float pret;
  char procesor;
};

struct Laptop initializare (int id, int ram, char* producator, float pret,  char procesor){
  struct Laptop l;
  l.id = id;
  l.RAM = ram;

  l.producator = (char*)malloc(strlen(producator)+1);
  strcpy_s(l.producator, (strlen(producator)+1), producator);

  l.pret = pret;
  l.procesor = procesor;

  return l;
}

void afisare(struct Laptop l){
  if(l.producator != NULL){
    printf("%d. Laptopul %s cu procesor %c, cu %d GB RAM costa %.2f RON.\n", l.id, l.producator, l.procesor, l.RAM, l.pret);
  }else{
    printf("%d. Laptopul fara producator, cu procesor %c, cu %d GB RAM costa %.2f RON.\n", l.id, l.procesor, l.RAM, l.pret);
  }
}

void modificarePret(struct Laptop* l, float noulPret){
  if(noulPret>0){
    l->pret = noulPret;
  }
}

void dezalocare(struct Laptop* l){
  if(l->producator != NULL){
    free(l->producator);
    l->producator = NULL;
  }
}

int main(){
  struct Laptop l;
  l=initializare(1,32, "ASUS", 3999.99, 'I');
  afisare(l);

  modificarePret(&l, 3500.34);
  afisare(l);

  dezalocare(&l);
  afisare(l);

  return 0;
}
