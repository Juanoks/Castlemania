#pragma once
#include <SFML/graphics.hpp>
#include "Obstaculo.h"

class Pinche : public Obstaculo{
private:
	sf::CircleShape _forma;
public:
	Pinche(sf::Vector2f posicion);
	void dibujar(sf::RenderWindow& ventana) override;
	sf::FloatRect getHitbox() const override;
};


