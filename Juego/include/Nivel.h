#pragma once
#include <vector>
#include <SFML/graphics.hpp>
#include "Obstaculo.h"
#include "Pinche.h"
#include "PilarMovil.h"
#include "Meta.h"
#include "Plataforma.h"
#include "Bloque.h"

class Nivel {
private:
	int _numeroNivel;
	std::vector<Obstaculo*> _obstaculos; 
	std::vector<Plataforma> _plataformas;
	std::vector<Bloque> _bloques;
	Meta _meta;
	sf::Vector2f _respawn;
public:
	Nivel(int numeroNivel);
	void actualizar(float dt);
	void cargarNivel(int numeroNivel);
	void dibujar(sf::RenderWindow& ventana);
	bool jugadorTocoObstaculo(sf::FloatRect hitboxJugador) const;
	bool jugadorTocoMeta(sf::FloatRect hitboxJugador) const;
	bool jugadorCayoAlVacio(sf::FloatRect hitboxJugador) const;
	const std::vector<Plataforma>& getPlataformas() const;
	const std::vector<Bloque>& getBloques() const;
	sf::Vector2f getRespawn() const;

	~Nivel();
};
