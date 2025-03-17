module;
#pragma once

#include <map>
#include "imgui.h"

export module editor_theme;
import std;

export struct Theme
{
    std::string name = "Theme";
};

export class ThemeManager
{
    friend class ThemeUI;
    typedef void (*ThemeFunction)(void);

    static void DarkTheme();
    static void DefaultTheme();
    static void LightTheme();
    static inline std::map<std::string, ThemeFunction> themes = { {"Dark", DarkTheme}, {"Classic", DefaultTheme}, {"Light", LightTheme}};
    static inline std::string currentTheme;

public:
    static void SetDefaultFont(std::string path);
    static void LoadTheme(std::string name);
    static void LoadTheme(ThemeFunction theme, std::string label);
    static void LoadTheme(Theme theme);
	
};

inline void ThemeManager::SetDefaultFont(std::string path)
{
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(path.c_str(), 15);
}