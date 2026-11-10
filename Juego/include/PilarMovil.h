#pragma once
#include <SFML/Graphics.hpp>
#include "Obstaculo.h"
#include "Texturas.h"

class PilarMovil : public Obstaculo{
private:
	sf::RectangleShape _forma; // hitbox: es la que se mueve y choca
	float _limiteIzquierdo;
	float _limiteDerecho;
	float _velocidad;
	sf::Sprite _sprite;        // lo que se ve: copia la posicion de la forma al dibujarse
public:
	PilarMovil(sf::Vector2f posicionInicial, sf::Vector2f tamanio, float limiteIzquierdo, float limiteDerecho, float velocidad, const Texturas& texturas);

	void actualizar(float dt) override;
	void dibujar(sf::RenderWindow& ventana) override;
	sf::FloatRect getHitbox() const override;
};
