#include "PilarMovil.h"

PilarMovil::PilarMovil(sf::Vector2f posicionInicial, sf::Vector2f tamanio, float limiteMinimo, float limiteMaximo, float velocidad, const Texturas& texturas, bool vertical, bool acostado)
	: _limiteMinimo(limiteMinimo), _limiteMaximo(limiteMaximo), _velocidad(velocidad), _sprite(texturas.get(TipoTextura::PilarPinches)), _vertical(vertical) {
	_forma.setSize(tamanio);
	_forma.setPosition(posicionInicial);
	_forma.setFillColor(sf::Color::Red);

	sf::Vector2u tamanioImagen = texturas.get(TipoTextura::PilarPinches).getSize();
	// el sprite gira sobre su centro; en dibujar se ubica en el centro de la forma
	_sprite.setOrigin(sf::Vector2f(tamanioImagen.x / 2.f, tamanioImagen.y / 2.f));

	if (acostado) {
		// tamanio es la hitbox ya acostada (ej: 70x30). La imagen es vertical: antes de girarla tiene que
		// medir 30x70, por eso la escala usa el ancho y el alto intercambiados
		_sprite.setScale(sf::Vector2f(tamanio.y / tamanioImagen.x, tamanio.x / tamanioImagen.y));
		_sprite.setRotation(sf::degrees(90.f));
	}
	else {
		_sprite.setScale(sf::Vector2f(tamanio.x / tamanioImagen.x, tamanio.y / tamanioImagen.y));
	}
}

void PilarMovil::actualizar(float dt) {
	if (_vertical) {
		// Movimiento vertical
		_forma.move(sf::Vector2f(0.f, _velocidad * dt));

		if (_forma.getPosition().y >= _limiteMaximo && _velocidad > 0) {
			_velocidad = -_velocidad;
		}

		if (_forma.getPosition().y <= _limiteMinimo && _velocidad < 0) {
			_velocidad = -_velocidad;
		}
	}
	else {
		_forma.move(sf::Vector2f(_velocidad * dt, 0.f));
		// se mueve en x; al llegar a un limite (yendo hacia ese lado) invierte la direccion
		if (_forma.getPosition().x >= _limiteMaximo && _velocidad > 0) {
			_velocidad = -_velocidad;  // -_velocidad le da el valor opuesto al que tiene
		}
		if (_forma.getPosition().x <= _limiteMinimo && _velocidad < 0) {
			_velocidad = -_velocidad;
		}
	}
}

void PilarMovil::dibujar(sf::RenderWindow& ventana) {
	// el pilar se mueve: en cada frame el sprite copia la posicion de la forma
	_sprite.setPosition(_forma.getPosition() + _forma.getSize() / 2.f); // el origen del sprite es su centro
	ventana.draw(_sprite);
}

sf::FloatRect PilarMovil::getHitbox() const {
	return _forma.getGlobalBounds();
}
