#include "playerSettings.h"

PlayerSettings::PlayerSettings()
{
  playerOneRemainingLives = 3;
  playerTwoRemainingLives = 3;
  invincibilityTime = 1000.0;
}

bool PlayerSettings::Stabbed()
{
   return stabbed;
}

void PlayerSettings::getPlayersLives(int &playerOneLives, int &playerTwoLives)
{
	playerOneLives = playerOneRemainingLives;
	playerTwoLives = playerTwoRemainingLives;
}

void PlayerSettings::setPlayersLives(int playerOneLives, int playerTwoLives)
{
	playerOneRemainingLives = playerOneLives;
	playerTwoRemainingLives = playerTwoLives;
}

float PlayerSettings::getInvincibilityTime()
{
	return invincibilityTime;
}
