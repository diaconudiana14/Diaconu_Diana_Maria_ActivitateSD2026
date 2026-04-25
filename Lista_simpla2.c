#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod{
  Masina info;
  struct Nod* next;
};
typedef struct Nod Nod;


Masina citireMasinaDinFisier(FILE* file){
  char buffer[100];
  char sep[3]=",\n";
  char* aux;
  Masina m;
  fgets(buffer, 100, file);

  m.id = atoi(strtok(buffer, sep));
  m.nrUsi = atoi(strtok(NULL, sep));
  m.pret = atof(strtok(NULL, sep));
  
  aux = strtok(NULL, sep);
  m.model = malloc(strlen(aux)+1);
  strcpy(m.model, aux);
  
  aux = strtok(NULL, sep);
  m.numeSofer = malloc(strlen(aux)+1);
  strcpy(m.numeSofer, aux);
  
  m.serie = *strtok(NULL, sep);
  
  return m;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* cap) {
	Nod* p = cap;
  while(p){
    afisareMasina(p->info);
    p = p->next;
  }
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
  Nod* nou = (Nod*) malloc(sizeof(Nod));
  nou->info = masinaNoua;
  nou->next = NULL;
  Nod* p = *cap;
  if(p){
    while(p->next){
      p = p->next;
    }
    p->next = nou;
  }
  else{
    *cap = nou;
  }
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
	Nod* nou = (Nod*) malloc(sizeof(Nod));
  nou->info = masinaNoua;
  nou->next = *cap;
  *cap = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
  Nod* cap = NULL;
  while(!feof(f)){
    adaugaMasinaInLista(&cap,citireMasinaDinFisier(f));
  }
  fclose(f);
  return cap;
}

void dezalocareListaMasini(Nod** cap) {
 while(*cap){
  Nod* p = *cap;
  *cap = p->next;
  free(p->info.numeSofer);
  free(p->info.model);
  free(p);
 }  
}

float calculeazaPretMediu(Nod* cap) {
  float total = 0;
  int counter = 0;
  Nod* p = cap;
  while(p){
    total += p->info.pret;
    counter++;
    p=p->next;
  }
  if(counter > 0){
	return total / counter;}
  else{
    return 0;}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap ,const char* numeSofer) {
	float total=0;
  Nod* p = cap;
  while(p){
    if(strcmp(p->info.numeSofer, numeSofer)==0){
      total+=p->info.pret;
    }
    p = p->next;
  }
	return total;
}

int main() {
  Nod* cap = citireListaMasiniDinFisier("masini.txt");
  afisareListaMasini(cap);
  printf("pret mediu: %.2f\n",calculeazaPretMediu(cap));
  printf("total sofer: %.2f\n",calculeazaPretulMasinilorUnuiSofer(cap, "Ionescu"));
	return 0;
}