#pragma once
#include <SFML/Graphics.hpp>

enum class TipoTextura {
	Lanza, Pinche, PilarPinches, BotonInactivo, BotonActivo, Bloque, BloqueMadera, Plataforma,
	LlaveAzul, LlaveAmarilla, LlaveVerde, LlaveRoja, HabilidadLanza, HabilidadDobleSalto,
	CompuertaCerrada, CompuertaAbierta, Jugador
};

constexpr int CANTIDAD_TEXTURAS = 17; // si se agrega un tipo al enum, hay que actualizar este numero

class Texturas {
private:
	sf::Texture _texturas[CANTIDAD_TEXTURAS];
public:
	Texturas();

	const sf::Texture& get(TipoTextura tipo) const; // sf::Texture&: devuelve una referencia a la textura que está guardada en el array
};

