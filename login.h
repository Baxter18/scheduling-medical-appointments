#ifndef LOGIN_H
#define LOGIN_H

#define PACIENTE 1
#define MEDICO   2
#define ADMIN    3

int autenticar(char usuario[], char clave[]);
void registrarPacienteLogin(char usuario[], char clave[]);
void guardarUsuarioMedico(char codigo[]);

#endif



