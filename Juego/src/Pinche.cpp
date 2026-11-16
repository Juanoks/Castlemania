#include "Pinche.h"
#include <SFML/Window.hpp>

// sf::Sprite no tiene constructor vacio: se crea en la lista de inicializacion con su textura
Pinche::Pinche(sf::Vector2f posicion, const Texturas& texturas, float angulo) : _sprite(texturas.get(TipoTextura::Pinche)) {

	// la caja del pinche: en el piso o el techo mide 44x38. en las paredes (90 o 270) esta acostado, asi que 38x44
	sf::Vector2f caja(44.f, 38.f);
	if (angulo == 90.f || angulo == 270.f) {
		caja = sf::Vector2f(38.f, 44.f);
	}
	// se gira alrededor del centro: se calcula sumandole a la esquina la mitad de la caja
	sf::Vector2f centro = posicion + caja / 2.f;

	// la forma (hitbox): primero se arma el triangulo, despues se pone el origen en su centro
	// (25; 18.75 dentro de su circulo de radio 25), se ubica en el centro y se gira
	_forma.setPointCount(3);
	_forma.setRadius(25.f);
	_forma.setFillColor(sf::Color(150, 150, 150));

	_forma.setOrigin(sf::Vector2f(25.f, 18.75f));
	_forma.setPosition(centro);
	_forma.setRotation(sf::degrees(angulo));

	// el sprite gira igual, sobre el centro de su imagen (44x38 -> 22, 19), para quedar justo encima de la hitbox
	_sprite.setOrigin(sf::Vector2f(22.f, 19.f));
	_sprite.setPosition(centro);
	_sprite.setRotation(sf::degrees(angulo));

}

void Pinche::dibujar(sf::RenderWindow& ventana) {
	ventana.draw(_sprite);
}

sf::FloatRect Pinche::getHitbox() const{
	return _forma.getGlobalBounds(); 
}
