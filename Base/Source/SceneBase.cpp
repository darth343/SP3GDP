#include "SceneBase.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
	// Blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(1, 1, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 0.4f;
	//lights[1].kC = 1.f;
	//lights[1].kL = 0.01f;
	//lights[1].kQ = 0.001f;
	//lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[1].cosInner = cos(Math::DegreeToRadian(30));
	//lights[1].exponent = 3.f;
	//lights[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	FontData.Init("Image//FontData.csv");
	// Load the ground mesh and texture
	meshList[GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//background.tga");
	meshList[GEO_GROUNDTOP] = MeshBuilder::Generate2DMesh("GEO_GROUNDTOP", Color(1, 1, 1), 0.0f, 0.0f, 32.0f, 32.0f);
	meshList[GEO_GROUNDTOP]->textureID = LoadTGA("Image//groundTop.tga");
	meshList[GEO_INVENTORYSEEKER] = MeshBuilder::Generate2DMesh("GEO_INVENTORYSEEKER", Color(1, 1, 1), 0.0f, 0.0f, 46.f, 46.f);
	meshList[GEO_INVENTORYSEEKER]->textureID = LoadTGA("Image//inventorySeeker.tga");
	meshList[GEO_INVENTORYBACKGROUND] = MeshBuilder::Generate2DMesh("GEO_INVENTORYBACKGROUND", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_INVENTORYBACKGROUND]->textureID = LoadTGA("Image//InventoryBackground.tga");
	meshList[GEO_INVENTORYSECONDBACKGROUND] = MeshBuilder::Generate2DMesh("GEO_INVENTORYSECONDBACKGROUND", Color(1, 1, 1), 0.0f, 0.0f, 1.f, 1.f);
	meshList[GEO_INVENTORYSECONDBACKGROUND]->textureID = LoadTGA("Image//InventorySecondBackground.tga");
	meshList[GEO_TAMAGUCCIBACKGROUND] = MeshBuilder::Generate2DMesh("GEO_TAMAGUCCIBACKGROUND", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_TAMAGUCCIBACKGROUND]->textureID = LoadTGA("Image//tamagucciBG.tga");
	meshList[GEO_TAMAGUCCIUIBACKGROUND] = MeshBuilder::GenerateQuad("GEO_TAMAGUCCIUIBACKGROUND", Color(1, 1, 1), 800, 3);
	meshList[GEO_TAMAGUCCIUIBACKGROUND]->textureID = LoadTGA("Image//tamagucciUIBG.tga");
	meshList[GEO_TAMBG1] = MeshBuilder::Generate2DMesh("GEO_TAMAGUCCIBACKGROUND", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_TAMBG1]->textureID = LoadTGA("Image//background1.tga");
	meshList[GEO_TAMLIVINGROOM] = MeshBuilder::Generate2DMesh("GEO_TAMAGUCCIBACKGROUND", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_TAMLIVINGROOM]->textureID = LoadTGA("Image//TamBedRoom.tga");
	meshList[GEO_TAMAGUCCI] = MeshBuilder::Generate2DMesh("GEO_TAMAGUCCIBACKGROUND", Color(1, 1, 1), 0.0f, 0.0f, 1.f, 1.0f);
	meshList[GEO_TAMAGUCCI]->textureID = LoadTGA("Image//Mimitchi.tga");
	meshList[GEO_HUNGERFRAME] = MeshBuilder::Generate2DMesh("GEO_HUNGERFRAME", Color(1, 1, 1), 0.0f, 0.0f, 1.f, 1.0f);
	meshList[GEO_HUNGERFRAME]->textureID = LoadTGA("Image//hungerframe.tga");
	meshList[GEO_ENERGYFRAME] = MeshBuilder::Generate2DMesh("GEO_ENERGYFRAME", Color(1, 1, 1), 0.0f, 0.0f, 1.f, 1.0f);
	meshList[GEO_ENERGYFRAME]->textureID = LoadTGA("Image//energyframe.tga");
	meshList[GEO_HAPPINESSFRAME] = MeshBuilder::Generate2DMesh("GEO_HAPPINESSFRAME", Color(1, 1, 1), 0.0f, 0.0f, 1.f, 1.0f);
	meshList[GEO_HAPPINESSFRAME]->textureID = LoadTGA("Image//happinessframe.tga");
	meshList[GEO_TAMHAPPY] = MeshBuilder::GenerateSpriteAnimation("sprite", 1, 2);
	meshList[GEO_TAMHAPPY]->textureID = LoadTGA("Image//Mimitchi_Happy.tga");
	meshList[GEO_STAR] = MeshBuilder::Generate2DMesh("GEO_TAMAGUCCIBACKGROUND", Color(1, 1, 1), 0.0f, 0.0f, 1.f, 1.0f);
	meshList[GEO_STAR]->textureID = LoadTGA("Image//Star.tga");
	meshList[GEO_GREENTILE] = MeshBuilder::Generate2DMesh("GEO_GREENTILE", Color(1, 1, 1), 0.0f, 0.0f, 32.0f, 32.0f);
	meshList[GEO_GREENTILE]->textureID = LoadTGA("Image//greenTile.tga");

	meshList[GEO_NPC] = MeshBuilder::Generate2DMesh("GEO_GREENTILE", Color(1, 1, 1), 0.0f, 0.0f, 1.f, 1.0f);
	meshList[GEO_NPC]->textureID = LoadTGA("Image//NPC1.tga");
	meshList[GEO_NPC1_LEFT] = MeshBuilder::GenerateSpriteAnimation("sprite", 1, 3);
	meshList[GEO_NPC1_LEFT]->textureID = LoadTGA("Image//NPC1_LEFT.tga");
	meshList[GEO_NPC2_LEFT] = MeshBuilder::GenerateSpriteAnimation("sprite", 1, 3);
	meshList[GEO_NPC2_LEFT]->textureID = LoadTGA("Image//NPC2_LEFT.tga");
	meshList[GEO_NPC3_LEFT] = MeshBuilder::GenerateSpriteAnimation("sprite", 1, 3);
	meshList[GEO_NPC3_LEFT]->textureID = LoadTGA("Image//NPC3_LEFT.tga");
	meshList[GEO_NPCPIC] = MeshBuilder::GenerateSpriteAnimation("sprite", 1, 6);
	meshList[GEO_NPCPIC]->textureID = LoadTGA("Image//NPC1_GIF.tga");
	meshList[GEO_NPCPIC2] = MeshBuilder::GenerateSpriteAnimation("sprite", 1, 6);
	meshList[GEO_NPCPIC2]->textureID = LoadTGA("Image//NPC2_GIF.tga");
	meshList[GEO_NPCPIC3] = MeshBuilder::GenerateSpriteAnimation("sprite", 1, 6);
	meshList[GEO_NPCPIC3]->textureID = LoadTGA("Image//NPC3_GIF.tga");

	meshList[GEO_REDTILE] = MeshBuilder::Generate2DMesh("GEO_REDTILE", Color(1, 1, 1), 0.0f, 0.0f, 32.0f, 32.0f);
	meshList[GEO_REDTILE]->textureID = LoadTGA("Image//redTile.tga");
	meshList[GEO_POOP] = MeshBuilder::Generate2DMesh("GEO_REDTILE", Color(1, 1, 1), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_POOP]->textureID = LoadTGA("Image//Poop.tga");
	meshList[GEO_BLUETILE] = MeshBuilder::Generate2DMesh("GEO_BLUETILE", Color(1, 1, 1), 0.0f, 0.0f, 32.0f, 32.0f);
	meshList[GEO_BLUETILE]->textureID = LoadTGA("Image//blueTile.tga");
	meshList[GEO_HEROWALK] = MeshBuilder::Generate2DMesh("Player", Color(1, 1, 1), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_HEROWALK]->textureID = LoadTGA("Image//Hero.tga");
	meshList[GEO_MONSTER] = MeshBuilder::Generate2DMesh("Monster", Color(1, 1, 1), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_MONSTER]->textureID = LoadTGA("Image//Monster.tga");
	meshList[GEO_POTION] = MeshBuilder::Generate2DMesh("Potion", Color(1, 1, 1), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_POTION]->textureID = LoadTGA("Image//Potion.tga");
	meshList[GEO_SWORD] = MeshBuilder::Generate2DMesh("GEO_SWORD", Color(1, 1, 1), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_SWORD]->textureID = LoadTGA("Image//sword.tga");
	meshList[GEO_SHIELD] = MeshBuilder::Generate2DMesh("GEO_SHIELD", Color(1, 1, 1), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_SHIELD]->textureID = LoadTGA("Image//shield.tga");
	meshList[GEO_CHESTPLATE] = MeshBuilder::Generate2DMesh("GEO_CHESTPLATE", Color(1, 1, 1), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_CHESTPLATE]->textureID = LoadTGA("Image//chestplate.tga");
	meshList[GEO_HELMET] = MeshBuilder::Generate2DMesh("GEO_HELMET", Color(1, 1, 1), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_HELMET]->textureID = LoadTGA("Image//helmet.tga");
	meshList[GEO_GREEN] = MeshBuilder::Generate2DMesh("GEO_GREEN", Color(0, 1, 0), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_BAR] = MeshBuilder::Generate2DMesh("Potion", Color(1, 1, 0), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_RED] = MeshBuilder::Generate2DMesh("Potion", Color(1, 0, 0), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_BATTLESCENE] = MeshBuilder::Generate2DMesh("GEO_BATTLESCENE", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_BATTLESCENE]->textureID = LoadTGA("Image//battleScene.tga");
	meshList[GEO_BATTLEMONSTER] = MeshBuilder::Generate2DMesh("GEO_BATTLESCENE", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_BATTLEMONSTER]->textureID = LoadTGA("Image//battleMonster.tga");
	meshList[GEO_BATTLEDIALOUGEBACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BATTLEDIALOUGEBACKGROUND", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_BATTLEDIALOUGEBACKGROUND]->textureID = LoadTGA("Image//dialougeBG.tga");
	meshList[GEO_BATTLEARROW] = MeshBuilder::Generate2DMesh("GEO_BATTLEARROW", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_BATTLEARROW]->textureID = LoadTGA("Image//arrow.tga");

	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_NPCPIC]);
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 5, 0, 1.f, true);
	}

	SpriteAnimation *npcwalk1 = dynamic_cast<SpriteAnimation*>(meshList[GEO_NPC1_LEFT]);
	if (npcwalk1)
	{
		npcwalk1->m_anim = new Animation();
		npcwalk1->m_anim->Set(0, 2, 0, 1.f, true);
	}
	SpriteAnimation *npcwalk2 = dynamic_cast<SpriteAnimation*>(meshList[GEO_NPC2_LEFT]);
	if (npcwalk2)
	{
		npcwalk2->m_anim = new Animation();
		npcwalk2->m_anim->Set(0, 2, 0, 1.f, true);
	}

	SpriteAnimation *npcwalk3 = dynamic_cast<SpriteAnimation*>(meshList[GEO_NPC3_LEFT]);
	if (npcwalk3)
	{
		npcwalk3->m_anim = new Animation();
		npcwalk3->m_anim->Set(0, 2, 0, 1.f, true);
	}
	

	SpriteAnimation *pic2 = dynamic_cast<SpriteAnimation*>(meshList[GEO_NPCPIC2]);
	if (pic2)
	{
		pic2->m_anim = new Animation();
		pic2->m_anim->Set(0, 5, 0, 1.f, true);
	}
	SpriteAnimation *pic3 = dynamic_cast<SpriteAnimation*>(meshList[GEO_NPCPIC3]);
	if (pic3)
	{
		pic3->m_anim = new Animation();
		pic3->m_anim->Set(0, 5, 0, 1.f, true);
	}
	SpriteAnimation *tamhappy = dynamic_cast<SpriteAnimation*>(meshList[GEO_TAMHAPPY]);
	if (tamhappy)
	{
		tamhappy->m_anim = new Animation();
		tamhappy->m_anim->Set(0, 1, 0, 1.f, true);
	}


	Math::InitRNG();
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	float pointer = 0.6f;
	ytranslate = 0.2f;
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		if (i > 0) {
			pointer += FontData.charOffsets[text[Math::Min(i, i - 1)]] + 0.18f;
			if (pointer >= 23.f) {
				ytranslate -= 1.f;
				pointer = 0.2f;
			}
		}
		//pointer += FontData.charOffsets[text[Math::Min(i, i - 1)]] + 0.18f;
		characterSpacing.SetToTranslation(pointer, ytranslate, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();

}

void SceneBase::Render2DMeshWScale(Mesh *mesh, const bool enableLight, const float sizeX, const float sizeY, const float x, const float y, const bool flip)
{
	glDisable(GL_CULL_FACE);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	if (flip)
	{
		modelStack.Translate(x + 0, y, 0);
	}
	else
	{
		modelStack.Translate(x, y, 0);
	}
	modelStack.Scale(sizeX, sizeY, 1);

	if (flip)
		modelStack.Rotate(180, 0, 1, 0);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_CULL_FACE);
}

void SceneBase::Render2DMesh(Mesh *mesh, bool enableLight, float size, float x, float y, bool flip)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	if (flip)
		modelStack.Rotate(180, 0, 1, 0);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneBase::RenderBackground(Mesh* mesh)
{
	Render2DMesh(mesh, false, 1.0f); // World Overlay Background
}

void SceneBase::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
}

void SceneBase::Update(double dt)
{
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(10.f * dt);
	if (Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(10.f * dt);
	if (Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(10.f * dt);
	if (Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(10.f * dt);
	if (Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(10.f * dt);
	if (Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(10.f * dt);

	camera.Update(dt);
}

void SceneBase::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
