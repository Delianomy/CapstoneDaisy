#ifndef DIALOGUE_SYSTEM_H
#define DIALOGUE_SYSTEM_H
#include <vector>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
class Dialogue {
public:
	const char* NPCname;
	const char* dialogueText;

	Dialogue(const char* NPCname_, const char* dialogueText_) :NPCname(NPCname_), dialogueText(dialogueText_) {}

};

class DialogueSystem
{
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
        if (!isDialogueOpen || dialogues.empty()) {
            return;
        }

        ImGui::SetNextWindowPos(ImVec2(500, 750)); // Set the window position
        ImGui::SetNextWindowSize(ImVec2(220, 220)); // Set the window size
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f)); // Slight transparency
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::Begin("Dialogue", &isDialogueOpen,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoScrollbar);

        // Display NPC name
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f),
            dialogues[currentDialogueIndex].NPCname);

        // Display dialogue text
        ImGui::TextWrapped(dialogues[currentDialogueIndex].dialogueText);

        // Next/Close button
        if (ImGui::Button("Close")) {
            isDialogueOpen = false;
        }

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