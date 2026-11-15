#include "PilarMovil.h"

PilarMovil::PilarMovil(sf::Vector2f posicionInicial, sf::Vector2f tamanio, float limiteIzquierdo, float limiteDerecho, float velocidad, const Texturas& texturas,bool vertical)
	: _limiteIzquierdo(limiteIzquierdo), _limiteDerecho(limiteDerecho), _velocidad(velocidad), _sprite(texturas.get(TipoTextura::PilarPinches)), _vertical(vertical) {
	_forma.setSize(tamanio);
	_forma.setPosition(posicionInicial);
	_forma.setFillColor(sf::Color::Red);

	sf::Vector2u tamanioImagen = texturas.get(TipoTextura::PilarPinches).getSize();
	_sprite.setScale(sf::Vector2f(tamanio.x / tamanioImagen.x, tamanio.y / tamanioImagen.y));
}

void PilarMovil::actualizar(float dt) {
	if (_vertical) {
		// Movimiento vertical
		_forma.move(sf::Vector2f(0.f, _velocidad * dt));

		if (_forma.getPosition().y >= _limiteDerecho && _velocidad > 0) {
			_velocidad = -_velocidad;
		}

		if (_forma.getPosition().y <= _limiteIzquierdo && _velocidad < 0) {
			_velocidad = -_velocidad;
		}
	}
	else {
		_forma.move(sf::Vector2f(_velocidad * dt, 0.f));
		// se mueve en x; al llegar a un limite (yendo hacia ese lado) invierte la direccion
		if (_forma.getPosition().x >= _limiteDerecho && _velocidad > 0) {
			_velocidad = -_velocidad;  // -_velocidad le da el valor opuesto al que tiene
		}
		if (_forma.getPosition().x <= _limiteIzquierdo && _velocidad < 0) {
			_velocidad = -_velocidad;
		}
	}
}

void PilarMovil::dibujar(sf::RenderWindow& ventana) {
	// el pilar se mueve: en cada frame el sprite copia la posicion de la forma
	_sprite.setPosition(_forma.getPosition());
	ventana.draw(_sprite);
}

sf::FloatRect PilarMovil::getHitbox() const {
	return _forma.getGlobalBounds();
}
