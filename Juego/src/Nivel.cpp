#include "Nivel.h"
#include "Constantes.h"
#include <string>
#include <iostream>

// Tamanio de la salida por defecto (un hueco angosto y alto, como la meta de antes)
sf::Vector2f salidaTamanio(30.f, 120.f);

// Mapa: cada fila es una sala y cada columna un lado de esa sala.
// izquierda, derecha, arriba, abajo | 0 = pared cerrada | -1 = fin del juego
// TERRENO_A_MANO (-2) = ese lado lo arma la sala a mano en cargarNivel (por ejemplo, con el hueco en otro lugar)
const int CONEXIONES[31][4] = {
	{0, 1, 0, TERRENO_A_MANO},  // 0: sala de prueba
	{0, TERRENO_A_MANO, 0, 0},  // 1: inicio.
	{TERRENO_A_MANO, 3, 0, 0},  
	{2, 0, 0, TERRENO_A_MANO},  
	{0, 0, TERRENO_A_MANO, TERRENO_A_MANO},  
	{0, TERRENO_A_MANO, TERRENO_A_MANO, 0},  // 5
	{TERRENO_A_MANO, TERRENO_A_MANO, 0, TERRENO_A_MANO},  
	{TERRENO_A_MANO, 8, 0, 0},  
	{7, -1, 0, 9},  // 8   sala de la puerta
	{0, 10, 8, 0},    
	{9, 0, 0, TERRENO_A_MANO}   // 10
};

Nivel::Nivel(int numeroNivel, Sonidos& sonidos, const Texturas& texturas)
	: _numeroNivel(numeroNivel),
	_sonidos(sonidos),
	_texturas(texturas),
	_fondo(_texturas.get(TipoTextura::FondoGeneral)) {

	cargarNivel(numeroNivel);
}

void Nivel::actualizar(float dt) {
	for (Obstaculo* o : _obstaculos) {
		o->actualizar(dt);
	}
	for (Mecanismo* m : _mecanismos) {
		m->actualizar(dt);
	}
}

