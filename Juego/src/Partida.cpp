#include "Partida.h"

// Nivel y Jugador no tienen constructor vacio: por eso se crean en la lista de inicializacion.
Partida::Partida() : _numeroNivel(0), _nivel(_numeroNivel), _jugador(_nivel.getRespawn()), 
_terminada(false), _lanzaDisponible(true), _tiempoDeCompletado(0), _cantidadMuertes(0) {
	for (int i = 0; i < CANTIDAD_COLECCIONABLES; i++) {
		_obtenidos[i] = false;
	}
}

// un frame de juego. Cada paso esta en su propio metodo (abajo), en el orden en que tiene que pasar.
void Partida::actualizar(float dt) {
	if (_terminada) { // la partida ya termino: no hay nada que actualizar
		return;
	}
	_nivel.actualizar(dt);   // obstaculos (por ejemplo, los pilares que se mueven)
	actualizarLanzas(dt);   // mover, clavar y borrar las que se fueron
	moverJugador(dt);        // movimiento por ejes y choques
	revisarMuerte();         // obstaculos o vacio
	revisarSalida();           // pasar de nivel o terminar la partida
	revisarColeccionables();

	_tiempoDeCompletado += dt; //sumando la duración de todos los frames da el tiempo total jugado.
}

void Partida::actualizarLanzas(float dt) { // lanzas: moverlas, clavarlas al tocar madera (en otros bloques vuelven).
	sf::Vector2f centroJugador = _jugador.getHitbox().getCenter();
	for (Lanza& l : _lanzas) {
		l.actualizar(dt, centroJugador);
	}
	for (Lanza& l : _lanzas) {
		if(l.estaClavada() || l.estaVolviendo()){
			continue; // las lanzas ya clavadas o volviendo no se revisan, las demas se clavan al tocar un bloque
		}
		for (const Bloque& b : _nivel.getBloques()) {
			// si la hitbox de la lanza se cruza con la del bloque; la lanza se clava si es de madera
			if (l.getHitbox().findIntersection(b.getHitbox())) {
				if (b.esMadera()) {
					l.clavar();
					break;
				}
				else {
					l.volver();
					break;
				}
				
			}
		}
	}
	// Borra lanzas en dos casos:
	for (int i = static_cast<int>(_lanzas.size()) - 1; i >= 0; i--) {
		sf::FloatRect lanzaHitBox =  _lanzas[i].getHitbox();
		// 1) salieron del mundo
		if ((lanzaHitBox.position.y > ALTO_MUNDO || lanzaHitBox.position.x > ANCHO_MUNDO ||
			lanzaHitBox.position.x + lanzaHitBox.size.x < 0) && !_lanzas[i].estaVolviendo()) {
			_lanzas.erase(_lanzas.begin() + i);
		}
		//  2) estaban volviendo y llegaron al jugador
		else if (_lanzas[i].estaVolviendo() && lanzaHitBox.findIntersection(_jugador.getHitbox())) {
			_lanzas.erase(_lanzas.begin() + i);
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

	// recarga la lanza solo si esta parado sobre un bloque o una plataforma
	if (_jugador.estaEnElSuelo()) {
		_lanzaDisponible = true;
	}
	// las lanzas clavadas funcionan como plataformas: se puede aterrizar encima
	for (const Lanza& l : _lanzas) {
		if (l.estaClavada()) {
			_jugador.aterrizarEn(l.getHitbox());
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
			quitarColeccionablesObtenidos();
			_lanzas.clear();
			_jugador.reiniciar(entrada);       // aparece donde indica la salida
		}
	}
}

void Partida::revisarColeccionables() { 
	// si toca un coleccionable: lo marca como obtenido y lo saca de la sala
	const Coleccionable* coleccionable = _nivel.coleccionableTocado(_jugador.getHitbox());
	if (coleccionable != nullptr) {
		TipoColeccionable tipo = coleccionable->getTipo();

		_obtenidos[static_cast<int>(tipo)] = true; // el tipo como numero = su casilla en el arreglo
		_nivel.quitarColeccionable(tipo);
	}
}

void Partida::quitarColeccionablesObtenidos() {
	for (int i = 0; i < CANTIDAD_COLECCIONABLES; i++) {
		if (_obtenidos[i]) {
			_nivel.quitarColeccionable(static_cast <TipoColeccionable>(i));
		}
	}
}

// primero el nivel, después el jugador y por último las lanzas
void Partida::dibujar(sf::RenderWindow& ventana) {
	_nivel.dibujar(ventana);
	_jugador.dibujar(ventana);
	for (Lanza& f : _lanzas) {
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
	_lanzas.clear();
	_lanzaDisponible = true;
	for (int i = 0; i < CANTIDAD_COLECCIONABLES; i++) {
		_obtenidos[i] = false;
	}
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

void Partida::lanzarLanza(sf::Vector2f destino) {
	if (!_lanzas.empty()) { // si ya hay una lanza en el mapa, el clic la hace volver
		_lanzas[0].volver();
		_lanzaDisponible = false;
		return;
	}

	if (!_lanzaDisponible) {
		return;
	}
	
	sf::Vector2f centroJugador = _jugador.getHitbox().getCenter();
	
	_lanzas.push_back(Lanza(centroJugador,destino));
	_lanzaDisponible = false;
}