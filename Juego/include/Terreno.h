#pragma once
#include <SFML/Graphics.hpp>

class Terreno {
private:
	sf::RectangleShape _forma;
	sf::Sprite _sprite;
public:
	Terreno(sf::Vector2f posicion, sf::Vector2f tamanio, const sf::Texture& textura);

	void dibujar(sf::RenderWindow& ventana);
	sf::FloatRect getHitbox() const;
};

