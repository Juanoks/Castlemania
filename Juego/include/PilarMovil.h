#pragma once
#include <SFML/Graphics.hpp>
#include "Obstaculo.h"
#include "Texturas.h"

class PilarMovil : public Obstaculo{
private:
	sf::RectangleShape _forma; // hitbox: es la que se mueve y choca
	// limites del recorrido: en x si se mueve de costado, en y si es vertical (minimo = izquierda o arriba)
	float _limiteMinimo;
	float _limiteMaximo;
	float _velocidad;
	sf::Sprite _sprite;        // lo que se ve: copia la posicion de la forma al dibujarse
	bool _vertical;
public:
	PilarMovil(sf::Vector2f posicionInicial, sf::Vector2f tamanio, float limiteMinimo, float limiteMaximo, float velocidad, const Texturas& texturas, bool vertical = false, bool acostado = false);

	void actualizar(float dt) override;
	void dibujar(sf::RenderWindow& ventana) override;
	sf::FloatRect getHitbox() const override;
};
