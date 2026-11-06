#pragma once
#include <SFML/graphics.hpp>

class Lanza {
private:
	sf::RectangleShape _forma;
	sf::Vector2f _velocidad; // va en un Vector2f y no en un float porque se mueve en X y en Y a la vez
	float _gravedad;
	bool _clavada;
public:
	Lanza(sf::Vector2f origen, sf::Vector2f destino);

	void actualizar(float dt);
	void clavar();
	bool estaClavada() const;
	void dibujar(sf::RenderWindow& ventana);
	sf::FloatRect getHitbox() const;

};

