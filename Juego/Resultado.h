#pragma once
class Resultado {
private:
	float _tiempo;
	int _muertes;
public:
	Resultado();
	Resultado(float tiempo, int muertes);

	float getTiempo() const;
	int getMuertes() const;
};

