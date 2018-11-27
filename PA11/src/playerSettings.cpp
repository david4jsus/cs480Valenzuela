#include "playerSettings.h"

PlayerSettings::PlayerSettings()
{
  playerOneRemainingLives = 100;
  playerTwoRemainingLives = 100;
  invincibilityTime = 1000.0;
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
