#include "Bloque.h"

Bloque::Bloque(sf::Vector2f posicion, sf::Vector2f tamanio, bool esMadera) 
	: Terreno(posicion, tamanio, esMadera ? sf::Color(90, 50, 20) : sf::Color::White), _esMadera(esMadera) {
	// El color se elige con el operador ternario "condicion ? valorSiTrue : valorSiFalse",
	// un if en miniatura que devuelve un valor (aca no se puede usar un if comun,
	// porque estamos en la lista de inicializacion).
}

bool Bloque::esMadera() const {return _esMadera;} // lo usa la lanza para saber si puede clavarse en este bloque.
