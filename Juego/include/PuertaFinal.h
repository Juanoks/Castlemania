#pragma once
#include <SFML/Graphics.hpp>
#include "Texturas.h"

constexpr float ESCALA_PUERTA = 2.f;

class PuertaFinal {
private:
	sf::RectangleShape _forma;
	sf::Sprite _sprite;
	sf::Sprite _capa;
	const Texturas* _texturas;
	bool _abierta;
	bool _jugadorEstabaCerca;
public:
	PuertaFinal(sf::Vector2f posicion,const Texturas& texturas);

	bool estaAbierta() const;
	void abrir();

	void dibujar(sf::RenderWindow& ventana, const bool llaves[]);
	sf::FloatRect getHitbox() const;

	bool jugadorEstabaCerca() const;
	void setJugadorCerca(bool cerca);
};

