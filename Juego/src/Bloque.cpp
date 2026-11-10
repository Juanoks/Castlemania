#include "Bloque.h"

Bloque::Bloque(sf::Vector2f posicion, sf::Vector2f tamanio, const Texturas& texturas, bool esMadera)
	: Terreno(posicion, tamanio, esMadera ? texturas.get(TipoTextura::BloqueMadera) : texturas.get(TipoTextura::Bloque)),
	_esMadera(esMadera) {
	// la textura se elige con el operador ternario "condicion ? valorSiTrue : valorSiFalse",
	// un if en miniatura que devuelve un valor (aca no se puede usar un if comun,
	// porque estamos en la lista de inicializacion).
}

bool Bloque::esMadera() const {return _esMadera;} // lo usa la lanza para saber si puede clavarse en este bloque.
