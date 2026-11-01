#include "Partida.h"

// Nivel y Jugador no tienen constructor vacio: por eso se crean en la lista de inicializacion.
Partida::Partida() : _numeroNivel(1), _totalNiveles(5), _nivel(_numeroNivel), _jugador(_nivel.getRespawn()), 
_terminada(false), _tiempoDeCompletado(0), _cantidadMuertes(0) {}

// un frame de juego: obstaculos, flechas, jugador (movimiento y choques), muertes y meta
void Partida::actualizar(float dt) {
	if (_terminada) { //la partida ya termino: no hay nada que actualizar
		return;
	}
	_nivel.actualizar(dt);

	for (Flecha& f : _flechas) {
		f.actualizar(dt);
	}
	for (Flecha& f : _flechas) {
		if(f.estaClavada()){
			continue; // las flechas ya clavadas no se revisan, las demas se clavan al tocar un bloque
		}
		for (const Bloque& b : _nivel.getBloques()) {
			// Si la hitbox de la flecha se cruza con la del bloque. la flecha se clava
			if (f.getHitbox().findIntersection(b.getHitbox())) {
				f.clavar();
			}
		}
	}
	// borra las flechas que salieron del mundo (por abajo o por los costados).
	for (int i = static_cast<int>(_flechas.size()) - 1; i >= 0; i--) {
		sf::FloatRect flechaHitBox =  _flechas[i].getHitbox();
		if (flechaHitBox.position.y > ALTO_MUNDO || flechaHitBox.position.x > ANCHO_MUNDO || 
			flechaHitBox.position.x + flechaHitBox.size.x < 0) {
			_flechas.erase(_flechas.begin() + i);
		}
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

	if (_nivel.jugadorTocoObstaculo(_jugador.getHitbox()) || _nivel.jugadorCayoAlVacio(_jugador.getHitbox())) {
		_jugador.reiniciar(_nivel.getRespawn());
		_cantidadMuertes++;
	}

	if (_nivel.jugadorTocoMeta(_jugador.getHitbox())) {
		if (_numeroNivel < _totalNiveles) {
			_numeroNivel++;	
			_nivel.cargarNivel(_numeroNivel);
			_flechas.clear();
			_jugador.reiniciar(_nivel.getRespawn());
		}
		else {
			_terminada = true;
		}
	}

	_tiempoDeCompletado += dt; //sumando la duración de todos los frames da el tiempo total jugado.
}

//primero el nivel y despues el jugador, asi el jugador queda dibujado por encima.
void Partida::dibujar(sf::RenderWindow& ventana) {
	_nivel.dibujar(ventana);
	_jugador.dibujar(ventana);
	for (Flecha& f : _flechas) {
		f.dibujar(ventana);
	}
}

//vuelve todo al estado inicial (igual que el constructor) sin copiar la Partida
void Partida::reiniciarPartida() {
	_numeroNivel = 1;
	_nivel.cargarNivel(_numeroNivel);
	_jugador.reiniciar(_nivel.getRespawn());
	_terminada = false;
	_tiempoDeCompletado = 0;
	_cantidadMuertes = 0;
	_flechas.clear();
}


//pregunta para main: ¿ya se paso el ultimo nivel?
bool Partida::termino() const {
	return _terminada;
}

float Partida::getTiempoDeCompletado() const{
	return _tiempoDeCompletado;
}

int Partida::getCantidadMuertes() const {
	return _cantidadMuertes;
}

void Partida::dispararFlecha(sf::Vector2f destino) {
	for (const Flecha& f : _flechas) {
		if (!f.estaClavada()) {
			return;
		}
	}
	sf::Vector2f centroJugador = _jugador.getHitbox().getCenter();
	if (_flechas.size() >= MAX_FLECHAS) {
		_flechas.erase(_flechas.begin());
	}
	_flechas.push_back(Flecha(centroJugador,destino));
}