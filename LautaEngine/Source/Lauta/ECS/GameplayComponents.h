#pragma once
#include "Components.h"
#include "../Application.h"




/////////////////////////////////////////////////////////////////////////////////
struct HealthDice
{
	TransformComponent* trn;
	SpriteComponent* spr;
	ColliderComponent* cdr;
	Entity* ent;
	Group gid;
	int hp;

	HealthDice(Entity& HealthBar, Group groupID, int maxHP)
	{
		this->ent = &HealthBar;
		this->trn = &HealthBar.GetComponent<TransformComponent>();
		this->spr = &HealthBar.GetComponent<SpriteComponent>();
		this->cdr = &HealthBar.GetComponent<ColliderComponent>();
		this->gid = groupID;
		this->hp = maxHP;
	}

};

//int Health, Vector2Di positionOnScreen, std::string textureID,
//std::string textureName, Group groupID
class HealthComponent : public Component
{
private:

	std::vector<HealthDice> hpd;
	int m_MaxHealth;
	int m_currentHealth;

public:

	HealthComponent(int Health, Vector2Di positionOnScreen, std::string textureID,
		std::string textureName, Group groupID)
	{
		for (int i = 0; i < 3; i++)
		{
			Entity& HealthBar(manager.addEntity());
			HealthBar.addComponent<TransformComponent>(positionOnScreen.x,
				positionOnScreen.y + 64 * i);
			HealthBar.addComponent<SpriteComponent>(textureID,
				AssetManager::Assets->getSourceRect(textureName), 1,
				SDL_FLIP_NONE, 0, false);

			HealthBar.addComponent<ColliderComponent>("HealthBar");
			HealthBar.addGroup(groupID);

			this->hpd.push_back(HealthDice(HealthBar, groupID, Health));
		}
		m_MaxHealth = Health + 2;
		m_currentHealth = Health;
	
	}

	Entity* getHealthBarEntity(int number)
	{
		return hpd.at(number).ent;
	}

	SpriteComponent* getHealthBarSprite(int number)
	{
		return hpd.at(number).spr;
	}

	ColliderComponent* getHealthBarCollider(int number)
	{
		return hpd.at(number).cdr;
	}

	void dropAllHealthToZero()
	{
		int dice = 3;
		for (int i = 0; i < 3; i++)
		{
			hpd.at(i).hp = 0;
		}
	}

	void resetHealth()
	{
		int dice = 3;
		for (int i = 0; i < 3; i++)
		{
			hpd.at(i).hp = 4;
		}
	}


	bool checkLosingCondition()
	{
		int dice = 3;
		for (int i = 0; i < 3; i++)
		{
			//std::cout << getHealth(i) << std::endl;
			if (getHealth(i) <= 0)
			{
				dice--;
			}
		}
		if (dice == 0)
			return true;
		else
			return false;
	}

	int getHealthbarAmount()
	{
		int dice = 3;
		for (int i = 0; i < 3; i++)
		{
			if (getHealth(i) <= 0)
			{
				dice--;
			}
		}
		return dice;
	}

	int getHealth(int number)
	{
		return hpd.at(number).hp;
	}

	int getTotalHealth()
	{
		int totalHealth = 0;
		for (int i = 0; i < 3; i++)
		{
			totalHealth += getHealth(i);
		}
		return totalHealth;
	}

	void increaseHealth(int number)
	{
		hpd.at(number).hp++;
		if (hpd.at(number).hp > m_MaxHealth)
		{
			hpd.at(number).hp = m_MaxHealth;
		}
	}

	void decreaseHealth(int number)
	{
		hpd.at(number).hp--;
		if (hpd.at(number).hp <= 0)
		{
			hpd.at(number).hp = 0;
			//printf("\nHealth has dropped to zero!");
		}

	}

};
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
struct Cannons
{
	Entity* ent;
	TransformComponent* trn;
	SpriteComponent* spr;
	Group gid;

	Cannons(Entity& cannon, Group groupID)
	{
		this->ent = &cannon;
		this->trn = &cannon.GetComponent<TransformComponent>();
		this->spr = &cannon.GetComponent<SpriteComponent>();
		this->gid = groupID;
	}
};

class CannonComponent : public Component
{
public:

	std::vector<Cannons> cns;

	CannonComponent(std::string textureID,
		std::string textureName, Group groupID)
	{
		for (int i = 0; i < maxCannons; i++)
		{
			Entity& cannon(manager.addEntity());
			cannon.addComponent<TransformComponent>(880 + 60 * i, 1700);
			cannon.addComponent<SpriteComponent>(textureID,
				AssetManager::Assets->getSourceRect(textureName), 3,
				SDL_FLIP_NONE, 270, false);
			trn = &cannon.GetComponent<TransformComponent>();
			spr = &cannon.GetComponent<SpriteComponent>();
		
			this->cns.push_back(Cannons(cannon, groupID));
		}
	}

	void resetCannons()
	{
		for (int i = 0; i < getCannonCount(); i++)
		{
			DecreaseCannonAmount();

		}
	}

	Entity* getCannonEntity(int number)
	{
		return cns.at(number).ent;
	}

	void MoveCannons(int xPosNew)
	{
		for (int i = 0; i < getCannonCount(); i++)
		{
			cns.at(i).trn->position.x = xPosNew + 60 * i;
		}
	}

	SpriteComponent* getCannonSprite(int number)
	{
		return cns.at(number).spr;
	}

	void drawCannons()
	{
		for (int i = 0; i < getCannonCount(); i++)
		{
			cns.at(i).spr->draw();
		}
	}

