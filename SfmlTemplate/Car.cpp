#include "Car.h"


Car::Car()
{
}

Car::Car(int _carNumber,sf::Vector2f _startPos)
{
	//set de la position du scenegraph en fonction du parametre envoyé au constructeur
	this->setPosition(_startPos);

	//set de l'id en fonction du numéro de la voiture
	ID = _carNumber;

	///on charge une texture selon le numéro de la voiture et l'applique au sprite de la voiture
	sf::Texture* texture = new sf::Texture();
	texture->loadFromFile("resources/car" + std::to_string(_carNumber) + ".png");
	carSprite.setTexture(*texture);
	carSprite.setOrigin (sf::Vector2f(carSprite.getGlobalBounds().width / 2, carSprite.getGlobalBounds().height / 2));
	///

	///on set le rectangle qui servira a visualiser la boundingBox
	boundingBox.setSize(sf::Vector2f(carSprite.getGlobalBounds().width, carSprite.getGlobalBounds().height));
	boundingBox.setFillColor(sf::Color::Transparent);
	boundingBox.setOrigin(sf::Vector2f(boundingBox.getGlobalBounds().width / 2, boundingBox.getGlobalBounds().height / 2));
	boundingBox.setOutlineColor(sf::Color::White);//on set la outline color en blanc elle sera colorée ensuite si la voiture est selectionnée ou non
	boundingBox.setOutlineThickness(2);
	///

	///set des positions originales des coins pour la transformation selon la rotation
	originalBoundingBoxCorners[FRONT_LEFT] = sf::Vector2f(-(carSprite.getGlobalBounds().width/2), carSprite.getGlobalBounds().height/2);
	originalBoundingBoxCorners[FRONT_RIGHT] = sf::Vector2f(carSprite.getGlobalBounds().width/2, carSprite.getGlobalBounds().height/2);
	originalBoundingBoxCorners[BACK_RIGHT] = sf::Vector2f(carSprite.getGlobalBounds().width/2, -(carSprite.getGlobalBounds().height / 2));
	originalBoundingBoxCorners[BACK_LEFT] = sf::Vector2f(-(carSprite.getGlobalBounds().width / 2), -(carSprite.getGlobalBounds().height / 2));
	///

	//set des nouveaux points calculés en fonction de la rotation de la voiture , à l'initialisation il sont égaux à ceux d'origine
	for (int i = 0; i < 4; i++)
	{
		newBoundingBoxCorners[i] = this->getTransform().transformPoint(originalBoundingBoxCorners[i]);
	}

	///set des cercles pour l'affichage des position des nouveaux points calculés , sert au debug
	for (int i = 0; i < 4; i++)
	{
		boundingBoxCornersDisplay[i].setRadius(4);
		boundingBoxCornersDisplay[i].setFillColor(sf::Color::Red);
		boundingBoxCornersDisplay[i].setOrigin(sf::Vector2f(4, 4));
		boundingBoxCornersDisplay[i].setOutlineColor(sf::Color::Black);
		boundingBoxCornersDisplay[i].setOutlineThickness(1.5f);
		boundingBoxCornersDisplay[i].setPosition (newBoundingBoxCorners[i]);
	}
	///

	//on initialise la direction actuelle avec l'angle de l'objet Car (+90 degrés car le sprite est orienté vers le bas)
	actualDirection = GetDirectionFromAngleDegrees(this->getRotation() + 90);

	//initialisation tu timer pour le deplacement
	movementTimer.restart();

	//par defaut toutes les voitures ne sont pas celles selectionnées
	isSelected = false;

	//par defaut toutes les voitures ne sont pas en collision
	isInCollision = false;

	//par defaut toutes les voitures sont dirigées vers l'avant
	drivingDirection = DrivingDirection::FORWARD;
}


Car::~Car()
{
}

void Car::update()
{
	///Deplacement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isInCollision)
	{
		drivingDirection = DrivingDirection::FORWARD;
		actualDirection = GetDirectionFromAngleDegrees(this->getRotation() + 90);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) )
		{
			this->rotate(ROTATE_SPEED * movementTimer.getElapsedTime().asSeconds());
			actualDirection = GetDirectionFromAngleDegrees(this->getRotation() + 90);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			this->rotate(-ROTATE_SPEED * movementTimer.getElapsedTime().asSeconds());
			actualDirection = GetDirectionFromAngleDegrees(this->getRotation() + 90);
		}
		this->setPosition(sf::Vector2f(this->getPosition().x + (actualDirection.x * MOVE_SPEED * movementTimer.getElapsedTime().asSeconds()), this->getPosition().y + (actualDirection.y * MOVE_SPEED * movementTimer.getElapsedTime().asSeconds())));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !isInCollision)
	{
		drivingDirection = DrivingDirection::BACKWARD;
		actualDirection = GetDirectionFromAngleDegrees(this->getRotation() + 90);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			this->rotate(-ROTATE_SPEED * movementTimer.getElapsedTime().asSeconds());
			actualDirection = GetDirectionFromAngleDegrees(this->getRotation() + 90);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			this->rotate(ROTATE_SPEED * movementTimer.getElapsedTime().asSeconds());
			actualDirection = GetDirectionFromAngleDegrees(this->getRotation() + 90);
		}
		this->setPosition(sf::Vector2f(this->getPosition().x - (actualDirection.x * MOVE_SPEED * movementTimer.getElapsedTime().asSeconds()), this->getPosition().y - (actualDirection.y * MOVE_SPEED * movementTimer.getElapsedTime().asSeconds())));
	}
	///

	//on transforme les nouveaux points des coins pour avoir leur position relative a la rotation
	//et on set la position des cercles Debug par rapport a ces points
	for (int i = 0; i < 4; i++)
	{
		newBoundingBoxCorners[i] = this->getTransform().transformPoint(originalBoundingBoxCorners[i]);
		boundingBoxCornersDisplay[i].setPosition(newBoundingBoxCorners[i]);
	}
	movementTimer.restart();
}

void Car::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(carSprite, this->getTransform());
	target.draw(boundingBox, this->getTransform());
	for (int i = 0; i < 4; i++)
	{
		target.draw(boundingBoxCornersDisplay[i]);
	}
}

void Car::setSelectedCar(bool _state)
{
	isSelected = _state;
}

bool Car::isSelectedCar()
{
	return isSelected;
}

void Car::setOutlineColor(sf::Color _color)
{
	boundingBox.setOutlineColor(_color);
}

sf::Vector2f Car::getBoundingBoxCornerAtIndex(int _index)
{
	return newBoundingBoxCorners[_index];
}

sf::RectangleShape Car::getBoundingBox()
{
	return boundingBox;
}

DrivingDirection Car::getDrivingDirection()
{
	return drivingDirection;
}

sf::FloatRect Car::getBounds()
{
	return this->getTransform().transformRect(boundingBox.getGlobalBounds());
}

bool Car::getCollisionState()
{
	return isInCollision;
}

void Car::setCollisionState(bool _state)
{
	isInCollision = _state;
}

int Car::getCarID()
{
	return ID;
}

void Car::resetPoints()
{
	for (int i = 0; i < 4; i++)
	{
		newBoundingBoxCorners[i] = this->getTransform().transformPoint(originalBoundingBoxCorners[i]);
		boundingBoxCornersDisplay[i].setPosition(newBoundingBoxCorners[i]);
	}
}

void Car::resetMovementTimer()
{
	movementTimer.restart();
}
