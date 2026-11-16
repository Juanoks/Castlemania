#include "PuertaFinal.h"
#include "Constantes.h"

PuertaFinal::PuertaFinal(sf::Vector2f posicion, const Texturas& texturas) 
	: _sprite(texturas.get(TipoTextura::PuertaCerrada)), _capa(texturas.get(TipoTextura::PuertaCapaAzul)),
	_texturas(&texturas), _abierta(false), _jugadorEstabaCerca(false) {

	_forma.setSize(sf::Vector2f(63.f * ESCALA_PUERTA, 96.f * ESCALA_PUERTA));
	_sprite.setScale(sf::Vector2f(ESCALA_PUERTA, ESCALA_PUERTA));
	_capa.setScale(sf::Vector2f(ESCALA_PUERTA, ESCALA_PUERTA));
	_forma.setPosition(posicion);
	_sprite.setPosition(posicion);
	_capa.setPosition(posicion);
}

bool PuertaFinal::estaAbierta() const {
	return _abierta;
}

void PuertaFinal::abrir() {
	_abierta = true;
}

void PuertaFinal::dibujar(sf::RenderWindow& ventana, const bool llaves[]) {
	if (_abierta) {
		_sprite.setTexture(_texturas->get(TipoTextura::PuertaAbierta));
		ventana.draw(_sprite);
		return;
	}
	bool todas = true;

	for (int i = 0; i < CANTIDAD_LLAVES; i++) {
		if (llaves[i] == false) { todas = false; }
	}
	if (todas == true) { 
		_sprite.setTexture(_texturas->get(TipoTextura::PuertaCompleta)); 
		ventana.draw(_sprite); 
		return; 
	}
	_sprite.setTexture(_texturas->get(TipoTextura::PuertaCerrada));
	ventana.draw(_sprite);
	for (int i = 0; i < CANTIDAD_LLAVES; i++) {
		if (llaves[i] == true) {
			// las 4 capas estan seguidas en el enum (azul, amarilla, verde, roja), igual que las llaves: la capa i es "la azul + i"
			// la capa de la llave i: se pasa PuertaCapaAzul a numero, se le suma i y se vuelve a pasar a TipoTextura.
			// hace falta pasarlo de nuevo porque get pide un TipoTextura, no un int.
			_capa.setTexture(_texturas->get(static_cast<TipoTextura>(static_cast<int>(TipoTextura::PuertaCapaAzul) + i)));
			ventana.draw(_capa);
		}
	}
}

sf::FloatRect PuertaFinal::getHitbox() const
{
	return _forma.getGlobalBounds();
}

bool PuertaFinal::jugadorEstabaCerca() const
{
	return _jugadorEstabaCerca;
}

void PuertaFinal::setJugadorCerca(bool cerca) {
	_jugadorEstabaCerca = cerca;
}