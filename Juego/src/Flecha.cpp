#include "Flecha.h"
#include "Constantes.h"
#include <cmath>
// "origen" (el jugador) que sale disparada hacia "destino" (el mouse)
Flecha::Flecha(sf::Vector2f origen, sf::Vector2f destino) : _gravedad(600.f), _clavada(false) {
	_forma.setSize(sf::Vector2f(16.f, 4.f));
	_forma.setFillColor(sf::Color::White);
	_forma.setPosition(origen);

	// vector que va del jugador al mouse (su largo depende de qué tan lejos esté el mouse)
	sf::Vector2f diferencia = destino - origen;

	// largo de ese vector: raíz de (x² + y²)
	float largo = std::sqrt((diferencia.x * diferencia.x) + (diferencia.y * diferencia.y));

	// si el clic fue justo sobre el origen, el largo es 0 y no se puede dividir
	if (largo > 0){
		sf::Vector2f direccion = diferencia / largo;
		_velocidad = direccion * VELOCIDAD_FLECHA;
	}
}

void Flecha::actualizar(float dt) {
	if (!_clavada) {
		_velocidad.y += _gravedad * dt;
		_forma.move(_velocidad * dt);
	}	
}


void Flecha::clavar() {
	_velocidad = sf::Vector2f(0.f,0.f);
	_clavada = true;
}

bool Flecha::estaClavada() const {
	return _clavada;
}

void Flecha::dibujar(sf::RenderWindow& ventana) {
	ventana.draw(_forma);
}

sf::FloatRect Flecha::getHitbox() const {
	return _forma.getGlobalBounds();
}