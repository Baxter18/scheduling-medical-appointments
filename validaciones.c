#include <string.h>
#include <ctype.h>
#include "validaciones.h"
#include <conio.h>
#include <stdio.h>

//Comprobacion de clave y censura
void leerPassword(char password[], int max) {
	int i = 0;
	char ch;
	
	while (1) {
		ch = getch();
		
		if (ch == 13) { // ENTER
			password[i] = '\0';
			printf("\n");
			break;
		}
		else if (ch == 8 && i > 0) { // BACKSPACE
			i--;
			printf("\b \b");
		}
		else if (ch >= 32 && ch <= 126 && i < max - 1) {
			password[i++] = ch;
			printf("*");
		}
	}
}

int cadenaVacia(char texto[]) {
	return strlen(texto) == 0;
}

int soloNumeros(char texto[]) {
	for (int i = 0; texto[i]; i++)
		if (!isdigit(texto[i]))
			return 0;
	return 1;
}

//Validacion Correo
int correoValido(char correo[]) {
	return strchr(correo, '@') != NULL;
}

//Validacion cedula ecuatoriana
int validarCedula(char cedula[]) {
	int suma = 0;
	int longitud = strlen(cedula);
	int digitos[10];
	int i, digito, verificador;
	
	//Debe tener exactamente 10 digitos y ser numerica
	if (longitud != 10) return 0;
	for (i = 0; i < 10; i++) {
		if (!isdigit(cedula[i])) return 0;
		digitos[i] = cedula[i] - '0';
	}
	
	//Validar codigo de provincia (01-24)
	int provincia = digitos[0] * 10 + digitos[1];
	if (provincia < 1 || provincia > 24) return 0;
	
	//Validar tercer digito (debe ser menor a 6 para personas naturales)
	if (digitos[2] >= 6) return 0; 
	
	//Algoritmo Modulo 10
	for (i = 0; i < 9; i++) {
		if (i % 2 == 0) { // Posiciones impares (0, 2, 4...) se multiplican por 2
			digito = digitos[i] * 2;
			if (digito > 9) digito -= 9;
		} else { // Posiciones pares (1, 3, 5...) se multiplican por 1
			digito = digitos[i];
		}
		suma += digito;
	}
	
	int decenaSuperior = (suma + 9) / 10 * 10;
	verificador = decenaSuperior - suma;
	
	//Si el verificador es 10, se toma como 0
	if (verificador == 10) verificador = 0;
	
	//Comparar con el ultimo digito de la cedula
	if (verificador == digitos[9]) {
		return 1; // VALIDA
	} else {
		return 0; // INVALIDA
	}
}
