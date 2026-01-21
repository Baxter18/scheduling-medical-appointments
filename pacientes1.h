#ifndef PACIENTES_H
#define PACIENTES_H

typedef struct {
	char nombre[50];
	char cedula[15];
	int edad;
	char telefono[15];
	char correo[50];
} Paciente;

void registrarPacienteDatos(void);
void guardarUsuarioPaciente(char usuario[], char clave[]);
void menuPaciente(char cedula[]); 
void verMisDiagnosticos(char cedula[]);

void menuPacientesAdmin(void); 
void listarPacientes(void);    
void eliminarPacienteAdmin(void);
#endif

