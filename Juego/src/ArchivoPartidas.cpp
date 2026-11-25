#define _CRT_SECURE_NO_WARNINGS // permite usar fopen sin que Visual Studio lo marque como inseguro
#include <cstdio>
#include "ArchivoPartidas.h"

ArchivoPartidas::ArchivoPartidas(std::string nombre) {
	_nombre = nombre;
}

bool ArchivoPartidas::guardar(RegistroPartida registroP) {
	FILE* p;

	p = fopen(_nombre.c_str(), "ab");
	if (p == nullptr) {
		return false;
	}	
	bool ok = fwrite(&registroP, sizeof(RegistroPartida), 1, p);
	fclose(p);
	return ok;
}

int ArchivoPartidas::contarRegistros() {
	FILE* p;

	p = fopen(_nombre.c_str(), "rb");
	if (p == nullptr) {
		return 0;
	}

	fseek(p, 0, SEEK_END);
	int bytes = ftell(p);
	fclose(p);

	return bytes/sizeof(RegistroPartida);
}

RegistroPartida ArchivoPartidas::leer(int pos) {
	RegistroPartida registroP;
	FILE* p;

	p = fopen(_nombre.c_str(), "rb");
	if (p == nullptr) {
		return registroP;
	}

	fseek(p, pos* sizeof(RegistroPartida), SEEK_SET);
	fread(&registroP, sizeof(RegistroPartida), 1, p);
	fclose(p);

	return registroP;
}

bool ArchivoPartidas::borrar() {

	FILE* p;

	p = fopen(_nombre.c_str(), "wb");
	if (p == nullptr) { return false; }

	fclose(p);

	return true;
}