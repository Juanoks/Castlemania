#pragma once
#include <SFML/graphics.hpp>

class Obstaculo {
public: //permite que diferentes obstáculos implementen sus propios comportamientos
	virtual void actualizar(float dt){}
	virtual sf::FloatRect getHitbox() const = 0;
	virtual void dibujar(sf::RenderWindow& ventana) = 0;
	bool tocaAlJugador(sf::FloatRect hitBoxJugador) const;

	virtual ~Obstaculo() {}
};

