#include <stdio.h>
#include <stdlib.h> 
#include "admin.h"
#include "pacientes1.h"
#include "medicos.h"
#include "citas.h"
#include "reportes.h"

void menuAdministrador(void) {
	int op;
	
	do {
		system("cls");
		printf("\n===== MENU ADMINISTRADOR =====\n");
		printf("1. Modulo Pacientes\n");
		printf("2. Modulo Medicos\n");
		printf("3. Modulo Citas\n");
		printf("4. Reportes\n");
		printf("0. Salir\n");
		printf("Opcion: ");
		
		if (scanf("%d", &op) != 1) {
			while (getchar() != '\n');
			op = -1;
		}
		getchar(); 
		
		switch (op) {
		case 1: menuPacientesAdmin(); break;
		case 2: menuMedicosAdmin(); break;
		case 3: menuCitasAdmin(); break;
		case 4: menuReportes(); break;
		case 0: printf("Saliendo...\n"); break;
		default: printf("Opcion invalida\n");
		}
		
		
		if (op != 0) {
			printf("\n");
			system("pause");
		}
		
	} while (op != 0);
}
