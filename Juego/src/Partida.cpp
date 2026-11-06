#include "Partida.h"

// Nivel y Jugador no tienen constructor vacio: por eso se crean en la lista de inicializacion.
Partida::Partida() : _numeroNivel(5), _nivel(_numeroNivel), _jugador(_nivel.getRespawn()), 
_terminada(false), _tiempoDeCompletado(0), _cantidadMuertes(0) {}

// un frame de juego. Cada paso esta en su propio metodo (abajo), en el orden en que tiene que pasar.
void Partida::actualizar(float dt) {
	if (_terminada) { // la partida ya termino: no hay nada que actualizar
		return;
	}
	_nivel.actualizar(dt);   // obstaculos (por ejemplo, los pilares que se mueven)
	actualizarFlechas(dt);   // mover, clavar y borrar las que se fueron
	moverJugador(dt);        // movimiento por ejes y choques
	revisarMuerte();         // obstaculos o vacio
	revisarSalida();           // pasar de nivel o terminar la partida

	_tiempoDeCompletado += dt; //sumando la duración de todos los frames da el tiempo total jugado.
}


void Partida::actualizarFlechas(float dt) { // flechas: moverlas, clavarlas al tocar un bloque y borrar las que salieron del mundo.
	for (Flecha& f : _flechas) {
		f.actualizar(dt);
	}
	for (Flecha& f : _flechas) {
		if(f.estaClavada()){
			continue; // las flechas ya clavadas no se revisan, las demas se clavan al tocar un bloque
		}
		for (const Bloque& b : _nivel.getBloques()) {
			// si la hitbox de la flecha se cruza con la del bloque. la flecha se clava
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
}

// jugador: se mueve un eje por vez y despues de cada uno se resuelven los choques de ese eje.
// (si se moviera en diagonal antes de revisar, no se sabria si choco de costado o desde arriba)
void Partida::moverJugador(float dt) {
	// eje X: teclas A/D (y el salto), despues choques de costado con los bloques
	_jugador.mover(dt);
	for (const Bloque& b : _nivel.getBloques()) {
		_jugador.chocarHorizontal(b.getHitbox());
	}

	// eje Y: gravedad, despues choques verticales y aterrizajes
	_jugador.actualizar(dt);
	for (const Bloque& b : _nivel.getBloques()) {
		_jugador.chocarVertical(b.getHitbox());
	}
	for (const Terreno& t : _nivel.getPlataformas()) {
		_jugador.aterrizarEn(t.getHitbox());
	}
	// las flechas clavadas funcionan como plataformas: se puede aterrizar encima
	for (const Flecha& f : _flechas) {
		if (f.estaClavada()) {
			_jugador.aterrizarEn(f.getHitbox());
		}
	}
}

// si toca un obstaculo o se cae al vacio: vuelve al respawn y suma una muerte.
void Partida::revisarMuerte() {
	if (_nivel.jugadorTocoObstaculo(_jugador.getHitbox()) || _nivel.jugadorCayoAlVacio(_jugador.getHitbox())) {
		_jugador.reiniciar(_nivel.getRespawn());
		_cantidadMuertes++;
	}
}

// si llega a la salida: carga el siguiente nivel, o termina la partida si era el ultimo.
void Partida::revisarSalida() {
	const Salida* salida = _nivel.salidaTocada(_jugador.getHitbox());
	if (salida != nullptr) {
		// se copian los datos ANTES de cargarNivel: al cargar se borran las salidas
		// y el puntero quedaria apuntando a algo que ya no existe
		int destino = salida->getSalaDestino();
		sf::Vector2f entrada = salida->getPosicionEntrada();

		if (destino == -1) {
			_terminada = true;   // salida final: fin del juego
		}
		else {
			_numeroNivel = destino;            // la salida decide a que sala se va
			_nivel.cargarNivel(_numeroNivel);
			_flechas.clear();
			_jugador.reiniciar(entrada);       // aparece donde indica la salida
		}
	}
}

// primero el nivel y despues el jugador, asi el jugador queda dibujado por encima.
void Partida::dibujar(sf::RenderWindow& ventana) {
	_nivel.dibujar(ventana);
	_jugador.dibujar(ventana);
	for (Flecha& f : _flechas) {
		f.dibujar(ventana);
	}
}

// vuelve todo al estado inicial (igual que el constructor) sin copiar la Partida
void Partida::reiniciarPartida() {
	_numeroNivel = 1;
	_nivel.cargarNivel(_numeroNivel);
	_jugador.reiniciar(_nivel.getRespawn());
	_terminada = false;
	_tiempoDeCompletado = 0;
	_cantidadMuertes = 0;
	_flechas.clear();
}


// pregunta para main: ¿ya se paso el ultimo nivel?
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