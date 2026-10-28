#pragma once
#include <SFML/graphics.hpp>
#include "Nivel.h"
#include "Jugador.h"

// Una partida en curso: el nivel en el que se esta, el jugador y cuantos niveles hay.
// main solo le pide que avance un frame, que se dibuje y si ya termino.
// Es composicion: la partida TIENE un nivel y un jugador, y viven mientras ella viva.
class Partida {
private:
	int _numeroNivel;
	int _totalNiveles;
	Nivel _nivel;      // va antes que _jugador: el jugador se crea en el respawn del nivel
	Jugador _jugador;  // (los atributos se inicializan en el orden en que estan declarados)
	bool _terminada;   // true cuando se paso la meta del ultimo nivel
public:
	Partida();

	void actualizar(float dt);
	void dibujar(sf::RenderWindow& ventana);
	bool termino() const;
};
