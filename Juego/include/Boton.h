#pragma once
#include <SFML/graphics.hpp>

// blanco donde se puede clavar la lanza. Esta activo mientras tenga una lanza clavada.
// _canal: el numero que lo conecta con sus mecanismos (el boton 1 activa los mecanismos del canal 1).
class Boton {
private:
	sf::RectangleShape _forma;
	int _canal;
	bool _activo;
public:
	Boton(sf::Vector2f posicion, int canal);

	void setActivo(bool activo);
	bool estaActivo() const;
	int getCanal() const;
	void dibujar(sf::RenderWindow& ventana);
	sf::FloatRect getHitbox() const;
};

