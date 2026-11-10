#include "Plataforma.h"

Plataforma::Plataforma(sf::Vector2f posicion, sf::Vector2f tamanio, const Texturas& texturas)
	: Terreno(posicion, tamanio, texturas.get(TipoTextura::Plataforma)) {
}
