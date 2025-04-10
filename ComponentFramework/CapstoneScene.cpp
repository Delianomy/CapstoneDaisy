#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "CapstoneScene.h"
#include "MMath.h""
#include <Plane.h>
//#include <PMath.h>

#include "Debug.h"
#include "CameraActor.h"
#include "LightActor.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "SkyBox.h"
#include "Room.h"
#include <string>
using namespace MATH;
//using namespace MATHEX;

//BEDROOM SCENE
CapstoneScene::CapstoneScene() :drawNormals(false), drawOverlay(false) {
	Debug::Info("Created Scene Bedroom: ", __FILE__, __LINE__);
	glGetIntegerv(GL_VIEWPORT, viewport);
}

bool CapstoneScene::OnCreate() {
	Debug::Info("Loading assets Scene Dream: ", __FILE__, __LINE__);
	assetManager = AssetManager::GetInstance();

	defaultCursorTexture = IMG_Load("textures/UI_elements/cursor/cursor_normal.png");
	hoveredCursorTexture = IMG_Load("textures/UI_elements/cursor//cursor_hovered.png");
	
	

	SDL_Surface* highlightTexure = IMG_Load("textures/UI_elements/cursor/hovered.png");

	audioManager = std::make_shared<AudioManager>();
	dialogueSystem = std::make_shared<DialogueSystem>();
	dialogueSystem->SetAudioManager(audioManager);
	dialogueSystem->SetSceneManager(sceneMan);

	if (defaultCursorTexture && hoveredCursorTexture) {
		SDL_SetColorKey(defaultCursorTexture, SDL_TRUE, SDL_MapRGB(defaultCursorTexture->format, 255, 0, 255));
		SDL_SetColorKey(hoveredCursorTexture, SDL_TRUE, SDL_MapRGB(hoveredCursorTexture->format, 255, 0, 255));
	}
	defaultCursor = SDL_CreateColorCursor(defaultCursorTexture, 0, 0);
	hoveredCursor = SDL_CreateColorCursor(hoveredCursorTexture, 0, 0);

	SDL_SetCursor(defaultCursor);



	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("TextureShader");
	Ref<ShaderComponent> simpleTextureShader = assetManager->GetComponent<ShaderComponent>("SimpleTextureShader");

	

	player = std::make_shared<Actor>(nullptr);
	Quaternion orientation = QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f));
	player->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, 0.0f), orientation);
	AddActor(player);
	
	camera = std::make_shared<CameraActor>(player.get());
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -3.8f), Quaternion());
	camera->OnCreate();
	camera->GetProjectionMatrix().print("ProjectionMatrix");
	camera->GetViewMatrix().print("ViewMatrix");
	AddActor(camera);

	bearInInventory = sceneMan->inventory.GetItemByName("Item1");
	if(bearInInventory == nullptr){
		bear = std::make_shared<PickableItem>(
			assetManager,                                          // AssetManager* assMan
			"Item1",                                               // std::string name_
			Vec3(-0.80f, -1.5f, 3.99f),                           // Vec3 pos // Quaternion orient
			1.0f,                                                  // float triggerRadius
			Vec3(0.1f, 0.1f, 0.1f),                               // Vec3 scale
			assetManager->GetComponent<MaterialComponent>("Bear"),// std::shared_ptr<MaterialComponent> material   // std::shared_ptr<MeshComponent> mesh
			assetManager->GetComponent<ShaderComponent>("simpleTextureShader")
		);
		triggerSystem.AddActor(bear);
		AddTransparentActor(bear);
		AddActor(bear);
	}
	else {
		bear = bearInInventory;
	}
	
	moonInInventory = sceneMan->inventory.GetItemByName("Item2");
	if (moonInInventory == nullptr) {
		moonTrinket = std::make_shared<PickableItem>(
			assetManager,                                          // Ref<AssetManager> assMan
			"Item2",                                               // std::string name_
			Vec3(-0.02f, 0.8f, 3.99f),                           // Vec3 pos // Quaternion orient
			1.0f,                                                  // float triggerRadius
			Vec3(0.25f, 0.25f, 0.25f),                               // Vec3 scale
			assetManager->GetComponent<MaterialComponent>("MoonTrinket") // std::shared_ptr<MaterialComponent> material   // std::shared_ptr<MeshComponent> mesh
			, assetManager->GetComponent<ShaderComponent>("simpleTextureShader")
		);
		triggerSystem.AddActor(moonTrinket);
		AddTransparentActor(moonTrinket);
		AddActor(moonTrinket);
	}
	else{
		moonTrinket = moonInInventory;
	}
	

	booksInInventory = sceneMan->inventory.GetItemByName("Item3");
	if (booksInInventory == nullptr) {
		books = std::make_shared<PickableItem>(
			assetManager,                                          // Ref<AssetManager> assMan
			"Item3",                                               // std::string name_
			Vec3(1.8f, 0.32f, -3.99f),                           // Vec3 pos // Quaternion orient
			1.0f,                                                  // float triggerRadius
			Vec3(0.15f, 0.15f, 0.15f),                               // Vec3 scale
			assetManager->GetComponent<MaterialComponent>("Books") // std::shared_ptr<MaterialComponent> material   // std::shared_ptr<MeshComponent> mesh
		, assetManager->GetComponent<ShaderComponent>("simpleTextureShader")
		);
		
		triggerSystem.AddActor(books);
		AddTransparentActor(books);
		AddActor(books);	
	}
	else {
		books = booksInInventory;
	}

	
	audioManager->Play(6, 0.13f);
	

	InitializeDialogue();
	room = std::make_shared<Room>(nullptr, "textures/SkyBoxes/Room/1.png", "textures/SkyBoxes/Room/2.png",
		"textures/SkyBoxes/Room/celling.png", "textures/SkyBoxes/Room/floor.png", "textures/SkyBoxes/Room/3.png",
		"textures/SkyBoxes/Room/4.png", 4.0f);

	room->OnCreate();

	light = std::make_shared<LightActor>(nullptr, LightStyle::DirectionLight, Vec3(0.0f, 5.0f, 1.0f), Vec4(0.85f, 0.6, 0.6f, 0.0f));
	light->OnCreate();

	return true;
}





