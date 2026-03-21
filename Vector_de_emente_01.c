#define _CRT_SERCURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Carte{
  int id;
  int nrPagini;
  char* titlu;
  float pret;
  char gen;
};
typedef struct Carte Carte;

Carte initializare(int id, int nrPagini, char* titlu, float pret, char gen){
  Carte c;
  c.id = id;
  c.nrPagini = nrPagini;
  c.titlu = (char*)malloc(strlen(titlu)+1);
  strcpy_s(c.titlu, (strlen(titlu)+1), titlu);
  c.pret = pret;
  c.gen = gen;

  return c;
}

void afisareCarte(Carte c){
  if(c.titlu != NULL){
    printf("Carte %d: titlu %s, numar pagini %d, pret %.2f, gen %c\n", c.id, c.titlu, c.nrPagini, c.pret, c.gen);
  }
}

void afisareVectorCarti(Carte* vector, int nrElemente){
  for(int i=0;i<nrElemente; i++){
    afisareCarte(vector[i]);
  }
}

Carte* copiazaPrimeleNCarti (Carte* vector, int Elemente, int n){
  Carte* copie = (Carte*)malloc(sizeof(Carte)*n);
  for(int i=0; i< n; i++){
    copie[i]=vector[i];
    copie[i].titlu = (char*)malloc(strlen(vector[i].titlu)+1);
    strcpy_s(copie[i].titlu, (strlen(vector[i].titlu)+1), vector[i].titlu);
  }
  return copie;
}
void copiazaCartiScumpe(Carte* vector, int nrElemente, float prag, Carte** rezultat, int* dimensiune){
  *dimensiune = 0;
  for(int i=0; i<nrElemente; i++){
    if(vector[i].pret>prag){
      (*dimensiune)++;
    }
  }
  *rezultat = (Carte*)malloc(sizeof(Carte)*(*dimensiune));
  int k=0;
  for(int i=0; i<nrElemente; i++){
    if(vector[i].pret>prag){
      (*rezultat)[k]=vector[i];
      (*rezultat)[k].titlu=(char*)malloc(strlen(vector[i].titlu)+1);
      strcpy((*rezultat)[k].titlu, vector[i].titlu);
      k++;
    }
  }
}

Carte getPrimaCarteDupaTitlu(Carte* vector, int nrElemente, const char* titlu){
  Carte c;
  c.titlu = NULL;
  for(int i=0; i<nrElemente; i++){
    if(strcmp(vector[i].titlu, titlu) == 0){
      c=vector[i];
      c.titlu = (char*)malloc(strlen(vector[i].titlu)+1);
      strcpy(c.titlu, vector[i].titlu);
      return c;
    }
  }
  return c;
}
float calculeazaPretMediu(Carte* vector, int nrElemente){
  float suma = 0;
  for(int i=0; i<nrElemente; i++){
    suma += vector[i].pret;
  }
  return suma/nrElemente;
}

void dezalocareVectorCarti(Carte** vector, int* nrElemente){
  for(int i=0; i<*nrElemente; i++){
    if((*vector)[i].titlu != NULL){
      free((*vector)[i].titlu);
    }
  }
  free(*vector);
  *vector = NULL;
  *nrElemente = 0;
}

int main(){
  int nrCarti = 3;
  Carte* carti = (Carte*)malloc(sizeof(Carte) * nrCarti);

  carti[0] = initializare(1, 150, "Baltagul", 32.5, 'R');
  carti[1] = initializare(2, 300, "Harry Potter", 70.54, 'F');
  carti[2] = initializare(3, 200, "Baltagul", 32.5, 'R');

  printf("\n Cartile: \n");
  afisareVectorCarti(carti, nrCarti);

  Carte* primeleCarti = copiazaPrimeleNCarti(carti, nrCarti, 2);
  int nrPrimeleCarti = 2;
  printf("\nPrimele carti sunt: \n");
  afisareVectorCarti(primeleCarti, nrPrimeleCarti);

  Carte* cartiScumpe = NULL;
  int nrCartiScumpe = 0;
  copiazaCartiScumpe(carti, nrCarti, 30, &cartiScumpe, &nrCartiScumpe);
  printf("\nCartile scumpe sunt:\n");
  afisareVectorCarti(cartiScumpe, nrCartiScumpe);

  struct Carte carteCautata  = getPrimaCarteDupaTitlu(carti, nrCarti, "Harry Potter");
  printf("\nCartea cautata dupa titlu este:\n");
  afisareCarte(carteCautata);

  printf("\nPret mediu: %.2f\n", calculeazaPretMediu(carti, nrCarti));

  dezalocareVectorCarti(&primeleCarti, &nrPrimeleCarti);
  dezalocareVectorCarti(&cartiScumpe, &nrCartiScumpe);
  if (carteCautata.titlu != NULL) {
    free(carteCautata.titlu);
  }

  dezalocareVectorCarti(&carti, &nrCarti);
  return 0;

}