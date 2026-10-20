#pragma once
#include <vector>
#include <SFML/graphics.hpp>
#include "Pinche.h"
#include "Meta.h"
#include "Plataforma.h"

class Nivel {
private:
	int _numeroNivel;
	std::vector<Pinche> _pinches; // vector de pinches
	std::vector<Plataforma> _plataformas;
	Meta _meta;
	sf::Vector2f _respawn;
public:
	Nivel(int numeroNivel);
	void dibujar(sf::RenderWindow& ventana);
	bool jugadorTocoPinche(sf::FloatRect hitboxJugador) const;
	bool jugadorTocoMeta(sf::FloatRect hitboxJugador) const;
	const std::vector<Plataforma>& getPlataformas() const;
	sf::Vector2f getRespawn() const;

};

