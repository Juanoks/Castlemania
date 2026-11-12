#include "Musica.h"
#include <string>
#include <iostream>

Musica::Musica() : _hayMusica(false) {
	_musica.setVolume(VOLUMEN_MUSICA);
}

// pone a sonar la musica pedida. Se llama en cada frame desde Juego.cpp,
void Musica::reproducir(TipoMusica tipo) {
	// si ya esta sonando esta misma, no se hace nada (si no, volveria a empezar 60 veces por segundo).
	if (_hayMusica && tipo == _actual) { return; }
	
	std::string archivos[CANTIDAD_CANCIONES] = {"musica_menu.ogg", "musica_partida.ogg", "musica_victoria.ogg"};
	
	// abrir otro archivo corta el que estaba sonando
	if (!_musica.openFromFile("sonidos/musica/" + archivos[static_cast<int>(tipo)])) {
		std::cout << "No se pudo abrir la musica: " << archivos[static_cast<int>(tipo)] << std::endl;
		return; 
	}
	// la del menu y la del juego se repiten; la de victoria suena una sola vez.
	_musica.setLooping(tipo != TipoMusica::Victoria); // si el tipo no es victoria, se repite
	_musica.play();

	// se guarda cual quedo sonando, para que la proxima llamada sepa si es la misma
	_actual = tipo;
	_hayMusica = true;	
}

