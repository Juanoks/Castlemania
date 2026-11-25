#include "Texturas.h"
#include <string>
#include <iostream>

Texturas::Texturas() {
	std::string archivos[CANTIDAD_TEXTURAS] = {
		"lanza.png", "pinche.png", "pilar_pinches.png", "boton_inactivo.png", "boton_activo.png",
		"bloque.png", "bloque_madera.png", "plataforma.png",
		"llave_azul_aura.png", "llave_amarilla_aura.png", "llave_verde_aura.png", "llave_roja_aura.png",
		"habilidad_lanza_aura.png", "habilidad_doble_salto_aura.png",
		"compuerta_cerrada.png", "compuerta_abierta.png", "personaje.png", "fondo_general.png", "fondo_puerta.png", "fondo_final.png",

		"puerta_cerrada.png", "puerta_completa.png", "puerta_abierta.png", 
		"puerta_capa_azul.png", "puerta_capa_amarilla.png", "puerta_capa_verde.png", "puerta_capa_roja.png",

		"cristal.png", "cristal_roto.png", "barrera_activa.png", "barrera_apagada.png",

		"puerta_llave_color.png", "puerta_llave_herrajes.png",

		"cartel_mover.png", "cartel_saltar.png", "cartel_lanza.png", "cartel_lanza_piso.png", "cartel_doble_salto.png"
	};
	for (int i = 0; i < CANTIDAD_TEXTURAS; i++) {
		if (!_texturas[i].loadFromFile("sprites/" + archivos[i])) {
			std::cout << "No se pudo cargar la textura: sprites/" << archivos[i] << std::endl;
		}
	}
	// se activa la repeticion de las 3 texturas de terreno. esto es para que no haga una imagen estirada
	_texturas[static_cast<int>(TipoTextura::Bloque)].setRepeated(true);
	_texturas[static_cast<int>(TipoTextura::BloqueMadera)].setRepeated(true);
	_texturas[static_cast<int>(TipoTextura::Plataforma)].setRepeated(true);
}

const sf::Texture& Texturas::get(TipoTextura tipo) const {
	return _texturas[static_cast<int>(tipo)]; // devuelve la posición del arreglo que corresponde al tipo
}
