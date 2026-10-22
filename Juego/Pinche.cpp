#include "Pinche.h"
#include <SFML/window.hpp>

Pinche::Pinche(sf::Vector2f posicion) {
	_forma.setPointCount(3);
	_forma.setRadius(25.f);
	_forma.setFillColor(sf::Color(150, 150, 150));
	_forma.setPosition(posicion);
}

void Pinche::dibujar(sf::RenderWindow& ventana) {
	ventana.draw(_forma);
}

sf::FloatRect Pinche::getHitbox() const{
	return _forma.getGlobalBounds(); 
}
