#include "Compuerta.h"

Compuerta::Compuerta(sf::Vector2f posicion, sf::Vector2f tamanio, int canal, const Texturas& texturas) 
	: Mecanismo(canal), _sprite(texturas.get(TipoTextura::CompuertaCerrada))
	, _cerrada(&texturas.get(TipoTextura::CompuertaCerrada)), _abierta(&texturas.get(TipoTextura::CompuertaAbierta)) {

	// el canal lo guarda el padre (Mecanismo)
	_forma.setSize(tamanio);
	_forma.setPosition(posicion);
	_sprite.setPosition(posicion);
}

void Compuerta::dibujar(sf::RenderWindow& ventana) {
	_forma.setFillColor(_activo ? sf::Color(255, 0, 0, 40) : sf::Color::Red); // casi transparente si esta abierto
	_sprite.setTexture(_activo ? *_abierta : *_cerrada);
	ventana.draw(_sprite);
}

sf::FloatRect Compuerta::getHitbox() const {
	return _forma.getGlobalBounds();
}

bool Compuerta::esSolido() const { // solida solo cuando esta cerrada (inactiva)
	return !_activo;
}
