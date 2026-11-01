#pragma once
#include <SFML/graphics.hpp>
#include <vector>
#include "Constantes.h"
#include "Nivel.h"
#include "Jugador.h"
#include "Flecha.h"

// la partida TIENE un nivel y un jugador, y viven mientras ella viva.
class Partida {
private:
	int _numeroNivel;
	int _totalNiveles;
	Nivel _nivel;      // va antes que _jugador: el jugador se crea en el respawn del nivel
	Jugador _jugador;  // (los atributos se inicializan en el orden en que estan declarados)
	std::vector<Flecha> _flechas;
	bool _terminada;   // true cuando se paso la meta del ultimo nivel
	float _tiempoDeCompletado;
	int _cantidadMuertes;
public:
	Partida();

	void actualizar(float dt);
	void dibujar(sf::RenderWindow& ventana);
	void reiniciarPartida();
	bool termino() const;
	float getTiempoDeCompletado() const;
	int getCantidadMuertes() const;

	void dispararFlecha(sf::Vector2f destino);
};
