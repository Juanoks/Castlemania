#include "Nivel.h"
#include "Constantes.h"

sf::Vector2f metaPosicion(860.f, 380.f);

Nivel::Nivel(int numeroNivel) : _numeroNivel(numeroNivel), _meta(metaPosicion){
	cargarNivel(numeroNivel);
}

void Nivel::actualizar(float dt) {
	for (Obstaculo* o : _obstaculos) {
		o->actualizar(dt);
	}
}

void Nivel::cargarNivel(int numeroNivel) {
	for (Obstaculo* o : _obstaculos) { delete o; }
	_obstaculos.clear(); _plataformas.clear(); _bloques.clear();

	// Valores por defecto de los niveles. si se quiere se puede cambiar en el nivel que se quiera
	_numeroNivel = numeroNivel;
	_meta = metaPosicion; 
	_respawn = sf::Vector2f(50.f, 420.f);

	if (numeroNivel == 1) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, 500.f), sf::Vector2f(960.f, 40.f))); // agrego bloques/suelo en el nivel
		// posicion y tamanio de bloque (x,y) (ancho,alto)
		_obstaculos.push_back(new Pinche(sf::Vector2f(350.f, 462.5f))); // agrego obstaculos en el nivel
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(500.f, 420.f), sf::Vector2f(40.f, 80.f),  500.f,  750.f,  150.f));
	}
	else if (numeroNivel == 2) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, 500.f), sf::Vector2f(960.f, 40.f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(350.f, 462.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(400.f, 462.5f)));
	}
	else if (numeroNivel == 3) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, 500.f), sf::Vector2f(960.f, 40.f)));
		_plataformas.push_back(Plataforma(sf::Vector2f(200.f, 400.f), sf::Vector2f(100.f, 20.f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(300.f, 462.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(350.f, 462.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(400.f, 462.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(450.f, 462.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(500.f, 462.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(550.f, 462.5f)));
	}
	else if (numeroNivel == 4) {
		_meta = sf::Vector2f(850.f, 180.f);
		_bloques.push_back(Bloque(sf::Vector2f(0.f, 500.f), sf::Vector2f(200.f, 40.f))); // cambio la meta de lugar
		_bloques.push_back(Bloque(sf::Vector2f(350.f, 400.f), sf::Vector2f(200.f, 40.f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(427.5f, 365.5f)));
		_bloques.push_back(Bloque(sf::Vector2f(700.f, 300.f), sf::Vector2f(200.f, 40.f)));

	} // posicion y tamanio de bloque (x,y) (ancho,alto)
	else if (numeroNivel == 5) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, 500.f), sf::Vector2f(150.f, 40.f)));
		_plataformas.push_back(Plataforma(sf::Vector2f(125.f, 400.f), sf::Vector2f(80.f, 10.f)));
		_plataformas.push_back(Plataforma(sf::Vector2f(50.f, 300.f), sf::Vector2f(80.f, 10.f)));
		_plataformas.push_back(Plataforma(sf::Vector2f(50.f, 180.f), sf::Vector2f(80.f, 10.f)));
		_bloques.push_back(Bloque(sf::Vector2f(130.f, 180.f), sf::Vector2f(400.f, 40.f)));
		_bloques.push_back(Bloque(sf::Vector2f(330.f, 180.f), sf::Vector2f(400.f, 40.f)));
		_bloques.push_back(Bloque(sf::Vector2f(600.f, 320.f), sf::Vector2f(500.f, 40.f)));
		_bloques.push_back(Bloque(sf::Vector2f(700.f, 500.f), sf::Vector2f(500.f, 40.f)));
		_plataformas.push_back(Plataforma(sf::Vector2f(600.f, 500.f), sf::Vector2f(40.f, 10.f)));

		_bloques.push_back(Bloque(sf::Vector2f(930.f, 0.f), sf::Vector2f(50.f, 600.f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(165.f, 142.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(205.f, 142.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(335.f, 142.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(375.f, 142.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(500.f, 142.5f)));
		_obstaculos.push_back(new Pinche(sf::Vector2f(540.f, 142.5f)));


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
	_meta.dibujar(ventana);
	
}

bool Nivel::jugadorTocoObstaculo(sf::FloatRect hitboxJugador) const{
	for (Obstaculo* o : _obstaculos) { // pregunta a cada obstaculo si toca al jugador; cada uno usa su propia hitbox
		if (o -> tocaAlJugador(hitboxJugador)) {
			return true;
		}
	}
	return false;
}

bool Nivel::jugadorTocoMeta(sf::FloatRect hitboxJugador) const {
	//sf::FloatRect tiene un método findIntersection() que recibe otro FloatRect
	//devuelve un std::optional<sf::FloatRect>, y vale lo mismo que con pollEvent()
	if (hitboxJugador.findIntersection(_meta.getHitbox())) {
		return true;
	}
	return false;
}

bool Nivel::jugadorCayoAlVacio(sf::FloatRect hitboxJugador) const {
	return hitboxJugador.position.y > ALTO_MUNDO; // si la posicion (y) del jugador es mayor a la del borde de abajo de la 
}											      // pantalla; devuelve true

const std::vector<Plataforma>& Nivel::getPlataformas() const {
	return _plataformas;
}

const std::vector<Bloque>& Nivel::getBloques() const {
	return _bloques;
}

sf::Vector2f Nivel::getRespawn() const {
	return _respawn;
}

Nivel::~Nivel(){
	for (Obstaculo* o : _obstaculos) {delete o;}
}