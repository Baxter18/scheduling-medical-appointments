#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "citas.h"

int citaDisponible(char codigoMedico[], char fecha[], char hora[]) {
	FILE *f = fopen("data/citas.txt", "r");
	char ced[15], cod[10], fch[15], hr[10];
	
	if (!f) return 1; 
	
	while (fscanf(f, "%14[^|]|%9[^|]|%14[^|]|%9[^\n]\n", ced,cod,fch,hr) != EOF) {
		if (strcmp(cod, codigoMedico) == 0 &&
			strcmp(fch, fecha) == 0 &&
			strcmp(hr, hora) == 0) {
			fclose(f);
			return 0; // NO disponible
		}
	}
	
	fclose(f);
	return 1; 
}
int verificarCitaMedicoPaciente(char codigoMedico[], char cedulaPaciente[]) {
	FILE *f = fopen("data/citas.txt", "r");
	char ced[15], cod[10], fch[15], hr[10];
	
	if (!f) return 0;
	
	while (fscanf(f, "%14[^|]|%9[^|]|%14[^|]|%9[^\n]\n", ced, cod, fch, hr) != EOF) {
		if (strcmp(cod, codigoMedico) == 0 && strcmp(ced, cedulaPaciente) == 0) {
			fclose(f);
			return 1; // SI existe cita
		}
	}
	
	fclose(f);
	return 0; // NO existe
}

//Cancelar Cita por Paciente Especifico
void cancelarCitaPorMedico(char codigoMedico[], char cedulaPaciente[]) {
	FILE *f = fopen("data/citas.txt", "r");
	FILE *temp = fopen("data/temp_citas.txt", "w");
	char ced[15], cod[10], fch[15], hr[10];
	int encontrado = 0;
	
	if (!f || !temp) return;
	
	while (fscanf(f, "%14[^|]|%9[^|]|%14[^|]|%9[^\n]\n", ced, cod, fch, hr) != EOF) {
		// Si coincide medico y paciente, NO lo copiamos (se borra)
		if (strcmp(cod, codigoMedico) == 0 && strcmp(ced, cedulaPaciente) == 0) {
			encontrado = 1;
			printf(">> Cita del %s a las %s ELIMINADA.\n", fch, hr);
		} else {
			fprintf(temp, "%s|%s|%s|%s\n", ced, cod, fch, hr);
		}
	}
	
	fclose(f);
	fclose(temp);
	
	remove("data/citas.txt");
	rename("data/temp_citas.txt", "data/citas.txt");
	
	if (!encontrado) printf("No se pudo cancelar la cita (o no existia).\n");
}

void agendarCita(char cedulaPaciente[]) {
	char codigoMedico[10], fecha[15], hora[10];
	
	printf("\n--- AGENDAR CITA ---\n");
	printf("Codigo medico: ");
	fgets(codigoMedico, 10, stdin);
	codigoMedico[strcspn(codigoMedico, "\n")] = 0;
	
	printf("Fecha (YYYY-MM-DD): ");
	fgets(fecha, 15, stdin);
	fecha[strcspn(fecha, "\n")] = 0;
	
	printf("Hora (HH:MM): ");
	fgets(hora, 10, stdin);
	hora[strcspn(hora, "\n")] = 0;
	
	if (!citaDisponible(codigoMedico, fecha, hora)) {
		printf("ERROR: El medico ya tiene una cita en ese horario.\n");
		return;
	}
	
	FILE *f = fopen("data/citas.txt", "a");
	if (f == NULL) {
		printf("ERROR: No se pudo acceder a la base de datos.\n");
		return;
	}
	
	fprintf(f, "%s|%s|%s|%s\n", cedulaPaciente, codigoMedico, fecha, hora);
	fclose(f);
	
	printf("Cita agendada correctamente.\n");
	system("pause");
}

void verCitasMedico(char codigoMedico[]) {
	FILE *f = fopen("data/citas.txt", "r");
	char ced[15], cod[10], fecha[15], hora[10];
	int hay = 0;
	
	if (!f) {
		printf("No hay citas registradas.\n");
		return;
	}
	
	printf("\n--- MIS CITAS PROGRAMADAS ---\n");
	while (fscanf(f, "%14[^|]|%9[^|]|%14[^|]|%9[^\n]\n", ced, cod, fecha, hora) != EOF) {
		if (strcmp(cod, codigoMedico) == 0) {
			printf("Paciente: %s | Fecha: %s | Hora: %s\n", ced, fecha, hora);
			hay = 1;
		}
	}
	if (!hay) printf("No tiene citas asignadas.\n");
	fclose(f);
}

