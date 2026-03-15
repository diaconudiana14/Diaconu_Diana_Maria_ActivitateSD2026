#include<stdio.h>
#include<stdlib.h>

struct Telefon { //Definirea unei structuri Telefon
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie;
};

struct Telefon initializare(int id, int ram, const char* producator, float pret, char serie)  { //Functie de initializare
	struct Telefon t;
  	//initializare structura 
	s.id = id;
  t.RAM = ram;
  t.producator = (char*)malloc(sizeof(char)*(strlen(producator)+1));
  strcpy_s(t.producator, (strlen(producator)+1), producator);
  t.pret = pret;
  t.serie = serie;
	return t;
}

void afisare(struct Telefon t) { //functie de afisare
	//afisarea tuturor atributelor.
  printf("%d. Telefonul %s seria %c are %d Gb RAM si costa %5.2f RON\n", t.id, t.producator, t.serie, t.RAM, t.pret)
}

void modifica_Atribut(struct Telefon t) { //Functie de modificare a valorii unui atribut
	//modificarea unui atribut
}

void dezalocare(struct Telefon *t) { //Functie de dezalocare care sa dezaloce ceea ce am alocat in cadrul acelui articol
	//dezalocare campuri alocate dinamic
}

int main() { //Functia main in care sa testam ceea ce am implementat
	struct Telefon t;
  t=initializare(1,256, "Samsung", 2000.5, 'A');
  afisare(t);

	return 0;
}