#pragma once
#include "DrawableObject.h"
#include "UtilsCpp.h"
#include "Defines.h"
#include "Enums.h"
#include <iostream>
#include <string>

#define ROTATE_SPEED 100.0f
#define MOVE_SPEED 300.0f

class Car :
	public DrawableObject
{
public:
	Car();
	Car(int _carNumber, sf::Vector2f _startPos);
	~Car();
	virtual void update();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//mutateur pour la variable de selection
	void setSelectedCar(bool _state);

	//accesseur pour la variable de selection
	bool isSelectedCar();

	//mutateur pour la couleur de outline de la boundingBox (vert si c'est la voiture selectionnée , rouge dans le cas contraire)
	void setOutlineColor(sf::Color _color);

	//accesseur pour recupérer les position des coins selon un index
	sf::Vector2f getBoundingBoxCornerAtIndex(int _index);

	//acceseur pour recupérer le rectangle qui fait office de boundingBox
	sf::RectangleShape getBoundingBox();

	//accesseur pour recupérer la direction de conduite
	DrivingDirection getDrivingDirection();

	//acesseur pour recupérer les globalBounds
	sf::FloatRect getBounds();

	//accesseur pour savoir si la voiture est en collision
	bool getCollisionState();

	//mutateur pour changer l'état de collision
	void setCollisionState(bool _state);

	//accesseur pour recupérer l'ID de la voiture
	int getCarID();

	//fonction pour reinitialiser les points des coins
	void resetPoints();

	//fonction pour reset le timer de movement
	void resetMovementTimer();

private:
	//identifiant de la voiture (DEBUG)
	int ID;
	//sprite de la voiture
	sf::Sprite carSprite; 
	//rectangle qui représentera la boundingBox (DEBUG)
	sf::RectangleShape boundingBox;
	//tableau de vecteur de float pour stocker les positions originales des coins de la boundingBox
	sf::Vector2f originalBoundingBoxCorners[4];
	//tableau de vecteur de float pour stocker les positions transformées des coins de la boundingBox selon la rotation
	sf::Vector2f newBoundingBoxCorners[4];
	//tableau de cercles pour afficher les coins de la bounding box (DEBUG)
	sf::CircleShape boundingBoxCornersDisplay[4];
	//direction actuelle de la voiture en fonction de l'angle de la voiture
	sf::Vector2f actualDirection;
    //timer pour le calcul des vitesses de deplacement (correspond au temps ecoulé entre chaques frames)
	sf::Clock movementTimer;
	//booleen pour savoir si la voiture est celle selectionnée
	bool isSelected;
	//booleen pour savoir si la voiture est en colision
	bool isInCollision;
	//direction de conduite
	DrivingDirection drivingDirection;
};

