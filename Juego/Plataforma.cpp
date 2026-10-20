#include "Plataforma.h"

Plataforma::Plataforma(sf::Vector2f posicion, sf::Vector2f tamanio) {
	_forma.setPosition(posicion);
	_forma.setSize(tamanio);
	_forma.setFillColor(sf::Color::Green);
}

void Plataforma::dibujar(sf::RenderWindow& ventana) {
	ventana.draw(_forma);
}

sf::FloatRect Plataforma::getHitbox() const{
	return _forma.getGlobalBounds();
}