void Nivel::cargarNivel(int numeroNivel, bool puertaAbierta) {
	// Si solo se hace clear(), los objetos creados con new quedan en memoria
	for (Obstaculo* o : _obstaculos) { delete o; }
	for (Mecanismo* m : _mecanismos) { delete m; } 

	_obstaculos.clear(); _plataformas.clear(); _bloques.clear(); _salidas.clear(); _coleccionables.clear(); _botones.clear();
	_mecanismos.clear();

	_numeroNivel = numeroNivel;
	_respawn = sf::Vector2f(50.f, Y_PISO - ALTO_JUGADOR); // parado sobre el piso

	TipoTextura tipoFondo = TipoTextura::FondoGeneral;
	// casi todas las salas usan el fondo general; algunas zonas especiales tienen el suyo
	if (numeroNivel == 8) {
		tipoFondo = TipoTextura::FondoPuerta;
	}
	else if (numeroNivel == 30) {
		tipoFondo = TipoTextura::FondoFinal;
	}
	// el true ajusta el recorte del sprite al tamanio de la imagen nueva
	_fondo.setTexture(_texturas.get(tipoFondo), true);
	sf::Vector2u tamanio = _texturas.get(tipoFondo).getSize();
	// setScale no recibe un tamanio sino un multiplicador: lo que quiero que mida / lo que mide la imagen.
	_fondo.setScale(sf::Vector2f(ANCHO_MUNDO / tamanio.x, ALTO_MUNDO / tamanio.y));


	paredIzquierda(CONEXIONES[numeroNivel][0]);
	paredDerecha(CONEXIONES[numeroNivel][1]);
	techo(CONEXIONES[numeroNivel][2]);
	piso(CONEXIONES[numeroNivel][3]);
	
	//                            posicion (x,y), tamanio (ancho,alto)
	if (numeroNivel == 0) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, ALTO_MUNDO-20.f), sf::Vector2f(400.f, 30.f), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(500.f, ALTO_MUNDO - 20.f), sf::Vector2f(400.f, 30.f), _texturas));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(300.f, 480.f), TipoColeccionable::LlaveAzul, _texturas));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(200.f, 480.f), TipoColeccionable::LlaveAmarilla, _texturas));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(500.f, 280.f), TipoColeccionable::Lanza, _texturas));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(100.f, 480.f), TipoColeccionable::DobleSalto, _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(350.f, 400.f), sf::Vector2f(200.f, 40.f), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(800.f, 300.f), sf::Vector2f(20.f, 100.f), _texturas, true));
		_obstaculos.push_back(new Pinche(sf::Vector2f(150.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(600.f, Y_PISO - 80.f), sf::Vector2f(40.f, 80.f), 600.f, 800.f, 150.f, _texturas));
		_plataformas.push_back(Plataforma(sf::Vector2f(200.f, 300.f), sf::Vector2f(100.f, 20.f), _texturas));
		_botones.push_back(Boton(sf::Vector2f(500.f, 200.f),1,_texturas));
		_mecanismos.push_back(new Compuerta(sf::Vector2f(500.f, 320.f), sf::Vector2f(20.f, 80.f), 1, _texturas));
	}


	if (numeroNivel == 1) {
		paredDerecha(2, 280.f); // hueco de y=280 a y=360

		_bloques.push_back(Bloque(sf::Vector2f(200.f, 470.f), sf::Vector2f(120.f, 50.f), _texturas));  
		_obstaculos.push_back(new Pinche(sf::Vector2f(326.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(380.f, Y_PISO - 37.5f), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(440.f, 420.f), sf::Vector2f(140.f, 100.f), _texturas)); 
		_obstaculos.push_back(new Pinche(sf::Vector2f(598.f, Y_PISO - 37.5f), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(660.f, 360.f), sf::Vector2f(280.f, 160.f), _texturas)); 
		_obstaculos.push_back(new Pinche(sf::Vector2f(780.f, 360.f - 37.5f), _texturas));              
	}
	else if (numeroNivel == 2) {
	
		paredIzquierda(1, 280.f); 
		_respawn = sf::Vector2f(50.f, 360.f - ALTO_JUGADOR); 

		_bloques.push_back(Bloque(sf::Vector2f(20.f, 360.f), sf::Vector2f(140.f, 160.f), _texturas));
		_plataformas.push_back(Plataforma(sf::Vector2f(230.f, 330.f), sf::Vector2f(90.f, 10.f), _texturas));
		_plataformas.push_back(Plataforma(sf::Vector2f(400.f, 290.f), sf::Vector2f(90.f, 10.f), _texturas));
		_plataformas.push_back(Plataforma(sf::Vector2f(570.f, 330.f), sf::Vector2f(90.f, 10.f), _texturas));
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(200.f, Y_PISO - 80.f), sf::Vector2f(40.f, 80.f), 180.f, 620.f, 150.f, _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(720.f, Y_PISO - 37.5f), _texturas)); 
	}
	else if (numeroNivel == 3) {
		
		piso(4, 800.f); // hueco del piso a la derecha (x de 800 a 880)

		_plataformas.push_back(Plataforma(sf::Vector2f(140.f, 450.f), sf::Vector2f(100.f, 10.f), _texturas));
		_plataformas.push_back(Plataforma(sf::Vector2f(290.f, 380.f), sf::Vector2f(100.f, 10.f), _texturas));
		_plataformas.push_back(Plataforma(sf::Vector2f(140.f, 310.f), sf::Vector2f(100.f, 10.f), _texturas));
		_plataformas.push_back(Plataforma(sf::Vector2f(290.f, 240.f), sf::Vector2f(130.f, 10.f), _texturas)); 
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(260.f, Y_PISO - 64.f), sf::Vector2f(40.f, 64.f), 260.f, 360.f, 100.f, _texturas)); // castiga caerse

		_bloques.push_back(Bloque(sf::Vector2f(420.f, 240.f), sf::Vector2f(80.f, 280.f), _texturas)); 

		_plataformas.push_back(Plataforma(sf::Vector2f(600.f, 360.f), sf::Vector2f(100.f, 10.f), _texturas)); 
		_obstaculos.push_back(new Pinche(sf::Vector2f(660.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(704.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(748.f, Y_PISO - 37.5f), _texturas)); 
	}
	else if (numeroNivel == 4) {
		
		techo(3, 800.f); 
		piso(5, 100.f);  
		_respawn = sf::Vector2f(828.f, 150.f - ALTO_JUGADOR); 

		_bloques.push_back(Bloque(sf::Vector2f(360.f, 150.f), sf::Vector2f(580.f, 30.f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(640.f, 150.f - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(480.f, 150.f - 37.5f), _texturas));

		_bloques.push_back(Bloque(sf::Vector2f(20.f, 330.f), sf::Vector2f(620.f, 30.f), _texturas)); 
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(380.f, 330.f - 56.f), sf::Vector2f(40.f, 56.f), 380.f, 560.f, 140.f, _texturas));
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(380.f, 270.f), sf::Vector2f(20.f, 60.f), 180.f, 270.f, 100.f, _texturas, true)); // pilar vertical

		_obstaculos.push_back(new Pinche(sf::Vector2f(520.f, Y_PISO - 37.5f), _texturas)); // piso
		_obstaculos.push_back(new Pinche(sf::Vector2f(564.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(330.f, Y_PISO - 37.5f), _texturas));
	}
	else if (numeroNivel == 5) {
		// SALA 5: se consigue la LANZA 
		techo(4, 100.f);        
		paredDerecha(6, 120.f); 

		_bloques.push_back(Bloque(sf::Vector2f(300.f, 470.f), sf::Vector2f(80.f, 50.f), _texturas)); // pedestal
		_coleccionables.push_back(Coleccionable(sf::Vector2f(320.f, 430.f), TipoColeccionable::Lanza, _texturas));

		_bloques.push_back(Bloque(sf::Vector2f(520.f, 400.f), sf::Vector2f(40.f, 120.f), _texturas, true)); 
		_bloques.push_back(Bloque(sf::Vector2f(560.f, 400.f), sf::Vector2f(380.f, 120.f), _texturas));      
		_bloques.push_back(Bloque(sf::Vector2f(720.f, 260.f), sf::Vector2f(40.f, 140.f), _texturas, true)); 
		_bloques.push_back(Bloque(sf::Vector2f(760.f, 260.f), sf::Vector2f(180.f, 140.f), _texturas));      
		_bloques.push_back(Bloque(sf::Vector2f(860.f, 200.f), sf::Vector2f(80.f, 60.f), _texturas));       
	}
	else if (numeroNivel == 6) {

		paredIzquierda(5, 120.f); // misma altura que la salida de la sala 5
		paredDerecha(7, 60.f);    
		_respawn = sf::Vector2f(50.f, 200.f - ALTO_JUGADOR);

		_bloques.push_back(Bloque(sf::Vector2f(20.f, 200.f), sf::Vector2f(160.f, 340.f), _texturas));       
		_bloques.push_back(Bloque(sf::Vector2f(330.f, 150.f), sf::Vector2f(40.f, 130.f), _texturas, true)); 
		_bloques.push_back(Bloque(sf::Vector2f(520.f, 220.f), sf::Vector2f(120.f, 30.f), _texturas));        
		_bloques.push_back(Bloque(sf::Vector2f(820.f, 140.f), sf::Vector2f(40.f, 200.f), _texturas, true)); 
		_bloques.push_back(Bloque(sf::Vector2f(860.f, 140.f), sf::Vector2f(80.f, 400.f), _texturas));        
	}
	else if (numeroNivel == 7) {
		// SALA 7: boton y compuerta. 
		paredIzquierda(6, 60.f);
		_respawn = sf::Vector2f(50.f, 140.f - ALTO_JUGADOR);

		_bloques.push_back(Bloque(sf::Vector2f(20.f, 140.f), sf::Vector2f(140.f, 380.f), _texturas)); 
		_bloques.push_back(Bloque(sf::Vector2f(230.f, 260.f), sf::Vector2f(200.f, 30.f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(308.f, 260.f - 37.5f), _texturas));
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(180.f, Y_PISO - 64.f), sf::Vector2f(40.f, 64.f), 180.f, 360.f, 150.f, _texturas)); // cuida el piso de abajo del estante

		_obstaculos.push_back(new Pinche(sf::Vector2f(496.f, Y_PISO - 37.5f), _texturas)); 
		_obstaculos.push_back(new Pinche(sf::Vector2f(540.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(584.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(628.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(672.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(500.f, 200.f), sf::Vector2f(40.f, 80.f), 200.f, 400.f, 150.f, _texturas, true)); // pilar vertical
		_botones.push_back(Boton(sf::Vector2f(604.f, 440.f), 1, _texturas)); 

		_bloques.push_back(Bloque(sf::Vector2f(860.f, 20.f), sf::Vector2f(20.f, 420.f), _texturas)); 
		_mecanismos.push_back(new Compuerta(sf::Vector2f(860.f, Y_PISO - 80.f), sf::Vector2f(20.f, 80.f), 1, _texturas));
	}
	else if (numeroNivel == 8) {
		if (!puertaAbierta) {
			_bloques.push_back(Bloque(sf::Vector2f
			(ANCHO_MUNDO - GROSOR_PARED * 2, Y_PISO - TAMANIO_HUECO), sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO), _texturas));
		}
	}
	else if (numeroNivel == 9) {	
		_respawn = sf::Vector2f(458.f, 130.f - ALTO_JUGADOR); 

		_bloques.push_back(Bloque(sf::Vector2f(380.f, 130.f), sf::Vector2f(180.f, 100.f), _texturas, true)); 
		_bloques.push_back(Bloque(sf::Vector2f(20.f, 250.f), sf::Vector2f(220.f, 270.f), _texturas));       
		_bloques.push_back(Bloque(sf::Vector2f(240.f, 250.f), sf::Vector2f(40.f, 150.f), _texturas, true)); 
		_bloques.push_back(Bloque(sf::Vector2f(400.f, 390.f), sf::Vector2f(200.f, 130.f), _texturas, true));
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(700.f, Y_PISO - 56.f), sf::Vector2f(40.f, 56.f), 660.f, 820.f, 130.f, _texturas)); 
	}
	else if (numeroNivel == 10) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, Y_PISO), sf::Vector2f(560.f, GROSOR_PARED), _texturas)); 

		_plataformas.push_back(Plataforma(sf::Vector2f(160.f, 450.f), sf::Vector2f(90.f, 10.f), _texturas));
		_plataformas.push_back(Plataforma(sf::Vector2f(300.f, 380.f), sf::Vector2f(90.f, 10.f), _texturas));
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(140.f, Y_PISO - 56.f), sf::Vector2f(40.f, 56.f), 140.f, 330.f, 120.f, _texturas));

		_bloques.push_back(Bloque(sf::Vector2f(400.f, 240.f), sf::Vector2f(40.f, 160.f), _texturas, true)); 
		_bloques.push_back(Bloque(sf::Vector2f(440.f, 240.f), sf::Vector2f(120.f, 300.f), _texturas));     
		_botones.push_back(Boton(sf::Vector2f(GROSOR_PARED, 120.f), 1, _texturas));                         
		_plataformas.push_back(Plataforma(sf::Vector2f(620.f, 200.f), sf::Vector2f(80.f, 10.f), _texturas));  
		_bloques.push_back(Bloque(sf::Vector2f(760.f, 170.f), sf::Vector2f(180.f, 30.f), _texturas));         
		_bloques.push_back(Bloque(sf::Vector2f(800.f, 20.f), sf::Vector2f(20.f, 70.f), _texturas));           
		_mecanismos.push_back(new Compuerta(sf::Vector2f(800.f, 90.f), sf::Vector2f(20.f, 80.f), 1, _texturas));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(870.f, 134.f), TipoColeccionable::LlaveAzul, _texturas));
	}
	//proximos niveles

}

