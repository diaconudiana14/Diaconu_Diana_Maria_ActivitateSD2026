#include<stdio.h>
#include<malloc.h>
#include<string.h>

struct Avion{
  char* model;
  int nrLocuri;
  int nrLocuriOcupate;
  float* preturiBilete;
};

struct Avion initAvion(const char* model, int nrLocuri, int nrLocuriOcupate, float* preturiBilete){
  struct Avion avion;
  avion.model = (char*)malloc(sizeof(char)*(strlen(model)+1));
  strcpy(avion.model, model);
  avion.nrLocuri = nrLocuri;
  avion.nrLocuriOcupate = nrLocuriOcupate;
  avion.preturiBilete = (float*)malloc(sizeof(float) * nrLocuriOcupate);
  for(int i = 0; i < nrLocuriOcupate; i++){
    avion.preturiBilete[i] = preturiBilete[i];
  }
  return avion;
}

void afisareAvion(struct Avion avion){
  printf("Avionul %s are %d locuri, dar au fost ocupate doar %d: ",
         avion.model, avion.nrLocuri, avion.nrLocuriOcupate);
  for(int i = 0; i < avion.nrLocuriOcupate; i++){
    printf("%5.2f, ", avion.preturiBilete[i]);
  }
  printf("\n");
}

int main(){
  float preturi[] = {10, 20, 30};
  struct Avion avion = initAvion("A330", 300, 3, preturi);
  afisareAvion(avion);
  return 0;
}