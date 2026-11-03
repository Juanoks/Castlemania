#include "Salida.h"

Salida::Salida(sf::Vector2f posicionHueco, sf::Vector2f tamanio, int salaDestino, sf::Vector2f posicionEntrada) 
	: _salaDestino(salaDestino), _posicionEntrada(posicionEntrada) {

	_forma.setSize(sf::Vector2f(tamanio));
	_forma.setFillColor(sf::Color::Yellow);
	_forma.setPosition(posicionHueco);
}

void Salida::dibujar(sf::RenderWindow& ventana){
	ventana.draw(_forma);
}

sf::FloatRect Salida::getHitbox() const {
	return _forma.getGlobalBounds();
}

int Salida::getSalaDestino() const {
	return _salaDestino;
}

sf::Vector2f Salida::getPosicionEntrada() const {
	return _posicionEntrada;
}