void Nivel::dibujar(sf::RenderWindow& ventana) {
	ventana.draw(_fondo);

	for (Obstaculo* o : _obstaculos) {
		o->dibujar(ventana);
	}
	for (Mecanismo* m : _mecanismos) {
		m->dibujar(ventana);
	}
	for (Plataforma& p : _plataformas) {
		p.dibujar(ventana);
	}
	for (Bloque& b : _bloques) {
		b.dibujar(ventana);
	}
	for (Salida& s : _salidas) {
		s.dibujar(ventana);
	}
	for (Coleccionable& c : _coleccionables) {
		c.dibujar(ventana);
	}
	for (Boton& b : _botones) {
		b.dibujar(ventana);
	}

}

bool Nivel::jugadorTocoObstaculo(sf::FloatRect hitboxJugador) const {
	for (Obstaculo* o : _obstaculos) { // pregunta a cada obstaculo si toca al jugador; cada uno usa su propia hitbox
		if (o->tocaAlJugador(hitboxJugador)) {
			return true;
		}
	}
	return false;
}

bool Nivel::jugadorCayoAlVacio(sf::FloatRect hitboxJugador) const {
	return hitboxJugador.position.y > ALTO_MUNDO; // si la posicion (y) del jugador es mayor a la del borde de abajo de la 
}											      // pantalla; devuelve true

