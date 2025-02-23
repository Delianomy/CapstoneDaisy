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


AssetManager::AssetManager() {
	
	AddComponent<MeshComponent>("Sphere", nullptr, "meshes/Sphere.obj");

	AddComponent<MeshComponent>("Square", nullptr, "meshes/Square.obj");
	AddComponent<MeshComponent>("Cube", nullptr, "meshes/Cube.obj");
	AddComponent<MeshComponent>("Plane", nullptr,"meshes/Plane8x8.obj");
	AddComponent<MeshComponent>("Island", nullptr, "meshes/Island.obj");
	AddComponent<MeshComponent>("QuirkyCube", nullptr, "meshes/CubeCore.obj");


	
	AddComponent<ShaderComponent>("DefaultShader",nullptr,"shaders/defaultVert.glsl", "shaders/defaultFrag.glsl");
	AddComponent<ShaderComponent>("ColorPickingShader",nullptr,"shaders/colorPickingVert.glsl", "shaders/colorPickingFrag.glsl");
	AddComponent<ShaderComponent>("TextureShader",nullptr,"shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	AddComponent<ShaderComponent>("ShadowDepthShader",nullptr,"shaders/shadowDepthVert.glsl", "shaders/shadowDepthFrag.glsl");
	AddComponent<ShaderComponent>("DrawNormalsShader",nullptr,"shaders/drawNormalsVert.glsl", "shaders/drawNormalsFrag.glsl","shaders/drawNormalsGeom.glsl");
	AddComponent<ShaderComponent>("WireFrameShader",nullptr,"shaders/wireFrameVert.glsl", "shaders/wireFrameFrag.glsl","shaders/wireFrameGeom.glsl");
	AddComponent<ShaderComponent>("RefularTexture", nullptr, "shaders/textureVertRegular.glsl", "shaders/textureFragRegular.glsl");
	AddComponent<ShaderComponent>("SimpleWavesShader", nullptr, "shaders/waveVert.glsl", "shaders/waveFrag.glsl");
	AddComponent<ShaderComponent>("CoolWaveShader", nullptr, "shaders/prettyWaterVert.glsl", "shaders/prettyWaterFrag.glsl");


	AddComponent<MaterialComponent>("Sprite_Sheet_HEAVY", nullptr, "textures/walking_sprite_sheet.png");

	AddComponent<MaterialComponent>("ChessBoard",nullptr, "textures/8x8_checkered_board.png");
	//AddComponent<MaterialComponent>("Sprite_Sheet", nullptr, "textures/sprite_sheet.png");
	//AddComponent<MaterialComponent>("Sprite_Sheet_JPG", nullptr, "textures/walking_sprite_sheet_test_2.png");

	


	//AddComponent<MaterialComponent>("Doll", nullptr, "textures/doll.png");
	OnCreate();
	ListAllComponents();
}

AssetManager::~AssetManager() {
	RemoveAllComponents();
}

bool AssetManager::OnCreate() {
	/// This is why auto cool sometimes
	for (std::pair<const char*, Ref<Component>> c : componentCatalog) {
		if (c.second->OnCreate()  == false) {
			/// Report error
			return false;
		}
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

