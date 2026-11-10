#pragma once
#include <SFML/Graphics.hpp>

class Obstaculo {
public:
	virtual void actualizar(float dt){}
	virtual sf::FloatRect getHitbox() const = 0;
	virtual void dibujar(sf::RenderWindow& ventana) = 0;
	bool tocaAlJugador(sf::FloatRect hitBoxJugador) const;

	virtual ~Obstaculo() {}
};

