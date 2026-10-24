#include <iostream>
#include <SFML/graphics.hpp> 
#include <SFML/window.hpp>
#include <optional>
#include "Constantes.h"
#include "EstadoJuego.h"
#include "Nivel.h"
#include "Jugador.h"

// Ajusta la cámara para que el mundo entre en una ventana de ese tamaño
// sin deformarse: lo que sobra queda en negro (letterbox).
void ajustarVista(sf::View& vista, sf::Vector2u tamanioVentana) {
	// Proporción = ancho / alto. Si son distintas, sobra espacio en algún lado.
	float proporcionVentana = static_cast<float>(tamanioVentana.x) / static_cast<float>(tamanioVentana.y);
	float proporcionMundo = ANCHO_MUNDO / ALTO_MUNDO;

	// El viewport es la parte de la ventana donde se dibuja el mundo, en fracciones (0 a 1).
	sf::FloatRect viewport({ 0.f, 0.f }, { 1.f, 1.f });

	if (proporcionVentana > proporcionMundo) {
		// Sobra a los costados
		viewport.size.x = proporcionMundo / proporcionVentana;
		viewport.position.x = (1 - viewport.size.x) / 2;   // lo que sobra, repartido en dos franjas
	}
	else {
		// Sobra arriba y abajo
		viewport.size.y = proporcionVentana / proporcionMundo;
		viewport.position.y = (1 - viewport.size.y) / 2;
	}
	vista.setViewport(viewport);
}

int main() {
	sf::RenderWindow ventana(sf::VideoMode({ static_cast<unsigned int>(ANCHO_MUNDO), static_cast<unsigned int>(ALTO_MUNDO) }), "Punto B"); // resolucion y titulo en la ventana
	//Si ponemos directamente {ANCHO_MUNDO, ALTO_MUNDO}, el compilador se queja porque 
	//pasar de float a entero puede perder decimales.
	//Hay que convertirlos explicitamente con static_cast<unsigned int>(...)

	ventana.setFramerateLimit(FPS); // setea los frames

	//Camara: muestra siempre el mundo entero (ANCHO_MUNDO x ALTO_MUNDO),
	//sin importar el tamaño de la ventana.
	sf::View vista(sf::FloatRect({ 0.f, 0.f }, { ANCHO_MUNDO, ALTO_MUNDO }));
	ventana.setView(vista); // todo lo que se dibuje pasa por esta cámara
	bool pantallaCompleta = false;

	EstadoJuego estadoActual = EstadoJuego::Jugando;


	int numeroNivel = 1;
	int totalNiveles = 5;
	Nivel nivel(numeroNivel);

	Jugador personaje(nivel.getRespawn());

	// Cronometro para medir cuánto dura cada frame.
	sf::Clock tiempo;

	float dt = 0.f;
	// dt (delta time): duración del frame anterior, en segundos (a 60 FPS, ~0.016).
	// Todas las velocidades del juego estan en "píxeles por segundo" y se multiplican
	// por dt, así el juego va igual de rápido en una PC lenta o en una rápida.

	while (ventana.isOpen()) {  // isOpen metodo de la clase RenderWindow que devuelve true o false si la ventana (el juego) está abierta
		while (const std::optional evento = ventana.pollEvent()) {
			if (evento->is<sf::Event::Closed>()) {
				ventana.close();
			}
			// La ventana cambió de tamaño: recalcular dónde se dibuja el mundo
			// para que no se deforme (si sobra espacio, queda en negro).
			if (const auto* cambio = evento->getIf<sf::Event::Resized>()) {
				ajustarVista(vista, cambio->size);
				ventana.setView(vista);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
				ventana.close();
			}

			// F11: alterna entre ventana y pantalla completa
			if (const auto* tecla = evento->getIf<sf::Event::KeyPressed>()) {
				if (tecla->code == sf::Keyboard::Key::F11) {
					pantallaCompleta = !pantallaCompleta; // // alterna: si estaba en pantalla completa sale, y si no, entra

					if (pantallaCompleta) {
						// Pantalla completa con la resolución del monitor
						ventana.create(sf::VideoMode::getDesktopMode(), "Punto B", sf::State::Fullscreen);
					}
					else {
						// Vuelve a la ventana normal, del tamaño del mundo
						ventana.create(sf::VideoMode({ static_cast<unsigned int>(ANCHO_MUNDO), static_cast<unsigned int>(ALTO_MUNDO) }), "Punto B");
					}

					// La ventana nueva arranca de cero: hay que volver a configurarla
					ventana.setFramerateLimit(FPS);
					ajustarVista(vista, ventana.getSize());
					ventana.setView(vista);
				}
			}
		}

		dt = tiempo.restart().asSeconds(); // cuanto pasó desde el frame anterior (y reinicia el cronómetro)

		switch (estadoActual) {
		case EstadoJuego::Menu:
			break;

		case EstadoJuego::Creditos:
			break;

		case EstadoJuego::Jugando:
			personaje.mover(dt);

			for (const Bloque& b : nivel.getBloques()) {
				personaje.chocarHorizontal(b.getHitbox());
			}

			personaje.actualizar(dt);

			for (const Bloque& b : nivel.getBloques()) {
				personaje.chocarVertical(b.getHitbox());
			}

			for (const Terreno& t : nivel.getPlataformas()) {
				personaje.aterrizarEn(t.getHitbox());
			}

			if (nivel.jugadorTocoPinche(personaje.getHitbox()) || nivel.jugadorCayoAlVacio(personaje.getHitbox())) {
				personaje.reiniciar(nivel.getRespawn());
			}

			if (nivel.jugadorTocoMeta(personaje.getHitbox())) {
				if (numeroNivel < totalNiveles) {
					numeroNivel++;
					nivel = Nivel(numeroNivel);
					personaje.reiniciar(nivel.getRespawn());
				}
				else {
					estadoActual = EstadoJuego::Victoria;
				}
			}
			break;

		case EstadoJuego::Victoria:
			std::cout << "Ganaste!" << std::endl;
			break;
		}

		ventana.clear();			// "Limpiar" la ventana borrando el fotograma anterior

		personaje.dibujar(ventana);	// "Dibujar" algo (recibe como argumento el objeto que querés dibujar, en este caso el rectángulo)
		nivel.dibujar(ventana);

		ventana.display();			// "Mostrar" en pantalla todo lo que se dibujó

	}

	return 0;
}