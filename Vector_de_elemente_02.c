#define _CRT_SERCURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Angajat{
  int id;
  int vechime;
  char* nume;ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
  float salariu;
  char departament;
};

struct Angajat initializare(int id, int vechime, char* nume, float salariu, char departament){
  struct Angajat a;
  a.id = id;
  a.vechime = vechime;
  a.nume = (char*)malloc(strlen(nume)+1);
  strcpy_s(a.nume, (strlen(nume)+1), nume);
  a.salariu = salariu;
  a.departament = departament;

  return a;
}

void afisareAngajat(struct Angajat a){
  if(a.nume!=NULL){
    printf("\n%d. Angajatul %s cu vechimea %d din departamentul %c are salariul %.2f ", a.id, a.nume, a.vechime, a.departament, a.salariu);
  }
}

void afisareVectorAngajati(struct Angajat* vector, int nrElemente){
  for(int i=0; i<nrElemente;i++){
    afisareAngajat(vector[i]);
  }
}

struct Angajat getPrimulAngajatDupaNume(struct Angajat* vector, int nrElemente, const char* nume){
  struct Angajat a;
  a.nume = NULL;
  for(int i=0; i<nrElemente;i++){
    if(strcmp(vector[i].nume,nume)==0){
      a=vector[i];
      a.nume=(char*)malloc(strlen(vector[i].nume)+1);
      strcpy(a.nume, vector[i].nume);
      return a;
    }
  }
  return a;
}

float salariulMaxim(struct Angajat* vector, int nrElemente){
  float max = vector[0].salariu;
  for (int i=0; i<nrElemente;i++){
    if(vector[i].salariu>max){
      max=vector[i].salariu;
    }
  }
  return max;
}

void dezalocareAngajati(struct Angajat** vector, int* nrElemente){
  for(int i=0; i<*nrElemente; i++){
      free((*vector)[i].nume);
  }
  free(*vector);
  *vector = NULL;
  *nrElemente = 0;
}

int main (){
  int nr = 3;
  struct Angajat* angajati = (struct Angajat*)malloc(sizeof(struct Angajat) * nr);
  angajati[0] = initializare(1, 3, "Popescu Ion", 6300, 'A');
  angajati[1] = initializare(2, 1, "Dumitru Alexandru", 4000, 'B');
  angajati[2] = initializare(3, 5, "Ilie Marius", 7000, 'C');

  afisareVectorAngajati(angajati, nr);

  struct Angajat a = getPrimulAngajatDupaNume(angajati, nr, "Dumitru Alexandru");
  printf("\nAngajatul cautat este:\n");
  afisareAngajat(a);

  printf("\nSalariul maxim este: %.2f\n", salariulMaxim(angajati, nr));

  if(a.nume != NULL){
    free (a.nume);
  }

  dezalocareAngajati(&angajati, &nr);
  return 0;
}