#include "Boton.h"

Boton::Boton(sf::Vector2f posicion, int canal, const Texturas& texturas) 
	: _canal(canal), _sprite(texturas.get(TipoTextura::BotonInactivo)), 
	_texturaInactivo(&texturas.get(TipoTextura::BotonInactivo)), _texturaActivo(&texturas.get(TipoTextura::BotonActivo)) {

	_forma.setSize(sf::Vector2f(24.f, 24.f));
	_forma.setPosition(posicion);
	setActivo(false); // arranca inactivo
}

void Boton::setActivo(bool activo) { 
	// el ternario elige el color segun el estado nuevo: verde activo, rojo inactivo
	_forma.setFillColor(activo ? sf::Color::Green : sf::Color::Red);
	_sprite.setTexture(activo ? *_texturaActivo : *_texturaInactivo);
	_activo = activo;
}

bool Boton::estaActivo() const
{
	return _activo;
}

int Boton::getCanal() const
{
	return _canal;
}

void Boton::dibujar(sf::RenderWindow& ventana) {
	_sprite.setPosition(_forma.getPosition());
	ventana.draw(_sprite);
}

sf::FloatRect Boton::getHitbox() const
{
	return _forma.getGlobalBounds();
}
