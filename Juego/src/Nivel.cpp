#include "Nivel.h"
#include "Constantes.h"

// Tamanio de la salida por defecto (un hueco angosto y alto, como la meta de antes)
sf::Vector2f salidaTamanio(30.f, 120.f);

// Mapa: cada fila es una sala y cada columna un lado de esa sala.
// izquierda, derecha, arriba, abajo | 0 = pared cerrada | -1 = fin del juego | -2/TERRENO_A_MANO = hacer el piso de esa sala a mano
const int CONEXIONES[31][4] = {
	{0, 1, 0, TERRENO_A_MANO},  // 0: no se usa (asi el numero de fila coincide con el numero de sala)
	{0, 2, 0, 0},  // 1: inicio
	{1, 3, 0, 0},  // 2
	{2, 0, 0, 4},  // 3   (arriba ira a la 26)
	{0, 0, 3, 5},  // 4
	{0, 6, 4, TERRENO_A_MANO},  // 5
	{5, 7, 0, 0},  // 6
	{6, 8, 0, 0},  // 7   (arriba ira a la 22)
	{7, -1, 0, 9},  // 8   sala central (derecha ira a la 30, el jefe; por ahora -1 = fin del juego; arriba ira a la 29)
	{0, 10, 8, 0},  // 9   (izquierda ira a la 21, abajo a la 14)
	{9, 0, 0, 0}   // 10  (derecha ira a la 11)
};

Nivel::Nivel(int numeroNivel) : _numeroNivel(numeroNivel) {
	cargarNivel(numeroNivel);
}

void Nivel::actualizar(float dt) {
	for (Obstaculo* o : _obstaculos) {
		o->actualizar(dt);
	}
	for (Mecanismo* m : _mecanismos) {
		m->actualizar(dt);
	}
}

