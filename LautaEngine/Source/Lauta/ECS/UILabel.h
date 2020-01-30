#pragma once
#include "ECS.h"
#include "../Globals.h"
#include "../AssetManager.h"
#include "../TextureManager.h"
#include "../LautaMath.h"

#include "SDL.h"
#include "SDL_ttf.h"
#include <string>

//Vector2Di position of the text on the UI, 
//std::string text to be written onto UI, 
//std::string font of choice, 
//std::string textID
//Vector2Di textBoxSize
//SDL_Color& color of the text, 
class UILabel : public Component
{
public:

	//contrcutor for the UI label. Text position will be fixed onto the given position variables.
	UILabel(Vector2Di pos, std::string text, std::string font, std::string textID, Vector2Di textBoxSize, SDL_Color& color)
		:  textColor(color)
	{
		position.x = pos.x;
		position.y = pos.y;

		textBoxBorders = textBoxSize;

		textFont = font;

		Screenplay.emplace(textID, setLabelText(text, font));
		Script.emplace(textID, text);
		positionOfText.emplace(textID, position);
	}

	UILabel() = default;

	//erase everything written for the UI Label
	~UILabel() 
	{
		Screenplay.clear();
		entity->destroy();
	}

	//Adding extra text for the UI Label
	//textID is used to retrieve text using the given ID
	//text is naturally the written text on the UI
	//font is the previously given font identifier for the asset manager
	void AddTextToScreenplay(std::string textID, std::string text, std::string font)
	{
		Screenplay.emplace(textID, setLabelText(text, font));
		Script.emplace(textID, text);
		positionOfText.emplace(textID, position);
	}

	void AddTextToScreenplayWithNewPosition(std::string textID, std::string text, std::string font, Vector2Di pos)
	{
		if (!textExistWithID(textID))
		{
			position.x = pos.x;
			position.y = pos.y;
			Screenplay.emplace(textID, setLabelText(text, font));
			Script.emplace(textID, text);
			positionOfText.emplace(textID, position);
		}
	}

	void replaceTextWithID(std::string textID, std::string newText)
	{
		RemoveTextFromScreenPlay(textID);

		AddTextToScreenplay(textID, newText, textFont);
	}

	bool textExistWithID(std::string textID)
	{
		std::map<std::string, std::string>::iterator it;

		it = Script.find(textID);
		if (it != Script.end())
		{
			return true;
		}

		return false;
	}

	void RemoveTextFromScreenPlay(std::string textID)
	{
		Screenplay.erase(textID);
		Script.erase(textID);
		positionOfText.erase(textID);

		//SDL_DestroyTexture(getLine(textID));
	}

	//move the location of the UI text to a different coordinate
	void setNewPositionForText(Vector2Di newPosition)
	{
		position.x = newPosition.x;
		position.y = newPosition.y;
	}

	void setPositionOfTextWithID(std::string textID, Vector2Di newPosition)
	{
		SDL_Rect pos = { newPosition.x, newPosition.y, getPositionOfText(textID).w , getPositionOfText(textID).h };
		positionOfText.erase(textID);

		positionOfText.emplace(textID, pos);
	}

	SDL_Texture* setLabelText(std::string text, std::string font)
	{
		SDL_Surface* surf = TTF_RenderText_Blended_Wrapped(AssetManager::Assets->GetFont(font), 
			text.c_str(), textColor, textBoxBorders.x);
		SDL_Texture* labelTexture = SDL_CreateTextureFromSurface(Globals::Global_Renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);

		return labelTexture;
	}

	//retrieve a line added to the Screenplay using the given text ID for that line of text
	SDL_Texture* getLine(std::string textID)
	{
		return Screenplay[textID];
	}

	SDL_Rect getPositionOfText(std::string tID)
	{
		return positionOfText[tID];
	}

	void setTextBoxSize(std::string textID)
	{
		SDL_QueryTexture(getLine(textID), nullptr, nullptr, &position.w, &position.h);
	}

	//print out the text with the given text
	void drawTextWithID(std::string textID)
	{
		setTextBoxSize(textID);;

		SDL_RenderCopy(Globals::Global_Renderer, getLine(textID), nullptr, &getPositionOfText(textID));
	}

private:

	//Location of the written text on the UI
	SDL_Rect position;
	Vector2Di textBoxBorders;
	std::string textFont;
	//Color of text specified in the constructor
	SDL_Color textColor;
	//The Screenplay contains all text written for the given UI Label. 
	//Specific text can be retrieved using text ID


	
	//textID, written text
	std::map<std::string, std::string> Script;
	std::map<std::string, SDL_Texture*> Screenplay;
	std::map<std::string, SDL_Rect> positionOfText;
	

};

