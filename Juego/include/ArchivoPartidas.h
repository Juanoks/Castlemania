#pragma once
#include <string>
#include "RegistroPartida.h"

class ArchivoPartidas {
private:
	std::string _nombre;
public:
	ArchivoPartidas(std::string nombre = "partidas.dat");
	
	bool guardar(RegistroPartida registroP);
	int contarRegistros();
	RegistroPartida leer(int pos);
	bool borrar();
};

