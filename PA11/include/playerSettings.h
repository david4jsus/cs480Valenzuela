#ifndef PLAYER_SETTINGS_H
#define PLAYER_SETTINGS_H

class PlayerSettings
{
  public:
		// constructor
      PlayerSettings();

      // stabbed
      bool Stabbed();

		// set each player's lives
		void getPlayersLives(int &playerOneLives, int &playerTwoLives);

		// get each player's lives
		void setPlayersLives(int playerOneLives, int playerTwoLives);

		// get invincibility time length
		float getInvincibilityTime();
    
	private:
    int playerOneRemainingLives;
    int playerTwoRemainingLives;
    float invincibilityTime;
    bool stabbed = true;
};

#endif // PLAYER_SETTINGS_H
