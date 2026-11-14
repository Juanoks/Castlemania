#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 
#include <vector>

enum class TipoSonido {
	// jugador
	Salto, DobleSalto, Aterrizar, Muerte, Respawn,
	// lanza
	LanzaLanzar, LanzaClavar, LanzaRebote, LanzaLlamar, LanzaAtrapar,
	// objetos y mundo
	Llave, Habilidad, PuertaAbrir, PuertaCerrada, BotonActivar, BotonDesactivar,
	Guardado, CambioSala,
	// menu y final
	MenuMover, MenuSeleccionar, MenuVolver, Victoria, NuevoRecord,
	// mecanismos, cristales y puerta final (agregados al final para no correr los numeros de los anteriores)
	CompuertaAbrir, CompuertaCerrar, CristalRomper, BarreraApagar, LlaveColocar
};

constexpr int CANTIDAD_SONIDOS = 28; // si se agrega un valor al enum, hay que actualizar este numero

// carga todos los efectos una vez y los reproduce por tipo: sonidos.reproducir(TipoSonido::Salto).
// _buffers[i] guarda el audio del archivo i; _sonidos[i] es el reproductor que usa ese buffer.
class Sonidos {
private:
	sf::SoundBuffer _buffers[CANTIDAD_SONIDOS]; // _buffers = sonidos cargados
	std::vector<sf::Sound> _sonidos; // en SFML 3, un sf::Sound no se puede crear vacío: necesita su buffer desde el principio

public:
	Sonidos();

	void reproducir(TipoSonido tipo);
};
