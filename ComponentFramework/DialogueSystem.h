#ifndef DIALOGUE_SYSTEM_H
#define DIALOGUE_SYSTEM_H
#include <vector>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "AssetManager.h"
#include "AudioSystem.h"
class Dialogue {
public:
	const char* NPCname;
	const char* dialogueText;
    unsigned int textureID;

	Dialogue(const char* NPCname_, const char* dialogueText_, unsigned int textureID_) :NPCname(NPCname_), dialogueText(dialogueText_), textureID(textureID_) {}

};

class DialogueSystem
{
    ImGuiIO& io = ImGui::GetIO();
    ImFont* customFont;
    Ref<AssetManager> assetMan;
    Ref<AudioManager> audioManager;


	std::vector<Dialogue> dialogues;
	int currentDialogueIndex = 0;
	bool isDialogueOpen = false;
    bool wasDialogueOpen = false;
public:

    DialogueSystem() {
       /// customFont = io.Fonts->AddFontFromFileTTF("fonts/lunchds.ttf", 20.0f);
      
    }
    void SetAudioManager(Ref<AudioManager> audio) {
        audioManager = audio;
    }

    void ClearDialogues() {
        dialogues.clear();
        currentDialogueIndex = 0;
    }


	void AddDialogueToSequence(Dialogue dialogue_) {
		dialogues.push_back(dialogue_);
	}
	void OpenDialogue(int index) {
        if (audioManager) {
            std::cout << "Playing opening sound..." << std::endl;
            audioManager->Play(2, 1.0f);
        }
        else {
            std::cout << "Audio manager not initialized in OpenDialogue!" << std::endl;
        }
        if (isDialogueOpen) {
            ClearDialogues();
            CloseDialogue();
        }

		if (index >= 0 && index < dialogues.size()) {
			currentDialogueIndex = index;
			isDialogueOpen = true;
            //audioManager->PlaySound("ost/UI_sounds/Retro8.wav");
		}
       
	}


    void Render() {
        int screenWidth = 1920;
        int screenHeight = 1080;
        int dialogueWidth = 600; // Full width
        int dialogueHeight = 700; // 
        wasDialogueOpen = isDialogueOpen;
        if (!isDialogueOpen || dialogues.empty()) {
            if (wasDialogueOpen && !isDialogueOpen) {
               
            }
            return;
        }




        ImGui::SetNextWindowPos(ImVec2((screenWidth - dialogueWidth) * 0.5f, screenHeight - dialogueHeight + 320)); // Set the window position
        ImGui::SetNextWindowSize(ImVec2(dialogueWidth, 200)); // Set the window size
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); 
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::Begin("Dialogue", &isDialogueOpen,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoScrollbar);

        Dialogue& currentDialogue = dialogues[currentDialogueIndex];
        // **Centered NPC Name at the Top**
        ImGui::SetCursorPosX((dialogueWidth - ImGui::CalcTextSize(currentDialogue.NPCname).x) * 0.5f);
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", currentDialogue.NPCname); // Yellow name
        ImGui::Separator();
        ImGui::Columns(2, nullptr, false);

        // **Left Column: Profile Picture**
        ImGui::SetColumnWidth(0, 150); // Adjust based on the size of the profile picture
        if (currentDialogue.textureID) {
            ImGui::Image((intptr_t)currentDialogue.textureID, ImVec2(140, 140));
        }


        ImGui::NextColumn();

        // **Right Column: Dialogue Text (Starts Lower & Is Larger)**
        ImGui::SetCursorPosY(40); 
        ImGui::PushFont(ImGui::GetFont()->Scale > 1.2f ? ImGui::GetFont() : ImGui::GetFont()); 
        //ImGui::PushFont(customFont);
        ImGui::TextWrapped(currentDialogue.dialogueText);
        ImGui::PopFont();

        ImGui::SetCursorPosX(dialogueWidth - 130);
        ImGui::SetCursorPosY( 160 );// Position button to the right
        if (ImGui::Button("Next")) {
            if (audioManager) {
                audioManager->Play(3, 1.0f);
            }
            if (currentDialogueIndex < dialogues.size() - 1) {
                currentDialogueIndex++;
            }
            else {
                 isDialogueOpen = false;
                 ClearDialogues();
            }
        }
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        if (wasDialogueOpen && !isDialogueOpen && audioManager) {
            std::cout << "Playing closing sound..." << std::endl;
            audioManager->Play(4, 1.0f);
        }
    }


	bool IsDialogueOpen() const {
		return isDialogueOpen;
	}

    void CloseDialogue() {
        isDialogueOpen = false;
    }
};

#endif // !DIALOGUE_SYSTEM_H