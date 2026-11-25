#pragma once
#include <SFML/Graphics.hpp>
#include "Texturas.h"

enum class TipoDecoracion { CartelMover, CartelSaltar, CartelLanza, CartelLanzaPiso, CartelDobleSalto };

class Decoracion {
private:
	TipoDecoracion _tipoDecoracion;
	sf::Sprite _sprite;
public:
	Decoracion(sf::Vector2f posicion, TipoDecoracion tipoDecoracion, const Texturas& texturas, sf::Color color = sf::Color::White);

	void dibujar(sf::RenderWindow& ventana);
};

