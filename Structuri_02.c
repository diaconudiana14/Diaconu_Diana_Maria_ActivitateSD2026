#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
  int nrMatricol;
  int varsta;
  char* nume;
  float medie;
  char grupa;
};

struct Student initializare(int nrMatricol, int varsta, char* nume, float medie, char grupa){
  struct Student s;
  s.nrMatricol = nrMatricol;
  s.varsta = varsta;
  s.nume = (char*)malloc(strlen(nume)+1);
  strcpy_s(s.nume, (strlen(nume)+1), nume);
  s.medie = medie;
  s.grupa = grupa;

  return s;
}

void afisareStudent(struct Student s){
  if(s.nume != NULL){
    printf("Studentul cu numarul matricol %d si numele %s are varsta %d, media %.2f si este din grupa %c.\n", s.nrMatricol, s.nume, s.varsta, s.medie, s.grupa);
  }
  else{
    printf("Studentul cu numarul matricol %d si fara numele are varsta %d, media %.2f si este din grupa %c.\n", s.nrMatricol, s.varsta, s.medie, s.grupa);
  }
}

void modificaMedie(struct Student* s, float medieNoua){
  if(medieNoua >=1 && medieNoua<=10){
    s->medie = medieNoua;
  }
}

void dezalocareStudent(struct Student* s){
  if(s->nume != NULL){
    free(s->nume);
    s->nume = NULL;
  }
}

int main(){
  struct Student s;
  s = initializare(34, 19, "Popescu Ion", 8.75, 'A');
  afisareStudent(s);

  modificaMedie(&s, 9.49);
  afisareStudent(s);

  dezalocareStudent(&s);
  afisareStudent(s);
}
