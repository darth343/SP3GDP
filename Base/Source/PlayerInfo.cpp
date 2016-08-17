#include "PlayerInfo.h"
#include "SceneText.h"
#include <iostream>
#include "Application.h"

CPlayerInfo::CPlayerInfo(void)
	: mapOffset(Vector3(0, 0, 0))
	, playerMesh(NULL)
	, FlipStatus(false)
	, collideWithEnemy(false)
	, hp(100)
	, damage(20)
{
}

const float MOVEMENT_SPEED = 250.f;
const float xOffset = 8.f;
const float yOffset = 2.f;

CPlayerInfo::~CPlayerInfo(void)
{
}

// Initialise this class instance
void CPlayerInfo::Init(void)
{
	theHeroPosition.x=0;
	theHeroPosition.y=0;
}

Mesh* CPlayerInfo::GetPlayerMesh()
{
	return playerMesh;
}

bool CPlayerInfo::GetFlipStatus()
{
	return FlipStatus;
}

void CPlayerInfo::SetPlayerMesh(Mesh* mesh)
{
	this->playerMesh = mesh;
}

/********************************************************************************
 Hero Move Up Down
 ********************************************************************************/
void CPlayerInfo::MoveUpDown(const bool mode, CMap* m_cMap, double dt)
{
	if (mode)
	{
		Vector3 nextPos = theHeroPosition;
		nextPos.y = theHeroPosition.y - (MOVEMENT_SPEED * dt);
		Vector3 checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
		if (checkPosition.y < 0)
		{
			checkPosition.y = 0;
		}
		if (checkPosition.y + 1 > m_cMap->GetNumOfTiles_Height())
		{
			checkPosition.y = m_cMap->GetNumOfTiles_Height();
		}
		if (fmod(theHeroPosition.x + mapOffset.x, m_cMap->GetTileSize()) < 0.2)
		{
			if (!m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].shouldCollide)
			{
				theHeroPosition.y = nextPos.y;
			}
			else
			{
				theHeroPosition.y = (m_cMap->theMap[(int)checkPosition.y + 1][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).y - mapOffset.y;
			}
		}
		else
		{
			nextPos.y = theHeroPosition.y - (MOVEMENT_SPEED * dt);
			nextPos.x += xOffset;
			checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
			if (!m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].shouldCollide)
			{
				nextPos.y = theHeroPosition.y - (MOVEMENT_SPEED * dt);
				nextPos.x -= xOffset;
				checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
				if (!m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x + 1].shouldCollide)
				{
					theHeroPosition.y = nextPos.y;
				}
				else
				{
					theHeroPosition.y = (m_cMap->theMap[(int)checkPosition.y + 1][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).y - mapOffset.y;
				}
			}
			else
			{
				theHeroPosition.y = (m_cMap->theMap[(int)checkPosition.y + 1][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).y - mapOffset.y;
			}
		}
	}
	else
	{
		Vector3 nextPos = theHeroPosition;
		nextPos.y = theHeroPosition.y + (MOVEMENT_SPEED * dt);
		Vector3 checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
		if (checkPosition.y < 0)
		{
			checkPosition.y = 0;
		}
		if (checkPosition.y + 1 > m_cMap->GetNumOfTiles_Height())
		{
			checkPosition.y = m_cMap->GetNumOfTiles_Height();
		}
		if (fmod(theHeroPosition.x + mapOffset.x, m_cMap->GetTileSize()) < 0.2)
		{
			if (!m_cMap->theMap[(int)checkPosition.y + 1][(int)checkPosition.x].shouldCollide)
			{
				theHeroPosition.y = nextPos.y;
			}
			else
			{
				theHeroPosition.y = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).y - mapOffset.y;
			}
		}
		else
		{
			nextPos.y = theHeroPosition.y + (MOVEMENT_SPEED * dt);
			nextPos.x += xOffset;
			checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
			if (!m_cMap->theMap[(int)checkPosition.y + 1][(int)checkPosition.x].shouldCollide)
			{
				nextPos.y = theHeroPosition.y + (MOVEMENT_SPEED * dt);
				nextPos.x -= xOffset;
				checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
				if (!m_cMap->theMap[(int)checkPosition.y + 1][(int)checkPosition.x + 1].shouldCollide)
				{
					theHeroPosition.y = nextPos.y;
				}
				else
				{
					theHeroPosition.y = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).y - mapOffset.y;
				}
			}
			else
			{
				theHeroPosition.y = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).y - mapOffset.y;
			}
		}
	}
}

