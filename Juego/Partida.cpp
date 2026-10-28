#include "Partida.h"

// Arranca en el nivel 1, con el jugador en el respawn de ese nivel.
// Nivel y Jugador no tienen constructor vacio: por eso se crean en la lista de inicializacion.
Partida::Partida() : _numeroNivel(1), _totalNiveles(5), _nivel(_numeroNivel), _jugador(_nivel.getRespawn()), _terminada(false) {
}

// Un frame de juego: mover, resolver choques y revisar pinches, vacio y meta.
void Partida::actualizar(float dt) {
	if (_terminada) { // la partida ya termino: no hay nada que actualizar
		return;
	}

	_jugador.mover(dt);
	for (const Bloque& b : _nivel.getBloques()) {
		_jugador.chocarHorizontal(b.getHitbox());
	}

	_jugador.actualizar(dt);
	for (const Bloque& b : _nivel.getBloques()) {
		_jugador.chocarVertical(b.getHitbox());
	}
	for (const Terreno& t : _nivel.getPlataformas()) {
		_jugador.aterrizarEn(t.getHitbox());
	}

	if (_nivel.jugadorTocoPinche(_jugador.getHitbox()) || _nivel.jugadorCayoAlVacio(_jugador.getHitbox())) {
		_jugador.reiniciar(_nivel.getRespawn());
	}

	if (_nivel.jugadorTocoMeta(_jugador.getHitbox())) {
		if (_numeroNivel < _totalNiveles) {
			_numeroNivel++;
			_nivel = Nivel(_numeroNivel);
			_jugador.reiniciar(_nivel.getRespawn());
		}
		else {
			_terminada = true;
		}
	}
}

// Primero el nivel y despues el jugador, asi el jugador queda dibujado por encima.
void Partida::dibujar(sf::RenderWindow& ventana) {
	_nivel.dibujar(ventana);
	_jugador.dibujar(ventana);
}

// Pregunta para main: ¿ya se paso el ultimo nivel?
bool Partida::termino() const {
	return _terminada;
}
