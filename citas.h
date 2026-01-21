#ifndef CITAS_H
#define CITAS_H

void verCitaPaciente(char cedula[]);
void verCitasMedico(char codigo[]);
void listarTodasLasCitas(void);
void menuCitasAdmin(void);
void agendarCita(char cedulaPaciente[]);
void cancelarCita(void); 
void cancelarCitaPaciente(char cedulaPaciente[]);
int verificarCitaMedicoPaciente(char codigoMedico[], char cedulaPaciente[]);
void cancelarCitaPorMedico(char codigoMedico[], char cedulaPaciente[]);
#endif