/********************************************************************************
 Hero Move Left Right
 ********************************************************************************/
void CPlayerInfo::MoveLeftRight(const bool mode, CMap* m_cMap, double dt)
{
	if (mode)
	{
		Vector3 nextPos = theHeroPosition;
		nextPos.x = theHeroPosition.x - (MOVEMENT_SPEED * dt);
		Vector3 checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
		if (checkPosition.x < 0)
		{
			checkPosition.x = 0;
		}
		if (checkPosition.x > m_cMap->GetNumOfTiles_Width())
		{
			checkPosition.x = m_cMap->GetNumOfTiles_Width();
		}
		if (fmod(theHeroPosition.y + mapOffset.y, m_cMap->GetTileSize()) < 0.2)
		{
			if (!m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].shouldCollide)
			{
				theHeroPosition.x = nextPos.x;
			}
			else
			{
				theHeroPosition.x = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x + 1].Pos * m_cMap->GetTileSize()).x - mapOffset.x;
			}
		}
		else{
			nextPos = theHeroPosition;
			nextPos.x = theHeroPosition.x - (MOVEMENT_SPEED * dt);
			nextPos.y += yOffset;
			checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
			if (!m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].shouldCollide)
			{
				nextPos = theHeroPosition;
				nextPos.x = theHeroPosition.x - (MOVEMENT_SPEED * dt);
				nextPos.y -= yOffset;
				checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
				if (!m_cMap->theMap[(int)checkPosition.y + 1][(int)checkPosition.x].shouldCollide)
				{
					theHeroPosition.x = nextPos.x;
				}
				else
				{
					theHeroPosition.x = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x + 1].Pos * m_cMap->GetTileSize()).x - mapOffset.x;
				}

			}
			else
			{
				theHeroPosition.x = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x + 1].Pos * m_cMap->GetTileSize()).x - mapOffset.x;
			}
		}
	}else
	{
		Vector3 nextPos = theHeroPosition;
		nextPos.x = theHeroPosition.x + (MOVEMENT_SPEED * dt);
		Vector3 checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
		if (checkPosition.x < 0)
		{
			checkPosition.x = 0;
		}
		if (checkPosition.x+1 > m_cMap->GetNumOfTiles_Width())
		{
			checkPosition.x = m_cMap->GetNumOfTiles_Width();
		}
		if (fmod(theHeroPosition.y + mapOffset.y, m_cMap->GetTileSize()) < 0.1)
		{
			if (!m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x + 1].shouldCollide)
			{
				theHeroPosition.x = nextPos.x;
			}
			else
			{
				theHeroPosition.x = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).x - mapOffset.x;
			}
		}
		else
		{
			nextPos = theHeroPosition;
			nextPos.x = theHeroPosition.x + (MOVEMENT_SPEED * dt);
			nextPos.y += yOffset;
			checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
			if (!m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x + 1].shouldCollide)
			{
				nextPos = theHeroPosition;
				nextPos.x = theHeroPosition.x + (MOVEMENT_SPEED * dt);
				nextPos.y -= yOffset;
				checkPosition = (nextPos + mapOffset) * (1.f / m_cMap->GetTileSize());
				if (!m_cMap->theMap[(int)checkPosition.y + 1][(int)checkPosition.x + 1].shouldCollide)
				{
					theHeroPosition.x = nextPos.x;
				}
				else
				{
					theHeroPosition.x = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).x - mapOffset.x;
				}
			}
			else
			{
				theHeroPosition.x = (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].Pos * m_cMap->GetTileSize()).x - mapOffset.x;
			}
		}
	}
}

Vector3 CPlayerInfo::GetMapOffset(void)
{
	return mapOffset;
}

void CPlayerInfo::SetMapOffset(Vector3 MapOffset)
{
	mapOffset = MapOffset;
}

void CPlayerInfo::SetFlipStatus(bool flip)
{
	FlipStatus = flip;
}

Vector3 CPlayerInfo::GetPosition()
{
	return theHeroPosition;
}

