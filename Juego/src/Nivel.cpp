#include "Nivel.h"
#include "Constantes.h"

// Tamanio de la salida por defecto (un hueco angosto y alto, como la meta de antes)
sf::Vector2f salidaTamanio(30.f, 120.f);

Nivel::Nivel(int numeroNivel) : _numeroNivel(numeroNivel) {
	cargarNivel(numeroNivel);
}

void Nivel::actualizar(float dt) {
	for (Obstaculo* o : _obstaculos) {
		o->actualizar(dt);
	}
}

void Nivel::cargarNivel(int numeroNivel) {
	for (Obstaculo* o : _obstaculos) { delete o; }
	_obstaculos.clear(); _plataformas.clear(); _bloques.clear(); _salidas.clear();

	// Valores por defecto de los niveles. si se quiere se puede cambiar en el nivel que se quiera
	_numeroNivel = numeroNivel;
	_respawn = sf::Vector2f(50.f, Y_PISO - ALTO_JUGADOR); // parado sobre el piso

	if (numeroNivel == 1) {
		// () = pared sin salida | (numero de sala) salida
		// si se quiere ajustar la posicion de la salida es en el segundo parametro. 
		// Por defecto: en las paredes el hueco queda pegado al piso, y en piso y techo queda centrado
		techo();
		piso();
		paredIzquierda();
		paredDerecha(2);

		_obstaculos.push_back(new Pinche(sf::Vector2f(350.f, Y_PISO - 37.5f))); // agrego obstaculos en el nivel
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(500.f, Y_PISO - 80.f), sf::Vector2f(40.f, 80.f), 500.f, 750.f, 150.f));

	}
	else if (numeroNivel == 2) {
		techo();
		piso();
		paredIzquierda(1); // vuelve a la sala 1
		paredDerecha(3); // va a sala 3
		_obstaculos.push_back(new Pinche(sf::Vector2f(350.f, Y_PISO - 37.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(400.f, Y_PISO - 37.5f)));
	}
	else if (numeroNivel == 3) {
		techo();
		piso();
		paredIzquierda(2);
		paredDerecha(4);
		_plataformas.push_back(Plataforma(sf::Vector2f(200.f, 400.f), sf::Vector2f(100.f, 20.f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(300.f, Y_PISO - 37.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(350.f, Y_PISO - 37.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(400.f, Y_PISO - 37.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(450.f, Y_PISO - 37.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(500.f, Y_PISO - 37.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(550.f, Y_PISO - 37.5f)));
	}
	else if (numeroNivel == 4) {
		techo();
		paredIzquierda(3);
		_salidas.push_back(Salida(sf::Vector2f(850.f, 180.f), salidaTamanio, 5, sf::Vector2f(50.f, Y_PISO - ALTO_JUGADOR))); // en este nivel la salida esta mas arriba
		_bloques.push_back(Bloque(sf::Vector2f(0.f, Y_PISO), sf::Vector2f(200.f, GROSOR_PARED)));
		_bloques.push_back(Bloque(sf::Vector2f(350.f, 400.f), sf::Vector2f(200.f, 40.f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(427.5f, 365.5f)));
		_bloques.push_back(Bloque(sf::Vector2f(700.f, 300.f), sf::Vector2f(200.f, 40.f)));

	} // posicion y tamanio de bloque (x,y) (ancho,alto)
	else if (numeroNivel == 5) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, Y_PISO), sf::Vector2f(150.f, GROSOR_PARED)));
		_plataformas.push_back(Plataforma(sf::Vector2f(125.f, 400.f), sf::Vector2f(80.f, 10.f)));
		_plataformas.push_back(Plataforma(sf::Vector2f(50.f, 300.f), sf::Vector2f(80.f, 10.f)));
		_plataformas.push_back(Plataforma(sf::Vector2f(50.f, 180.f), sf::Vector2f(80.f, 10.f)));
		_bloques.push_back(Bloque(sf::Vector2f(130.f, 180.f), sf::Vector2f(400.f, 40.f)));
		_bloques.push_back(Bloque(sf::Vector2f(330.f, 180.f), sf::Vector2f(400.f, 40.f)));
		_bloques.push_back(Bloque(sf::Vector2f(600.f, 320.f), sf::Vector2f(500.f, 40.f)));
		_bloques.push_back(Bloque(sf::Vector2f(700.f, Y_PISO), sf::Vector2f(500.f, GROSOR_PARED)));
		_plataformas.push_back(Plataforma(sf::Vector2f(600.f, Y_PISO), sf::Vector2f(40.f, 10.f)));

		_bloques.push_back(Bloque(sf::Vector2f(930.f, 0.f), sf::Vector2f(50.f, 600.f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(165.f, 142.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(205.f, 142.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(335.f, 142.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(375.f, 142.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(500.f, 142.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(540.f, 142.5f)));
		// ultimo nivel: destino -1 = fin del juego
		_salidas.push_back(Salida(sf::Vector2f(860.f, 380.f), salidaTamanio, -1, sf::Vector2f(50.f, 420.f)));


	}
	//proximos niveles

}

void Nivel::dibujar(sf::RenderWindow& ventana) {
	for (Obstaculo* o : _obstaculos) {
		o->dibujar(ventana);
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

const std::vector<Plataforma>& Nivel::getPlataformas() const {
	return _plataformas;
}

const std::vector<Bloque>& Nivel::getBloques() const {
	return _bloques;
}

sf::Vector2f Nivel::getRespawn() const {
	return _respawn;
}

Nivel::~Nivel() {
	for (Obstaculo* o : _obstaculos) { delete o; }
}

// pared derecha de la sala. destino 0 = pared cerrada.
void Nivel::paredDerecha(int destino, float inicioHueco) {
	float posicionX = ANCHO_MUNDO - GROSOR_PARED; // la pared va pegada al borde derecho del mundo
	float finHueco = inicioHueco + TAMANIO_HUECO; // donde termina el hueco (y empieza el bloque de abajo)
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