#pragma once
#include "Pinche.h"

class PincheParpadea : public Pinche{
private:
	bool _estaEscondido;
	float _tiempo;
	float _intervalo;
public:
	PincheParpadea(sf::Vector2f posicion, const Texturas& texturas, float intervalo, float angulo = 0.f);

	void actualizar(float dt) override;
	void dibujar(sf::RenderWindow& ventana) override;
	sf::FloatRect getHitbox() const override;
};

