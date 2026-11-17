#pragma once
#include "Mecanismo.h"
#include "Texturas.h"

// Puerta que se abre al tener la llave de su color. Esta hecha de dos capas:

class PuertaLlave : public Mecanismo {
private:
	sf::RectangleShape _forma;   
	sf::Sprite _spriteColor;     // capa que se pinta
	sf::Sprite _spriteHerrajes;  
public:
	PuertaLlave(sf::Vector2f posicion, int canal, sf::Color color, const Texturas& texturas);

	void dibujar(sf::RenderWindow& ventana) override;
	sf::FloatRect getHitbox() const override;
	bool esSolido() const override;
};
