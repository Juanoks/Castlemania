#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Obstaculo.h"
#include "Pinche.h"
#include "PilarMovil.h"
#include "Salida.h"
#include "Constantes.h"
#include "Lanza.h"
#include "Plataforma.h"
#include "Bloque.h"
#include "Coleccionable.h"
#include "Boton.h"
#include "Compuerta.h"
#include "Sonidos.h"
#include "Texturas.h"


class Nivel {
private:
	int _numeroNivel;
	sf::Texture _texturaFondo; //para guardar imagen en memoria
	sf::Sprite _fondo; 
	bool _fondoCargado = false;
	std::vector<Obstaculo*> _obstaculos; 
	std::vector<Mecanismo*> _mecanismos;
	std::vector<Plataforma> _plataformas;
	std::vector<Bloque> _bloques;
	std::vector<Salida> _salidas;
	std::vector<Boton> _botones;
	std::vector<Coleccionable> _coleccionables;
	sf::Vector2f _respawn;
	Sonidos& _sonidos; 
	const Texturas& _texturas;
public:
	Nivel(int numeroNivel, Sonidos& sonidos, const Texturas& texturas);
	void actualizar(float dt);
	void cargarNivel(int numeroNivel, bool puertaAbierta = false);
	void dibujar(sf::RenderWindow& ventana);
	bool jugadorTocoObstaculo(sf::FloatRect hitboxJugador) const;
	bool jugadorCayoAlVacio(sf::FloatRect hitboxJugador) const;
	const Salida* salidaTocada(sf::FloatRect hitboxJugador) const;
	const Coleccionable* coleccionableTocado(sf::FloatRect hitboxJugador) const;
	void quitarColeccionable(TipoColeccionable tipo);
	const std::vector<Plataforma>& getPlataformas() const;
	const std::vector<Bloque>& getBloques() const;
	const std::vector<Boton>& getBotones() const;
	void actualizarBotonesYMecanismos(const std::vector<Lanza>& lanzas);
	sf::Vector2f getRespawn() const;
	const std::vector<Mecanismo*>& getMecanismos() const;

	~Nivel();
private:
	// por omision: huecos de las paredes pegados al piso, y los de techo y piso centrados
	void paredDerecha(int destino = 0, float inicioHueco = Y_PISO - TAMANIO_HUECO);
	void paredIzquierda(int destino = 0, float inicioHueco = Y_PISO - TAMANIO_HUECO);
	void techo(int destino = 0, float inicioHueco = (ANCHO_MUNDO - TAMANIO_HUECO) / 2);
	void piso(int destino = 0, float inicioHueco = (ANCHO_MUNDO - TAMANIO_HUECO) / 2);

};
