#pragma once
#include <SFML/graphics.hpp>

class Meta {
private:
	sf::RectangleShape _forma;
public:
	Meta(sf::Vector2f posicion);

	void dibujar(sf::RenderWindow& ventana);
	sf::FloatRect getHitbox() const;
};

