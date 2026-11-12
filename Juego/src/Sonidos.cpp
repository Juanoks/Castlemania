#include "Sonidos.h"
#include <string>
#include <iostream>

Sonidos::Sonidos() {
	// nombres de los archivos, en el MISMO orden que el enum TipoSonido
	// (la posicion de cada nombre es el numero de su valor en el enum)
	std::string archivos[CANTIDAD_SONIDOS] = {
		// jugador
		"salto.wav", "doble_salto.wav", "aterrizar.wav", "muerte.wav", "respawn.wav",
		// lanza
		"lanza_lanzar.wav", "lanza_clavar.wav", "lanza_rebote.wav", "lanza_llamar.wav", "lanza_atrapar.wav",
		// objetos y mundo
		"llave.wav", "habilidad.wav", "puerta_abrir.wav", "puerta_cerrada.wav", "boton_activar.wav", "boton_desactivar.wav",
		"guardado.wav", "cambio_sala.wav", "pilar_golpe.wav",
		// menu y final
		"menu_mover.wav", "menu_seleccionar.wav", "menu_volver.wav", "victoria.wav", "nuevo_record.wav"
	};

	// carga cada archivo en su buffer (en la misma posicion que su valor del enum)
	for (int i = 0; i < CANTIDAD_SONIDOS; i++) {
		if (!_buffers[i].loadFromFile("sonidos/" + archivos[i])) {
			std::cout << "No se pudo cargar el sonido: sonidos/" << archivos[i] << std::endl;
		}
		// crea el reproductor de este sonido, conectado a su buffer (queda en la misma posicion i)
		_sonidos.push_back(sf::Sound(_buffers[i]));
	}
}

// busca el reproductor de ese tipo (el tipo convertido a numero es su posicion) y lo hace sonar
void Sonidos::reproducir(TipoSonido tipo) {
	_sonidos[static_cast<int>(tipo)].play();
}
