#include "MemeField.h"
#include <assert.h>
#include <random>
#include "SpriteCodex.h"


MemeField::MemeField(int nMemes)
{
	assert(nMemes > 0 && nMemes < width * height);

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, height - 1);

	for (int i = 0; i < nMemes; i++) {
		Vei2 spawnPos;
		do {
			spawnPos = { xDist(rng), yDist(rng) };
		} 
		while	(TileAt(spawnPos).HasMeme());

		TileAt(spawnPos).SpawnMeme();
	}

	//reveal test
	for (int i = 0; i < 20; i++) {
		const Vei2 gridPos = { xDist(rng), yDist(rng) };
		if (!TileAt(gridPos).IsRevealed()) TileAt(gridPos).Reveal();  //don't I need to explicitly initialize the Vei2 that TileAt is calling?
	}
}

void MemeField::Draw(Graphics & gfx) const
{
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);
	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++) {
		//for ( ; gridPos.x < width; gridPos.x++) {  //note we don't have to start w initial val here, as gridPos.x will always be given in outer loop
		//fucking hell, I don't get this one! we had to change this loop and add gridPos.x = 0;
		// oh wait, it's cuz gridPos.x took the last value of the first loop through y, which was 20 (I think)
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++) {
			TileAt(gridPos).Draw(gridPos * SpriteCodex::tileSize, gfx);  //look into this. Tile Draw function sig wasn't const
		}
	}
}

RectI MemeField::GetRect() const
{
	return RectI(0, width * SpriteCodex::tileSize, 0, height * SpriteCodex::tileSize);
}

void MemeField::OnRevealClick(const Vei2 & screenPos)
{
	const Vei2 gridPos = ScreenToGrid(screenPos);
	assert(gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < height);
	Tile& tile = TileAt(gridPos);
	if (!tile.IsRevealed()) tile.Reveal();
}

MemeField::Tile& MemeField::TileAt(const Vei2 & gridPos)
{
	return field[gridPos.y * width + gridPos.x];
}

const MemeField::Tile& MemeField::TileAt(const Vei2 & gridPos) const
{
	return field[gridPos.y * width + gridPos.x];
}

Vei2 MemeField::ScreenToGrid(const Vei2 screenPos)
{
	return screenPos / SpriteCodex::tileSize;
	//return Vei2({ screenPos.x / SpriteCodex::tileSize, screenPos.y / SpriteCodex::tileSize });
	//this was what i'd have had to do before the Vei2 divide method
}

void MemeField::Tile::SpawnMeme()
{
	assert(!hasMeme);
	hasMeme = true;
}

bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}

void MemeField::Tile::Draw(const Vei2& screenPos, Graphics& gfx) const
{
	switch (state)
	{
	case State::Hidden:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case State::Flagged:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	case State::Revealed:
		if (!HasMeme()) {
			SpriteCodex::DrawTile0(screenPos, gfx);
		}
		else {
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}
		break;
	}
}

void MemeField::Tile::Reveal()
{
	assert(state == State::Hidden);
	state = State::Revealed;
}

bool MemeField::Tile::IsRevealed() const
{
	return (state == State::Revealed);
}
