#include "stdafx.h"
#include "ProfilerUI.h"
#include "Imgui/imgui.h"
#include "Profiler.h"

using namespace std;

void ProfilerUI::RenderUIWindow(bool* isWindowOpen)
{
    ImGuiWindowFlags window_flags = 0;

    // Main body of the Demo window starts here.
    if (!ImGui::Begin("Profiler", isWindowOpen, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    shared_ptr<ProfilerFrame> frame = FrameProfiler.GetLastFrameData();
    float frameTime = frame->TimeSpan.GetDuration();

    ImGui::Text("Frame Time mS: %f", frame->TimeSpan.GetDurationMS());
    ImGui::Text("FPS: %f", 1.0f / frameTime);

    ImGui::Separator();


    if (ImGui::BeginTable("Sections", 3))
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Section");
        ImGui::TableNextColumn();
        ImGui::Text("Time mS");
        ImGui::TableNextColumn();
        ImGui::Text("Percent Time used");
    }

    for (auto& section : frame->Sections)
    {
        float sectionPercentage = section.SectionSpan.GetDuration() / frameTime;
        sectionPercentage *= 100.0f;

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text(section.SectionName.c_str());

        ImGui::TableNextColumn();
        ImGui::Text("%8.2f", section.SectionSpan.GetDurationMS());

        ImGui::TableNextColumn();
        ImGui::Text("%5.2f %%", sectionPercentage);



       // ImGui::Text("%s: mS %8.2f  -- %5.2f %%", section.SectionName.c_str(),
       //     section.SectionSpan.GetDurationMS(), sectionPercentage);
    }

    ImGui::EndTable();

    ImGui::End();

}