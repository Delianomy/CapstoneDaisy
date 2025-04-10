#include <memory>
#include <iostream>
#include <fstream>
#include "tinyxml2.h"
#include <string>
#include "AssetManager.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"

AssetManager* AssetManager::instance = nullptr;

AssetManager::AssetManager() {
	


	//Actual level MODELS
	AddComponent<MaterialComponent>("Daisy_spriteSheet", nullptr, "textures/SpriteSheets/Player_spritesheet.png");
	
		AddComponent<MeshComponent>("Flower_obj", nullptr, "meshes/Flower.obj");
		AddComponent<MeshComponent>("Tower_obj", nullptr, "meshes/Tower.obj");
		AddComponent<MeshComponent>("Island1_obj", nullptr, "meshes/Island_1.obj");
		AddComponent<MeshComponent>("Island2_obj", nullptr, "meshes/Island_2.obj");


		AddComponent<MeshComponent>("Sphere", nullptr, "meshes/Sphere.obj");
		AddComponent<MeshComponent>("Square", nullptr, "meshes/Square.obj");
		AddComponent<MeshComponent>("Cube", nullptr, "meshes/Cube.obj");
		AddComponent<MeshComponent>("Plane", nullptr, "meshes/Plane8x8.obj");

		AddComponent<MeshComponent>("House_obj", nullptr, "meshes/House.obj");

		AddComponent<MeshComponent>("Beanstalk_obj", nullptr, "meshes/Beanstalk.obj");
		AddComponent<MeshComponent>("Leaf_obj", nullptr, "meshes/Leaf.obj");

		AddComponent<MeshComponent>("Bridge_obj", nullptr, "meshes/Bridge.obj");
		AddComponent<MeshComponent>("Bush_obj", nullptr, "meshes/bush.obj");
		AddComponent<MeshComponent>("Grass1_obj", nullptr, "meshes/grass_1.obj");

	
	//15 models

	
		
		AddComponent<MaterialComponent>("Island1_mat", nullptr, "textures/Environment/Island_1_test.png");
		AddComponent<MaterialComponent>("Island2_mat", nullptr, "textures/Environment/island_2_test_2.png");
		AddComponent<MaterialComponent>("House_mat", nullptr, "textures/Environment/house_test.png");
		AddComponent<MaterialComponent>("Flower_v1_mat", nullptr, "textures/Environment/flower_v1.png");
		AddComponent<MaterialComponent>("Beanstalk_mat", nullptr, "textures/Environment/beanstalk.png");
		AddComponent<MaterialComponent>("Leaf_mat", nullptr, "textures/Environment/leaf.png");
		AddComponent<MaterialComponent>("Tower_mat", nullptr, "textures/Environment/tower_test.png");
		AddComponent<MaterialComponent>("Bridge_mat", nullptr, "textures/Environment/bridge_test.png");
		AddComponent<MaterialComponent>("Bush_mat", nullptr, "textures/Environment/bush.png");
		AddComponent<MaterialComponent>("Grass1_mat", nullptr, "textures/Environment/grass_1.png");

	
	//25

	
		
		AddComponent<ShaderComponent>("DefaultShader", nullptr, "shaders/defaultVert.glsl", "shaders/defaultFrag.glsl");
		AddComponent<ShaderComponent>("ColorPickingShader", nullptr, "shaders/colorPickingVert.glsl", "shaders/colorPickingFrag.glsl");
		AddComponent<ShaderComponent>("TextureShader", nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
		AddComponent<ShaderComponent>("ShadowDepthShader", nullptr, "shaders/shadowDepthVert.glsl", "shaders/shadowDepthFrag.glsl");
		AddComponent<ShaderComponent>("DrawNormalsShader", nullptr, "shaders/drawNormalsVert.glsl", "shaders/drawNormalsFrag.glsl", "shaders/drawNormalsGeom.glsl");
		AddComponent<ShaderComponent>("WireFrameShader", nullptr, "shaders/wireFrameVert.glsl", "shaders/wireFrameFrag.glsl", "shaders/wireFrameGeom.glsl");
		AddComponent<ShaderComponent>("RegularTextureShader", nullptr, "shaders/textureVertRegular.glsl", "shaders/textureFragRegular.glsl");
		AddComponent<ShaderComponent>("SimpleWavesShader", nullptr, "shaders/waveVert.glsl", "shaders/waveFrag.glsl");
		AddComponent<ShaderComponent>("CoolWaveShader", nullptr, "shaders/prettyWaterVert.glsl", "shaders/prettyWaterFrag.glsl");


		AddComponent<ShaderComponent>("SimpleTextureShader", nullptr, "shaders/simpleTextureVert.glsl", "shaders/simpleTextureFrag.glsl");
		AddComponent<ShaderComponent>("ColorPicker", nullptr, "shaders/colorPickingVert.glsl", "shaders/colorPickingFrag.glsl");
		AddComponent<ShaderComponent>("Frame buffer", nullptr, "shaders/bufferVert.glsl", "shaders/bufferFrag.glsl");
		AddComponent<ShaderComponent>("Water Shader", nullptr, "shaders/waterShaderVert.glsl", "shaders/waterShaderFrag.glsl");

	


	//SPRITE SHEETS
	
		
		AddComponent<MaterialComponent>("Mermaid", nullptr, "textures/SpriteSheets/mermaid_spritesheet.png");
		AddComponent<MaterialComponent>("Mouse", nullptr, "textures/SpriteSheets/Mouse_spritesheet.png");
		AddComponent<MaterialComponent>("Owl", nullptr, "textures/SpriteSheets/Owl_spritesheet.png");
		AddComponent<MaterialComponent>("Fairy", nullptr, "textures/SpriteSheets/fairy_spritesheet.png");
	
	


	
		
		AddComponent<MaterialComponent>("Water_normal", nullptr, "textures/water_normal.jpg");

		AddComponent<MaterialComponent>("ChessBoard", nullptr, "textures/8x8_checkered_board.png");


		//House:



		//UI elements:
		AddComponent<MaterialComponent>("inventory_closed", nullptr, "textures/UI_elements/inventory_2.png");
		AddComponent<MaterialComponent>("inventory_open", nullptr, "textures/UI_elements/inventory_open.png");

		AddComponent<MaterialComponent>("E button", nullptr, "textures/UI_elements/E_button.png");
		AddComponent<MaterialComponent>("inventory slot", nullptr, "textures/UI_elements/inventory_slot_test2.png");

		AddComponent<MaterialComponent>("number 1", nullptr, "textures/UI_elements/number_1.png");
		AddComponent<MaterialComponent>("number 2", nullptr, "textures/UI_elements/number_2.png");
		AddComponent<MaterialComponent>("number 3", nullptr, "textures/UI_elements/number_3.png");

		AddComponent<MaterialComponent>("Game Title", nullptr, "textures/UI_elements/Game Title.png");
		AddComponent<MaterialComponent>("quest", nullptr, "textures/UI_elements/cursor/cursor_hovered.png");


		//Dialogue design
		AddComponent<MaterialComponent>("Dialogue_profileFrame", nullptr, "textures/UI_elements/frame.png");
		AddComponent<MaterialComponent>("Dialogue_profileBackground", nullptr, "textures/UI_elements/frame_bg.png");
		AddComponent<MaterialComponent>("Dialogue_background", nullptr, "textures/UI_elements/text_tablet.png");


		//Dialogue pfps
		//AddComponent<MaterialComponent>("Game Title", nullptr, "textures/UI_elements/Game Title.png");
		AddComponent<MaterialComponent>("Player_surprise", nullptr, "textures/UI_elements/pfps/player_1.png");
		AddComponent<MaterialComponent>("Player_question", nullptr, "textures/UI_elements/pfps/player_2.png");
		AddComponent<MaterialComponent>("Player_laugh", nullptr, "textures/UI_elements/pfps/player_3.png");
		AddComponent<MaterialComponent>("Player_smile", nullptr, "textures/UI_elements/pfps/player_4.png");


		AddComponent<MaterialComponent>("Owl_1", nullptr, "textures/UI_elements/pfps/mrOwl_1.png");
		AddComponent<MaterialComponent>("Owl_2", nullptr, "textures/UI_elements/pfps/mrOwl_2.png");
		AddComponent<MaterialComponent>("Fairy_1", nullptr, "textures/UI_elements/pfps/fairy_1.png");
		AddComponent<MaterialComponent>("Fairy_2", nullptr, "textures/UI_elements/pfps/fairy_2.png");
		AddComponent<MaterialComponent>("Mouse_1", nullptr, "textures/UI_elements/pfps/mrsMouse_1.png");
		AddComponent<MaterialComponent>("Mouse_2", nullptr, "textures/UI_elements/pfps/mrsMouse_2.png");



		//Pickable objects ROOM
		AddComponent<MaterialComponent>("Books", nullptr, "textures/Items/books.png");
		AddComponent<MaterialComponent>("Bear", nullptr, "textures/Items/item_bear.png");
		AddComponent<MaterialComponent>("MoonTrinket", nullptr, "textures/Items/moon_trinket.png");

		AddComponent<MaterialComponent>("arrowLeft", nullptr, "textures/UI_elements/arrow_L.png");
		AddComponent<MaterialComponent>("arrowRight", nullptr, "textures/UI_elements/arrow_r.png");
	


	//AddComponent<MaterialComponent>("Doll", nullptr, "textures/doll.png");
	{
		Profiler timer("ON CREATE TIME");
		OnCreate(); }
	ListAllComponents();
}

AssetManager::~AssetManager() {
	RemoveAllComponents();
}

bool AssetManager::OnCreate() {
	/// This is why auto cool sometimes
	static int count = 0;
	
	for (std::pair<const char*, Ref<Component>> c : componentCatalog) {
		{
			std::cout << count<<std::endl;
			Profiler timer("Item");
			if (c.second->OnCreate() == false) {
				/// Report error
				return false;
			}
		}
		count++;
	}
	return true;
}


void AssetManager::RemoveAllComponents() {
	componentCatalog.clear();
}

void AssetManager::ListAllComponents() const {
	for (auto c : componentCatalog) {
		std::cout << c.first << ": " << typeid(*(c.second.get()) ).name() << std::endl;
	}
}

