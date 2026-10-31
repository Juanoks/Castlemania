#include "PilarMovil.h"

PilarMovil::PilarMovil(sf::Vector2f posicionInicial, sf::Vector2f tamanio, float limiteIzquierdo, float limiteDerecho, float velocidad) 
	: _limiteIzquierdo(limiteIzquierdo), _limiteDerecho(limiteDerecho), _velocidad(velocidad){
	_forma.setSize(tamanio);
	_forma.setPosition(posicionInicial);
	_forma.setFillColor(sf::Color::Red);
}

void PilarMovil::actualizar(float dt) {
	_forma.move(sf::Vector2f(_velocidad * dt, 0.f));
	// se mueve en x; al llegar a un limite (yendo hacia ese lado) invierte la direccion
	if (_forma.getPosition().x >= _limiteDerecho && _velocidad > 0) {
		_velocidad = -_velocidad;  // -_velocidad le da el valor opuesto al que tiene
	}
	if (_forma.getPosition().x <= _limiteIzquierdo && _velocidad < 0) {
		_velocidad = -_velocidad;
	}
}

void PilarMovil::dibujar(sf::RenderWindow& ventana) {
	ventana.draw(_forma);
}

sf::FloatRect PilarMovil::getHitbox() const {
	return _forma.getGlobalBounds();
}