	void IncreaseCannonAmount()
	{
		if(currentCannonCount < maxCannons)
			currentCannonCount++;
	}

	void DecreaseCannonAmount()
	{
		if (currentCannonCount > 0)
			currentCannonCount--;
	}

	int getCannonCount()
	{
		return currentCannonCount;
	}

private:

	int currentCannonCount = 0;
	const int maxCannons = 2;
	TransformComponent* trn;
	SpriteComponent* spr;
};
/////////////////////////////////////////////////////////////////////////////////
class GoldComponent : public Component
{
public:

	GoldComponent(Vector2Di positionOnScreen, std::string textureID,
		std::string textureName, Group groupID)
	{
		Entity& gold(manager.addEntity());

		gold.addComponent<TransformComponent>(positionOnScreen.x, positionOnScreen.y);
		gold.addComponent<SpriteComponent>(textureID,
			AssetManager::Assets->getSourceRect(textureName), 1,
			SDL_FLIP_NONE, 0, false);
		gold.addComponent<ColliderComponent>("gold");
		gold.addGroup(groupID);

		hasGold = false;

		trn = &gold.GetComponent<TransformComponent>();
		spr = &gold.GetComponent<SpriteComponent>();
		col = &gold.GetComponent<ColliderComponent>();
	}

	void resetGold()
	{
		hasGold = false;
		trn->position = Vector2D(SCREEN_WIDTH, 1600);
	}

	void movePosition(Vector2D newPos)
	{
		trn->position = newPos;
	}

	TransformComponent* getTransform()
	{
		return trn;
	}

	SpriteComponent* getSprite()
	{
		return spr;
	}

	ColliderComponent* getCollider()
	{
		return col;
	}

	void setGold(bool state)
	{
		hasGold = state;
	}

	bool getGoldState()
	{
		return hasGold;
	}

private:

	bool hasGold;
	TransformComponent* trn;
	SpriteComponent* spr;
	ColliderComponent* col;

};

inline int checkDamageFromDiceThrow(int number)
{
	switch (number)
	{
	case 1:
		return 0;
		break;
	case 2:
		return 0;
		break;
	case 3:
		return 0;
		break;
	case 4:
		return 1;
		break;
	case 5:
		return 1;
		break;
	case 6:
		return 2;
		break;
	default:
		return 0;
		break;
	}
}



class DiceComponent : public Component
{
public:

	DiceComponent() {};

	DiceComponent(int numberOfDice, Vector2Di tileTextureCoordinates)
	{
		diceAmount = numberOfDice;
		tileCoord = tileTextureCoordinates;
	}

	DiceComponent(int numberOfDice)
	{
		diceAmount = numberOfDice;
	}

	int getDiceAmount()
	{
		return diceAmount;
	}

	Vector2Di getTileTextureCoordinates()
	{
		return tileCoord;
	}

	int calculateDice(int cannonModifier, bool PVP, std::vector<int> results)
	{
		int result = 0;
		std::stringstream ss;
		if (!PVP)
		{
			result = -cannonModifier;
		}
		else 
		{
			result += cannonModifier;
		}

		for (int i = 0; i < results.size(); i++)
		{
			
			result += checkDamageFromDiceThrow(results[i]);

			ss << "Dice " << i + 1 << ": " << results[i] << "\n";

		}
		if (result < 0)
			result = 0;
		if (!PVP)
		{
			ss << "\nWith " << cannonModifier << " cannons\n" << "\nTotal Damage taken: " << result << "HP";
		}
		else {

			ss << "\nWith " << cannonModifier << " enemy cannons\n" << "\nTotal Damage taken: " << result << "HP";

		}


		UItext = ss.str();
		return result;

	}



	int throwDice(int cannonModifier, bool PVP)
	{
		int result = 0;
		//std::cout << "result: " << result <<std::endl;

		if (diceAmount != 0)
		{
			std::stringstream ss;
			
			if(!PVP)
			{
				result = -cannonModifier;
			}
			else {
				result += cannonModifier;
			}
			
			for (int i = 1; i < diceAmount + 1; i++)
			{
				int number = pickRandomNumber(1, 6);
				result += checkDamageFromDiceThrow(number);

				ss << "Dice " << i << ": " << number << "\n";

			}

			if (result < 0)
				result = 0;
			if (!PVP)
			{
				ss << "\nWith " << cannonModifier << " cannons\n" << "\nTotal Damage taken: " << result << "HP";
			}
			else {

				ss << "\nWith " << cannonModifier << " enemy cannons\n" << "\nTotal Damage taken: " << result << "HP";

			}
				

			UItext = ss.str();
			return result;
		}
		else
			return 0;
	}

	

	std::string getTextForUI()
	{
		return UItext;
	}
	
	bool revealed = false;

private:

	std::string UItext;
	int diceAmount;
	Vector2Di tileCoord;
};



class TurnComponent : public Component
{
public:

	TurnComponent(int turnNumber) 
	{
		tID = turnNumber;
		playingState = true;
		if (turnNumber == 1)
		{
			turn = true;
		}
		else {
			turn = false;
		}
	}

	int getTurnNumber()
	{
		return tID;
	}

	void playerLost()
	{
		playingState = false;
	}

	void togglePlayingState()
	{
		playingState = !playingState;
	}
	

	bool stillPlaying()
	{
		return playingState;
	}

	void changeTurnStateTo(bool state)
	{
		turn = state;
	}

	bool getTurnState()
	{
		return turn;
	}

	void changeTurnID(int turnNumber)
	{
		tID = turnNumber;
	}

private:

	int tID;
	bool turn;
	bool playingState;

};