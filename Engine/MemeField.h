#pragma once

#include "Vei2.h"
#include "Graphics.h"
#include "SpriteCodex.h"

class MemeField
{
private:
	class Tile {
	public:
		enum class State {
			Hidden, 
			Flagged, 
			Revealed
		};
	public:
		void SpawnMeme();
		bool HasMeme() const;
		void Draw(const Vei2& screenPos, bool isKvorked, Graphics& gfx) const;  //add const here
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
		void SetNeighborMemeCount(int memeCount);
		bool HasNoNeighborMemes ( ) const;
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int nNeighborMemes = -1;
	};
	// end of Tile member class

public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx) const;
	RectI GetRect() const;
	bool OnRevealClick(const Vei2& screenPos);
	void OnFlagClick(const Vei2& screenPos);
	int CountNeighborMemes(const Vei2& gridPos);  //const?
	const Vei2 GetTopLeft() const;
	int GetCountTotalTiles();
	bool AllTilesAreRevealed();
	void RevealTile ( const Vei2& gridPos );
	
private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
	Vei2 ScreenToGrid(const Vei2 screenPos);
	
private:
	static constexpr int width = 20;
	static constexpr int height = 15;
	Tile field[width * height];
	static constexpr int borderWidth = 8;
	static constexpr Color borderColor = Colors::Blue;

	const Vei2 topLeft = Vei2 { (Graphics::ScreenWidth - (width * SpriteCodex::tileSize)) / 2 ,  (Graphics::ScreenHeight - (height * SpriteCodex::tileSize)) / 2 };
	//chili initialized topLeft in the ctor, using a 'center' value that he passed in from game.cpp 
	//(He created GetRect() in Graphics, which returns a RectI for whole screen.  and then called GetRect().GetCenter()
	bool isKvorked = false;
	bool isMuyMacho = false;
	};

