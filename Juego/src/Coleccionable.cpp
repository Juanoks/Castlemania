#include "Coleccionable.h"

Coleccionable::Coleccionable(sf::Vector2f posicion, TipoColeccionable tipoColeccionable, const Texturas& texturas)
	: _tipoColeccionable(tipoColeccionable), _sprite(texturas.get(TipoTextura::LlaveAzul)) {

	// el switch traduce el tipo de coleccionable al tipo de textura que le corresponde
	TipoTextura tipoTextura = TipoTextura::LlaveAzul;
	switch (tipoColeccionable) {
	case TipoColeccionable::LlaveAzul:
		tipoTextura = TipoTextura::LlaveAzul;
		break;

	case TipoColeccionable::LlaveAmarilla:
		tipoTextura = TipoTextura::LlaveAmarilla;
		break;

	case TipoColeccionable::LlaveVerde:
		tipoTextura = TipoTextura::LlaveVerde;
		break;

	case TipoColeccionable::LlaveRoja:
		tipoTextura = TipoTextura::LlaveRoja;
		break;

	case TipoColeccionable::Lanza:
		tipoTextura = TipoTextura::HabilidadLanza;
		break;

	case TipoColeccionable::DobleSalto:
		tipoTextura = TipoTextura::HabilidadDobleSalto;
		break;
	}

	const sf::Texture& textura = texturas.get(tipoTextura);
	// el true ajusta el recorte del sprite al tamanio de la imagen nueva
	// (sin el, se quedaria con el de la textura inicial: las llaves miden 32x32 y las habilidades 40x40)
	_sprite.setTexture(textura, true);
	_sprite.setPosition(posicion); // no se mueve: alcanza con ubicarlo una vez

	// la hitbox mide lo mismo que la imagen. getSize() devuelve enteros: sf::Vector2f(...) los pasa a floats
	_forma.setSize(sf::Vector2f(textura.getSize()));
	_forma.setPosition(posicion);
}

void Coleccionable::dibujar(sf::RenderWindow& ventana){
	ventana.draw(_sprite);
}

TipoColeccionable Coleccionable::getTipo() const
{
	return _tipoColeccionable;
}

sf::FloatRect Coleccionable::getHitbox() const
{
	return _forma.getGlobalBounds();
}
