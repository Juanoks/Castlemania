#pragma once
#include <SFML/graphics.hpp>

// Clase base (abstracta) de todo lo que se activa con un boton: compuertas, bloques que se mueven, etc.
// _canal y _activo son protected: las hijas los usan (por ejemplo, para saber si estan abiertas)
class Mecanismo {
protected:
	int _canal;
	bool _activo;
public:
	Mecanismo(int canal);

	int getCanal() const;
	bool estaActivo() const;
	void setActivo(bool activo);

	virtual void actualizar(float dt){}
	virtual void dibujar(sf::RenderWindow&) = 0;
	virtual sf::FloatRect getHitbox() const = 0;
	virtual bool esSolido() const = 0;

	virtual ~Mecanismo() {}
};

