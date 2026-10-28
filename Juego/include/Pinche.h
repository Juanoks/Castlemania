#pragma once
#include <SFML/graphics.hpp>

class Pinche {
private:
	sf::CircleShape _forma;
public:
	Pinche(sf::Vector2f posicion);
	void dibujar(sf::RenderWindow& ventana);
	sf::FloatRect getHitbox() const;
};


