#pragma once
#include "Vector2.h"
#include "Map.h"
#include "SpriteAnimation.h"
#include "Mesh.h"
#include "Items.h"

class CPlayerInfo
{
public:
	CPlayerInfo(void);
	~CPlayerInfo(void);

	enum GEOMETRY_TYPE
	{
		GEO_TILEHERO_FRAME0,
		GEO_TILEHERO_FRAME1,
		GEO_TILEHERO_FRAME2,
		GEO_TILEHERO_FRAME3,
		NUM_GEOMETRY,
	};

	// Initialise this class instance
	void Init(void);
	// Update Movements
	void MoveUpDown(const bool mode, CMap* m_cMap, double dt);
	void MoveLeftRight(const bool mode, CMap* m_cMap, double dt);
	// Get Position
	Vector3 GetPosition();
	// Set Position
	void SetPosition(Vector3 pos);
	// Get mapOffset
	Vector3 GetMapOffset(void);
	// Set mapOffset
	void SetMapOffset(Vector3 MapOffset);
	// Hero Update
	void HeroUpdate(CMap* m_cMap, double dt, Mesh **meshList);
	// Get Player Mesh
	Mesh* GetPlayerMesh();
	// Set Player Mesh
	void SetPlayerMesh(Mesh* mesh);
	// Get Flip Status
	bool GetFlipStatus();
	// Set Flip Status
	void SetFlipStatus(bool flip);
	// Constrain the position of the Hero to within the border
	void ConstrainHero(const int leftBorder, const int rightBorder, 
					   const int topBorder, const int bottomBorder, 
					   double dt, CMap* m_cMap, bool constrainX, bool constrainY);

	float GetHP();
	void SetHP(float hp);
	float GetDMG();
	void SetDMG(float hp);

private:
	// Hero's information
	Vector3 theHeroPosition;
	bool FlipStatus;
	Mesh* playerMesh;
	// For scrolling.
	Vector3 mapOffset;
	// Array for inventory
	Items Inventory[Items::ITEM_TOTAL];

	float hp;
	float damage;
	bool collideWithEnemy;
};
