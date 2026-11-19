#include "Salida.h"
#include "Constantes.h"

Salida::Salida(sf::Vector2f posicionHueco, sf::Vector2f tamanio, int salaDestino, sf::Vector2f posicionEntrada) 
	: _luz(sf::PrimitiveType::TriangleStrip, 4), _salaDestino(salaDestino), _posicionEntrada(posicionEntrada) {

	_forma.setSize(sf::Vector2f(tamanio));
	_forma.setPosition(posicionHueco);

	float largo = 60.f; // cuanto entra la luz en la sala
	sf::Vector2f p = posicionHueco;
	sf::Vector2f t = tamanio;

	// los dos primeros puntos van en el borde del hueco y los dos ultimos adentro de la sala
	if (t.y > t.x) { // pared
		if (p.x < ANCHO_MUNDO / 2) { // izquierda: la luz va hacia la derecha
			_luz[0].position = p;
			_luz[1].position = p + sf::Vector2f(0.f, t.y);
			_luz[2].position = p + sf::Vector2f(largo, 0.f);
			_luz[3].position = p + sf::Vector2f(largo, t.y);
		}
		else { // derecha: la luz va hacia la izquierda
			_luz[0].position = p + sf::Vector2f(t.x, 0.f);
			_luz[1].position = p + sf::Vector2f(t.x, t.y);
			_luz[2].position = p + sf::Vector2f(t.x - largo, 0.f);
			_luz[3].position = p + sf::Vector2f(t.x - largo, t.y);
		}
	}
	else { // techo o piso
		if (p.y < ALTO_MUNDO / 2) { // techo: la luz baja
			_luz[0].position = p;
			_luz[1].position = p + sf::Vector2f(t.x, 0.f);
			_luz[2].position = p + sf::Vector2f(0.f, largo);
			_luz[3].position = p + sf::Vector2f(t.x, largo);
		}
		else { // piso: la luz sube
			_luz[0].position = p + sf::Vector2f(0.f, t.y);
			_luz[1].position = p + sf::Vector2f(t.x, t.y);
			_luz[2].position = p + sf::Vector2f(0.f, -largo);
			_luz[3].position = p + sf::Vector2f(t.x, -largo);
		}
	}

	sf::Color fuerte(255, 240, 200, 120);
	sf::Color nada(255, 240, 200, 0);
	_luz[0].color = fuerte;
	_luz[1].color = fuerte;
	_luz[2].color = nada;
	_luz[3].color = nada;
}

void Salida::dibujar(sf::RenderWindow& ventana){
	ventana.draw(_luz);
}

sf::FloatRect Salida::getHitbox() const {
	return _forma.getGlobalBounds();
}

int Salida::getSalaDestino() const {
	return _salaDestino;
}

sf::Vector2f Salida::getPosicionEntrada() const {
	return _posicionEntrada;
}