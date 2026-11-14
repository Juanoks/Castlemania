#include "Cristal.h"

Cristal::Cristal(sf::Vector2f posicion, int canal, const Texturas& texturas)
	: _canal(canal), _roto(false), _sprite(texturas.get(TipoTextura::Cristal)),
	_texturaEntero(&texturas.get(TipoTextura::Cristal)), _texturaRoto(&texturas.get(TipoTextura::CristalRoto)) {
	
	_forma.setSize(sf::Vector2f(texturas.get(TipoTextura::Cristal).getSize()));
	_forma.setPosition(posicion);
	_sprite.setPosition(posicion); 
}

void Cristal::romper() {
	_roto = true;
}

bool Cristal::estaRoto() const {
	return _roto;
}

int Cristal::getCanal() const {
	return _canal;
}

void Cristal::dibujar(sf::RenderWindow& ventana) {
	// roto o entero. el ternario elige el puntero y el * da la textura a la que apunta.
	_sprite.setTexture(_roto ? *_texturaRoto : *_texturaEntero);
	ventana.draw(_sprite);
}

sf::FloatRect Cristal::getHitbox() const {
	return _forma.getGlobalBounds();
}
