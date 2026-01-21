#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "medicos.h"
#include "login.h"
#include "validaciones.h"
#include "citas.h"

//Registra el Diagnostico de cada cita
void registrarDiagnosticoDirecto(char codigoMedico[], char cedulaPaciente[]) {
	char fecha[15], diagnostico[200];
	
	printf("\n>> Ingrese Fecha de atencion (YYYY-MM-DD): ");
	fflush(stdin); 
	fgets(fecha, 15, stdin);
	fecha[strcspn(fecha, "\n")] = 0;
	
	printf(">> Escriba Diagnostico y Prescripcion:\n> ");
	fgets(diagnostico, 200, stdin);
	diagnostico[strcspn(diagnostico, "\n")] = 0;
	
	FILE *f = fopen("data/diagnosticos.txt", "a");
	if (f == NULL) {
		printf("Error al guardar diagnostico.\n");
		return;
	}
	
	fprintf(f, "%s|%s|%s|%s\n", cedulaPaciente, codigoMedico, fecha, diagnostico);
	fclose(f);
	printf(">> [EXITO] Diagnostico registrado correctamente.\n");
}

//Gestion de Cita Especifica
void gestionarCitaEspecifica(char codigoMedico[]) {
	char cedula[15];
	int op;
	
	printf("\n--- GESTIONAR CITA DE PACIENTE ---\n");
	printf("Ingrese el numero de Cedula del Paciente: ");
	if(scanf("%s", cedula) != 1) return;
	getchar(); 
	
	if (!verificarCitaMedicoPaciente(codigoMedico, cedula)) {
		printf("\n[ALERTA] No tiene citas programadas con el paciente %s.\n", cedula);
		return;
	}
	
	do {
		system("cls"); // Limpieza visual
		printf("\n=== GESTIONANDO PACIENTE: %s ===\n", cedula);
		printf("1. Registrar Diagnostico\n");
		printf("2. Cancelar esta Cita\n");
		printf("0. Volver al menu anterior\n");
		printf("Opcion: ");
		scanf("%d", &op);
		getchar();
		
		switch(op) {
		case 1:
			registrarDiagnosticoDirecto(codigoMedico, cedula);
			system("pause");
			break;
		case 2: {
			char confirmacion;
			printf("Seguro que desea cancelar la cita? (s/n): ");
			scanf("%c", &confirmacion);
			getchar();
			if(confirmacion == 's' || confirmacion == 'S') {
				cancelarCitaPorMedico(codigoMedico, cedula);
				op = 0; 
				system("pause");
			}
			break;
		} 
		case 0: break;
		default: printf("Opcion invalida.\n");
		}
	} while(op != 0);
}

//Elimar medico por Admin
void eliminarMedicoAdmin(void) {
	FILE *f, *temp;
	char codigo[10], codLeido[10], restoLinea[200];
	int encontrado = 0;
	
	printf("\n--- ELIMINAR MEDICO ---\n");
	printf("Ingrese el CODIGO del medico a eliminar: ");
	scanf("%s", codigo);
	getchar();
	
	// 1. Borrar de la lista de datos
	f = fopen("data/medicos.txt", "r");
	temp = fopen("data/temp_medicos.txt", "w");
	
	if (!f || !temp) {
		printf("Error al abrir archivos.\n");
		if(f) fclose(f); if(temp) fclose(temp);
		return;
	}
	
	while (fscanf(f, "%9[^|]|%[^\n]\n", codLeido, restoLinea) != EOF) {
		if (strcmp(codLeido, codigo) == 0) {
			encontrado = 1; 
		} else {
			fprintf(temp, "%s|%s\n", codLeido, restoLinea);
		}
	}
	fclose(f);
	fclose(temp);
	remove("data/medicos.txt");
	rename("data/temp_medicos.txt", "data/medicos.txt");
	
	// 2. Borrar de la lista de usuarios
	if (encontrado) {
		f = fopen("data/usuarios_medicos.txt", "r");
		temp = fopen("data/temp_us_med.txt", "w");
		
		if (f && temp) {
			while (fscanf(f, "%9[^|]|%[^\n]\n", codLeido, restoLinea) != EOF) {
				if (strcmp(codLeido, codigo) == 0) {
					// saltar
				} else {
					fprintf(temp, "%s|%s\n", codLeido, restoLinea);
				}
			}
			fclose(f); fclose(temp);
			remove("data/usuarios_medicos.txt");
			rename("data/temp_us_med.txt", "data/usuarios_medicos.txt");
		}
		printf(">> Medico eliminado correctamente.\n");
	} else {
		printf(">> No se encontro ningun medico con ese codigo.\n");
		remove("data/temp_medicos.txt");
	}
}

