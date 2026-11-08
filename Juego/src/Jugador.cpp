#include "Jugador.h"
#include "Constantes.h"
#include <SFML/window.hpp>

Jugador::Jugador(sf::Vector2f posicion) : _tieneDobleSalto(false){
	_forma.setSize(sf::Vector2f(ANCHO_JUGADOR, ALTO_JUGADOR));
	// Vector2f Es una clase simple que representa un par de números: una coordenada x y una coordenada y.
	// Vector2f = vector 2D de floats (10.f = float | 10.0 = double) Vector2f espera 2 floats.  
	_forma.setFillColor(sf::Color::Blue);

	_velocidadX = 300.f;
	_gravedad = 980.f;
	_fuerzaSalto = -420.f;
	_cortarSalto = 0.45f;
	_velocidadMaximaCaida = 500.f;

	reiniciar(posicion);
}

void Jugador::dibujar(sf::RenderWindow& ventana) {
	ventana.draw(_forma);
}

void Jugador::reiniciar(sf::Vector2f posicion) {
	// SFML ubica la esquina superior izquierda del rectángulo en esa coordenada, no el centro del rectángulo.
	_forma.setPosition(posicion);
	_velocidadY = 0.f;
	_desplazamientoX = 0.f;
	_enElSuelo = false;
	_saltosRestantes = _tieneDobleSalto ? 1 : 0;// "?" es un operador ternario, es como un mini if
	         									//"_saltosRestantes vale 1 si tiene doble salto, y si no, 0"
	_saltoApretadoAntes = false;
}

void Jugador::mover(float dt) { //“¿que teclas apreto el jugador?” y reaccionar a eso.
	sf::Vector2f movimiento(0.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		movimiento.x -= _velocidadX * dt;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		movimiento.x += _velocidadX * dt;
	}
	bool wApretada = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
	bool acabaDeApretar = wApretada && !_saltoApretadoAntes;
	bool acabaDeSoltar = !wApretada && _saltoApretadoAntes;
	_saltoApretadoAntes = wApretada;
	if (acabaDeApretar) {
		if (_enElSuelo) {
			_velocidadY = _fuerzaSalto;
			_enElSuelo = false;
		}
		else if (_saltosRestantes > 0) {
			_velocidadY = _fuerzaSalto;
			_saltosRestantes--;
		}
	}
	if (acabaDeSoltar && _velocidadY < 0) {
		_velocidadY *= _cortarSalto;
	}

	_forma.move(movimiento); //Para mover la _forma, las clases sf::RectangleShape (y en general las formas de SFML)...
	//tienen un método move(Vector2f) que le suma un desplazamiento a la posición actual.
	_desplazamientoX = movimiento.x;
}

void Jugador::actualizar(float dt) {
	// Lo que le hace el mundo al jugador en este frame: la gravedad acelera la caída
	//  y después se aplica el movimiento vertical.

	_alturaPiesAnterior = getHitbox().position.y + getHitbox().size.y;
	_enElSuelo = false;
	_velocidadY += _gravedad * dt;  // la gravedad cambia la VELOCIDAD

	if (_velocidadY > _velocidadMaximaCaida) { _velocidadY = _velocidadMaximaCaida; } // tope de caida: evita caer cada vez mas rapido

	_forma.move(sf::Vector2f(0.f, _velocidadY * dt)); // la velocidad cambia la POSICION
}

