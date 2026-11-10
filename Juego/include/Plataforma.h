#pragma once
#include "Terreno.h"
#include "Texturas.h"

class Plataforma : public Terreno {
public:
	Plataforma(sf::Vector2f posicion, sf::Vector2f tamanio, const Texturas& texturas);
};
