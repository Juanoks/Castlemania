#pragma once
#include <SFML/Graphics.hpp>
#include "Obstaculo.h"
#include "Texturas.h"

class Pinche : public Obstaculo{
private:
	sf::CircleShape _forma; // hitbox (el triangulo); no se dibuja
	sf::Sprite _sprite;     // lo que se ve: se dibuja encima de la hitbox
public:
	Pinche(sf::Vector2f posicion, const Texturas& texturas, float angulo = 0.f);
	void dibujar(sf::RenderWindow& ventana) override;
	sf::FloatRect getHitbox() const override;
};
