#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include <list>
#include "Avatar.h"

using namespace sf;
class Game
{
private:
	//Propiedades de la ventana
	int alto;
	int ancho;
	RenderWindow *wnd;
	Color clearColor;

	//objetos de box2d
	b2World *phyWorld;
	SFMLRenderer *debugRender;

	//tiempo de frame
	float frameTime;
	int fps;

	//cuerpo de box2d 
	b2Body* controlBody;
	Avatar* controlBodyAvatar;
	
	sf::Texture texturaPelota;
		

public:

	//Constructores, destructores e inicializadores
	Game(int ancho, int alto,std::string titulo);
	void CreateEnemy(int x, int y);
	void InitPhysics();

	//Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();
	

	
};

