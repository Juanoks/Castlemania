#include "Barrera.h"

Barrera::Barrera(sf::Vector2f posicion, sf::Vector2f tamanio, int canal, const Texturas& texturas)
	: Mecanismo(canal), _sprite(texturas.get(TipoTextura::BarreraActiva))
	, _activa(&texturas.get(TipoTextura::BarreraActiva)), _apagada(&texturas.get(TipoTextura::BarreraApagada)) {

	_forma.setSize(tamanio);
	sf::Vector2u tamanioImagen = texturas.get(TipoTextura::BarreraActiva).getSize();
	_sprite.setScale(sf::Vector2f(tamanio.x / tamanioImagen.x, tamanio.y / tamanioImagen.y));

	_forma.setPosition(posicion);
	_sprite.setPosition(posicion);
}

void Barrera::dibujar(sf::RenderWindow& ventana) {
	_sprite.setTexture(_activo ? *_apagada : *_activa);
	ventana.draw(_sprite);	
}

sf::FloatRect Barrera::getHitbox() const {
	return _forma.getGlobalBounds();
}

bool Barrera::esSolido() const { // solida solo cuando esta cerrada/activa
	return !_activo;	
}