#pragma once
#include <SFML/Graphics.hpp>
#include "Texturas.h"

// blanco donde se puede clavar la lanza. Esta activo mientras tenga una lanza clavada.
// _canal: el numero que lo conecta con sus mecanismos (el boton 1 activa los mecanismos del canal 1).
class Boton {
private:
	sf::RectangleShape _forma;
	int _canal;
	bool _activo;
	sf::Sprite _sprite;
	// punteros a las dos texturas (guardados en Texturas): el sprite cambia entre una y otra en setActivo
	const sf::Texture* _texturaInactivo; 
	const sf::Texture* _texturaActivo;
public:
	Boton(sf::Vector2f posicion, int canal, const Texturas& texturas);

	// el ternario elige la textura; el * va a la direccion del puntero y da la textura misma
	void setActivo(bool activo);
	bool estaActivo() const;
	int getCanal() const;
	void dibujar(sf::RenderWindow& ventana);
	sf::FloatRect getHitbox() const;
};

