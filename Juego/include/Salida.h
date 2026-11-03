#pragma once
#include <SFML/graphics.hpp>

class Salida {
private:
	sf::RectangleShape _forma;
	int _salaDestino;
	sf::Vector2f _posicionEntrada;
public:
	Salida(sf::Vector2f posicionHueco, sf::Vector2f tamanio, int salaDestino, sf::Vector2f posicionEntrada);

	void dibujar(sf::RenderWindow& ventana);
	sf::FloatRect getHitbox() const;
	int getSalaDestino() const;
	sf::Vector2f getPosicionEntrada() const;
};

