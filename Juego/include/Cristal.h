#pragma once
#include <SFML/Graphics.hpp>
#include "Texturas.h"

class Cristal {
private:
	sf::RectangleShape _forma;         
	int _canal;                         
	bool _roto;
	sf::Sprite _sprite;
	const sf::Texture* _texturaEntero;  // punteros a las dos texturas
	const sf::Texture* _texturaRoto;  
public:
	Cristal(sf::Vector2f posicion, int canal, const Texturas& texturas);

	void romper();
	bool estaRoto() const;
	int getCanal() const;

	void dibujar(sf::RenderWindow& ventana);
	sf::FloatRect getHitbox() const;
};
