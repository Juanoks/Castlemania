#pragma once
#include <vector>
#include <SFML/graphics.hpp>
#include "Obstaculo.h"
#include "Pinche.h"
#include "PilarMovil.h"
#include "Salida.h"
#include "Plataforma.h"
#include "Bloque.h"
#include "Constantes.h"

class Nivel {
private:
	int _numeroNivel;
	std::vector<Obstaculo*> _obstaculos; 
	std::vector<Plataforma> _plataformas;
	std::vector<Bloque> _bloques;
	std::vector<Salida> _salidas;
	sf::Vector2f _respawn;
public:
	Nivel(int numeroNivel);
	void actualizar(float dt);
	void cargarNivel(int numeroNivel);
	void dibujar(sf::RenderWindow& ventana);
	bool jugadorTocoObstaculo(sf::FloatRect hitboxJugador) const;
	bool jugadorCayoAlVacio(sf::FloatRect hitboxJugador) const;
	const Salida* salidaTocada(sf::FloatRect hitboxJugador)   const;
	const std::vector<Plataforma>& getPlataformas() const;
	const std::vector<Bloque>& getBloques() const;
	sf::Vector2f getRespawn() const;

	~Nivel();
private:
	// por omision: huecos de las paredes pegados al piso, y los de techo y piso centrados
	void paredDerecha(int destino = 0, float inicioHueco = Y_PISO - TAMANIO_HUECO);
	void paredIzquierda(int destino = 0, float inicioHueco = Y_PISO - TAMANIO_HUECO);
	void techo(int destino = 0, float inicioHueco = (ANCHO_MUNDO - TAMANIO_HUECO) / 2);
	void piso(int destino = 0, float inicioHueco = (ANCHO_MUNDO - TAMANIO_HUECO) / 2);

};