CapstoneScene::~CapstoneScene() {
	Debug::Info("Deleted Scene Bedroom: ", __FILE__, __LINE__);
	OnDestroy();
}

void CapstoneScene::OnDestroy() {
	Debug::Info("Deleting assets Scene Bedroom: ", __FILE__, __LINE__);
	if (defaultCursor) {
		SDL_FreeCursor(defaultCursor);
		defaultCursor = nullptr;
	}
	if (hoveredCursor) {
		SDL_FreeCursor(hoveredCursor);
		hoveredCursor = nullptr;
	}
	if (defaultCursorTexture) {
		SDL_FreeSurface(defaultCursorTexture);
		defaultCursorTexture = nullptr;
	}
	if (hoveredCursorTexture) {
		SDL_FreeSurface(hoveredCursorTexture);
		hoveredCursorTexture = nullptr;
	}

	auto test = &CapstoneScene::Update;
}	

void CapstoneScene::HandleEvents(const SDL_Event& sdlEvent) {
	static int objID = -1;
	static Vec2 currentMousePos;
	static Vec2	lastMousePos;
	static float flip = 1.0f;
	Ref<TransformComponent> cameraTC;
	Ref<TransformComponent> gameBoardTC;

	/// Handle Camera movement 
	switch (sdlEvent.type) {
	case SDL_MOUSEMOTION:
		mouseX = sdlEvent.motion.x;
		mouseY = sdlEvent.motion.y;
		hovering = isCursoreOnObject(mouseX, mouseY);
		if (hovering != isHovering) {
			isHovering = hovering;
			SDL_SetCursor(isHovering ? hoveredCursor : defaultCursor);
		}
		break;
	case SDL_KEYDOWN:
		cameraTC = camera->GetComponent<TransformComponent>();
		switch (sdlEvent.key.keysym.scancode) {

			
		case SDL_SCANCODE_LEFT:
		{
			Quaternion rotate = cameraTC->GetQuaternion() * QMath::angleAxisRotation(-1.0f, Vec3(0.0f, 1.0f, 0.0f));
			cameraTC->SetTransform(cameraTC->GetPosition(), rotate);
			camera->UpdateViewMatrix();
			light->UpdateUBO();
			break;
		}

		case  SDL_SCANCODE_RIGHT:
		{
			Quaternion rotate = cameraTC->GetQuaternion() * QMath::angleAxisRotation(1.0f, Vec3(0.0f, 1.0f, 0.0f));
			cameraTC->SetTransform(cameraTC->GetPosition(), rotate);
			camera->UpdateViewMatrix();
			light->UpdateUBO();
			break;
		}

		case SDL_SCANCODE_UP:
			cameraTC->SetTransform(cameraTC->GetPosition() + Vec3(0.0f, 0.0f, 0.1f), cameraTC->GetQuaternion());
			camera->UpdateViewMatrix();
			break;

		case SDL_SCANCODE_DOWN:
			cameraTC->SetTransform(cameraTC->GetPosition() + Vec3(0.0f, 0.0f, -0.1f), cameraTC->GetQuaternion());
			camera->UpdateViewMatrix();
			break;

		case SDL_SCANCODE_SPACE:
		{
			/*flip *= -1.0f;
			gameBoardTC = gameboard->GetComponent<TransformComponent>();
			Quaternion start = gameBoardTC->GetQuaternion();
			Quaternion end = QMath::angleAxisRotation(180.0f * flip, Vec3(0.0f, 0.0f, 1.0f)) * start;
			gameBoardTC->SlerpOrientation(start, end, 3.0f);*/
			break;
		}
		}


		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_E:
			inventoryButtonPressed = !inventoryButtonPressed;
			if (inventoryButtonPressed) {
				audioManager->Play(0, 0.5f);
			}
			else {
				audioManager->Play(1, 0.1f);
			}
			
			break;

		case SDL_SCANCODE_Q:
			cameraTC->SetTransform(cameraTC->GetPosition(), cameraTC->GetQuaternion() *
				QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)));
			break;


			case SDL_SCANCODE_A:
				rotatePlayerLeft = true;

				break;
			case SDL_SCANCODE_D:
				rotatePlayerRight = true;
				break;

		

		case SDL_SCANCODE_N:
			if (drawNormals == false) drawNormals = true;
			
			else drawNormals = false;
			break;

		case SDL_SCANCODE_O:
			if (drawOverlay == false) drawOverlay = true;
			else drawOverlay = false;
			break;

		default:
			break;
		}
		break;

	case SDL_KEYUP:
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_A:
			rotatePlayerLeft = false;
			break;

		case SDL_SCANCODE_D:
			rotatePlayerRight = false;
			break;

		default:
			break;

		}
	case SDL_MOUSEBUTTONDOWN:
		if (sdlEvent.button.button == (SDL_BUTTON_RIGHT)) {
	
		}
		if (sdlEvent.button.button == (SDL_BUTTON_LEFT)) {
		
			currentMousePos = Vec2(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y));
			lastMousePos = currentMousePos;
			objID = Pick(sdlEvent.button.x, sdlEvent.button.y);
			printf("0x%X %d\n", objID, objID);
			break;
		}
		break;

	case SDL_MOUSEBUTTONUP:
	{
		if (sdlEvent.button.button == (SDL_BUTTON_RIGHT)) {
		
			break;
		}
		if (sdlEvent.button.button == (SDL_BUTTON_LEFT)) {
		
			break;
		}
		break;
	}
	}
	ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
}