// Devuelve un puntero a la salida que toca el jugador, o nullptr si no toca ninguna (igual que fopen).
const Salida* Nivel::salidaTocada(sf::FloatRect hitboxJugador) const {
	for (const Salida& s : _salidas) {
		if (hitboxJugador.findIntersection(s.getHitbox())) {
			return &s; // & = la direccion de esta salida (un puntero a ella)
		}
	}
	return nullptr;
}

const Coleccionable* Nivel::coleccionableTocado(sf::FloatRect hitboxJugador) const {
	for (const Coleccionable& c : _coleccionables) {
		if (hitboxJugador.findIntersection(c.getHitbox())) {
			return &c; // retorna &c = la direccion de este coleccionable (un puntero a el)
		}
	}
	return nullptr;
}

void Nivel::quitarColeccionable(TipoColeccionable tipo) { 
	// Saca de la sala el coleccionable de ese tipo (cada uno es unico en todo el mapa).
	for (int i = 0; i < static_cast<int>(_coleccionables.size()); i++) {
		if (_coleccionables[i].getTipo() == tipo) {
			_coleccionables.erase(_coleccionables.begin() + i);
			return;
		}
	}
}

const std::vector<Plataforma>& Nivel::getPlataformas() const {
	return _plataformas;
}

const std::vector<Bloque>& Nivel::getBloques() const {
	return _bloques;
}

