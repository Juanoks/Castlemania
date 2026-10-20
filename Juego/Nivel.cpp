#include "Nivel.h"
sf::Vector2f metaPosicion(700.f, 440.f);


Nivel::Nivel(int numeroNivel) : _numeroNivel(numeroNivel), _meta(metaPosicion){
	if (numeroNivel == 1) {	
		_respawn = sf::Vector2f (50.f, 480.f);
		_plataformas.push_back(Plataforma(sf::Vector2f(0.f, 560.f), sf::Vector2f(800.f, 40.f)));
		// posicion y tamanio de plataforma (x,y) (ancho,alto)
		_plataformas.push_back(Plataforma(sf::Vector2f(120.f, 460.f), sf::Vector2f(130.f, 20.f)));
		_pinches.push_back(sf::Vector2f(350.f, 522.5f)); // agrego pinches en el nivel
	}		
	else if (numeroNivel == 2) {
		_respawn = sf::Vector2f(50.f, 480.f);
		_plataformas.push_back(Plataforma(sf::Vector2f(0.f, 560.f), sf::Vector2f(800.f, 40.f)));
		_pinches.push_back(sf::Vector2f(350.f, 522.5f));
		_pinches.push_back(sf::Vector2f(400.f, 522.5f));
	}
	else if (numeroNivel == 3) {
		_respawn = sf::Vector2f(50.f, 480.f);
		_plataformas.push_back(Plataforma(sf::Vector2f(0.f, 560.f), sf::Vector2f(800.f, 40.f)));
		_pinches.push_back(sf::Vector2f(150.f, 522.5f));
		_pinches.push_back(sf::Vector2f(350.f, 522.5f));
		_pinches.push_back(sf::Vector2f(400.f, 522.5f));
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

const std::vector<Plataforma>& Nivel::getPlataformas() const {
	return _plataformas;
}

sf::Vector2f Nivel::getRespawn() const {
	return _respawn;
}