void CapstoneScene::Update(const float deltaTime) {
	DrawUI_imgui();


	dialogueSystem->Render();
	
	if (goRight) {
		currentTime += deltaTime;
		animIndex = static_cast<int>(currentTime / frameSpeed) % 8;  
		
		player->GetComponent<TransformComponent>()->SetPosition(player->GetComponent<TransformComponent>()->GetPosition()
			+ MMath::toMatrix4(player->GetComponent<TransformComponent>()->GetQuaternion()) * lefr_right_Vector);
	}
	if (goLeft) {
		currentTime += deltaTime;
		animIndex = static_cast<int>(currentTime / frameSpeed) % 8;

		player->GetComponent<TransformComponent>()->SetPosition(player->GetComponent<TransformComponent>()->GetPosition()
			- MMath::toMatrix4(player->GetComponent<TransformComponent>()->GetQuaternion()) * lefr_right_Vector);
	}
	if (goForward) {
		currentTime += deltaTime;
		animIndex = static_cast<int>(currentTime / frameSpeed) % 8;

		player->GetComponent<TransformComponent>()->SetPosition(player->GetComponent<TransformComponent>()->GetPosition()
			+ MMath::toMatrix4(player->GetComponent<TransformComponent>()->GetQuaternion()) * forwardVector);
	}

	if (goBackwards) {
		currentTime += deltaTime;
		animIndex = static_cast<int>(currentTime / frameSpeed) % 8;

		player->GetComponent<TransformComponent>()->SetPosition(player->GetComponent<TransformComponent>()->GetPosition()
			- MMath::toMatrix4(player->GetComponent<TransformComponent>()->GetQuaternion()) * forwardVector);
	}

	if (rotatePlayerLeft) {

		playerAngle += 0.4f;
		Quaternion newQuat = QMath::angleAxisRotation(playerAngle, Vec3(0.0f, 1.0f, 0.0f));

		player->GetComponent<TransformComponent>()->SetQuaternion(newQuat);
	}

	if (rotatePlayerRight) {

		playerAngle -= 0.4f;
		Quaternion newQuat = QMath::angleAxisRotation(playerAngle, Vec3(0.0f, 1.0f, 0.0f));

		player->GetComponent<TransformComponent>()->SetQuaternion(newQuat);
	}
	camera->UpdateViewMatrix();
	player->GetComponent<TransformComponent>()->Update(deltaTime);
	audioManager->Update();
	
}