const std::vector<Boton>& Nivel::getBotones() const {
	return _botones;
}

void Nivel::actualizarBotonesYMecanismos(const std::vector<Lanza>& lanzas) {
	// un boton esta activo si hay una lanza clavada tocandolo.
	// se recalcula en cada frame: si la lanza vuelve al jugador, el boton se apaga solo.
	for (Boton& b : _botones) {
		bool tocado = false; // se empieza suponiendo que ninguna lanza lo toca
		for (const Lanza& l : lanzas) {
			if (l.estaClavada() && l.getHitbox().findIntersection(b.getHitbox())) { // solo cuenta una lanza clavada 
				tocado = true;
			}
		}
		// sonido solo cuando el boton CAMBIA de estado (si no, sonaria en todos los frames)
		if (tocado != b.estaActivo()) {
			_sonidos.reproducir(tocado ? TipoSonido::BotonActivar : TipoSonido::BotonDesactivar);
		}
		b.setActivo(tocado);
	}
	// un mecanismo esta activo si alguno de los botones de su canal esta activo.
	// mismo patron que los botones: se empieza en false, se busca si hay alguno, y al final se avisa.
	for (Mecanismo* m : _mecanismos) {
		bool hayBotonActivo = false;
		for (Boton& b : _botones) {
			if (b.estaActivo() && b.getCanal() == m->getCanal()) {
				hayBotonActivo = true;
			}
		}
		m->setActivo(hayBotonActivo);
	}
}

sf::Vector2f Nivel::getRespawn() const {
	return _respawn;
}

const std::vector<Mecanismo*>& Nivel::getMecanismos() const {return _mecanismos;}

Nivel::~Nivel() {
	for (Obstaculo* o : _obstaculos) { delete o; }
	for (Mecanismo* m : _mecanismos) { delete m; }
}

