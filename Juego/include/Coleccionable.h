#pragma once
#include <SFML/Graphics.hpp>

enum class TipoColeccionable {LlaveAzul, LlaveAmarilla, LlaveVerde, LlaveRoja, Lanza, DobleSalto};
constexpr int CANTIDAD_COLECCIONABLES = 6; // si se agrega un tipo al enum, hay que actualizar este numero

class Coleccionable {
private:
	sf::RectangleShape _forma;
	TipoColeccionable _tipoColeccionable;
public:
	Coleccionable(sf::Vector2f posicion, TipoColeccionable tipoColeccionable);

	void dibujar(sf::RenderWindow& ventana);
	TipoColeccionable getTipo() const;
	sf::FloatRect getHitbox() const;
};

