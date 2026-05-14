#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraCredit {
	int id;
	int durataLuni;
	float suma;
	float dobanda;
	char* numeClient;
	char* banca;
	unsigned char risc;
};

typedef struct StructuraCredit Credit;

struct Nod {
	Credit info;
	struct Nod* next;
};

typedef struct Nod Nod;

struct HashTable {
	int dim;
	Nod** vector;
};

typedef struct HashTable HashTable;

char* copiazaString(const char* text) {
	if (text == NULL) {
		return NULL;
	}

	char* copie = (char*)malloc(strlen(text) + 1);
	strcpy(copie, text);
	return copie;
}

Credit initializareCreditInvalid() {
	Credit c;
	c.id = -1;
	c.durataLuni = 0;
	c.suma = 0;
	c.dobanda = 0;
	c.numeClient = NULL;
	c.banca = NULL;
	c.risc = '-';
	return c;
}

Credit citireCreditDinFisier(FILE* file) {
	char buffer[200];
	char sep[3] = ",\n";

	Credit c = initializareCreditInvalid();

	if (fgets(buffer, 200, file) == NULL) {
		return c;
	}

	char* aux = strtok(buffer, sep);
	if (aux == NULL) {
		return c;
	}

	c.id = atoi(aux);

	aux = strtok(NULL, sep);
	c.durataLuni = atoi(aux);

	aux = strtok(NULL, sep);
	c.suma = (float)atof(aux);

	aux = strtok(NULL, sep);
	c.dobanda = (float)atof(aux);

	aux = strtok(NULL, sep);
	c.numeClient = copiazaString(aux);

	aux = strtok(NULL, sep);
	c.banca = copiazaString(aux);

	aux = strtok(NULL, sep);
	c.risc = aux[0];

	return c;
}

void afisareCredit(Credit credit) {
	printf("Id credit: %d\n", credit.id);
	printf("Durata luni: %d\n", credit.durataLuni);
	printf("Suma: %.2f\n", credit.suma);
	printf("Dobanda: %.2f%%\n", credit.dobanda);
	printf("Nume client: %s\n", credit.numeClient);
	printf("Banca: %s\n", credit.banca);
	printf("Risc: %c\n\n", credit.risc);
}

void afisareListaCredite(Nod* cap) {
	while (cap != NULL) {
		afisareCredit(cap->info);
		cap = cap->next;
	}
}

void adaugaCreditInLista(Nod** cap, Credit creditNou) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = creditNou;
	nodNou->next = NULL;

	if (*cap == NULL) {
		*cap = nodNou;
	}
	else {
		Nod* temp = *cap;

		while (temp->next != NULL) {
			temp = temp->next;
		}

		temp->next = nodNou;
	}
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);

	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}

	return ht;
}

int calculeazaHash(int idCredit, int dimensiune) {
	return idCredit % dimensiune;
}

void inserareCreditInTabela(HashTable hash, Credit credit) {
	int pozitie = calculeazaHash(credit.id, hash.dim);
	adaugaCreditInLista(&(hash.vector[pozitie]), credit);
}

HashTable citireCrediteDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");

	HashTable ht = initializareHashTable(5);

	if (file != NULL) {
		while (!feof(file)) {
			Credit c = citireCreditDinFisier(file);

			if (c.id != -1) {
				inserareCreditInTabela(ht, c);
			}
		}

		fclose(file);
	}
	else {
		printf("Fisierul nu a putut fi deschis.\n");
	}

	return ht;
}

void afisareTabelaDeCredite(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("Cluster %d:\n", i);

		if (ht.vector[i] == NULL) {
			printf("Cluster gol.\n\n");
		}
		else {
			afisareListaCredite(ht.vector[i]);
		}
	}
}

void dezalocareListaCredite(Nod** cap) {
	while (*cap != NULL) {
		Nod* temp = *cap;
		*cap = (*cap)->next;

		free(temp->info.numeClient);
		free(temp->info.banca);
		free(temp);
	}
}

void dezalocareTabelaDeCredite(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		dezalocareListaCredite(&(ht->vector[i]));
	}

	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

float* calculeazaSumeMediiPerClustere(HashTable ht, int* nrClustere) {
	*nrClustere = 0;

	for (int i = 0; i < ht.dim; i++) {
		if (ht.vector[i] != NULL) {
			(*nrClustere)++;
		}
	}

	if (*nrClustere == 0) {
		return NULL;
	}

	float* medii = (float*)malloc(sizeof(float) * (*nrClustere));
	int index = 0;

	for (int i = 0; i < ht.dim; i++) {
		if (ht.vector[i] != NULL) {
			float sumaTotala = 0;
			int nrCredite = 0;

			Nod* temp = ht.vector[i];

			while (temp != NULL) {
				sumaTotala += temp->info.suma;
				nrCredite++;
				temp = temp->next;
			}

			medii[index] = sumaTotala / nrCredite;
			index++;
		}
	}

	return medii;
}

Credit getCreditDupaId(HashTable ht, int idCautat) {
	int pozitie = calculeazaHash(idCautat, ht.dim);

	Nod* temp = ht.vector[pozitie];

	while (temp != NULL) {
		if (temp->info.id == idCautat) {
			return temp->info;
		}

		temp = temp->next;
	}

	return initializareCreditInvalid();
}

int main() {
	HashTable ht = citireCrediteDinFisier("crediteHashTable.txt");

	printf("Tabela de dispersie:\n\n");
	afisareTabelaDeCredite(ht);

	printf("Cautare credit dupa id:\n\n");
	Credit creditCautat = getCreditDupaId(ht, 104);

	if (creditCautat.id != -1) {
		printf("Credit gasit:\n");
		afisareCredit(creditCautat);
	}
	else {
		printf("Creditul nu a fost gasit.\n");
	}

	int nrClustere = 0;
	float* medii = calculeazaSumeMediiPerClustere(ht, &nrClustere);

	printf("Sume medii per clustere ocupate:\n");

	for (int i = 0; i < nrClustere; i++) {
		printf("Cluster ocupat %d - suma medie: %.2f\n", i + 1, medii[i]);
	}

	free(medii);

	dezalocareTabelaDeCredite(&ht);

	return 0;
}