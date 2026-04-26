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

//creare structura pentru un nod dintr-o lista dublu inlantuita
struct Nod{
  Masina info;
  struct Nod* next;
  struct Nod* prev;
};
typedef struct Nod Nod;
//creare structura pentru Lista Dubla 
struct ListaDubla{
  Nod* cap;
  Nod* tail;
  int nrNoduri;
};
typedef struct ListaDubla Lista;


Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Lista lista) {
	Nod* cap = lista.cap;
  while(cap){
    afisareMasina(cap->info);
    cap = cap->next; 
  }
}

void adaugaMasinaInLista(Lista* lista,Masina masinaNoua) {
  Nod* nou = (Nod*) malloc(sizeof(Nod));
  nou->info = masinaNoua;
  nou->next = NULL;
  if((lista->cap)){
    lista->tail->next = nou;
    nou->prev = lista->tail;
    lista->tail = nou;
  }
  else{
    nou->prev = NULL;
    lista->cap = nou;
    lista->tail = nou;
  }
  lista->nrNoduri++;
}

void adaugaLaInceputInLista(Lista* lista, Masina masinaNoua) {
	Nod* nou = (Nod*) malloc(sizeof(Nod));
  nou->info = masinaNoua;
  nou->prev = NULL;
  nou->next = lista->cap;
  if(lista->cap){
    lista->cap->prev = nou;
  }
  else{
    lista->tail = nou;
  }
  lista->cap = nou;
  lista->nrNoduri++;
}

Lista* citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f=fopen(numeFisier, "r");
  Lista* lista = malloc(sizeof(Lista));
  lista->nrNoduri = 0;
  lista->cap = NULL;
  lista->tail = NULL;
  while(!feof(f)){
    adaugaMasinaInLista(lista,citireMasinaDinFisier(f));
  }
  fclose(f);
  return lista;
}

void dezalocareLDMasini(Lista* lista) {
	while(lista->cap){
    Nod* p = lista->cap;
    lista->cap = p->next;
    free(p->info.model);
    free(p->info.numeSofer);
    free(p);
    lista->nrNoduri--;
  }
  lista->tail = NULL;
  free(lista);
}

float calculeazaPretMediu(Lista lista) {
	float total = 0;
  int counter = 0;
  Nod* p = lista.cap;
  while(p){
    total+=p->info.pret;
    counter++;
    p = p->next;
  }
  if(counter>0){
    return total / counter;
  }
	else{
    return 0;}
}

void stergeMasinaDupaID(Lista* lista, int id) {
  Nod* p = lista->cap;
  while(p && p->info.id != id){
    p = p->next;
  }
  if(!p){
    return;
  }
   
  if(p->prev){
    p->prev->next = p->next;
    }
  else{
    lista->cap = p->next;
  }
  if(p->next){
    p->next->prev = p->prev;
  }
  else{
    lista->tail = p->prev;
  }
    
     free(p->info.model);
     free(p->info.numeSofer);
     free(p);
     lista->nrNoduri--;
  }

char* getNumeSoferMasinaScumpa(Lista lista) {
	Nod* p = lista.cap;
  float max = 0;
  char* nume;
  while(p){
    if(p->info.pret > max){
      max = p->info.pret;
      nume = p->info.numeSofer;
    }
    p = p->next;
  }
  return nume;
}

int main() {
  Lista* lista = citireLDMasiniDinFisier("masini.txt");
  afisareListaMasini(*lista);
  // dezalocareLDMasini(lista);
  // afisareListaMasini(*lista);
  printf("pret mediu: %.2f\n",calculeazaPretMediu(*lista));
  // stergeMasinaDupaID(lista, 2);
  // afisareListaMasini(*lista);
  printf("nume sofer: %s\n",getNumeSoferMasinaScumpa(*lista));
	return 0;
}