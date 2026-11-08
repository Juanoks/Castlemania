#include "Coleccionable.h"

Coleccionable::Coleccionable(sf::Vector2f posicion, TipoColeccionable tipoColeccionable) : _tipoColeccionable(tipoColeccionable){
	_forma.setSize(sf::Vector2f(16.f, 16.f));
	_forma.setPosition(posicion);
	switch (tipoColeccionable) {
	case TipoColeccionable::LlaveAzul:
		_forma.setFillColor(sf::Color(100, 180, 255));
		break;

	case TipoColeccionable::LlaveAmarilla:
		_forma.setFillColor(sf::Color::Yellow);
		break;

	case TipoColeccionable::LlaveVerde:
		_forma.setFillColor(sf::Color::Green);
		break;

	case TipoColeccionable::LlaveRoja:
		_forma.setFillColor(sf::Color::Red);
		break;

	case TipoColeccionable::Lanza:
		_forma.setFillColor(sf::Color::White);
		break;

	case TipoColeccionable::DobleSalto:
		_forma.setFillColor(sf::Color::Magenta);
		break;
	}

}

void Coleccionable::dibujar(sf::RenderWindow& ventana){
	ventana.draw(_forma);
}

TipoColeccionable Coleccionable::getTipo() const
{
	return _tipoColeccionable;
}

sf::FloatRect Coleccionable::getHitbox() const
{
	return _forma.getGlobalBounds();
}
