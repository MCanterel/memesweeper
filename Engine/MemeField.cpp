#include "MemeField.h"
#include <assert.h>
#include <random>
#include "SpriteCodex.h"



MemeField::MemeField ( int nMemes )
{
	assert ( nMemes > 0 && nMemes < width * height );

	std::random_device rd;
	std::mt19937 rng ( rd ( ) );
	std::uniform_int_distribution<int> xDist ( 0, width - 1 );
	std::uniform_int_distribution<int> yDist ( 0, height - 1 );

	for ( int i = 0; i < nMemes; i++ ) {
		Vei2 spawnPos;
		do {
			spawnPos = { xDist ( rng ), yDist ( rng ) };
		} while ( TileAt ( spawnPos ).HasMeme ( ) );

		TileAt ( spawnPos ).SpawnMeme ( );
	}

	for ( Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++ ) {
		for ( gridPos.x = 0; gridPos.x < width; gridPos.x++ ) {
			TileAt ( gridPos ).SetNeighborMemeCount ( CountNeighborMemes ( gridPos ) );
		}
	}

	//reveal test
	//for (int i = 0; i < 20; i++) {
	//	const Vei2 gridPos = { xDist(rng), yDist(rng) };
	//	if (!TileAt(gridPos).IsRevealed()) TileAt(gridPos).Reveal();  //don't I need to explicitly initialize the Vei2 that TileAt is calling?
	//}
}

void MemeField::Draw ( Graphics & gfx ) const
{
	gfx.DrawRect ( GetRect ( ).GetExpanded ( borderWidth ), borderColor );
	gfx.DrawRect ( GetRect ( ), SpriteCodex::baseColor );
	for ( Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++ ) {
		//for ( ; gridPos.x < width; gridPos.x++) {  //note we don't have to start w initial val here, as gridPos.x will always be given in outer loop
		//fucking hell, I don't get this one! we had to change this loop and add gridPos.x = 0;
		// oh wait, it's cuz gridPos.x took the last value of the first loop through y, which was 20 (I think)
		for ( gridPos.x = 0; gridPos.x < width; gridPos.x++ ) {
			TileAt ( gridPos ).Draw ( topLeft + ( gridPos * SpriteCodex::tileSize ), isKvorked, gfx );  //look into this. Tile Draw function sig wasn't const
		}
	}
}

RectI MemeField::GetRect ( ) const
{
	return ( RectI ( topLeft, width * SpriteCodex::tileSize, height * SpriteCodex::tileSize ) );
	//return RectI(0, width * SpriteCodex::tileSize, 0, height * SpriteCodex::tileSize);
}

bool MemeField::OnRevealClick ( const Vei2 & screenPos )
{
	if ( !isKvorked ) {
		const Vei2 gridPos = ScreenToGrid ( screenPos );
		assert ( gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < height );


		RevealTile ( gridPos );


		return isKvorked;
	}
}

void MemeField::OnFlagClick ( const Vei2 & screenPos )
{
	if ( !isKvorked ) {

		const Vei2 gridPos = ScreenToGrid ( screenPos );
		assert ( gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < height );
		Tile& tile = TileAt ( gridPos );
		if ( !tile.IsRevealed ( ) ) tile.ToggleFlag ( );
	}
}

int MemeField::CountNeighborMemes ( const Vei2 & gridPos )

{
	const int xStart = std::max ( 0, gridPos.x - 1 );
	const int yStart = std::max ( 0, gridPos.y - 1 );
	const int xEnd = std::min ( width - 1, gridPos.x + 1 );
	const int yEnd = std::min ( height - 1, gridPos.y + 1 );

	int nCount = 0;

	for ( Vei2 gPos = { xStart,yStart }; gPos.y <= yEnd; gPos.y++ ) {
		for ( gPos.x = xStart; gPos.x <= xEnd; gPos.x++ ) {
			if ( TileAt ( gPos ).HasMeme ( ) ) nCount++;
		}
	}
	return nCount;
}

MemeField::Tile& MemeField::TileAt ( const Vei2 & gridPos )
{
	return field [ gridPos.y * width + gridPos.x ];
}

const MemeField::Tile& MemeField::TileAt ( const Vei2 & gridPos ) const
{
	return field [ gridPos.y * width + gridPos.x ];
}

Vei2 MemeField::ScreenToGrid ( const Vei2 screenPos )
{
	return ( screenPos - topLeft ) / SpriteCodex::tileSize;  //WHOAH this was hard to get the translation of mouse pos to screen/grid space:  screenPos - topLeft

}

const Vei2 MemeField::GetTopLeft ( ) const
{
	return topLeft;
}

