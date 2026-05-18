#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraCredit {
	int idCredit;
	int durataLuni;
	float suma;
	char* banca;
	char* numeClient;
	unsigned char rating;
};
typedef struct StructuraCredit Credit;

struct Heap {
	int lungime;
	Credit* vector;
	int nrCredite;
};
typedef struct Heap Heap;

Credit citireCreditDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);

	char* aux;
	Credit c;

	aux = strtok(buffer, sep);
	c.idCredit = atoi(aux);

	c.durataLuni = atoi(strtok(NULL, sep));
	c.suma = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	c.banca = malloc(strlen(aux) + 1);
	strcpy_s(c.banca, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	c.numeClient = malloc(strlen(aux) + 1);
	strcpy_s(c.numeClient, strlen(aux) + 1, aux);

	c.rating = *strtok(NULL, sep);

	return c;
}

void afisareCredit(Credit credit) {
	printf("Id credit: %d\n", credit.idCredit);
	printf("Durata luni: %d\n", credit.durataLuni);
	printf("Suma credit: %.2f\n", credit.suma);
	printf("Banca: %s\n", credit.banca);
	printf("Nume client: %s\n", credit.numeClient);
	printf("Rating: %c\n\n", credit.rating);
}

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrCredite = 0;
	heap.vector = (Credit*)malloc(sizeof(Credit) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozFiuSt = 2 * pozitieNod + 1;
	int pozFiuDr = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;

	if (pozFiuSt < heap.nrCredite &&
		heap.vector[pozMax].suma < heap.vector[pozFiuSt].suma) {
		pozMax = pozFiuSt;
	}

	if (pozFiuDr < heap.nrCredite &&
		heap.vector[pozMax].suma < heap.vector[pozFiuDr].suma) {
		pozMax = pozFiuDr;
	}

	if (pozMax != pozitieNod) {
		Credit aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;

		if (pozMax <= (heap.nrCredite - 2) / 2) {
			filtreazaHeap(heap, pozMax);
		}
	}
}

Heap citireHeapDeCrediteDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");

	Heap heap = initializareHeap(10);

	while (!feof(file)) {
		heap.vector[heap.nrCredite++] = citireCreditDinFisier(file);
	}

	fclose(file);

	for (int i = (heap.nrCredite - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}

	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrCredite; i++) {
		afisareCredit(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrCredite; i < heap.lungime; i++) {
		afisareCredit(heap.vector[i]);
	}
}

Credit extrageCredit(Heap* heap) {
	if (heap->nrCredite > 0) {
		Credit aux = heap->vector[0];

		heap->vector[0] = heap->vector[heap->nrCredite - 1];
		heap->vector[heap->nrCredite - 1] = aux;

		heap->nrCredite--;

		for (int i = (heap->nrCredite - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*heap, i);
		}

		return aux;
	}
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].banca);
		free(heap->vector[i].numeClient);
	}

	free(heap->vector);

	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrCredite = 0;
}

int main() {
	Heap heap = citireHeapDeCrediteDinFisier("crediteHeap.txt");

	printf("Heap credite:\n");
	afisareHeap(heap);

	printf("Credite extrase:\n");
	afisareCredit(extrageCredit(&heap));
	afisareCredit(extrageCredit(&heap));
	afisareCredit(extrageCredit(&heap));

	printf("Heap-ul ascuns:\n");
	afiseazaHeapAscuns(heap);

	dezalocareHeap(&heap);

	return 0;
}