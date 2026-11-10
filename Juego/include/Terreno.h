#pragma once
#include <SFML/Graphics.hpp>

class Terreno {
private:
	sf::RectangleShape _forma;
public:
	Terreno(sf::Vector2f posicion, sf::Vector2f tamanio, sf::Color color);

	void dibujar(sf::RenderWindow& ventana);
	sf::FloatRect getHitbox() const;
};

