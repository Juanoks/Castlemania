#include "Lanza.h"
#include "Constantes.h"
#include <cmath>
#include <algorithm>

// "origen" (el jugador) que sale lanzada hacia "destino" (el mouse)
Lanza::Lanza(sf::Vector2f origen, sf::Vector2f destino, const sf::Texture& textura) 
	: _gravedad(600.f), _estado(EstadoLanza::Volando), _sprite(textura) {

	_forma.setSize(sf::Vector2f(48.f, 4.f)); 

	// el "punto de agarre" de la lanza es su centro (la mitad del tamanio):
	// asi gira sobre si misma, y setPosition ubica su centro (no su esquina). algo asi como un "engranaje"
	_forma.setOrigin(sf::Vector2f(24.f, 2.f));
	_sprite.setOrigin(sf::Vector2f(26.f, 6.f)); 
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
		// la gravedad cambia la velocidad en y: por eso la lanza hace una curva
		_velocidad.y += _gravedad * dt;

		// _velocidad dice hacia donde va. atan2(y, x) convierte esa direccion en un angulo, en radianes.
		// equivale a: (700, 0) = 0° derecha, (0, 700) = 90° abajo, (-700, 0) = 180° izquierda,
		// (500, 500) = 45° diagonal abajo-derecha. Asi la punta sigue la curva.
		float angulo = std::atan2(_velocidad.y, _velocidad.x);
		_forma.setRotation(sf::radians(angulo)); // sf::radians: le dice a SFML que el angulo esta en radianes

		_forma.move(_velocidad * dt);
	}
	// volviendo: sin gravedad, en linea recta hacia el jugador.
	// es el mismo calculo que el constructor, pero se repite en cada frame porque el jugador se mueve.
	else if (_estado == EstadoLanza::Volviendo) {
		sf::Vector2f diferencia = posicionJugador - getHitbox().getCenter(); // de la lanza al jugador
		float largo = std::sqrt((diferencia.x * diferencia.x) + (diferencia.y * diferencia.y));
		if (largo > 0) {
			sf::Vector2f direccion = diferencia / largo; // largo 1: solo la direccion
			_velocidad = direccion * VELOCIDAD_LANZA;
			float angulo = std::atan2(_velocidad.y, _velocidad.x);
			_forma.setRotation(sf::radians(angulo)); // apunta hacia el jugador mientras vuelve
			_forma.move(_velocidad * dt);
		}
	}
	// clavada: no se mueve
}

void Lanza::clavar(sf::FloatRect zonaDeChoque, sf::FloatRect objetivo) {

	sf::Vector2f punta = zonaDeChoque.getCenter(); // asigna el centro del choque a la variable punta
	sf::Vector2f centro = punta;

	// se mira a que borde del objetivo quedo pegada la zona de choque: si queda mas cerca de un costado
	// que de arriba/abajo, choco de costado (horizontal); si no, cayo arriba o abajo (vertical)
	float distanciaCostado = std::min(zonaDeChoque.position.x - objetivo.position.x,
		(objetivo.position.x + objetivo.size.x) - (zonaDeChoque.position.x + zonaDeChoque.size.x));
	float distanciaArribaAbajo = std::min(zonaDeChoque.position.y - objetivo.position.y,
		(objetivo.position.y + objetivo.size.y) - (zonaDeChoque.position.y + zonaDeChoque.size.y));
	// std::min recibe dos valores y devuelve el más chico

	if (distanciaCostado < distanciaArribaAbajo) {
		float signo = (_velocidad.x > 0) ? 1.f : -1.f;  // 1 si iba a la derecha, -1 si iba a la izquierda
		centro.x -= signo * 16.f;                        // el centro queda 16 px detras de la punta

		// la imagen tiene la punta a la derecha (0°): si iba a la izquierda se gira 180° para que la punta quede contra la pared
		if(_velocidad.x > 0) {
			_forma.setRotation(sf::degrees(0.f));
		}
		else {
			_forma.setRotation(sf::degrees(180.f));
		}
	}
	// si no, la lanza venia desde arriba o abajo y establece la rotacion en vertical.
	else {
		float signo = (_velocidad.y > 0) ? 1.f : -1.f;  // 1 si iba cayendo, -1 si iba subiendo
		centro.y -= signo * 16.f;                       // el centro queda 16 px detras de la punta (arriba o abajo)
		if (_velocidad.y > 0) {
			_forma.setRotation(sf::degrees(90.f));
		}
		else {
			_forma.setRotation(sf::degrees(-90.f));
		}
		
	}
	_forma.setPosition(centro);
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
	// _forma es la hitbox (la que se mueve y choca); el sprite solo copia su posición y rotación para dibujarse encima
	_sprite.setPosition(_forma.getPosition());
	_sprite.setRotation(_forma.getRotation());
	// le pide a la forma su posicion y rotacion y se la da a sprite
	ventana.draw(_sprite);
}

sf::FloatRect Lanza::getHitbox() const {
	return _forma.getGlobalBounds();
}