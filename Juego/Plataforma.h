#pragma once
#include <SFML/graphics.hpp>

class Plataforma {
private:
	sf::RectangleShape _forma;
public:
	Plataforma(sf::Vector2f posicion, sf::Vector2f tamanio);

	void dibujar(sf::RenderWindow& ventana);
	sf::FloatRect getHitbox() const;
};

