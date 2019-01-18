#pragma once


//defines des index des points des angles de la boundingbox
//le sprite de la voiture est orienté avec l'avant qui pointe vers le bas

#define FRONT_LEFT 0
#define FRONT_RIGHT 1
#define BACK_RIGHT 2
#define BACK_LEFT 3

//defines definissant le couple d'index représentant le coté
//sert a savoir quel coté est entré en colision

#define FRONT std::make_pair(FRONT_LEFT,FRONT_RIGHT)
#define RIGHT std::make_pair(FRONT_RIGHT,BACK_RIGHT)
#define BACK std::make_pair(BACK_RIGHT,BACK_LEFT)
#define LEFT  std::make_pair(BACK_LEFT,FRONT_LEFT)