void CPlayerInfo::SetPosition(Vector3 pos)
{
	theHeroPosition = pos;
}

// Constrain the position of the Hero to within the border
void CPlayerInfo::ConstrainHero(const int leftBorder, const int rightBorder,
	const int topBorder, const int bottomBorder,
	double dt, CMap* m_cMap, bool constrainX, bool constrainY)
{
	if (constrainX)
	{
		if (mapOffset.x != 0)
		{
			if (theHeroPosition.x < leftBorder)
			{
				float displacement = leftBorder - theHeroPosition.x;
				mapOffset.x -= displacement;
				theHeroPosition.x = leftBorder;
				//mapOffset.x = mapOffset.x - (int)(CONSTRAINTSPEED * dt);
				if (mapOffset.x < 0)
					mapOffset.x = 0;
			}
		}
		if (mapOffset.x < m_cMap->GetNumOfTiles_Width() * m_cMap->GetTileSize() - 800)
		{
			if (mapOffset.x < m_cMap->getScreenWidth() - 1.f)
			{
				if (theHeroPosition.x > rightBorder)
				{
					float displacement = theHeroPosition.x - rightBorder;
					mapOffset.x += displacement;
					theHeroPosition.x = rightBorder;
					//mapOffset.x = mapOffset.x + (int)(CONSTRAINTSPEED * dt);
					if (mapOffset.x > m_cMap->theScreen_Width)	// This must be changed to soft-coded
						mapOffset.x = m_cMap->theScreen_Width;
				}
			}
		}
		else
		{
			mapOffset.x = m_cMap->GetNumOfTiles_Width() * m_cMap->GetTileSize() - 800;
		}
	}
	else
	{
		if (theHeroPosition.x > 800 - m_cMap->GetTileSize())
		{
			theHeroPosition.x = 800 - m_cMap->GetTileSize();
		}
	}

	if (theHeroPosition.x < 0)
	{
		theHeroPosition.x = 0;
	}
	else if (theHeroPosition.x >((m_cMap->GetNumOfTiles_Width() - 1) * m_cMap->GetTileSize() - mapOffset.x) - m_cMap->GetTileSize())
	{
		theHeroPosition.x = ((m_cMap->GetNumOfTiles_Width() - 1) * m_cMap->GetTileSize() - mapOffset.x) - m_cMap->GetTileSize();
	}

	if (constrainY)
	{
		if (mapOffset.y < m_cMap->GetNumOfTiles_Height() * m_cMap->GetTileSize() - 600)
		{
			if (theHeroPosition.y > topBorder)
			{
				float displacement = theHeroPosition.y - topBorder;
				mapOffset.y += displacement;
				theHeroPosition.y = topBorder;
			}
		}
		else
		{
			mapOffset.y = m_cMap->GetNumOfTiles_Height() * m_cMap->GetTileSize() - 600;
		}

		if (mapOffset.y > 0)
		{
			if (theHeroPosition.y < bottomBorder)
			{
				float displacement = bottomBorder - theHeroPosition.y;
				mapOffset.y -= displacement;
				theHeroPosition.y = bottomBorder;
			}
		}
		else
		{
			mapOffset.y = 0;
		}
	}
	else
	{
		if (theHeroPosition.y + m_cMap->GetTileSize() > 600)
		{
			theHeroPosition.y = 600 - m_cMap->GetTileSize();
		}
	}
}

/********************************************************************************
 Hero Update
 ********************************************************************************/
void CPlayerInfo::HeroUpdate(CMap* m_cMap, double dt, Mesh**meshList)
{
	Vector3 checkPosition = (theHeroPosition + mapOffset) * (1.f / m_cMap->GetTileSize());
	if (m_cMap->theMap[(int)checkPosition.y][(int)checkPosition.x].BlockID == 2)
	{
		collideWithEnemy = true;
		cout << "Collided with Enemy" << endl;
	}
	if (collideWithEnemy)
	{
		//Enter BattleScene

	}
	ConstrainHero(m_cMap->getScreenWidth() * 0.25, m_cMap->getScreenWidth() * 0.5, m_cMap->getScreenHeight() * 0.5, m_cMap->getScreenHeight() * 0.15, dt, m_cMap, true, true);
}