
#include "UtilsCpp.h"



#ifndef PI
#define PI 3.14159265358979323846264338327
#endif

#define COMPUTE_ERROR_TOLERANCE 0.2f
#define COMPUTE_MAGNITUDE_ERROR_TOLERANCE 1.0f



//////////////////////////////////////
/// Check if mouse is over a rectangle
/// \_window : The render window
/// \_rectangle : The rectangle to check if we are over
/// return EXIT_SUCCESS if we are over the sprite, else EXIT_FAILURE
//////////////////////////////////////
int IsOverRect(RenderWindow *_window, RectangleShape *_rectangle)
{
	FloatRect boundingBox = _rectangle->getGlobalBounds();

	Vector2i mousePos = Mouse::getPosition(*_window);

	if (mousePos.x > boundingBox.left && mousePos.x < boundingBox.left + boundingBox.width
		&& mousePos.y > boundingBox.top && mousePos.y < boundingBox.top + boundingBox.height)
	{
		return 1;
	}

	return 0;
}

//////////////////////////////////////
/// Check if 2 float rect are colliding as AABB (Axis Aligned Bounding Box)
/// \_rect1 : The first AABB
/// \_rect2 : The second AABB
/// return 1 if collision detected, else 0
//////////////////////////////////////
int CollisionAABBVsAABB(FloatRect _rect1, FloatRect _rect2)
{
	/// Check if we are out of the box
	if ((_rect2.left >= _rect1.left + _rect1.width)      /// AABB 2 is on the right
		|| (_rect2.left + _rect2.width <= _rect1.left) /// AABB 2 is on the left
		|| (_rect2.top >= _rect1.top + _rect1.height) /// AABB 2 is at the bottom
		|| (_rect2.top + _rect2.height <= _rect1.top))  /// AABB 2 is at the top
	{
		return 0;
	}
	else
		return 1;
}

//////////////////////////////////////
/// Check if a circle is colliding with a float rect are colliding
/// \_circlePosition : The circle position
/// \_circleBoundingBox : The circle bounding box
/// \_AABB : The rect bounding box
/// return 1 if collision detected, else 0
//////////////////////////////////////
int CollisionCircleVsAABB(Vector2f _circlePosition, FloatRect _circleBoundingBox, FloatRect _AABB)
{
	/// Check if the two AABB (Circle and AABB) are colliding
	if (CollisionAABBVsAABB(_AABB, _circleBoundingBox) == 0)
	{
		return 0;
	}

	//int projvertical = ProjectionSurSegment(_circlePosition.x, _circlePosition.y, _AABB.left, _AABB.top, _AABB.left, _AABB.top + _AABB.height);

	//if (projvertical == 1)
	//{
	//	return 1;   // cas E
	//}

	return 1;  // cas B   
}