void Nivel::cargarNivel(int numeroNivel, bool puertaAbierta) {
	// Si solo se hace clear(), los objetos creados con new quedan en memoria
	for (Obstaculo* o : _obstaculos) { delete o; }
	for (Mecanismo* m : _mecanismos) { delete m; } 

	_obstaculos.clear(); _plataformas.clear(); _bloques.clear(); _salidas.clear(); _coleccionables.clear(); _botones.clear();
	_mecanismos.clear();

	_numeroNivel = numeroNivel;
	_respawn = sf::Vector2f(50.f, Y_PISO - ALTO_JUGADOR); // parado sobre el piso

	paredIzquierda(CONEXIONES[numeroNivel][0]);
	paredDerecha(CONEXIONES[numeroNivel][1]);
	techo(CONEXIONES[numeroNivel][2]);
	piso(CONEXIONES[numeroNivel][3]);

	//                            posicion (x,y), tamanio (ancho,alto)
	if (numeroNivel == 0) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, ALTO_MUNDO-20.f), sf::Vector2f(400.f, 30.f)));
		_bloques.push_back(Bloque(sf::Vector2f(500.f, ALTO_MUNDO - 20.f), sf::Vector2f(400.f, 30.f)));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(300.f, 480.f), TipoColeccionable::LlaveAzul));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(200.f, 480.f), TipoColeccionable::LlaveAmarilla));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(500.f, 280.f), TipoColeccionable::Lanza));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(100.f, 480.f), TipoColeccionable::DobleSalto));
		_bloques.push_back(Bloque(sf::Vector2f(350.f, 400.f), sf::Vector2f(200.f, 40.f)));
		_bloques.push_back(Bloque(sf::Vector2f(800.f, 300.f), sf::Vector2f(20.f, 100.f), true));
		_obstaculos.push_back(new Pinche(sf::Vector2f(150.f, Y_PISO - 37.5f)));
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(600.f, Y_PISO - 80.f), sf::Vector2f(40.f, 80.f), 600.f, 800.f, 150.f));
		_plataformas.push_back(Plataforma(sf::Vector2f(200.f, 300.f), sf::Vector2f(100.f, 20.f)));
		_botones.push_back(Boton(sf::Vector2f(500.f, 200.f),1));
		_mecanismos.push_back(new Compuerta(sf::Vector2f(500.f, 300.f), sf::Vector2f(ANCHO_JUGADOR, ALTO_JUGADOR+60.f), 1));
	}

	if (numeroNivel == 1) {
		_obstaculos.push_back(new Pinche(sf::Vector2f(350.f, Y_PISO - 37.5f))); // agrego obstaculos en el nivel
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(500.f, Y_PISO - 80.f), sf::Vector2f(40.f, 80.f), 500.f, 750.f, 150.f));
	}
	else if (numeroNivel == 2) {
		_obstaculos.push_back(new Pinche(sf::Vector2f(350.f, Y_PISO - 37.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(400.f, Y_PISO - 37.5f)));
	}
	else if (numeroNivel == 3) {
		_plataformas.push_back(Plataforma(sf::Vector2f(200.f, 400.f), sf::Vector2f(100.f, 20.f)));
	}
	else if (numeroNivel == 4) {
		_bloques.push_back(Bloque(sf::Vector2f(350.f, 400.f), sf::Vector2f(200.f, 40.f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(427.5f, 365.5f)));
		_bloques.push_back(Bloque(sf::Vector2f(700.f, 300.f), sf::Vector2f(200.f, 40.f)));

	} 
	else if (numeroNivel == 5) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, Y_PISO), sf::Vector2f(300.f, GROSOR_PARED)));
		_bloques.push_back(Bloque(sf::Vector2f(450.f, Y_PISO), sf::Vector2f(510.f, GROSOR_PARED)));

		_plataformas.push_back(Plataforma(sf::Vector2f(125.f, 400.f), sf::Vector2f(80.f, 10.f)));
		_plataformas.push_back(Plataforma(sf::Vector2f(50.f, 300.f), sf::Vector2f(80.f, 10.f)));
		_plataformas.push_back(Plataforma(sf::Vector2f(50.f, 180.f), sf::Vector2f(80.f, 10.f)));
		_bloques.push_back(Bloque(sf::Vector2f(130.f, 180.f), sf::Vector2f(400.f, 40.f)));
		_bloques.push_back(Bloque(sf::Vector2f(330.f, 180.f), sf::Vector2f(400.f, 40.f)));
		_bloques.push_back(Bloque(sf::Vector2f(600.f, 320.f), sf::Vector2f(500.f, 40.f)));

		_obstaculos.push_back(new Pinche(sf::Vector2f(165.f, 142.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(205.f, 142.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(335.f, 142.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(375.f, 142.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(500.f, 142.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(540.f, 142.5f)));

	}
	else if (numeroNivel == 8) {
		if (!puertaAbierta) {
			_bloques.push_back(Bloque(sf::Vector2f
			(ANCHO_MUNDO - GROSOR_PARED * 2, Y_PISO - TAMANIO_HUECO), sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO)));
		}
	}
	//proximos niveles

}

void Nivel::dibujar(sf::RenderWindow& ventana) {
	for (Obstaculo* o : _obstaculos) {
		o->dibujar(ventana);
	}
	for (Mecanismo* m : _mecanismos) {
		m->dibujar(ventana);
	}
	for (Plataforma& p : _plataformas) {
		p.dibujar(ventana);
	}
	for (Bloque& b : _bloques) {
		b.dibujar(ventana);
	}
	for (Salida& s : _salidas) {
		s.dibujar(ventana);
	}
	for (Coleccionable& c : _coleccionables) {
		c.dibujar(ventana);
	}
	for (Boton& b : _botones) {
		b.dibujar(ventana);
	}

}

bool Nivel::jugadorTocoObstaculo(sf::FloatRect hitboxJugador) const {
	for (Obstaculo* o : _obstaculos) { // pregunta a cada obstaculo si toca al jugador; cada uno usa su propia hitbox
		if (o->tocaAlJugador(hitboxJugador)) {
			return true;
		}
	}
	return false;
}

bool Nivel::jugadorCayoAlVacio(sf::FloatRect hitboxJugador) const {
	return hitboxJugador.position.y > ALTO_MUNDO; // si la posicion (y) del jugador es mayor a la del borde de abajo de la 
}											      // pantalla; devuelve true

// Devuelve un puntero a la salida que toca el jugador, o nullptr si no toca ninguna (igual que fopen).
const Salida* Nivel::salidaTocada(sf::FloatRect hitboxJugador) const {
	for (const Salida& s : _salidas) {
		if (hitboxJugador.findIntersection(s.getHitbox())) {
			return &s; // & = la direccion de esta salida (un puntero a ella)
		}
	}
	return nullptr;
}

const Coleccionable* Nivel::coleccionableTocado(sf::FloatRect hitboxJugador) const {
	for (const Coleccionable& c : _coleccionables) {
		if (hitboxJugador.findIntersection(c.getHitbox())) {
			return &c; // retorna &c = la direccion de este coleccionable (un puntero a el)
		}
	}
	return nullptr;
}

void Nivel::quitarColeccionable(TipoColeccionable tipo) { 
	// Saca de la sala el coleccionable de ese tipo (cada uno es unico en todo el mapa).
	for (int i = 0; i < static_cast<int>(_coleccionables.size()); i++) {
		if (_coleccionables[i].getTipo() == tipo) {
			_coleccionables.erase(_coleccionables.begin() + i);
			return;
		}
	}
}

const std::vector<Plataforma>& Nivel::getPlataformas() const {
	return _plataformas;
}

const std::vector<Bloque>& Nivel::getBloques() const {
	return _bloques;
}

const std::vector<Boton>& Nivel::getBotones() const {
	return _botones;
}

void Nivel::actualizarBotonesYMecanismos(const std::vector<Lanza>& lanzas) {
	// Un boton esta activo si hay una lanza clavada tocandolo.
	// Se recalcula en cada frame: si la lanza vuelve al jugador, el boton se apaga solo.
	for (Boton& b : _botones) {
		bool tocado = false; // se empieza suponiendo que ninguna lanza lo toca
		for (const Lanza& l : lanzas) {
			if (l.estaClavada() && l.getHitbox().findIntersection(b.getHitbox())) { // solo cuenta una lanza clavada 
				tocado = true;
			}
		}
		// afuera del for de las lanzas: asi lo prende (true) o lo apaga (false) segun lo que se encontro
		b.setActivo(tocado);
	}
	// un mecanismo esta activo si alguno de los botones de su canal esta activo.
	// mismo patron que los botones: se empieza en false, se busca si hay alguno, y al final se avisa.
	for (Mecanismo* m : _mecanismos) {
		bool hayBotonActivo = false;
		for (Boton& b : _botones) {
			if (b.estaActivo() && b.getCanal() == m->getCanal()) {
				hayBotonActivo = true;
			}
		}
		m->setActivo(hayBotonActivo);
	}
}

sf::Vector2f Nivel::getRespawn() const {
	return _respawn;
}

const std::vector<Mecanismo*>& Nivel::getMecanismos() const {return _mecanismos;}

Nivel::~Nivel() {
	for (Obstaculo* o : _obstaculos) { delete o; }
	for (Mecanismo* m : _mecanismos) { delete m; }
}

// pared derecha de la sala. destino 0 = pared cerrada.
void Nivel::paredDerecha(int destino, float inicioHueco) {
	float posicionX = ANCHO_MUNDO - GROSOR_PARED; // la pared va pegada al borde derecho del mundo
	float finHueco = inicioHueco + TAMANIO_HUECO; // donde termina el hueco (y empieza el bloque de abajo)
	if (destino == TERRENO_A_MANO) {return;}
	if (destino == 0) {
		// sin salida: un solo bloque de arriba a abajo
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, 0.f), sf::Vector2f(GROSOR_PARED, ALTO_MUNDO)));
	}
	else {
		// bloque de arriba del hueco (de y=0 hasta inicioHueco)
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, 0.f), sf::Vector2f(GROSOR_PARED, inicioHueco)));
		// bloque de abajo del hueco (de finHueco hasta el fondo)
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, finHueco), sf::Vector2f(GROSOR_PARED, ALTO_MUNDO - finHueco)));
		// la salida ocupa el hueco
		_salidas.push_back(Salida
		(sf::Vector2f(posicionX, inicioHueco), // donde está el hueco
			sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO), // tamaño del hueco
			destino, // a que sala lleva
			sf::Vector2f(GROSOR_PARED + 10.f, finHueco - ALTO_JUGADOR))); // donde aparece el jugador en esa sala
	}
}

