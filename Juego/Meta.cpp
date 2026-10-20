#include "Meta.h"

Meta::Meta(sf::Vector2f posicion) {
	_forma.setSize(sf::Vector2f(30.f, 120.f));
	_forma.setFillColor(sf::Color::Yellow);
	_forma.setPosition(posicion);
}

void Meta::dibujar(sf::RenderWindow& ventana){
	ventana.draw(_forma);
}

sf::FloatRect Meta::getHitbox() const {
	return _forma.getGlobalBounds();
}