void Jugador::aterrizarEn(sf::FloatRect plataformaHitbox) {
	if (!getHitbox().findIntersection(plataformaHitbox)) { // ¿este rectángulo NO superpone con el de la plataforma?
		return;  // NO se superponen
	}
	if (_velocidadY < 0) { // si el jugador va subiendo, la plataforma no lo frena: la atraviesa desde abajo
		return;
	}
	if (_alturaPiesAnterior > plataformaHitbox.position.y + 0.5f) { // si los pies del jugador estaban más abajo que la parte de 
		return;                                                     // arriba de la plataforma, no aterriza.
	}
	// Si llego hasta aca, ningun chequeo lo descartó: aterrizo.
	// Se lo apoya con los pies justo sobre la plataforma y se frena la caída.
	_forma.setPosition(sf::Vector2f(_forma.getPosition().x, plataformaHitbox.position.y - getHitbox().size.y));
	// y = parte de arriba de la plataforma menos el alto del jugador
	_velocidadY = 0.f;
	_enElSuelo = true;
	_saltosRestantes = _tieneDobleSalto ? 1 : 0;
}

void Jugador::chocarHorizontal(sf::FloatRect bloqueHitbox) { // Choque de costado contra un bloque sólido.
	// ¿No lo toca? Nada que hacer.
	if (!getHitbox().findIntersection(bloqueHitbox)) {
		return;
	}
	// ¿Esta parado encima del bloque? Entonces no es un choque de costado
	// (sin esto, al caminar sobre un bloque lo empujaría hacia un lado).
	if (getHitbox().size.y + _forma.getPosition().y <= bloqueHitbox.position.y + 0.5f) {
		return;
	}
	// si jugador viene desde la izquierda hacia un bloque a su derecha
	if (_desplazamientoX > 0) {
		// lo dejamos pegado al lado izquierdo del bloque.
		_forma.setPosition(sf::Vector2f(bloqueHitbox.position.x - getHitbox().size.x, _forma.getPosition().y));
	}            // borde de la izquierda del bloque(su posicion) - ancho el jugador | La altura del jugador permanece igual.
				 // esto establece la posicion del jugador hacia atras (su ancho) evitando que atraviese el bloque

	// si jugador viene desde la derecha hacia un bloque a su izquierda
	if (_desplazamientoX < 0) {
		_forma.setPosition(sf::Vector2f(bloqueHitbox.position.x + bloqueHitbox.size.x, _forma.getPosition().y));
		// aca necesitamos hacer: posicion del bloque en eje x mas su ancho dado que la posicion x siempre empieza
		// en el borde izquierdo
	}
}

void Jugador::chocarVertical(sf::FloatRect bloqueHitbox) {
	// ¿No lo toca? Nada que hacer.
	if (!getHitbox().findIntersection(bloqueHitbox)) {
		return;
	}
	// jugador choca el bloque por arriba
	if (_velocidadY > 0) {
		_forma.setPosition(sf::Vector2f(_forma.getPosition().x, bloqueHitbox.position.y - getHitbox().size.y));
		_velocidadY = 0.f; // reinicia velocidad de caida
		_enElSuelo = true;
		_saltosRestantes = _tieneDobleSalto ? 1 : 0;
	}
	// jugador choca el bloque por abajo
	else if (_velocidadY < 0) {
		_forma.setPosition(sf::Vector2f(_forma.getPosition().x, bloqueHitbox.position.y + bloqueHitbox.size.y));
		_velocidadY = 0.f;
	}
}

bool Jugador::estaEnElSuelo() const {
	return _enElSuelo;
}

void Jugador::setDobleSalto(bool tieneDobleSalto) {
	_tieneDobleSalto = tieneDobleSalto;
}

/*
sf::FloatRect es “un rectángulo guardado como números”. Es una clase simple de SFML, parecida a Vector2f:
así como Vector2f guarda un par de floats (x, y), un FloatRect guarda dos Vector2f:

position: la esquina superior izquierda del rectángulo (x, y).
size: su ancho y alto (x = ancho, y = alto).
No dibuja nada, es solo información. Es como una “caja invisible” que envuelve a la forma. Eso es lo que se llama hitbox.
*/
sf::FloatRect Jugador::getHitbox() const {
	return _forma.getGlobalBounds();
} ///_forma.getGlobalBounds() ya devuelve esa caja armada, ajustada a donde está la forma en este momento.

