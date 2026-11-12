#include <iostream>  // Ctrl+K, Ctrl+D para ordenar el codigo
#include <string>
#include <vector>
#include <SFML/Graphics.hpp> 
#include <SFML/Window.hpp>
#include <optional>
#include "ArchivoPartidas.h"
#include "Constantes.h"
#include "MenuPrincipal.h"
#include "EstadoJuego.h"
#include "Partida.h"
#include "Sonidos.h"
#include "Texturas.h"

// convierte segundos (con decimales) en texto con formato minutos:segundos
std::string ajustarTiempo(float segundos) {
	int totalSegundos = static_cast<int>(segundos); 
	int minutos = totalSegundos / 60;               
	int segundosRestantes = totalSegundos % 60;    

	std::string tiempo = std::to_string(minutos) + ":";
	if (segundosRestantes < 10) {
		tiempo += "0"; // para que quede "2:05" y no "2:5"
	}
	tiempo += std::to_string(segundosRestantes);
	return tiempo;
}

// ajusta la camara para que el mundo entre en una ventana de ese tamaño
// sin deformarse: lo que sobra queda en negro (letterbox).
void ajustarVista(sf::View& vista, sf::Vector2u tamanioVentana) {
	// proporción = ancho / alto. Si son distintas, sobra espacio en algún lado.
	float proporcionVentana = static_cast<float>(tamanioVentana.x) / static_cast<float>(tamanioVentana.y);
	float proporcionMundo = ANCHO_MUNDO / ALTO_MUNDO;

	// el viewport es la parte de la ventana donde se dibuja el mundo, en fracciones (0 a 1).
	sf::FloatRect viewport({ 0.f, 0.f }, { 1.f, 1.f });

	if (proporcionVentana > proporcionMundo) {
		// sobra a los costados
		viewport.size.x = proporcionMundo / proporcionVentana;
		viewport.position.x = (1 - viewport.size.x) / 2;   // lo que sobra, repartido en dos franjas
	}
	else {
		// sobra arriba y abajo
		viewport.size.y = proporcionVentana / proporcionMundo;
		viewport.position.y = (1 - viewport.size.y) / 2;
	}
	vista.setViewport(viewport);
}

