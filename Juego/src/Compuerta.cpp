#include "Compuerta.h"

Compuerta::Compuerta(sf::Vector2f posicion, sf::Vector2f tamanio, int canal, const Texturas& texturas, bool horizontal, bool invertida)
	: Mecanismo(canal), _sprite(texturas.get(TipoTextura::CompuertaCerrada))
	, _cerrada(&texturas.get(TipoTextura::CompuertaCerrada)), _abierta(&texturas.get(TipoTextura::CompuertaAbierta)), _invertida(invertida) {

	// el canal lo guarda el padre (Mecanismo)
	_forma.setSize(tamanio);
	_forma.setPosition(posicion);

	// el sprite se estira al tamanio pedido y gira sobre su centro 
	sf::Vector2u tamanioImagen = texturas.get(TipoTextura::CompuertaCerrada).getSize();
	_sprite.setOrigin(sf::Vector2f(tamanioImagen.x / 2.f, tamanioImagen.y / 2.f));
	if (horizontal) {
		// la imagen es vertical: antes de girarla tiene que medir lo mismo pero con ancho y alto intercambiados
		_sprite.setScale(sf::Vector2f(tamanio.y / tamanioImagen.x, tamanio.x / tamanioImagen.y));
		_sprite.setRotation(sf::degrees(90.f));
	}
	else {
		_sprite.setScale(sf::Vector2f(tamanio.x / tamanioImagen.x, tamanio.y / tamanioImagen.y));
	}
	_sprite.setPosition(posicion + tamanio / 2.f); // el origen del sprite es su centro
}

void Compuerta::dibujar(sf::RenderWindow& ventana) {
	_sprite.setTexture(esSolido() ? *_cerrada : *_abierta);
	ventana.draw(_sprite);
}

sf::FloatRect Compuerta::getHitbox() const {
	return _forma.getGlobalBounds();
}

bool Compuerta::esSolido() const {
	// normal: solida mientras esta inactiva. Invertida: al reves, solida cuando su boton esta activo
	return _invertida ? _activo : !_activo;
}
