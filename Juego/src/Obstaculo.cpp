#include "Obstaculo.h"

//comun a todos los obstaculos: usa la hitbox de cada hijo (getHitbox es virtual)
bool Obstaculo::tocaAlJugador(sf::FloatRect hitBoxJugador) const {
	if (hitBoxJugador.findIntersection(getHitbox())) { 
		return true;
	}
	return false;
}
