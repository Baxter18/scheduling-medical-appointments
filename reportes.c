#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "reportes.h"

void menuReportes(void) {
	int op;
	do{
	system("cls");
	printf("\n--- REPORTES ---\n");
	printf("1. Citas por medico\n");
	printf("2. Lista general de citas\n");
	printf("3. Medicos con mas demanda\n");
	printf("0. Volver\n");
	printf("Opcion: ");
	if(scanf("%d", &op) !=1){
		while(getchar() !='\n');
		op = -1;
	}
	getchar();
	
	switch(op){
	case 1: reporteCitasPorMedico(); break;
	case 2: reporteGeneralCitas(); break;
	case 3: reporteMedicosMasDemanda(); break;
	case 0: break;
	default: printf("Opcion Invalida.\n");
	}
	} while (op !=0);
}

void reporteCitasPorMedico(void) {
	char codBusqueda[15], ced[20], cod[20], fch[20], hr[20];
	int hay = 0;
	
	printf("Ingrese el codigo del medico: ");
	fgets(codBusqueda, 15, stdin);
	codBusqueda[strcspn(codBusqueda, "\n")] = 0;
	
	FILE *f = fopen("data/citas.txt", "r");
	if (!f) {
		printf("No hay citas registradas.\n");
		return;
	}
	
	printf("\n--- CITAS DEL MEDICO: %s ---\n", codBusqueda);
	while (fscanf(f, " %19[^|]|%19[^|]|%19[^|]|%19[^\n]\n", ced, cod, fch, hr) == 4) {
		if (strcmp(cod, codBusqueda) == 0) {
			printf("Fecha: %s | Hora: %s | Paciente: %s\n", fch, hr, ced);
			hay = 1;
		}
	}
	if (!hay) printf("No se encontraron citas.\n");
	fclose(f);
}

void reporteGeneralCitas(void) {
	char ced[20], cod[20], fch[20], hr[20];
	FILE *f = fopen("data/citas.txt", "r");
	if (!f) return;
	
	printf("\n--- REPORTE GENERAL DE CITAS ---\n");
	while (fscanf(f, " %19[^|]|%19[^|]|%19[^|]|%19[^\n]\n", ced, cod, fch, hr) == 4) {
		printf("Med: %-10s | Paciente: %-15s | Fecha: %s | Hora: %s\n", cod, ced, fch, hr);
	}
	fclose(f);
}
	
void reporteMedicosMasDemanda(void) {
	char ced[20], cod[20], fch[20], hr[20];
	char medicosProcesados[100][20]; 
	int conteo[100];
	int totalMedicos = 0;
	
	FILE *f = fopen("data/citas.txt", "r");
	if (!f) {
		printf("No hay datos para procesar.\n");
		return;
	}
	
	while (fscanf(f, " %19[^|]|%19[^|]|%19[^|]|%19[^\n]\n", ced, cod, fch, hr) == 4) {
		int encontrado = -1;
		for (int i = 0; i < totalMedicos; i++) {
			if (strcmp(medicosProcesados[i], cod) == 0) {
				encontrado = i;
				break;
			}
		}
		
		if (encontrado != -1) {
			conteo[encontrado]++;
		} else {
			strcpy(medicosProcesados[totalMedicos], cod);
			conteo[totalMedicos] = 1;
			totalMedicos++;
		}
	}
	fclose(f);
		
	printf("\n--- RANKING DE DEMANDA (CITAS POR MEDICO) ---\n");
	for (int i = 0; i < totalMedicos; i++) {
		printf("Medico: %-10s | Total Citas: %d\n", medicosProcesados[i], conteo[i]);
	}
}
