#pragma once
#include <SFML/Audio.hpp> 


enum class TipoMusica { Menu, Juego, Victoria };

constexpr int CANTIDAD_CANCIONES = 3;
constexpr float VOLUMEN_MUSICA = 40.f;

class Musica {
private:
	sf::Music _musica;
	TipoMusica _actual;
	bool _hayMusica;
public:
	Musica();

	void reproducir(TipoMusica tipo);
};


