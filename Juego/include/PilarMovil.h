#pragma once
#include <SFML/Graphics.hpp>
#include "Obstaculo.h"

class PilarMovil : public Obstaculo{
private:
	sf::RectangleShape _forma;
	float _limiteIzquierdo;
	float _limiteDerecho;
	float _velocidad;
public:
	PilarMovil(sf::Vector2f posicionInicial, sf::Vector2f tamanio, float limiteIzquierdo, float limiteDerecho, float velocidad);

	void actualizar(float dt) override;
	void dibujar(sf::RenderWindow& ventana) override;
	sf::FloatRect getHitbox() const override;
};