int MemeField::GetCountTotalTiles ( )
{
	return width * height;
}

bool MemeField::AllTilesAreRevealed ( )
{
	// omg this test needs to return true ONLY if all of the tiles are either revealed or flagged!
	// I finally got it, but not easy. Had to first test for revealed. If not revealed, then check
	// if flagged. Dependent. Can't check for both at the same time, right?

	bool test = true;
	for ( Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++ ) {
		for ( gridPos.x = 0; gridPos.x < width; gridPos.x++ ) {
			if ( !( TileAt ( gridPos ).IsRevealed ( ) ) )
			{
				if ( !( TileAt ( gridPos ).IsFlagged ( ) ) ) {
					test = false;
				}
			}
		}
	}
	return test;
}

void MemeField::RevealTile ( const Vei2 & gridPos )
{
	Tile& tile = TileAt ( gridPos );

	if ( !tile.IsRevealed ( ) && !tile.IsFlagged ( ) ) {
		tile.Reveal ( );

		if ( tile.HasMeme ( ) )
			isKvorked = true;

		if ( tile.HasNoNeighborMemes ( ) ) {
			const int xStart = std::max ( 0, gridPos.x - 1 );
			const int yStart = std::max ( 0, gridPos.y - 1 );
			const int xEnd = std::min ( width - 1, gridPos.x + 1 );
			const int yEnd = std::min ( height - 1, gridPos.y + 1 );

			//int nCount = 0;

			for ( Vei2 gPos = { xStart,yStart }; gPos.y <= yEnd; gPos.y++ ) {
				for ( gPos.x = xStart; gPos.x <= xEnd; gPos.x++ ) {
					//Tile& tile = TileAt ( gPos );
					/*if ( !tile.IsRevealed ( ) ) {
						tile.Reveal ( );
					}*/
					RevealTile ( gPos );
				}
			}
		}

	}

}

void MemeField::Tile::SpawnMeme ( )
{
	assert ( !hasMeme );
	hasMeme = true;
}

bool MemeField::Tile::HasMeme ( ) const
{
	return hasMeme;
}

void MemeField::Tile::Draw ( const Vei2& screenPos, bool isKvorked, Graphics& gfx ) const
{
	if ( !isKvorked ) {
		switch ( state )
		{
		case State::Hidden:
			SpriteCodex::DrawTileButton ( screenPos, gfx );
			break;
		case State::Flagged:
			SpriteCodex::DrawTileButton ( screenPos, gfx );
			SpriteCodex::DrawTileFlag ( screenPos, gfx );
			break;
		case State::Revealed:
			if ( !HasMeme ( ) ) {
				SpriteCodex::DrawTileNumber ( screenPos, nNeighborMemes, gfx );
			}
			else {
				SpriteCodex::DrawTileBomb ( screenPos, gfx );
			}
			break;
		}
	}
	else {
		switch ( state )
		{
		case State::Hidden:
			if ( HasMeme ( ) ) {
				SpriteCodex::DrawTileBomb ( screenPos, gfx );
			}
			else {
				SpriteCodex::DrawTileButton ( screenPos, gfx );
			}
			break;
		case State::Flagged:
			if ( HasMeme ( ) ) {
				SpriteCodex::DrawTileBomb ( screenPos, gfx );
				SpriteCodex::DrawTileFlag ( screenPos, gfx );
			}
			else {
				SpriteCodex::DrawTileBomb ( screenPos, gfx );
				SpriteCodex::DrawTileCross ( screenPos, gfx );
			}
			break;
		case State::Revealed:
			if ( !HasMeme ( ) ) {
				SpriteCodex::DrawTileNumber ( screenPos, nNeighborMemes, gfx );
			}
			else {
				SpriteCodex::DrawTileBombRed ( screenPos, gfx );
			}
			break;
		}

	}
}

void MemeField::Tile::Reveal ( )
{
	assert ( state == State::Hidden );
	state = State::Revealed;
}

bool MemeField::Tile::IsRevealed ( ) const
{
	return ( state == State::Revealed );
}

void MemeField::Tile::ToggleFlag ( )
{
	assert ( !IsRevealed ( ) );
	if ( state == State::Hidden ) state = State::Flagged;
	else state = State::Hidden;
}

bool MemeField::Tile::IsFlagged ( ) const
{
	return state == State::Flagged;
}

void MemeField::Tile::SetNeighborMemeCount ( int memeCount )
{
	assert ( nNeighborMemes == -1 );
	nNeighborMemes = memeCount;
}

bool MemeField::Tile::HasNoNeighborMemes ( ) const
{
	return nNeighborMemes == 0;
}


