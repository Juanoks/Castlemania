#include "Pinche.h"
#include <SFML/Window.hpp>

// sf::Sprite no tiene constructor vacio: se crea en la lista de inicializacion con su textura
Pinche::Pinche(sf::Vector2f posicion, const Texturas& texturas) : _sprite(texturas.get(TipoTextura::Pinche)) {
	_forma.setPointCount(3);
	_forma.setRadius(25.f);
	_forma.setFillColor(sf::Color(150, 150, 150));
	_forma.setPosition(posicion);

	_sprite.setPosition(getHitbox().position);
}

void Pinche::dibujar(sf::RenderWindow& ventana) {
	ventana.draw(_sprite);
}

sf::FloatRect Pinche::getHitbox() const{
	return _forma.getGlobalBounds(); 
}
