#pragma once
#ifndef _CHARACTERMARIO_H
#define _CHARACTERMARIO_H
#include "Character.h"


class CharacterMario : public Character
{
public:
	CharacterMario(SDL_Renderer* renderer, string imagePath, Vector2D start_position, LevelMap* map);
	~CharacterMario();

	virtual void Update(float deltaTime, SDL_Event e);
};


#endif //_CHARACTERMARIO_H