#include "Lanza.h"
#include "Constantes.h"
#include <cmath>
// "origen" (el jugador) que sale lanzada hacia "destino" (el mouse)
Lanza::Lanza(sf::Vector2f origen, sf::Vector2f destino) : _gravedad(600.f), _estado(EstadoLanza::Volando) {
	_forma.setSize(sf::Vector2f(48.f, 4.f)); 

	// el "punto de agarre" de la lanza es su centro (la mitad del tamanio):
	// asi gira sobre si misma, y setPosition ubica su centro (no su esquina). algo asi como un "engranaje"
	_forma.setOrigin(sf::Vector2f(24.f, 2.f));
	_forma.setPosition(origen);

	_forma.setFillColor(sf::Color::White);
	

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

		// _velocidad dice cuantos pixeles avanza por segundo en x y en y (hacia donde va).
		// atan2(y, x) convierte esa direccion en un angulo ° (en radianes):
		// por ejemplo: (700, 0) = 0° grados derecha, (0, 700) = 90° apunta para abajo.
		// (-700, 0) = 180° izquierda, (500, 500) = 45° grados diagonal abajo-derecha.
		// Asi la punta sigue la curva: sube apuntando arriba y cae apuntando abajo.
		float angulo = std::atan2(_velocidad.y,_velocidad.x); 
		_forma.setRotation(sf::radians(angulo));                      

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
			float angulo = std::atan2(_velocidad.y, _velocidad.x); 
			_forma.setRotation(sf::radians(angulo)); // apunta hacia el jugador mientras vuelve
			_forma.move(_velocidad * dt);
		}
	}
}

void Lanza::clavar() {
	// si la velocidad de x es mayor a la de y significa que la lanza venia de un costado y se clava en horizontal
	if (std::abs(_velocidad.x) > std::abs(_velocidad.y)) {
		_forma.setRotation(sf::degrees(0.f));
	}
	// si no, la lanza venia desde arriba o abajo y establece la rotacion en vertical.
	else {
		_forma.setRotation(sf::degrees(90.f));
	}
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