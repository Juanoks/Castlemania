#pragma once
#include "Mecanismo.h"
#include "Texturas.h"

// Mecanismo que bloquea el paso mientras su boton esta apagado o encendido si está invertida.
// Activa = abierta (se puede atravesar). Inactiva = cerrada (solida).
class Compuerta : public Mecanismo {
private:
	sf::RectangleShape _forma;
	sf::Sprite _sprite;
	const sf::Texture* _cerrada;
	const sf::Texture* _abierta;
	bool _invertida; // abierta mientras su boton esta apagado, y se cierra al activarlo
public:
	Compuerta(sf::Vector2f posicion, sf::Vector2f tamanio, int canal, const Texturas& texturas, bool horizontal = false, bool invertida = false);

	void dibujar(sf::RenderWindow& ventana) override;
	sf::FloatRect getHitbox() const override;
	bool esSolido() const override;
};

