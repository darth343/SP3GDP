#pragma once
#include "Vector2.h"
#include "Map.h"
#include "SpriteAnimation.h"
#include "Mesh.h"

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
	// Returns true if the player is on ground
	bool isOnGround(void);
	// Returns true if the player is jumping upwards
	bool isJumpUpwards(void);
	// Returns true if the player is on freefall
	bool isFreeFall(void);
	// Set the player's status to free fall mode
	void SetOnFreeFall(bool isOnFreeFall);
	// Set the player to jumping upwards
	//void SetToJumpUpwards(bool isOnJumpUpwards);
	void HeroJump();
	// Stop the player's movement
	void SetToStop(void);
	// Set position x of the player
	void SetPos_x(float pos_x);
	// Set position y of the player
	void SetPos_y(int pos_y);
	// Set Jumpspeed of the player
	void SetJumpspeed(int jumpspeed);

	// Update Movements
	void MoveUpDown(const bool mode, CMap* m_cMap, double dt);
	void MoveLeftRight(const bool mode, CMap* m_cMap, double dt);

	// Get position x of the player
	int GetPos_x(void);
	// Get position y of the player
	int GetPos_y(void);

	Vector3 GetPosition()
	{
		return theHeroPosition;
	}

	void SetPosition(Vector3 pos)
	{
		theHeroPosition = pos;
	}
	// Get Jumpspeed of the player
	int GetJumpspeed(void);
	// Get mapOffset
	Vector3 GetMapOffset(void)
	{
		return mapOffset;
	}
	void SetMapOffset(Vector3 MapOffset)
	{
		mapOffset = MapOffset;
	}
	// Get mapFineOffset_x
	int GetMapFineOffset_x(void);
	// Get mapFineOffset_y
	int GetMapFineOffset_y(void);

	// Update Jump Upwards
	void UpdateJumpUpwards();
	// Update FreeFall
	void UpdateFreeFall();
	// Hero Update
	void HeroUpdate(CMap* m_cMap, double dt, Mesh **meshList);

	// Set Animation Invert status of the player
	void SetAnimationInvert(bool heroAnimationInvert);
	// Get Animation Invert status of the player
	bool GetAnimationInvert(void);
	// Set Animation Counter of the player
	void SetAnimationCounter(int heroAnimationCounter);
	// Get Animation Counter of the player
	int GetAnimationCounter(void);

	Mesh* GetPlayerMesh();
	void SetPlayerMesh(Mesh* mesh);

	bool GetFlipStatus();
	void SetFlipStatus(bool flip)
	{
		FlipStatus = flip;
	}
	// Constrain the position of the Hero to within the border
	void ConstrainHero(const int leftBorder, const int rightBorder, 
					   const int topBorder, const int bottomBorder, 
					   double dt, CMap* m_cMap, bool constrainX, bool constrainY);
	int lives;
private:
	// Hero's information
	Vector3 theHeroPosition;
	float jumpspeed;
	bool hero_inMidAir_Up;
	bool hero_inMidAir_Down;
	bool heroAnimationInvert;
	int heroAnimationCounter;
	bool FlipStatus;
	Mesh* playerMesh;
	// For scrolling.
	Vector3 mapOffset;
};
