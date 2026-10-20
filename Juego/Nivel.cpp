#include "Nivel.h"
#include "Constantes.h"

sf::Vector2f metaPosicion(860.f, 380.f);

Nivel::Nivel(int numeroNivel) : _numeroNivel(numeroNivel), _meta(metaPosicion){
	if (numeroNivel == 1) {	
		_respawn = sf::Vector2f (50.f, 420.f);
		_plataformas.push_back(Plataforma(sf::Vector2f(0.f, 500.f), sf::Vector2f(960.f, 40.f)));
		// posicion y tamanio de plataforma (x,y) (ancho,alto)
		_pinches.push_back(sf::Vector2f(350.f, 462.5f)); // agrego pinches en el nivel
	}		
	else if (numeroNivel == 2) {
		_respawn = sf::Vector2f(50.f, 420.f);
		_plataformas.push_back(Plataforma(sf::Vector2f(0.f, 500.f), sf::Vector2f(960.f, 40.f)));
		_pinches.push_back(sf::Vector2f(350.f, 462.5f));
		_pinches.push_back(sf::Vector2f(400.f, 462.5f));
	}
	else if (numeroNivel == 3) {
		_respawn = sf::Vector2f(50.f, 420.f);
		_plataformas.push_back(Plataforma(sf::Vector2f(0.f, 500.f), sf::Vector2f(960.f, 40.f)));
		_plataformas.push_back(Plataforma(sf::Vector2f(200.f, 400.f), sf::Vector2f(100.f, 20.f)));
		_pinches.push_back(sf::Vector2f(300.f, 462.5f));
		_pinches.push_back(sf::Vector2f(350.f, 462.5f));
		_pinches.push_back(sf::Vector2f(400.f, 462.5f));
		_pinches.push_back(sf::Vector2f(450.f, 462.5f));
		_pinches.push_back(sf::Vector2f(500.f, 462.5f));
		_pinches.push_back(sf::Vector2f(550.f, 462.5f));
	}
	else if (numeroNivel == 4) {
		_respawn = sf::Vector2f(50.f, 420.f);
		_meta = sf::Vector2f(850.f, 180.f);
		_plataformas.push_back(Plataforma(sf::Vector2f(0.f, 500.f), sf::Vector2f(200.f, 40.f)));
		_plataformas.push_back(Plataforma(sf::Vector2f(350.f, 400.f), sf::Vector2f(200.f, 40.f)));
		_pinches.push_back(sf::Vector2f(427.5f, 365.5f));
		_plataformas.push_back(Plataforma(sf::Vector2f(700.f, 300.f), sf::Vector2f(200.f, 40.f)));

	}
	//proximos niveles
}

void Nivel::dibujar(sf::RenderWindow& ventana) {
	for (Pinche& p : _pinches) {
		p.dibujar(ventana);
	}
	for (Plataforma& p : _plataformas) {
		p.dibujar(ventana);
	}
	_meta.dibujar(ventana);
	
}

bool Nivel::jugadorTocoPinche(sf::FloatRect hitBoxJugador) const{
	for (const Pinche& p : _pinches) { // El & hace que "p" sea el pinche de la lista y no una copia.
		//sf::FloatRect tiene un método findIntersection() que recibe otro FloatRect
		//Devuelve un std::optional<sf::FloatRect>, y vale lo mismo que con pollEvent()
		if (hitBoxJugador.findIntersection(p.getHitbox())) {
			return true;
		}
	}
	return false;
}

bool Nivel::jugadorTocoMeta(sf::FloatRect hitBoxJugador) const {
	if (hitBoxJugador.findIntersection(_meta.getHitbox())) {
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

sf::Vector2f Nivel::getRespawn() const {
	return _respawn;
}
