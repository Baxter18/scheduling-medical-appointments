#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "login.h"
#include "pacientes1.h"
#include "validaciones.h"

//Guardar usuario medico con contraseña oculta
void guardarUsuarioMedico(char codigo[]) {
	char clave[20], confirmacion[20];
	
	printf("\n--- SEGURIDAD MEDICO ---\n");
	do {
		printf("Cree una clave para el medico %s: ", codigo);
		leerPassword(clave, 20);
		
		printf("Confirme la clave: ");
		leerPassword(confirmacion, 20);
		
		if (strcmp(clave, confirmacion) != 0) {
			printf("ERROR: Las claves no coinciden. Intente de nuevo.\n");
		}
	} while (strcmp(clave, confirmacion) != 0);
	
	FILE *f = fopen("data/usuarios_medicos.txt", "a");
	if (f != NULL) {
		fprintf(f, "%s|%s\n", codigo, clave);
		fclose(f);
		printf("Credenciales del medico guardadas.\n");
	}
}

//Registrar paciente nuevo si falla el login
void registrarPacienteLogin(char usuario[], char clave[]) {
	char confirmacion[20];
	
	// Validacion estricta de cedula
	if (!validarCedula(usuario)) {
		printf("\n[ACCESO DENEGADO]\n");
		printf(" - El usuario no existe como medico o administrador.\n");
		printf(" - La cedula ingresada NO es valida para un nuevo paciente.\n");
		return; 
	}
	
	printf("\n--- REGISTRO DE NUEVO PACIENTE ---\n");
	printf("Cedula valida detectada: %s\n", usuario);
	
	do {
		printf("Defina su clave: ");
		leerPassword(clave, 20);
		
		printf("Confirme su clave: ");
		leerPassword(confirmacion, 20);
		
		if (strcmp(clave, confirmacion) != 0) {
			printf("ERROR: Las claves no coinciden.\n");
		}
	} while (strcmp(clave, confirmacion) != 0);
	
	guardarUsuarioPaciente(usuario, clave);
	registrarPacienteDatos();
	
	printf("Registro exitoso. Inicie sesion nuevamente.\n");
}

//Autenticacion principal
int autenticar(char usuario[], char clave[]) {
	FILE *f;
	char u[20], c[20];
	
	// 1. ADMIN
	f = fopen("data/admin.txt", "r");
	if (f) {
		fscanf(f, "%19[^|]|%19[^\n]\n", u, c);
		fclose(f);
		if (strcmp(u, usuario) == 0 && strcmp(c, clave) == 0) return ADMIN;
	}
	
	// 2. PACIENTE
	f = fopen("data/usuarios_pacientes.txt", "r");
	if (f) {
		while (fscanf(f, "%19[^|]|%19[^\n]\n", u, c) != EOF) {
			if (strcmp(u, usuario) == 0 && strcmp(c, clave) == 0) {
				fclose(f);
				return PACIENTE;
			}
		}
		fclose(f);
	}
	
	// 3. MEDICO
	f = fopen("data/usuarios_medicos.txt", "r");
	if (f) {
		while (fscanf(f, "%19[^|]|%19[^\n]\n", u, c) != EOF) {
			if (strcmp(u, usuario) == 0 && strcmp(c, clave) == 0) {
				fclose(f);
				return MEDICO;
			}
		}
		fclose(f);
	}
	
	return 0; // No encontrado
}