//Menu de Modulo Medico de Admin
void menuMedicosAdmin(void) {
	int op;
	do {
		system("cls"); 
		printf("\n--- MODULO MEDICOS (ADMIN) ---\n");
		printf("1. Registrar medico\n");
		printf("2. Listar medicos\n");
		printf("3. Eliminar medico\n");
		printf("0. Volver\n");
		printf("Opcion: ");
		if (scanf("%d", &op) != 1) { while (getchar() != '\n'); op = -1; }
		getchar();
		switch (op) {
		case 1: registrarMedicoAdmin(); break;
		case 2: listarMedicos(); break;
		case 3: eliminarMedicoAdmin(); break;
		case 0: break;
		default: printf("Opcion invalida\n");
		}
		if (op != 0) system("pause");
	} while (op != 0);
}

//Modulo Medico
void menuMedico(char codigo[]) {
	int op;
	do {
		system("cls");
		printf("\n--- MENU MEDICO (%s) ---\n", codigo);
		printf("1. Ver mis citas\n");
		printf("2. Gestionar Cita (Diagnostico/Cancelar)\n");
		printf("0. Salir\n");
		printf("Opcion: ");
		if (scanf("%d", &op) != 1) { while (getchar() != '\n'); op = -1; }
		getchar();
		switch (op) {
		case 1: verCitasMedico(codigo); break;
		case 2: gestionarCitaEspecifica(codigo); break;
		case 0: break;
		default: printf("Opcion invalida\n");
		}
		if (op != 0) system("pause");
	} while (op != 0);
}

void registrarMedicoAdmin(void) {
	Medico m;
	FILE *f = fopen("data/medicos.txt", "a+");
	if (!f) return;
	printf("Codigo medico: "); fgets(m.codigo, 10, stdin); m.codigo[strcspn(m.codigo, "\n")] = 0;
	printf("Nombre: "); fgets(m.nombre, 50, stdin); m.nombre[strcspn(m.nombre, "\n")] = 0;
	printf("Especialidad: "); fgets(m.especialidad, 30, stdin); m.especialidad[strcspn(m.especialidad, "\n")] = 0;
	if (cadenaVacia(m.codigo) || cadenaVacia(m.nombre)) { printf("Datos invalidos.\n"); fclose(f); return; }
	fprintf(f, "%s|%s|%s\n", m.codigo, m.nombre, m.especialidad);
	fclose(f);
	guardarUsuarioMedico(m.codigo);
	printf("Medico registrado.\n");
}

void listarMedicos(void) {
	FILE *f = fopen("data/medicos.txt", "r");
	Medico m;
	if (!f) { printf("No hay medicos.\n"); return; }
	printf("\n--- LISTA DE MEDICOS ---\n");
	while (fscanf(f, "%9[^|]|%49[^|]|%29[^\n]\n", m.codigo, m.nombre, m.especialidad) != EOF) {
		printf("Codigo: %s | Nombre: %s | Especialidad: %s\n", m.codigo, m.nombre, m.especialidad);
	}
	fclose(f);
}

void registrarDiagnostico(char codigoMedico[]) { gestionarCitaEspecifica(codigoMedico); }
