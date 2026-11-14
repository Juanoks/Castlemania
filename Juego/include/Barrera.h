#pragma once
#include "Mecanismo.h"
#include "Texturas.h"

// Mecanismo que bloquea el paso hasta que se rompen todos los cristales de su canal.

class Barrera: public Mecanismo {
private:
	sf::RectangleShape _forma;
	sf::Sprite _sprite;
	const sf::Texture* _activa;
	const sf::Texture* _apagada;
public:
	Barrera(sf::Vector2f posicion, sf::Vector2f tamanio, int canal, const Texturas& texturas);

	void dibujar(sf::RenderWindow& ventana) override;
	sf::FloatRect getHitbox() const override;
	bool esSolido() const override;
};