bool collisionBetweenRotatedRectAndPoint(sf::Transform _carTransform,sf::RectangleShape _targetRect, sf::Vector2f _pointToCheck)
{

	sf::Transform tempTransform = _carTransform * _targetRect.getTransform();
	// tableau ou on stocke les positions des 4 coins du rectangle
	// ordre des points, on rentre les points par rapport a l'origine du rectangle (centre)
	// et comme si il était en position x:0 y:0
	//
	//                x:0
	//                 V
	//        0---------------- 1
	// y:0 >  |        X        |
	//        3-----------------2
	//
	sf::Vector2f orginalCornersPos[4];
	orginalCornersPos[0] = { -(_targetRect.getSize().x / 2),-(_targetRect.getSize().y / 2) };
	orginalCornersPos[1] = { _targetRect.getSize().x / 2,-(_targetRect.getSize().y / 2) };
	orginalCornersPos[2] = { _targetRect.getSize().x / 2,_targetRect.getSize().y / 2 };
	orginalCornersPos[3] = { -(_targetRect.getSize().x / 2),_targetRect.getSize().y / 2 };

	//tableau ou on stockera les nouveaux points calculés
	sf::Vector2f newCornersPos[4];
	for (int i = 0; i < 4; i++)
	{
		newCornersPos[i] = tempTransform.transformPoint(orginalCornersPos[i] + _targetRect.getOrigin());
	}


	//variable pour stocker l'aire du rectangle
	float rectArea = _targetRect.getSize().x *_targetRect.getSize().y;

	//tableau pour stocker l'aire des 4 triangles formés entre les 4 coins du rectangle et la position de la souris
	float triangleArea[4] = { 0,0,0,0 };

	//variable pour stocker la valeur du demi perimetre du triangle (nescessaire au calcul de l'aire)
	float triangleHalfP = 0;

	//variable pour stocker le total des aires des 4 triangles
	float trianglesAreaSum = 0;

	//tableau pour calculer la longueur de chaque côtés du triangle
	float hedgeLenght[3] = { 0,0,0 };

	//variable pour calculer la difference d'aire entre le total des 4 triangles et de l'aire du rectangle
	float triangleAreaDif = 0;

	//boucle pour le calcul du total des aires de chaques triangles formés par les coins du rectangle et la position de la souris
	for (int i = 0; i < 4; i++)
	{
		//variable pour stocker l'indice du coin suivant (si l'indice passe a 4 alors il s'agit du coin 0)
		int indexPlus = i + 1;
		if (indexPlus >= 4)
		{
			indexPlus = 0;
		}
		//on calcule la longueur de chaque côtés du triangle
		hedgeLenght[0] = Magnitude(newCornersPos[i] - newCornersPos[indexPlus]);
		hedgeLenght[1] = Magnitude(newCornersPos[i] - _pointToCheck);
		hedgeLenght[2] = Magnitude(newCornersPos[indexPlus] - _pointToCheck);

		//on calcule le demi perimetre du triangle
		triangleHalfP = (hedgeLenght[0] + hedgeLenght[1] + hedgeLenght[2]) / 2;
		triangleArea[i] = sqrtf(triangleHalfP * (triangleHalfP - hedgeLenght[0])* (triangleHalfP - hedgeLenght[1])* (triangleHalfP - hedgeLenght[2]));
		trianglesAreaSum += triangleArea[i];
	}

	//variable pour calculer la difference de taille , pour set un offset de verif
	triangleAreaDif = trianglesAreaSum - rectArea;
	//si la valeur est negative on la repasse en positif
	if (triangleAreaDif <= 0.0f)
	{
		triangleAreaDif *= -1.0f;
	}
	//si la difference entre le total des aires des 4 triangles et l'aire du rectangle est dans la tolérance d'écart de calcul, la souris est sur le rectangle
	if (triangleAreaDif <= COMPUTE_ERROR_TOLERANCE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool checkSideCollision(sf::Vector2f _pointA, sf::Vector2f _pointB, sf::Vector2f _pointToCheck)
{
	//calcul de la distance entre le point A et le point qui entre en collision
	float magnitudeA = Magnitude(_pointA - _pointToCheck);
	//calcul de la distance entre le point B et le point qui entre en collision
	float magnitudeB = Magnitude(_pointB - _pointToCheck);
	//calcul de la distance entre le point A et le point B
	float magnitudeAB = Magnitude(_pointA - _pointB);

	//calcul de la difference entre la somme de magnitude A et B et la longueur du coté de la voiture
	float magnitudeDifference = (magnitudeA + magnitudeB) - magnitudeAB;

	//si la valeur est negative on la repasse en positif
	if (magnitudeDifference < 0)
	{
		magnitudeDifference *= -1;
	}

	//si la difference est dans la marge d'erreur alors il y a collision avec le coté recherché
	if (magnitudeDifference <= COMPUTE_MAGNITUDE_ERROR_TOLERANCE)
	{
		return true;
	}
	//sinon il n'y a pas collision avec le coté recherché
	else
	{
		return false;
	}
}

int ProjectionSurSegment(float Cx, float Cy, float Ax, float Ay, float Bx, float By)
{
	float ACx = Cx - Ax;
	float ACy = Cy - Ay;
	float ABx = Bx - Ax;
	float ABy = By - Ay;
	float BCx = Cx - Bx;
	float BCy = Cy - By;

	float s1 = (ACx*ABx) + (ACy*ABy);
	float s2 = (BCx*ABx) + (BCy*ABy);

	printf("Product between %.2f & %.2f = %.2f\n", s1, s2, s1 * s2);

	if (s1*s2 > 0)
	{
		return 0;
	}

	return 1;
}

Vector2f GetDirectionFromAngleDegrees(float _angleDegrees)
{
	return GetDirectionFromAngleRadians(DegToRad(_angleDegrees));
}

Vector2f GetDirectionFromAngleRadians(double _angleRadians)
{
	//printf("Angle = %.2f - Radians = %.2f\n", _angleDegree, angleRadians);
	Vector2f direction;

	direction.x = cos(_angleRadians);
	direction.y = sin(_angleRadians);

	//printf("Direction : %.2f, %.2f\n", direction.x, direction.y);
	return direction;
}

float GetAngleRadiansBetweenPoints(Vector2f _pointA, Vector2f _pointB)
{
	return atan2(_pointB.y - _pointA.y, _pointB.x - _pointA.x);
}

float GetAngleRadiansBetweenPointAndMouse(Vector2f _pointA, Vector2i _pointB)
{
	return atan2(_pointB.y - _pointA.y, _pointB.x - _pointA.x);
}

float GetAngleRadiansFromVector(Vector2f _vector)
{
	return atan2(_vector.y, _vector.x);
}

float GetAngleDegreeFromVector(Vector2f _vector)
{
	float angleRadians = atan2(_vector.y, _vector.x);

	return RadToDeg(angleRadians);
}

Vector2f getPosOnCircleFromAngleDegrees(float _angle, float _circleRadius, Vector2f _circleCenterPos)
{
	Vector2f destPos = { 0,0 };
	destPos.x = _circleRadius * cos(DegToRad(_angle)) + _circleCenterPos.x;
	destPos.y = _circleRadius * sin(DegToRad(_angle)) + _circleCenterPos.y;

	return destPos;
}

double DegToRad(float _degrees)
{
	return (_degrees * PI) / 180;
}

float RadToDeg(double _radians)
{
	return _radians * (180 / PI);
}

//////////////////////////////////////
/// Normalize a vector 2 floats
/// \_vector : The vector to normalize
/// return a normalized vector
/////////////////////////////////////
Vector2f Normalize(Vector2f _vector)
{
	double length = Magnitude(_vector);
	//printf("Magnitude = %f\n", length);

	Vector2f newVector;
	newVector.x = _vector.x / length;
	newVector.y = _vector.y / length;
	//printf("Vector normalized, x = %f, y = %f\n", newVector.x, newVector.y);

	return newVector;
}

//////////////////////////////////////
/// Get the length of a vector
/// \_vector : The vector to get the length from
/// return the length
/////////////////////////////////////
float Magnitude(Vector2f _vector)
{
	return sqrt(_vector.x*_vector.x + _vector.y*_vector.y);
}

//////////////////////////////////////
/// Clamp a vector with a max length
/// \_vector : The vector to clamp
/// \_max : the max length
/// return the truncated vector
/////////////////////////////////////
Vector2f Truncate(Vector2f _vector, float _max)
{
	if (Magnitude(_vector) > _max)
	{
		_vector = Normalize(_vector);

		_vector.x *= _max;
		_vector.y *= _max;
	}
	return _vector;
}

