#include "PincheParpadea.h"

PincheParpadea::PincheParpadea(sf::Vector2f posicion, const Texturas& texturas, float intervalo, float angulo) 
	: Pinche(posicion, texturas, angulo), _estaEscondido(false), _tiempo(0.f), _intervalo(intervalo) {
}

void PincheParpadea::actualizar(float dt) {
	_tiempo += dt;
	if (_tiempo > _intervalo) { 
		_estaEscondido = !_estaEscondido;
		_tiempo = 0.f;
	}
}

void PincheParpadea::dibujar(sf::RenderWindow& ventana) {
	if (!_estaEscondido) { Pinche::dibujar(ventana); }
}

sf::FloatRect PincheParpadea::getHitbox() const {
	if (_estaEscondido){ return sf::FloatRect(); } // devuelve un rectangulo vacio
	else { return Pinche::getHitbox(); }
}
