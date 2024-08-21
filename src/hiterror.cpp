#include "hiterror.h"
#include "hooks.h"
#include "overlay.h"
#include <imgui.h>

void hiterror::Render()
{
    if (hiterror::open || (hiterror::config && overlay::open)) {
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::SetNextWindowSize(ImVec2{ (float)width, (float)height });

        ImGui::Begin("Hit Error", 0, bg_enabled || hovered ? ImGuiWindowFlags_NoDecoration : ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
        hovered = ImGui::IsWindowHovered();
        const ImVec2 p = ImGui::GetWindowPos();

        const float x = p.x;
        const float y = p.y;

        ImDrawList* draw_list = ImGui::GetForegroundDrawList();
        ImVec2 size = ImGui::GetWindowSize();

        const ImVec2 center = { size.x / 2, size.y / 2 };

        if (using_ema) {
            // Current Ema
            draw_list->AddRectFilled(
                ImVec2{ center.x - (thickness / 2) + x + hiterror::ema * (width / 256.0f), y },
                ImVec2{ center.x + (thickness / 2) + x + hiterror::ema * (width / 256.0f), size.y + y },
                overlay::ReplaceAlpha(colors::ema, 255)
            );
        }
        else {
            ImGuiIO& io = ImGui::GetIO();
            for (size_t i = 0; i < lines; ++i) {
                ImU32 color = IM_COL32(255, 255, 255, 255);

                switch(buffer[i].judgement) {
                case JUDGEMENT::pgreat:
                    color = overlay::ReplaceAlpha(colors::pgreat, buffer[i].opacity);
                    break;
                case JUDGEMENT::great:
                    color = overlay::ReplaceAlpha(colors::great, buffer[i].opacity);
                    break;
                case JUDGEMENT::good:
                    color = overlay::ReplaceAlpha(colors::good, buffer[i].opacity);
                    break;
                default:
                    color = overlay::ReplaceAlpha(colors::cb, buffer[i].opacity);
                    break;
                }

                draw_list->AddRectFilled(
                    ImVec2{ center.x - (thickness / 2) + x + buffer[i].delta * (width / 256.0f), y },
                    ImVec2{ center.x + (thickness / 2) + x + buffer[i].delta * (width / 256.0f), size.y + y },
                    color
                );
                buffer[i].opacity = overlay::FadeOut(buffer[i].opacity, 255.0, 0.0, 0.5, io.DeltaTime);
            }
        }

        // Center
        draw_list->AddRectFilled(
            ImVec2{ center.x - (thickness / 2) + x, y },
            ImVec2{ center.x + (thickness / 2) + x, size.y + y },
            IM_COL32(255, 255, 255, 255)
        );
        ImGui::End();
    }
}

void hiterror::UpdateEma(int value)
{
    ema = ema + (alpha * (value - ema));
}

void hiterror::InsertBuffer(int delta, JUDGEMENT judgement)
{
    buffer[buffer_current % lines] = note_delta{ delta, judgement, 255.0f };
    ++buffer_current;
}

void hiterror::ClearBuffer()
{
    for (int i = 0; i < BUFFER_MAX_SIZE; ++i) {
        buffer[i] = { 0, JUDGEMENT::pgreat, 0.f };
    }
}