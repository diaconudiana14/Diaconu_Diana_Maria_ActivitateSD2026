#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StructuraCredit {
	int idCredit;
	int durataLuni;
	float sumaImprumutata;
	char* banca;
	char* numeClient;
	unsigned char tipDobanda;
} Credit;

typedef struct NodArbore {
	Credit info;
	struct NodArbore* stanga;
	struct NodArbore* dreapta;
} NodArbore;

char* copiazaString(const char* sursa) {
	if (sursa == NULL) {
		return NULL;
	}

	char* copie = (char*)malloc(strlen(sursa) + 1);

	if (copie != NULL) {
		strcpy(copie, sursa);
	}

	return copie;
}

Credit initializareCreditGol() {
	Credit c;

	c.idCredit = -1;
	c.durataLuni = 0;
	c.sumaImprumutata = 0;
	c.banca = NULL;
	c.numeClient = NULL;
	c.tipDobanda = '-';

	return c;
}

void dezalocareCredit(Credit* c) {
	if (c != NULL) {
		free(c->banca);
		free(c->numeClient);

		c->banca = NULL;
		c->numeClient = NULL;
	}
}

int citireCreditDinFisier(FILE* file, Credit* c) {
	char buffer[256];

	if (fgets(buffer, sizeof(buffer), file) == NULL) {
		return 0;
	}

	char* token = strtok(buffer, ",\n");
	if (token == NULL) return 0;
	c->idCredit = atoi(token);

	token = strtok(NULL, ",\n");
	if (token == NULL) return 0;
	c->durataLuni = atoi(token);

	token = strtok(NULL, ",\n");
	if (token == NULL) return 0;
	c->sumaImprumutata = (float)atof(token);

	token = strtok(NULL, ",\n");
	if (token == NULL) return 0;
	c->banca = copiazaString(token);

	token = strtok(NULL, ",\n");
	if (token == NULL) {
		dezalocareCredit(c);
		return 0;
	}
	c->numeClient = copiazaString(token);

	token = strtok(NULL, ",\n");
	if (token == NULL) {
		dezalocareCredit(c);
		return 0;
	}
	c->tipDobanda = token[0];

	return 1;
}

void afisareCredit(Credit credit) {
	printf("Id credit: %d\n", credit.idCredit);
	printf("Durata luni: %d\n", credit.durataLuni);
	printf("Suma imprumutata: %.2f\n", credit.sumaImprumutata);
	printf("Banca: %s\n", credit.banca);
	printf("Nume client: %s\n", credit.numeClient);
	printf("Tip dobanda: %c\n\n", credit.tipDobanda);
}

void adaugaCreditInArbore(NodArbore** radacina, Credit creditNou) {
	if (*radacina == NULL) {
		NodArbore* nodNou = (NodArbore*)malloc(sizeof(NodArbore));

		if (nodNou != NULL) {
			nodNou->info = creditNou;
			nodNou->stanga = NULL;
			nodNou->dreapta = NULL;
			*radacina = nodNou;
		}
	}
	else {
		if (creditNou.idCredit < (*radacina)->info.idCredit) {
			adaugaCreditInArbore(&((*radacina)->stanga), creditNou);
		}
		else if (creditNou.idCredit > (*radacina)->info.idCredit) {
			adaugaCreditInArbore(&((*radacina)->dreapta), creditNou);
		}
		else {
			printf("Creditul cu ID-ul %d exista deja. Nu a fost adaugat.\n", creditNou.idCredit);
			dezalocareCredit(&creditNou);
		}
	}
}

NodArbore* citireArboreDeCrediteDinFisier(const char* numeFisier) {
	NodArbore* radacina = NULL;

	FILE* file = fopen(numeFisier, "r");

	if (file == NULL) {
		printf("Nu s-a putut deschide fisierul: %s\n", numeFisier);
		perror("Eroare fopen");
		return NULL;
	}

	while (!feof(file)) {
		Credit c = initializareCreditGol();

		if (citireCreditDinFisier(file, &c)) {
			adaugaCreditInArbore(&radacina, c);
		}
	}

	fclose(file);

	return radacina;
}

void afisareCrediteInordine(NodArbore* radacina) {
	if (radacina != NULL) {
		afisareCrediteInordine(radacina->stanga);
		afisareCredit(radacina->info);
		afisareCrediteInordine(radacina->dreapta);
	}
}

void afisareCreditePreordine(NodArbore* radacina) {
	if (radacina != NULL) {
		afisareCredit(radacina->info);
		afisareCreditePreordine(radacina->stanga);
		afisareCreditePreordine(radacina->dreapta);
	}
}

void afisareCreditePostordine(NodArbore* radacina) {
	if (radacina != NULL) {
		afisareCreditePostordine(radacina->stanga);
		afisareCreditePostordine(radacina->dreapta);
		afisareCredit(radacina->info);
	}
}