int CapstoneScene::Pick(int x, int y) {
	int index = 0;
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_BLEND);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White background with alpha=1
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get and use the color picker shader
	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("ColorPicker");
	glUseProgram(shader->GetProgram());

	// Set matrices
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetViewMatrix());

	// Render opaque actors with unique colors
	for (GLuint i = 0; i < opaqueActors.size(); i++) {
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, opaqueActors[i]->GetModelMatrix());
		glUniform1ui(shader->GetUniformID("colorID"), i + 1); // Start from 1 to avoid black
		opaqueActors[i]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}

	// Render transparent actors with unique colors
	for (GLuint i = 0; i < transparentActors.size(); i++) {
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transparentActors[i]->GetModelMatrix());
		glUniform1ui(shader->GetUniformID("colorID"), opaqueActors.size() + i + 1); // Continue numbering from opaque actors
		transparentActors[i]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}

	// Unbind shader
	glUseProgram(0);

	// Read pixel color at mouse position
	unsigned char pixel[4];
	glReadPixels(x, viewport.height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

	// Convert RGB to color index
	GLuint colorIndex = pixel[0] + (pixel[1] << 8) + (pixel[2] << 16);

	// Debug output
	printf("Read pixel color: R=%d, G=%d, B=%d, Index=0x%06X\n",
		pixel[0], pixel[1], pixel[2], colorIndex);

	// Check if background was clicked (white)

	
	if (colorIndex == 0xFFFFFF) {
		index = -1;
	}
	else {
		index= colorIndex - 1; // Subtract 1 to get back to 0-based index
	}

	if (index >= 0 && index < dialogueSequences.size()) {
		// Clear any existing dialogues
		dialogueSystem->ClearDialogues();

		// Add the dialogues for this object
		for (const auto& dialogue : dialogueSequences[index]) {
			dialogueSystem->AddDialogueToSequence(dialogue);
		}

		dialogueSystem->OpenDialogue(0);
	}


	glEnable(GL_BLEND);

	return index;
	///HERE AFTER WE PICKED THEOBJECT START THE DIALOGUE
}

void CapstoneScene::Render() const {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID());
	

	Ref<ShaderComponent> roomShader = room->GetComponent<ShaderComponent>();
	glUseProgram(roomShader->GetProgram());
	glUniformMatrix4fv(roomShader->GetUniformID("modelMatrix"), 1, GL_FALSE, room->GetModelMatrix());
	glUniformMatrix4fv(roomShader->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetViewMatrix());
	glUniformMatrix4fv(roomShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	std::dynamic_pointer_cast<Room>(room)->Render();
	glUseProgram(0);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	for (auto transparentActor : transparentActors) {
		glUseProgram(transparentActor->GetComponent<ShaderComponent>()->GetProgram());
		glUniform1f(transparentActor->GetComponent<ShaderComponent>()->GetUniformID("index"), animIndex);
		glUniformMatrix4fv(transparentActor->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, transparentActor->GetModelMatrix());
		if (transparentActor->GetComponent<MaterialComponent>()->getTextureID()) {
			glBindTexture(GL_TEXTURE_2D, transparentActor->GetComponent<MaterialComponent>()->getTextureID());
			transparentActor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
		}
	}


	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}




