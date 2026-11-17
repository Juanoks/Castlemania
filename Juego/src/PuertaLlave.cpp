#include "PuertaLlave.h"

PuertaLlave::PuertaLlave(sf::Vector2f posicion, int canal, sf::Color color, const Texturas& texturas)
	: Mecanismo(canal), _spriteColor(texturas.get(TipoTextura::PuertaLlaveColor)),
	_spriteHerrajes(texturas.get(TipoTextura::PuertaLlaveHerrajes)) {

	_forma.setSize(sf::Vector2f(20.f, 80.f)); 
	_forma.setPosition(posicion);

	_spriteColor.setPosition(posicion);
	_spriteHerrajes.setPosition(posicion);

	_spriteColor.setColor(color);
}

void PuertaLlave::dibujar(sf::RenderWindow& ventana) {
	// solo se dibuja si esta cerrada
	if (!_activo) {
		ventana.draw(_spriteColor);
		ventana.draw(_spriteHerrajes);
	}
}

sf::FloatRect PuertaLlave::getHitbox() const {
	return _forma.getGlobalBounds();
}

bool PuertaLlave::esSolido() const { // solo bloquea el paso mientras esta cerrada
	return !_activo;
}
