#include "Decoracion.h"

Decoracion::Decoracion(sf::Vector2f posicion, TipoDecoracion tipoDecoracion, const Texturas& texturas, sf::Color color) 
	: _tipoDecoracion(tipoDecoracion), _sprite(texturas.get(TipoTextura::CartelMover)){

	switch (tipoDecoracion) {
	case TipoDecoracion::CartelMover:
		_sprite.setTexture(texturas.get(TipoTextura::CartelMover), true);
		break;
	case TipoDecoracion::CartelSaltar:
		_sprite.setTexture(texturas.get(TipoTextura::CartelSaltar), true);
		break;
	case TipoDecoracion::CartelLanza:
		_sprite.setTexture(texturas.get(TipoTextura::CartelLanza), true);
		break;
	case TipoDecoracion::CartelLanzaPiso:
		_sprite.setTexture(texturas.get(TipoTextura::CartelLanzaPiso), true);
		break;
	case TipoDecoracion::CartelDobleSalto:
		_sprite.setTexture(texturas.get(TipoTextura::CartelDobleSalto), true);
		break;
	}

	_sprite.setPosition(posicion);
	_sprite.setColor(color);
}

void Decoracion::dibujar(sf::RenderWindow& ventana) {
	ventana.draw(_sprite);
}
	