bool CapstoneScene::isCursoreOnObject(int x, int y)
{
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_BLEND);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get and use the color picker shader
	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("ColorPicker");
	glUseProgram(shader->GetProgram());

	// Set matrices
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetViewMatrix());

	// Render transparent actors with unique colors
	for (GLuint i = 0; i < transparentActors.size(); i++) {
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transparentActors[i]->GetModelMatrix());
		glUniform1ui(shader->GetUniformID("colorID"), opaqueActors.size() + i + 1);
		transparentActors[i]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}

	// Unbind shader
	glUseProgram(0);

	// Read pixel color at mouse position
	unsigned char pixel[4];
	glReadPixels(x, viewport.height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

	// Convert RGB to color index
	GLuint colorIndex = pixel[0] + (pixel[1] << 8) + (pixel[2] << 16);

	// Restore OpenGL state
	glEnable(GL_BLEND);

	// Return true if we're hovering over a transparent actor
	return colorIndex != 0xFFFFFF;
}

void CapstoneScene::InitializeDialogue() {

	dialogueSequences.resize(transparentActors.size());


	//Teddy Bear

	if (!bearInInventory) {
			unsigned int textureID = assetManager->GetComponent<MaterialComponent>("Player_smile")->getTextureID();
			Dialogue teddy_bear = Dialogue("Daisy", "Oh! Thats my Teddy bear!", textureID);
			teddy_bear.isFromPickableItem = true;
			teddy_bear.SetOnItemTaken([this]() {sceneMan->AddItemToInventory(bear, 0); });
			dialogueSequences[0].push_back(teddy_bear);

			textureID = assetManager->GetComponent<MaterialComponent>("Player_laugh")->getTextureID();
			teddy_bear = Dialogue("Daisy", "My mom gave him to me on Christmas.", textureID);
			dialogueSequences[0].push_back(teddy_bear);

			textureID = assetManager->GetComponent<MaterialComponent>("Player_smile")->getTextureID();
			teddy_bear = Dialogue("Daisy", "Maybe this could be a good gift to Mr Owl and Mrs Mouse?", textureID);
			dialogueSequences[0].push_back(teddy_bear);
	}
	
	
	if (!moonInInventory) {
	
		unsigned int textureID = assetManager->GetComponent<MaterialComponent>("Player_question")->getTextureID();
		Dialogue trinket = Dialogue("Daisy", "This is my favourite thing. Cant tell whats it made of.", textureID);
		trinket.SetOnItemTaken([this]() {sceneMan->AddItemToInventory(moonTrinket, 1); });
		trinket.isFromPickableItem = true;
		dialogueSequences[1].push_back(trinket);

		textureID = assetManager->GetComponent<MaterialComponent>("Player_smile")->getTextureID();
		trinket = Dialogue("Daisy", "But in the night when the light hits it, its very shiny.", textureID);
		trinket.SetOnItemTaken([this]() {sceneMan->AddItemToInventory(moonTrinket, 1); });
		dialogueSequences[1].push_back(trinket);
	}

	if (!booksInInventory) {

		unsigned int textureID = assetManager->GetComponent<MaterialComponent>("Player_surprise")->getTextureID();
		Dialogue books_dialogue = Dialogue("Daisy", "I think my mom put those books here, because she wanted to read them", textureID);
		books_dialogue.SetOnItemTaken([this]() {sceneMan->AddItemToInventory(books, 2); });
		books_dialogue.isFromPickableItem = true;
		dialogueSequences[2].push_back(books_dialogue);

		textureID = assetManager->GetComponent<MaterialComponent>("Player_smile")->getTextureID();
		books_dialogue = Dialogue("Daisy", "Eh. Reading is not as fun as drawing anyways.", textureID);
		books_dialogue.SetOnItemTaken([this]() {sceneMan->AddItemToInventory(books, 2); });
		dialogueSequences[2].push_back(books_dialogue);
	}

}

