#ifndef SCENEBASE_H
#define SCENEBASE_H
#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "CharacterData.h"
class SceneBase : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_COLOR_BLEND,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
public:
	static enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_BACKGROUND,
		GEO_GROUNDTOP,
		GEO_HEROWALK,
		GEO_MONSTER,
		GEO_RED,
		GEO_GREEN,
		GEO_NPC,
		GEO_NPC1_LEFT,
		GEO_NPC2_LEFT,
		GEO_NPC3_LEFT,
		GEO_NPCPIC,
		GEO_NPCPIC2,
		GEO_NPCPIC3,
		GEO_POPUP,
		GEO_POPUPNO,

		GEO_HEROLR,
		GEO_HEROUP,
		GEO_HEROD,

		GEO_MONSTERBANSHEE,
		GEO_MONSTERCEREBUS,
		GEO_MONSTERGOLEM,
		GEO_MONSTERDRAGON,


		GEO_STORYBACKGROUND,
		GEO_BOSS,
		GEO_WIN,
		GEO_LOSE,
		GEO_LIVES,

		GEO_PORTAL,


		GEO_LOGO,
		GEO_MENUBG,
		GEO_INSTRUCTIONBG,
		GEO_MENUPLAY,
		GEO_MENUOPT,
		GEO_MENUQUIT,
		GEO_MENUSTORY,
		GEO_MENUINST,
		GEO_ICONINV,
		GEO_ICONTAM,
		GEO_EXCLAIM,

		GEO_SALAD,
		GEO_HAMGURGER,
		GEO_PORK,
		GEO_TILESET1,
		GEO_TILESET2,
		GEO_TILESET3,
		GEO_TILESET4,
		GEO_HPBARDESIGN,
		GEO_HPBAR,
		GEO_MPBAR,
		GEO_SLASHANIMATION,
		GEO_STABANIMATION,
		GEO_BITEANIMATION,
		GEO_SCREAMANIMATION,
		GEO_ROARANIMATION,
		GEO_SKINANIMATION,
		GEO_BATTLESCENE,
		GEO_BATTLEARROW,
		GEO_BATTLEMONSTER,
		GEO_BATTLEDIALOUGEBACKGROUND,
		GEO_TAMAGUCCIBACKGROUND,
		GEO_TAMAGUCCIUIBACKGROUND,
		GEO_TAMAGUCCI,
		GEO_HUNGERFRAME,
		GEO_ENERGYFRAME,
		GEO_HAPPINESSFRAME,
		GEO_TAMHAPPY,
		GEO_TAMFOOD,
		GEO_TAMSLEEP,
		GEO_TAMBG1,
		GEO_TAMBG2,
		GEO_TAMLIVINGROOM,
		GEO_BLACK,
		GEO_STAR,
		GEO_POOP,
		GEO_INVENTORYBACKGROUND,
		GEO_INVENTORYSECONDBACKGROUND,
		GEO_INVENTORYSEEKER,
		GEO_SWORD,
		GEO_SHIELD,
		GEO_HELMET,
		GEO_CHESTPLATE,
		GEO_LEG,
		GEO_BAR,
		GEO_POTION,
		GEO_TRAP,
		GEO_GREENTILE,
		GEO_REDTILE,
		GEO_BLUETILE,
		GEO_TEXT,
		NUM_GEOMETRY,
	};
	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderTile(Mesh* mesh, unsigned tileID, float size, float x, float y);
	void RenderBackground(Mesh* mesh);
	void Render2DMesh(Mesh *mesh, const bool enableLight, const float size = 1.0f, const float x = 0.0f, const float y = 0.0f, const bool flip = false);
	void Render2DMeshWScale(Mesh *mesh, const bool enableLight, const float sizeX, const float sizeY, const float x = 0.0f, const float y = 0.0f, const bool flip = false, const float offset = 0);

protected:
	CharacterData FontData;
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	Camera3 camera;
	MS modelStack;
	MS viewStack;
	MS projectionStack;
	Light lights[2];
	float fps;
	float xtranslate;
	float ytranslate;
};

#endif // !SCENEBASE_H