void Nivel::paredIzquierda(int destino, float inicioHueco) {
	float posicionX = 0.f;
	float finHueco = inicioHueco + TAMANIO_HUECO;
	if (destino == TERRENO_A_MANO) { return; }
	if (destino == 0) {
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, 0.f), sf::Vector2f(GROSOR_PARED, ALTO_MUNDO)));
	}
	else {
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, 0.f), sf::Vector2f(GROSOR_PARED, inicioHueco)));
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, finHueco), sf::Vector2f(GROSOR_PARED, ALTO_MUNDO - finHueco)));

		_salidas.push_back(Salida
		(sf::Vector2f(posicionX, inicioHueco), sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO),
			destino, sf::Vector2f(ANCHO_MUNDO - GROSOR_PARED - ANCHO_JUGADOR - 10.f, finHueco - ALTO_JUGADOR)));
	}
}

void Nivel::techo(int destino, float inicioHueco) {
	float finHueco = inicioHueco + TAMANIO_HUECO;
	if (destino == TERRENO_A_MANO) { return; }
	if (destino == 0) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, 0.f), sf::Vector2f(ANCHO_MUNDO, GROSOR_PARED)));
	}
	else {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, 0.f), sf::Vector2f(inicioHueco, GROSOR_PARED)));
		_bloques.push_back(Bloque(sf::Vector2f(finHueco, 0.f), sf::Vector2f(ANCHO_MUNDO - finHueco, GROSOR_PARED)));

		_salidas.push_back(Salida(sf::Vector2f(inicioHueco, 0.f),
			sf::Vector2f(TAMANIO_HUECO, GROSOR_PARED), destino, sf::Vector2f(finHueco + 10.f, Y_PISO - ALTO_JUGADOR)));
	}
}

void Nivel::piso(int destino, float inicioHueco) {
	float posicionY = Y_PISO;
	float alto = ALTO_MUNDO - posicionY;
	float finHueco = inicioHueco + TAMANIO_HUECO;
	if (destino == TERRENO_A_MANO) { return; }
	if (destino == 0) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, posicionY), sf::Vector2f(ANCHO_MUNDO, alto)));
	}
	else {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, posicionY), sf::Vector2f(inicioHueco, alto)));
		_bloques.push_back(Bloque(sf::Vector2f(finHueco, posicionY), sf::Vector2f(ANCHO_MUNDO - finHueco, alto)));

		_salidas.push_back(Salida(sf::Vector2f(inicioHueco, posicionY),
			sf::Vector2f(TAMANIO_HUECO, alto), destino,
			sf::Vector2f(inicioHueco + (TAMANIO_HUECO - ANCHO_JUGADOR) / 2, GROSOR_PARED + 5.f)));
	}
}