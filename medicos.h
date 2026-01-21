#ifndef MEDICOS_H
#define MEDICOS_H

typedef struct {
	char codigo[10];
	char nombre[50];
	char especialidad[30];
} Medico;

void menuMedicosAdmin(void);
void menuMedico(char codigo[]);

void registrarMedicoAdmin(void);
void listarMedicos(void);
void registrarDiagnostico(char codigoMedico[]); // <--- NUEVA FUNCION
void gestionarCitaEspecifica(char codigoMedico[]);
void eliminarMedicoAdmin(void);
#endif