void verCitaPaciente(char cedulaPaciente[]) {
	FILE *f = fopen("data/citas.txt", "r");
	char ced[15], cod[10], fecha[15], hora[10];
	int hay = 0;
	
	if (!f) {
		printf("No tiene citas.\n");
		return;
	}
	
	printf("\n--- MIS CITAS AGENDADAS ---\n");
	while (fscanf(f, "%14[^|]|%9[^|]|%14[^|]|%9[^\n]\n", ced, cod, fecha, hora) != EOF) {
		if (strcmp(ced, cedulaPaciente) == 0) {
			printf("Medico: %s | Fecha: %s | Hora: %s\n", cod, fecha, hora);
			hay = 1;
		}
	}
	if (!hay) printf("No tiene citas registradas.\n");
	fclose(f);
}

void cancelarCita(void) {
	FILE *f = fopen("data/citas.txt", "r");
	FILE *temp = fopen("data/temp_citas.txt", "w");
	char ced[15], cod[10], fch[15], hr[10];
	char cedBuscar[15];
	int encontrado = 0;
	
	if (!f || !temp) return;
	
	printf("Ingrese la Cedula del Paciente a cancelar: ");
	scanf("%s", cedBuscar);
	
	while (fscanf(f, "%14[^|]|%9[^|]|%14[^|]|%9[^\n]\n", ced, cod, fch, hr) != EOF) {
		if (strcmp(ced, cedBuscar) == 0) {
			printf("Eliminando cita: Medico %s | Fecha %s\n", cod, fch);
			encontrado = 1;
		} else {
			fprintf(temp, "%s|%s|%s|%s\n", ced, cod, fch, hr);
		}
	}
	
	fclose(f);
	fclose(temp);
	
	remove("data/citas.txt");
	rename("data/temp_citas.txt", "data/citas.txt");
	
	if (encontrado) printf("Cita(s) cancelada(s).\n");
	else printf("No se encontro cita.\n");
}

//Paciente cancela su cita
void cancelarCitaPaciente(char cedulaPaciente[]) {
	FILE *f = fopen("data/citas.txt", "r");
	FILE *temp = fopen("data/temp_citas.txt", "w");
	char ced[15], cod[10], fch[15], hr[10];
	char fechaB[15], horaB[10];
	int encontrado = 0;
	
	verCitaPaciente(cedulaPaciente); 
	
	printf("\n--- CANCELAR MI CITA ---\n");
	printf("Ingrese la Fecha exacta (YYYY-MM-DD): ");
	scanf("%s", fechaB);
	printf("Ingrese la Hora exacta (HH:MM): ");
	scanf("%s", horaB);
	
	if (!f || !temp) return;
	
	while (fscanf(f, "%14[^|]|%9[^|]|%14[^|]|%9[^\n]\n", ced, cod, fch, hr) != EOF) {
		if (strcmp(ced, cedulaPaciente) == 0 && 
			strcmp(fch, fechaB) == 0 && 
			strcmp(hr, horaB) == 0) {
			
			printf("Cita con medico %s eliminada exitosamente.\n", cod);
			encontrado = 1;
		} else {
			fprintf(temp, "%s|%s|%s|%s\n", ced, cod, fch, hr);
		}
	}
	
	fclose(f);
	fclose(temp);
	
	remove("data/citas.txt");
	rename("data/temp_citas.txt", "data/citas.txt");
	
	if (!encontrado) printf("No se encontro una cita suya en esa fecha y hora.\n");
}

void menuCitasAdmin(void) {
	int op;
	do {
		system("cls");
		printf("\n--- MODULO CITAS (ADMIN) ---\n");
		printf("1. Cancelar Cita por Paciente\n");
		printf("0. Volver\n");
		printf("Opcion: ");
		if(scanf("%d", &op)!=1) op=-1;
		getchar();
		
		switch(op) {
		case 1: cancelarCita(); break;
		case 0: break;
		default: printf("Opcion invalida.\n");
		}
	} while(op != 0);
}
