#pragma once
#include "Mecanismo.h"

// Mecanismo que bloquea el paso mientras su boton esta apagado.
// Activa = abierta (se puede atravesar). Inactiva = cerrada (solida).
class Compuerta : public Mecanismo {
private:
	sf::RectangleShape _forma;
public:
	Compuerta(sf::Vector2f posicion, sf::Vector2f tamanio, int canal);

	void dibujar(sf::RenderWindow& ventana) override;
	sf::FloatRect getHitbox() const override;
	bool esSolido() const override;
};

