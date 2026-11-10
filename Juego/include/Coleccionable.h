#pragma once
#include <SFML/Graphics.hpp>
#include "Texturas.h"

enum class TipoColeccionable {LlaveAzul, LlaveAmarilla, LlaveVerde, LlaveRoja, Lanza, DobleSalto};
constexpr int CANTIDAD_COLECCIONABLES = 6; // si se agrega un tipo al enum, hay que actualizar este numero

class Coleccionable {
private:
	sf::RectangleShape _forma; // hitbox: mide lo mismo que la imagen
	TipoColeccionable _tipoColeccionable;
	sf::Sprite _sprite;        // lo que se ve; su textura depende del tipo
public:
	Coleccionable(sf::Vector2f posicion, TipoColeccionable tipoColeccionable, const Texturas& texturas);

	void dibujar(sf::RenderWindow& ventana);
	TipoColeccionable getTipo() const;
	sf::FloatRect getHitbox() const;
};

