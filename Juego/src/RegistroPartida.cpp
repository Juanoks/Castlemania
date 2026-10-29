#include "RegistroPartida.h"

RegistroPartida::RegistroPartida() : _tiempo(0), _muertes(0) {}

RegistroPartida::RegistroPartida(float tiempo, int muertes) {
	_tiempo = tiempo;
	_muertes = muertes;
}

float RegistroPartida::getTiempo() const {
	return _tiempo;
}

int RegistroPartida::getMuertes() const {
	return _muertes;
}