int main() {
	sf::RenderWindow ventana(sf::VideoMode({ static_cast<unsigned int>(ANCHO_MUNDO), static_cast<unsigned int>(ALTO_MUNDO) }), "Punto B"); // resolucion y titulo en la ventana
	// si ponemos directamente {ANCHO_MUNDO, ALTO_MUNDO}, el compilador se queja porque 
	// pasar de float a entero puede perder decimales.
	// hay que convertirlos explicitamente con static_cast<unsigned int>(...)

	ventana.setFramerateLimit(FPS); //setea los frames

	// camara: muestra siempre el mundo entero (ANCHO_MUNDO x ALTO_MUNDO),
	// sin importar el tamaño de la ventana.
	sf::View vista(sf::FloatRect({ 0.f, 0.f }, { ANCHO_MUNDO, ALTO_MUNDO }));
	ventana.setView(vista); // todo lo que se dibuje pasa por esta cámara
	bool pantallaCompleta = false;

	// titulo y fuentes
	sf::Font fuente;
	if (!fuente.openFromFile("fuentes/PressStart2P-Regular.ttf")) {
		std::cout << "No se pudo cargar la fuente";
		return -1;
	}
	Sonidos sonidos; Texturas texturas;
	MenuPrincipal menuPrincipal(fuente);

	sf::Text instrucciones(fuente, "", 15);
	instrucciones.setString(
		"INSTRUCCIONES\n\n\n\n\n"
		"MOVIMIENTO\n\n\n"
		"- A / D o Flechas: Movimientos horizontales\n\n"
		"- W o Flecha arriba: Saltar\n\n\n\n"
		"ACCIONES\n\n\n"
		"- Click izquierdo: Lanzar\n\n\n\n"
		"OBJETIVO\n\n\n"
		"- Explorar los niveles, conseguir llaves\n"
		"y habilidades, y encontrar las salidas.\n\n\n\n\n\n"
		"Presiona ESC para volver al menu principal"
	);
	instrucciones.setPosition(sf::Vector2f(70.f, 30.f));
	instrucciones.setFillColor(sf::Color::White);

	sf::Text nombre(fuente, "Integrantes", 30);
	nombre.setPosition(sf::Vector2f(330.f, 200.f));
	sf::Text victoria(fuente, "", 30);
	victoria.setPosition(sf::Vector2f(250.f, 200.f));
	victoria.setFillColor(sf::Color::Magenta);
	sf::Text records(fuente, "", 15);
	records.setPosition(sf::Vector2f(100.f, 80.f));

	EstadoJuego estadoActual = EstadoJuego::Menu;

	Partida partida(sonidos, texturas); // el nivel, el jugador, el progreso de la partida en curso y sonidos
	ArchivoPartidas registro;

	// cronometro para medir cuánto dura cada frame.
	sf::Clock tiempo;

	float dt = 0.f;
	// dt (delta time): duración del frame anterior, en segundos (a 60 FPS, ~0.016).
	// todas las velocidades del juego estan en "píxeles por segundo" y se multiplican
	// por dt, así el juego va igual de rápido en una PC lenta o en una rápida.

	while (ventana.isOpen()) {  // isOpen metodo de la clase RenderWindow que devuelve true o false si la ventana (el juego) está abierta
		while (const std::optional evento = ventana.pollEvent()) {
			if (evento->is<sf::Event::Closed>()) {
				ventana.close();
			}
			// la ventana cambió de tamaño: recalcular dónde se dibuja el mundo
			// para que no se deforme (si sobra espacio, queda en negro).
			if (const auto* cambio = evento->getIf<sf::Event::Resized>()) {
				ajustarVista(vista, cambio->size);
				ventana.setView(vista);
			}

			if (const auto* clic = evento->getIf<sf::Event::MouseButtonPressed>()) {
				if (clic->button == sf::Mouse::Button::Left && estadoActual == EstadoJuego::Jugando) {
					// el mouse da pixeles de la ventana; se pasan a coordenadas del mundo (por la camara)
					sf::Vector2f destino = ventana.mapPixelToCoords(clic->position);
					partida.lanzarLanza(destino);
				}
			}

			// F11: alterna entre ventana y pantalla completa
			if (const auto* tecla = evento->getIf<sf::Event::KeyPressed>()) {
				if (tecla->code == sf::Keyboard::Key::F11) {
					pantallaCompleta = !pantallaCompleta; // // alterna: si estaba en pantalla completa sale, y si no, entra

					if (pantallaCompleta) {
						// pantalla completa con la resolución del monitor
						ventana.create(sf::VideoMode::getDesktopMode(), "Punto B", sf::State::Fullscreen);
					}
					else {
						// vuelve a la ventana normal, del tamaño del mundo
						ventana.create(sf::VideoMode({ static_cast<unsigned int>(ANCHO_MUNDO), static_cast<unsigned int>(ALTO_MUNDO) }), "Punto B");
					}

					// la ventana nueva arranca de cero: hay que volver a configurarla
					ventana.setFramerateLimit(FPS);
					ajustarVista(vista, ventana.getSize());
					ventana.setView(vista);
				}
				
				// selecciona las opciones del menu
				if (estadoActual == EstadoJuego::Menu) {
					if (tecla->code == sf::Keyboard::Key::Up) {
						menuPrincipal.subir();
						sonidos.reproducir(TipoSonido::MenuMover);
					}
					if (tecla->code == sf::Keyboard::Key::Down) {
						menuPrincipal.bajar();
						sonidos.reproducir(TipoSonido::MenuMover);
					}

					if (tecla->code == sf::Keyboard::Key::Enter) { 
						if (menuPrincipal.opcionElegida() == OpcionMenu::Jugar) {
							estadoActual = EstadoJuego::Jugando;

							if (partida.termino()) {
								partida.reiniciarPartida();
							}
						}
						if (menuPrincipal.opcionElegida() == OpcionMenu::Instrucciones) {
							estadoActual = EstadoJuego::Instrucciones;
						}

						if (menuPrincipal.opcionElegida() == OpcionMenu::Records) {
							estadoActual = EstadoJuego::Records;

							int recordsRegistro = registro.contarRegistros();
							std::vector<RegistroPartida> lista;
							std::string texto = "RECORDS\n\n";
							for (int i = 0; i < recordsRegistro; i++) {
								RegistroPartida registrosP = registro.leer(i);
								lista.push_back(registrosP);
							}

							for (int p = 0; p < static_cast<int>(lista.size()) - 1; p++) {
								for (int j = 0; j < static_cast<int>(lista.size()) - 1; j++) {        // recorre los pares de vecinos
									if (lista[j].getTiempo() > lista[j+1].getTiempo()) {  
										RegistroPartida aux = lista[j];     
										lista[j] = lista[j+1];                
										lista[j + 1] = aux;          
									}
								}
							}

							for (int i = 0; i < lista.size() && i < MAX_RECORDS; i++) {
								texto += "Tiempo: " + ajustarTiempo(lista[i].getTiempo()) +
									" | Muertes: " + std::to_string(lista[i].getMuertes()) + "\n";
							}


							// a la variable "texto" se le asigna RECORDS y cada Tiempo y Muerte en el registro
							records.setString(texto);
						}
						if (menuPrincipal.opcionElegida() == OpcionMenu::Creditos) {
							estadoActual = EstadoJuego::Creditos;
						}
						if (menuPrincipal.opcionElegida() == OpcionMenu::Salir) {
							ventana.close();
						}
						sonidos.reproducir(TipoSonido::MenuSeleccionar);
					}
				}

				if (tecla->code == sf::Keyboard::Key::Escape) {
					estadoActual = EstadoJuego::Menu;
					sonidos.reproducir(TipoSonido::MenuVolver);
				}
			}
		}

		dt = tiempo.restart().asSeconds(); // cuanto paso desde el frame anterior (y reinicia el cronómetro)

		switch (estadoActual) {
		case EstadoJuego::Menu:

			break;

		case EstadoJuego::Creditos:


			break;

		case EstadoJuego::Jugando:
			partida.actualizar(dt);

			if (partida.termino()) {
				RegistroPartida registroPartida(partida.getTiempoDeCompletado(),partida.getCantidadMuertes());
				registro.guardar(registroPartida);


				estadoActual = EstadoJuego::Victoria;
				victoria.setString("Gracias por jugar\n\nMuertes: " + std::to_string(partida.getCantidadMuertes()) + 
					"\n\nTiempo: " + ajustarTiempo(partida.getTiempoDeCompletado()));
			}
			break;

		case EstadoJuego::Victoria:		

			break;
		}

		ventana.clear();			// "Limpiar" la ventana borrando el fotograma anterior
		switch (estadoActual) {
		case EstadoJuego::Menu:
			menuPrincipal.dibujar(ventana);
			break;
		case EstadoJuego::Instrucciones:
			ventana.draw(instrucciones);
			break;
		case EstadoJuego::Records:
			ventana.draw(records);
			break;
		case EstadoJuego::Creditos:
			ventana.draw(nombre);
			break;
		case EstadoJuego::Jugando:
			partida.dibujar(ventana);
			break;
		case EstadoJuego::Victoria:
			partida.dibujar(ventana);
			ventana.draw(victoria);

			break;
		}

		ventana.display();			// "Mostrar" en pantalla todo lo que se dibujó

	}

	return 0;
}