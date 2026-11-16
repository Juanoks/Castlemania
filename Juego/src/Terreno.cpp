#include "Terreno.h"

Terreno::Terreno(sf::Vector2f posicion, sf::Vector2f tamanio, const sf::Texture& textura) : _sprite(textura){
	_forma.setPosition(posicion);
	_sprite.setPosition(posicion);
	_forma.setSize(tamanio);

	// muestra un rectangulo de la imagen del tamanio del bloque, desde la esquina (0,0);
	// con setRepeated la imagen se repite en mosaico en vez de estirarse
	_sprite.setTextureRect(sf::IntRect({ 0, 0 }, sf::Vector2i(static_cast<int>(tamanio.x), static_cast<int>(tamanio.y))));

}

void Terreno::setColor(sf::Color color) {
	_sprite.setColor(color);
}

void Terreno::dibujar(sf::RenderWindow& ventana) {
	ventana.draw(_sprite);
}

sf::FloatRect Terreno::getHitbox() const {
	return _forma.getGlobalBounds();	
}