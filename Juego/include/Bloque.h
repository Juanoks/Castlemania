#pragma once
#include "Terreno.h"

class Bloque : public Terreno {
private:
	bool _esMadera;
public:
	Bloque(sf::Vector2f posicion, sf::Vector2f tamanio, bool esMadera = false);
	bool esMadera() const;
};

