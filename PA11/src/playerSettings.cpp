#include "playerSettings.h"

PlayerSettings::PlayerSettings()
{
  playerOneRemainingLives = 3;
  playerTwoRemainingLives = 3;
  invincibilityTime = 5000.0;
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

bool PlayerSettings::GameSet()
{
   gameWon = true;  
   return gameWon;
}

bool PlayerSettings::GameReset()
{
  gameReset = true;
  return gameReset;
}

bool PlayerSettings::GameOver()
{
   return GameSet();
}

bool PlayerSettings::GameRestart()
{
   return GameReset();
}






