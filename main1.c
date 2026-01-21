#include <stdio.h>
#include <string.h>
#include "login.h"
#include "validaciones.h"
#include "pacientes1.h"
#include "medicos.h"
#include "admin.h"


int main() {
	char usuario[20], clave[20];
	int rol;
	
	//Inicio de Sesion para los Usuarios
	do {
		printf("\n---Ingreso Clinica Serkeo---\n");
		printf("Usuario: ");
		fgets(usuario, sizeof(usuario), stdin);
		usuario[strcspn(usuario, "\n")] = 0;
	} while (cadenaVacia(usuario));
	
	do {
		printf("Clave: ");
		leerPassword(clave, sizeof(clave)); // <--- CAMBIA ESTO
	} while (cadenaVacia(clave));
	
	//Roles y Modulos
	rol = autenticar(usuario, clave);
	if (rol == ADMIN)
		menuAdministrador();
	else if (rol == PACIENTE)
		menuPaciente(usuario);
	else if (rol == MEDICO)
		menuMedico(usuario);
	else {
		printf("Usuario no registrado.\n");
		registrarPacienteLogin(usuario, clave);
	}
	
	return 0;
}
