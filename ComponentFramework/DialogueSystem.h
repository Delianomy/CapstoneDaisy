#ifndef DIALOGUE_SYSTEM_H
#define DIALOGUE_SYSTEM_H
#include <vector>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "AssetManager.h"
class Dialogue {
public:
	const char* NPCname;
	const char* dialogueText;
    unsigned int textureID;

	Dialogue(const char* NPCname_, const char* dialogueText_, unsigned int textureID_) :NPCname(NPCname_), dialogueText(dialogueText_), textureID(textureID_) {}

};

class DialogueSystem
{
    Ref<AssetManager> assetMan;
	std::vector<Dialogue> dialogues;
	int currentDialogueIndex = 0;
	bool isDialogueOpen = false;
public:
	void AddDialogueToSequence(Dialogue dialogue_) {
		dialogues.push_back(dialogue_);
	}
	void OpenDialogue(int index) {
		if (index >= 0 && index < dialogues.size()) {
			currentDialogueIndex = index;
			isDialogueOpen = true;
		}
	}


    void Render() {
        int screenWidth = 1920;
        int screenHeight = 1080;
        int dialogueWidth = 600; // Full width
        int dialogueHeight = 700; // 

        if (!isDialogueOpen || dialogues.empty()) {
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
        ImGui::SetCursorPosY(40); // Push the text a bit lower
        ImGui::PushFont(ImGui::GetFont()->Scale > 1.2f ? ImGui::GetFont() : ImGui::GetFont()); // Make text bigger if possible
        ImGui::TextWrapped(currentDialogue.dialogueText);
        ImGui::PopFont();


        //// Display NPC name
        //if (!dialogues.empty()) {
        //    

        //    ImGui::Columns(2, nullptr, false);  
        //    ImGui::SetColumnWidth(1, 180); // Set fixed width for the profile picture
        //    if (currentDialogue.textureID) {
        //        ImGui::Image(
        //            (intptr_t)dialogues[currentDialogueIndex].textureID,
        //            ImVec2(200, 200));
        //    
        //    }

        //    ImGui::SetCursorPosX(150);
        //    ImGui::SetCursorPosY(10); // Adjust vertical position
        //    ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", currentDialogue.NPCname); // Yellow name


        //    ImGui::NextColumn();
        //    ImGui::Separator(); // Add a line separator
        //    ImGui::TextWrapped(currentDialogue.dialogueText);


        //    ImGui::Columns(1); // Reset columns


        ImGui::SetCursorPosX(dialogueWidth - 80); // Position button to the right
        if (ImGui::Button("Next")) {
            if (currentDialogueIndex < dialogues.size() - 1) {
                currentDialogueIndex++;
            }
            else {
                 isDialogueOpen = false;
            }
        }

   
        //    ImGui::End();
        //    ImGui::PopStyleVar();
        //    ImGui::PopStyleColor();
        //}
        ImGui::End();
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
    }


	bool IsDialogueOpen() const {
		return isDialogueOpen;
	}

    void CloseDialogue() {
        isDialogueOpen = false;
    }
};

#endif // !DIALOGUE_SYSTEM_H