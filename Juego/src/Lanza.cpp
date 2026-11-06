#include "Lanza.h"
#include "Constantes.h"
#include <cmath>
// "origen" (el jugador) que sale lanzada hacia "destino" (el mouse)
Lanza::Lanza(sf::Vector2f origen, sf::Vector2f destino) : _gravedad(600.f), _estado(EstadoLanza::Volando) {
	_forma.setSize(sf::Vector2f(48.f, 4.f)); // larga (el doble del jugador) para poder pararse encima
	_forma.setFillColor(sf::Color::White);
	_forma.setPosition(origen);

	// vector que va del jugador al mouse (su largo depende de qué tan lejos esté el mouse)
	sf::Vector2f diferencia = destino - origen;

	// largo de ese vector: raíz de (x² + y²)
	float largo = std::sqrt((diferencia.x * diferencia.x) + (diferencia.y * diferencia.y));

	// si el clic fue justo sobre el origen, el largo es 0 y no se puede dividir
	if (largo > 0){
		sf::Vector2f direccion = diferencia / largo;
		_velocidad = direccion * VELOCIDAD_LANZA;
	}
}

void Lanza::actualizar(float dt, sf::Vector2f posicionJugador) {
	if (_estado == EstadoLanza::Volando) {
		_velocidad.y += _gravedad * dt;
		_forma.move(_velocidad * dt);
	}
	// mismo calculo que el constructor, pero en cada frame porque el jugador se mueve
	// de la lanza al jugador.
	else if (_estado == EstadoLanza::Volviendo) {
		sf::Vector2f diferencia = posicionJugador - getHitbox().getCenter();
		float largo = std::sqrt((diferencia.x * diferencia.x) + (diferencia.y * diferencia.y));
		if (largo > 0) {
			sf::Vector2f direccion = diferencia / largo;
			_velocidad = direccion * VELOCIDAD_LANZA;
			_forma.move(_velocidad * dt);
		}
	}
}

void Lanza::clavar() {
	_velocidad = sf::Vector2f(0.f,0.f);
	_estado = EstadoLanza::Clavada;
}

bool Lanza::estaClavada() const {
	return _estado == EstadoLanza::Clavada;
}

void Lanza::volver() {
	_estado = EstadoLanza::Volviendo;
}

bool Lanza::estaVolviendo() const {
	return _estado == EstadoLanza::Volviendo;
}

void Lanza::dibujar(sf::RenderWindow& ventana) {
	ventana.draw(_forma);
}

sf::FloatRect Lanza::getHitbox() const {
	return _forma.getGlobalBounds();
}