void CapstoneScene::DrawUI_imgui()
{
	GLuint inventoryTextureID;
	if (inventoryButtonPressed) {
		inventoryTextureID = assetManager->GetComponent<MaterialComponent>("inventory_closed")->getTextureID();
	}
	else {
		inventoryTextureID = assetManager->GetComponent<MaterialComponent>("inventory_open")->getTextureID();
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();



	//Inventory 
	ImGui::SetNextWindowPos(ImVec2(220, 750)); // Set the window position here (x, y)
	ImGui::SetNextWindowSize(ImVec2(220, 220)); // Set the window size

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin("inventory", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
	//ImGui::SetCursorPos(ImVec2(-400, 400));
	//draw image
	ImGui::Image((intptr_t)inventoryTextureID, ImVec2(200, 200));
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();



	if (inventoryButtonPressed) {
		

		//E - button
		ImGui::SetNextWindowPos(ImVec2(360, 860)); // Set the window position here (x, y)
		ImGui::SetNextWindowSize(ImVec2(120, 120)); // Set the window size

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("E button", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
		//draw image
		ImGui::Image((intptr_t)assetManager->GetComponent<MaterialComponent>("E button")->getTextureID(), ImVec2(99, 99));
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

	}
	if (!inventoryButtonPressed) {

		/// ---------------------------INVENTORY SLOT 1 ----------------------------------------------------------------------
		ImGui::SetNextWindowPos(ImVec2(250, 590)); // Set the window position here (x, y)
		ImGui::SetNextWindowSize(ImVec2(150, 150)); // Set the window size
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("inventory slot 1", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
		ImGui::Image((intptr_t)assetManager->GetComponent<MaterialComponent>("inventory slot")->getTextureID(), ImVec2(140, 140));
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

		/// Drawing the object texture on top
		if (sceneMan->inventory.items[0] != nullptr) {
			ImGui::SetNextWindowPos(ImVec2(285, 625)); // Window position + 35
			ImGui::SetNextWindowSize(ImVec2(150, 150)); // Set the window size
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::Begin("Item slot 1", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
			ImGui::Image((intptr_t)sceneMan->inventory.items[0]->GetComponent<MaterialComponent>()->getTextureID(), ImVec2(80, 80));
			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}
		ImGui::SetNextWindowPos(ImVec2(285 + 35, 625 + 35)); // Set the window position here (x, y)
		ImGui::SetNextWindowSize(ImVec2(80, 80)); // Set the window size
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("1 button", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
		ImGui::Image((intptr_t)assetManager->GetComponent<MaterialComponent>("number 1")->getTextureID(), ImVec2(80, 80));
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();




		/// ---------------------------INVENTORY SLOT 2 ----------------------------------------------------------------------

		ImGui::SetNextWindowPos(ImVec2(395, 665)); // Set the window position here (x, y)
		ImGui::SetNextWindowSize(ImVec2(150, 150)); // Set the window size
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("inventory slot 2", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
		ImGui::Image((intptr_t)assetManager->GetComponent<MaterialComponent>("inventory slot")->getTextureID(), ImVec2(140, 140));
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

		if (sceneMan->inventory.items[1] != nullptr) {
			ImGui::SetNextWindowPos(ImVec2(395 + 35, 665 + 35));
			ImGui::SetNextWindowSize(ImVec2(150, 150));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::Begin("Item slot 2", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
			ImGui::Image((intptr_t)sceneMan->inventory.items[1]->GetComponent<MaterialComponent>()->getTextureID(), ImVec2(80, 80));
			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}

		ImGui::SetNextWindowPos(ImVec2(475, 735)); // Set the window position here (x, y)
		ImGui::SetNextWindowSize(ImVec2(80, 80)); // Set the window size
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("2 button", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
		ImGui::Image((intptr_t)assetManager->GetComponent<MaterialComponent>("number 2")->getTextureID(), ImVec2(80, 80));
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();



		/// ---------------------------INVENTORY SLOT 3 ----------------------------------------------------------------------
		ImGui::SetNextWindowPos(ImVec2(465, 815)); // Set the window position here (x, y)
		ImGui::SetNextWindowSize(ImVec2(150, 150)); // Set the window size
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("inventory slot 3", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
		ImGui::Image((intptr_t)assetManager->GetComponent<MaterialComponent>("inventory slot")->getTextureID(), ImVec2(140, 140));
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

		if (sceneMan->inventory.items[2] != nullptr) {
			ImGui::SetNextWindowPos(ImVec2(465 + 35, 815 + 35)); // Set the window position here (x, y)
			ImGui::SetNextWindowSize(ImVec2(150, 150)); // Set the window size
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::Begin("Item slot 3", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
			ImGui::Image((intptr_t)sceneMan->inventory.items[2]->GetComponent<MaterialComponent>()->getTextureID(), ImVec2(80, 80));
			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}

		ImGui::SetNextWindowPos(ImVec2(545, 885)); // Set the window position here (x, y)
		ImGui::SetNextWindowSize(ImVec2(80, 80)); // Set the window size
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("3 button", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
		ImGui::Image((intptr_t)assetManager->GetComponent<MaterialComponent>("number 3")->getTextureID(), ImVec2(80, 80));
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();


	}


}