void dezalocareArboreDeCredite(NodArbore** radacina) {
	if (*radacina != NULL) {
		dezalocareArboreDeCredite(&((*radacina)->stanga));
		dezalocareArboreDeCredite(&((*radacina)->dreapta));

		dezalocareCredit(&((*radacina)->info));
		free(*radacina);

		*radacina = NULL;
	}
}

Credit getCreditByID(NodArbore* radacina, int idCredit) {
	if (radacina == NULL) {
		return initializareCreditGol();
	}

	if (idCredit == radacina->info.idCredit) {
		return radacina->info;
	}
	else if (idCredit < radacina->info.idCredit) {
		return getCreditByID(radacina->stanga, idCredit);
	}
	else {
		return getCreditByID(radacina->dreapta, idCredit);
	}
}

int determinaNumarNoduri(NodArbore* radacina) {
	if (radacina == NULL) {
		return 0;
	}

	return 1 +
		determinaNumarNoduri(radacina->stanga) +
		determinaNumarNoduri(radacina->dreapta);
}

int calculeazaInaltimeArbore(NodArbore* radacina) {
	if (radacina == NULL) {
		return 0;
	}

	int inaltimeStanga = calculeazaInaltimeArbore(radacina->stanga);
	int inaltimeDreapta = calculeazaInaltimeArbore(radacina->dreapta);

	if (inaltimeStanga > inaltimeDreapta) {
		return 1 + inaltimeStanga;
	}
	else {
		return 1 + inaltimeDreapta;
	}
}

float calculeazaSumaTotalaCredite(NodArbore* radacina) {
	if (radacina == NULL) {
		return 0;
	}

	return radacina->info.sumaImprumutata +
		calculeazaSumaTotalaCredite(radacina->stanga) +
		calculeazaSumaTotalaCredite(radacina->dreapta);
}

float calculeazaSumaCreditelorUnuiClient(NodArbore* radacina, const char* numeClient) {
	if (radacina == NULL) {
		return 0;
	}

	float suma = 0;

	if (strcmp(radacina->info.numeClient, numeClient) == 0) {
		suma = radacina->info.sumaImprumutata;
	}

	return suma +
		calculeazaSumaCreditelorUnuiClient(radacina->stanga, numeClient) +
		calculeazaSumaCreditelorUnuiClient(radacina->dreapta, numeClient);
}

int determinaNumarCrediteCuDobandaFixa(NodArbore* radacina) {
	if (radacina == NULL) {
		return 0;
	}

	int contor = 0;

	if (radacina->info.tipDobanda == 'F') {
		contor = 1;
	}

	return contor +
		determinaNumarCrediteCuDobandaFixa(radacina->stanga) +
		determinaNumarCrediteCuDobandaFixa(radacina->dreapta);
}

Credit getCreditCuSumaMaxima(NodArbore* radacina) {
	if (radacina == NULL) {
		return initializareCreditGol();
	}

	Credit maxim = radacina->info;

	Credit maximStanga = getCreditCuSumaMaxima(radacina->stanga);
	Credit maximDreapta = getCreditCuSumaMaxima(radacina->dreapta);

	if (maximStanga.idCredit != -1 &&
		maximStanga.sumaImprumutata > maxim.sumaImprumutata) {
		maxim = maximStanga;
	}

	if (maximDreapta.idCredit != -1 &&
		maximDreapta.sumaImprumutata > maxim.sumaImprumutata) {
		maxim = maximDreapta;
	}

	return maxim;
}

int main() {
	NodArbore* arbore = citireArboreDeCrediteDinFisier("credite.txt");

	if (arbore == NULL) {
		printf("Arborele nu contine niciun credit.\n");
		return 0;
	}

	printf("Afisare inordine:\n");
	afisareCrediteInordine(arbore);

	printf("Afisare preordine:\n");
	afisareCreditePreordine(arbore);

	printf("Afisare postordine:\n");
	afisareCreditePostordine(arbore);

	printf("Numar noduri: %d\n", determinaNumarNoduri(arbore));

	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(arbore));

	printf("Suma totala credite: %.2f\n", calculeazaSumaTotalaCredite(arbore));

	printf("Suma creditelor clientului Popescu Ana: %.2f\n",
		calculeazaSumaCreditelorUnuiClient(arbore, "Popescu Ana"));

	printf("Numar credite cu dobanda fixa: %d\n",
		determinaNumarCrediteCuDobandaFixa(arbore));

	printf("\nCredit cautat dupa ID:\n");
	Credit c = getCreditByID(arbore, 101);

	if (c.idCredit != -1) {
		afisareCredit(c);
	}
	else {
		printf("Creditul nu a fost gasit.\n");
	}

	printf("\nCredit cu suma maxima:\n");
	Credit creditMaxim = getCreditCuSumaMaxima(arbore);

	if (creditMaxim.idCredit != -1) {
		afisareCredit(creditMaxim);
	}
	else {
		printf("Nu exista credit cu suma maxima.\n");
	}

	dezalocareArboreDeCredite(&arbore);

	return 0;
}