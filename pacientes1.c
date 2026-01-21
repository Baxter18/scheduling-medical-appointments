#include <stdio.h>
#include <string.h>
#include "pacientes1.h"
#include "citas.h"
#include "validaciones.h"
#include <stdlib.h>

//Guardar en Archivo
void guardarUsuarioPaciente(char usuario[], char clave[]) {
	FILE *f = fopen("data/usuarios_pacientes.txt", "a");
	if (f == NULL) return;
	fprintf(f, "%s|%s\n", usuario, clave);
	fclose(f);
}

//Datos Personales del Paciente
void registrarPacienteDatos(void) {
	char nombre[50], apellido[50], direccion[100], telefono[15];
	int edad;
	printf("\n--- DATOS PERSONALES ---\n");
	fflush(stdin); 
	printf("Nombre: "); fgets(nombre, 50, stdin); nombre[strcspn(nombre, "\n")] = 0;
	printf("Apellido: "); fgets(apellido, 50, stdin); apellido[strcspn(apellido, "\n")] = 0;
	printf("Edad: "); scanf("%d", &edad); getchar(); 
	printf("Direccion: "); fgets(direccion, 100, stdin); direccion[strcspn(direccion, "\n")] = 0;
	printf("Telefono: "); fgets(telefono, 15, stdin); telefono[strcspn(telefono, "\n")] = 0;
	FILE *f = fopen("data/datos_pacientes.txt", "a");
	if (f) {
		fprintf(f, "%s|%s|%d|%s|%s\n", nombre, apellido, edad, direccion, telefono);
		fclose(f);
	}
}

//Diagnostico del Modulo Paciente
void verMisDiagnosticos(char cedula[]) {
	FILE *f = fopen("data/diagnosticos.txt", "r");
	char p_ced[15], m_cod[10], fecha[15], diag[200];
	int hay = 0;
	if (!f) { printf("\nAun no tiene historial medico.\n"); return; }
	printf("\n=== MIS DIAGNOSTICOS ===\n");
	while (fscanf(f, "%14[^|]|%9[^|]|%14[^|]|%199[^\n]\n", p_ced, m_cod, fecha, diag) != EOF) {
		if (strcmp(p_ced, cedula) == 0) {
			printf("> Fecha: %s | ID Dr: %s | Diagnostico: %s\n", fecha, m_cod, diag);
			hay = 1;
		}
	}
	if (!hay) printf("Sin registros.\n");
	fclose(f);
}

//Menu del Modulo Paciente
void menuPaciente(char cedula[]) {
	int op;
	do {
		system("cls"); // <--- BORRAR PANTALLA
		printf("\n===== MENU PACIENTE =====\nUsuario: %s\n", cedula);
		printf("1. Agendar Cita\n");
		printf("2. Ver mis Citas\n");
		printf("3. Cancelar Cita\n");
		printf("4. Ver Diagnosticos\n");
		printf("0. Salir\n");
		printf("Opcion: ");
		
		if (scanf("%d", &op) != 1) { while(getchar() != '\n'); op = -1; }
		getchar();
		switch (op) {
		case 1: agendarCita(cedula); break;
		case 2: verCitaPaciente(cedula); break;
		case 3: cancelarCitaPaciente(cedula); break;
		case 4: verMisDiagnosticos(cedula); break;
		case 0: printf("Cerrando sesion...\n"); break;
		default: printf("Opcion invalida.\n");
		}
		
		if (op != 0) system("pause"); // <--- PAUSA PARA LEER
		
	} while (op != 0);
}

//Eliminar Paciente para Modulo Admin
void eliminarPacienteAdmin(void) {
	FILE *f, *temp;
	char cedula[20], cedulaLeida[20], pass[20];
	int encontrado = 0;
	
	printf("\n--- ELIMINAR PACIENTE ---\n");
	printf("Ingrese la CEDULA del paciente a eliminar: ");
	scanf("%s", cedula);
	getchar();
	
	// Borramos el acceso (Login)
	f = fopen("data/usuarios_pacientes.txt", "r");
	temp = fopen("data/temp_usuarios_pacientes.txt", "w");
	
	if (!f || !temp) {
		printf("Error al abrir base de datos de usuarios.\n");
		return;
	}
	
	while (fscanf(f, "%19[^|]|%19[^\n]\n", cedulaLeida, pass) != EOF) {
		if (strcmp(cedulaLeida, cedula) == 0) {
			encontrado = 1;
			printf(">> Usuario con cedula %s eliminado.\n", cedula);
		} else {
			fprintf(temp, "%s|%s\n", cedulaLeida, pass);
		}
	}
	
	fclose(f);
	fclose(temp);
	
	remove("data/usuarios_pacientes.txt");
	rename("data/temp_usuarios_pacientes.txt", "data/usuarios_pacientes.txt");
	
	if (!encontrado) {
		printf(">> No se encontro ese usuario.\n");
		remove("data/temp_usuarios_pacientes.txt");
	}
}

//Listar Pacientes para Modulo Admin
void listarPacientes(void) {
	FILE *f = fopen("data/datos_pacientes.txt", "r");
	char nom[50], ape[50], dir[100], tel[15];
	int edad;
	if (!f) { printf("No hay datos de pacientes.\n"); return; }
	printf("\n%-15s %-15s %-5s %-15s %-20s\n", "NOMBRE", "APELLIDO", "EDAD", "TELEFONO", "DIRECCION");
	printf("--------------------------------------------------------------------------\n");
	while (fscanf(f, "%49[^|]|%49[^|]|%d|%99[^|]|%14[^\n]\n", nom, ape, &edad, dir, tel) != EOF) {
		printf("%-15s %-15s %-5d %-15s %-20s\n", nom, ape, edad, tel, dir);
	}
	fclose(f);
}

//Modulo de Pacientes para Admin
void menuPacientesAdmin(void) {
	int op;
	do {
		system("cls"); // <--- BORRAR
		printf("\n--- GESTION DE PACIENTES (ADMIN) ---\n");
		printf("1. Listar todos los pacientes\n");
		printf("2. Eliminar paciente\n");
		printf("0. Volver al menu principal\n");
		printf("Opcion: ");
		scanf("%d", &op);
		getchar();
		switch(op) {
		case 1: listarPacientes(); break;
		case 2: eliminarPacienteAdmin(); break;
		case 0: break;
		default: printf("Opcion no valida.\n");
		}
		if (op != 0) system("pause"); // <--- PAUSAR
	} while(op != 0);
}