// pared derecha de la sala. destino 0 = pared cerrada.
void Nivel::paredDerecha(int destino, float inicioHueco) {
	float posicionX = ANCHO_MUNDO - GROSOR_PARED; // la pared va pegada al borde derecho del mundo
	float finHueco = inicioHueco + TAMANIO_HUECO; // donde termina el hueco (y empieza el bloque de abajo)
	if (destino == TERRENO_A_MANO) {return;}
	if (destino == 0) {
		// sin salida: un solo bloque de arriba a abajo
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, 0.f), sf::Vector2f(GROSOR_PARED, ALTO_MUNDO), _texturas));
	}
	else {
		// bloque de arriba del hueco (de y=0 hasta inicioHueco)
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, 0.f), sf::Vector2f(GROSOR_PARED, inicioHueco), _texturas));
		// bloque de abajo del hueco (de finHueco hasta el fondo)
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, finHueco), sf::Vector2f(GROSOR_PARED, ALTO_MUNDO - finHueco), _texturas));
		// la salida ocupa el hueco
		_salidas.push_back(Salida
		(sf::Vector2f(posicionX, inicioHueco), // donde está el hueco
			sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO), // tamaño del hueco
			destino, // a que sala lleva
			sf::Vector2f(GROSOR_PARED + 10.f, finHueco - ALTO_JUGADOR))); // donde aparece el jugador en esa sala
	}
}

void Nivel::paredIzquierda(int destino, float inicioHueco) {
	float posicionX = 0.f;
	float finHueco = inicioHueco + TAMANIO_HUECO;
	if (destino == TERRENO_A_MANO) { return; }
	if (destino == 0) {
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, 0.f), sf::Vector2f(GROSOR_PARED, ALTO_MUNDO), _texturas));
	}
	else {
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, 0.f), sf::Vector2f(GROSOR_PARED, inicioHueco), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, finHueco), sf::Vector2f(GROSOR_PARED, ALTO_MUNDO - finHueco), _texturas));

		_salidas.push_back(Salida
		(sf::Vector2f(posicionX, inicioHueco), sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO),
			destino, sf::Vector2f(ANCHO_MUNDO - GROSOR_PARED - ANCHO_JUGADOR - 10.f, finHueco - ALTO_JUGADOR)));
	}
}

void Nivel::techo(int destino, float inicioHueco) {
	float finHueco = inicioHueco + TAMANIO_HUECO;
	if (destino == TERRENO_A_MANO) { return; }
	if (destino == 0) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, 0.f), sf::Vector2f(ANCHO_MUNDO, GROSOR_PARED), _texturas));
	}
	else {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, 0.f), sf::Vector2f(inicioHueco, GROSOR_PARED), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(finHueco, 0.f), sf::Vector2f(ANCHO_MUNDO - finHueco, GROSOR_PARED), _texturas));

		_salidas.push_back(Salida(sf::Vector2f(inicioHueco, 0.f),
			sf::Vector2f(TAMANIO_HUECO, GROSOR_PARED), destino, sf::Vector2f(finHueco + 10.f, Y_PISO - ALTO_JUGADOR)));
	}
}

void Nivel::piso(int destino, float inicioHueco) {
	float posicionY = Y_PISO;
	float alto = ALTO_MUNDO - posicionY;
	float finHueco = inicioHueco + TAMANIO_HUECO;
	if (destino == TERRENO_A_MANO) { return; }
	if (destino == 0) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, posicionY), sf::Vector2f(ANCHO_MUNDO, alto), _texturas));
	}
	else {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, posicionY), sf::Vector2f(inicioHueco, alto), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(finHueco, posicionY), sf::Vector2f(ANCHO_MUNDO - finHueco, alto), _texturas));

		_salidas.push_back(Salida(sf::Vector2f(inicioHueco, posicionY),
			sf::Vector2f(TAMANIO_HUECO, alto), destino,
			sf::Vector2f(inicioHueco + (TAMANIO_HUECO - ANCHO_JUGADOR) / 2, GROSOR_PARED + 5.f)));
	}
}