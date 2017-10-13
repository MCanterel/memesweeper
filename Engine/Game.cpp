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


Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	field(nMemes),
	soundPad(L"Sounds\\pad.wav"),
	soundFart(L"Sounds\\fart.wav")
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

	switch (gameState) {
	case GameState::Waiting:
		SpriteCodex::DrawWaitScreen(Vei2(gfx.ScreenWidth / 2 - 100, 10), gfx);
		//SpriteCodex::DrawWaitScreen(Vei2(gfx.ScreenWidth / 2, gfx.ScreenHeight / 2), gfx);
		if (wnd.kbd.KeyIsPressed(VK_RETURN)) gameState = GameState::Playing;
		break;
	case GameState::Playing:
		while (!wnd.mouse.IsEmpty()) {
			const Mouse::Event e = wnd.mouse.Read();
			Vei2 mousePos = wnd.mouse.GetPos();
			// -field.GetTopLeft();  //not necessary, as this is handled in memefield::screentogrid
			//GetPos() is just passed the raw mousePos

			if (e.GetType() == Mouse::Event::Type::LPress)
			{
				
				if (field.GetRect().Contains(mousePos))	
					if (field.OnRevealClick(mousePos)) {
						soundFart.Play(0.75f, 0.75f);
						gameState = GameState::Lost;
					}
					else {
						soundPad.Play(0.75f, 0.2f);
					}
			}
			if (e.GetType() == Mouse::Event::Type::RPress)
			{
				soundPad.Play(0.5f, 0.1f);
				if (field.GetRect().Contains(mousePos))	
					field.OnFlagClick(mousePos);
			}
		}

		if (field.AllTilesAreRevealed()) 
			gameState = GameState::Won;
		break;
	}

}


void Game::ComposeFrame()
{
	field.Draw(gfx);
	switch (gameState) {
	case GameState::Won:
		SpriteCodex::DrawWinScreen(Vei2(gfx.ScreenWidth / 2 - 100, 375), gfx);
		break;
	case GameState::Lost:
		SpriteCodex::DrawLoseScreen(Vei2(gfx.ScreenWidth / 2 - 140, 375), gfx);
	}
	
}
