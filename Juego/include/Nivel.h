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
#include "Cristal.h"
#include "Barrera.h"
#include "PuertaLlave.h"
#include "PuertaFinal.h"
#include "Sonidos.h"
#include "Texturas.h"
#include "Decoracion.h"


class Nivel {
private:
	int _numeroNivel;
	std::vector<Obstaculo*> _obstaculos; 
	std::vector<Mecanismo*> _mecanismos;
	std::vector<Plataforma> _plataformas;
	std::vector<Bloque> _bloques;
	std::vector<Salida> _salidas;
	std::vector<Boton> _botones;
	std::vector<Cristal> _cristales;
	std::vector<Coleccionable> _coleccionables;
	sf::Vector2f _respawn;
	std::vector<PuertaFinal> _puertas;
	Sonidos& _sonidos; 
	const Texturas& _texturas;
	sf::Sprite _fondo;
	std::vector<Decoracion> _decoraciones;
public:
	Nivel(int numeroNivel, Sonidos& sonidos, const Texturas& texturas);

	void actualizar(float dt);
	void cargarNivel(int numeroNivel);
	void dibujar(sf::RenderWindow& ventana, const bool llaves[]);

	// hitbox
	bool jugadorTocoObstaculo(sf::FloatRect hitboxJugador) const;
	bool jugadorCayoAlVacio(sf::FloatRect hitboxJugador) const;
	const Salida* salidaTocada(sf::FloatRect hitboxJugador) const;
	const Coleccionable* coleccionableTocado(sf::FloatRect hitboxJugador) const;

	// getters
	const std::vector<Plataforma>& getPlataformas() const;
	const std::vector<Bloque>& getBloques() const;
	const std::vector<Boton>& getBotones() const;
	const std::vector<PuertaFinal>& getPuertas() const;
	sf::Vector2f getRespawn() const;
	void setRespawn(sf::Vector2f posicion);
	const std::vector<Mecanismo*>& getMecanismos() const;

	void quitarColeccionable(TipoColeccionable tipo);
	void actualizarBotonesYMecanismos(const std::vector<Lanza>& lanzas, bool tieneLlaveAzul, bool tieneLlaveAmarilla, bool tieneLlaveVerde);
	void actualizarPuertas(sf::FloatRect hitboxJugador, bool tieneLasLlaves);

	~Nivel();
private:
	// por omision: huecos de las paredes pegados al piso, y los de techo y piso centrados
	void paredDerecha(int destino = 0, float inicioHueco = Y_PISO - TAMANIO_HUECO, float huecoDestino = -1.f);
	void paredIzquierda(int destino = 0, float inicioHueco = Y_PISO - TAMANIO_HUECO, float huecoDestino = -1.f);
	void techo(int destino = 0, float inicioHueco = (ANCHO_MUNDO - TAMANIO_HUECO) / 2, float huecoDestino = -1.f);
	void piso(int destino = 0, float inicioHueco = (ANCHO_MUNDO - TAMANIO_HUECO) / 2, float huecoDestino = -1.f);
	// cajita de piedra cerrada con un cristal adentro
	void cajaConCristal(sf::Vector2f posicion, int canal);

	void cargarSala0();
	void cargarSala1();
	void cargarSala2();
	void cargarSala3();
	void cargarSala4();
	void cargarSala5();
	void cargarSala6();
	void cargarSala7();
	void cargarSala8();
	void cargarSala9();
	void cargarSala10();
	void cargarSala11();
	void cargarSala12();
	void cargarSala13();
	void cargarSala14();
	void cargarSala15();
	void cargarSala16();
	void cargarSala17();
	void cargarSala18();
	void cargarSala19();
	void cargarSala20();
	void cargarSala21();
	void cargarSala22();
	void cargarSala23();
	void cargarSala24();
	void cargarSala25();
	void cargarSala26();
	void cargarSala27();
	void cargarSala28();
	void cargarSala29();
	void cargarSala30();

};
