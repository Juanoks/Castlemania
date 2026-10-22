#include "Terreno.h"

Terreno::Terreno(sf::Vector2f posicion, sf::Vector2f tamanio, sf::Color color) {
	_forma.setPosition(posicion);
	_forma.setSize(tamanio);
	_forma.setFillColor(color);
}

void Terreno::dibujar(sf::RenderWindow& ventana) {
	ventana.draw(_forma);
}

sf::FloatRect Terreno::getHitbox() const {
	return _forma.getGlobalBounds();
}