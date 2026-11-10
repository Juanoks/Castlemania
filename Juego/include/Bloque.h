#pragma once
#include "Terreno.h"
#include "Texturas.h"

class Bloque : public Terreno {
private:
	bool _esMadera;
public:
	Bloque(sf::Vector2f posicion, sf::Vector2f tamanio, const Texturas& texturas, bool esMadera = false);
	bool esMadera() const;
};

