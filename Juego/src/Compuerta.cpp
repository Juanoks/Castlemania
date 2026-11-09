#include "Compuerta.h"

Compuerta::Compuerta(sf::Vector2f posicion, sf::Vector2f tamanio, int canal) : Mecanismo(canal){
	// el canal lo guarda el padre (Mecanismo)
	_forma.setSize(tamanio);
	_forma.setPosition(posicion);
}

void Compuerta::dibujar(sf::RenderWindow& ventana) {
	_forma.setFillColor(_activo ? sf::Color(255, 0, 0, 40) : sf::Color::Red); // casi transparente si esta abierto
	ventana.draw(_forma);
}

sf::FloatRect Compuerta::getHitbox() const {
	return _forma.getGlobalBounds();
}

bool Compuerta::esSolido() const { // solida solo cuando esta cerrada (inactiva)
	return !_activo;
}
