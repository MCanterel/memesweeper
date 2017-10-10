/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include  "SpriteCodex.h"


Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	field(20)
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}
void Game::UpdateModel()
{
	//TestTiles();
}
void Game::TestTiles()
{
	SpriteCodex::DrawTile0(Vei2(100, 100), gfx);
	SpriteCodex::DrawTile1(Vei2(150, 100), gfx);
	SpriteCodex::DrawTile2(Vei2(200, 100), gfx);
	SpriteCodex::DrawTile3(Vei2(250, 100), gfx);
	SpriteCodex::DrawTile4(Vei2(300, 100), gfx);
	SpriteCodex::DrawTile5(Vei2(350, 100), gfx);
	SpriteCodex::DrawTile6(Vei2(400, 100), gfx);
	SpriteCodex::DrawTile7(Vei2(450, 100), gfx);
	SpriteCodex::DrawTile8(Vei2(500, 100), gfx);
	SpriteCodex::DrawTileButton(Vei2(100, 150), gfx);
	SpriteCodex::DrawTileCross(Vei2(150, 150), gfx);
	SpriteCodex::DrawTileFlag(Vei2(200, 150), gfx);
	SpriteCodex::DrawTileBomb(Vei2(250, 150), gfx);
	SpriteCodex::DrawTileBombRed(Vei2(300, 150), gfx);
}

void Game::ComposeFrame()
{
	field.Draw(gfx);
}
