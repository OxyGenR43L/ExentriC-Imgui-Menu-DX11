#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include "imgui_freetype.h"
#include <d3d11.h>
#include <tchar.h>

#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")

#include "font.h"
#include "image.h"

#include "notification.h"

POINT mouse;
HWND hwnd;
RECT rc;

namespace image
{
    ID3D11ShaderResourceView* logo = nullptr;
    ID3D11ShaderResourceView* user = nullptr;
    ID3D11ShaderResourceView* background = nullptr;

    ID3D11ShaderResourceView* aimbot = nullptr;
    ID3D11ShaderResourceView* visuals = nullptr;
    ID3D11ShaderResourceView* skins = nullptr;
    ID3D11ShaderResourceView* misc = nullptr;
    ID3D11ShaderResourceView* settings = nullptr;

}

namespace font
{
    ImFont* inter_bold = nullptr;
    ImFont* inter_child = nullptr;
    ImFont* manrope_widgets = nullptr;

    ImFont* icon_font = nullptr;
}

namespace var
{
    namespace checkbox
    {
        bool keybind = false;
    }

    namespace keybind_checkbox
    {
        bool keybind = false;
        int key, m;
    }

    namespace keybind_pickerbox
    {
        float color_picker[4] = { 179 / 255.f, 80 / 255.f, 220 / 255.f, 1.f };
        bool checkbox = false;
        int key, m;
    }

    namespace picker_box
    {
        bool accent = false;
        float color[4] = { 179 / 255.f, 80 / 255.f, 220 / 255.f, 1.f };
    }

    namespace combo
    {
        int select = 0;
        const char* items[3]{ "Selected", "Selected ", "Selected  " };
    }

    namespace multi_combo
    {
        bool multi_num[10] = { true, true, true, true, true, false, false, false, false, false };
        const char* multi_items[10] = { "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten" };
    }

    namespace input_text
    {
        char input[16] = { "" };
    }

    namespace keybind
    {
        int key, m;
    }

    namespace slider
    {
        int slider_int = 50;
        float slider_float = 1.f;
    }

}

namespace addition
{
    const char* cheat_name = "ExentriC";
    const char* user_name = "Admin";
    ImVec2 logo_size = ImVec2(30, 30);
    ImVec2 user_circle_size = ImVec2(26, 26);

    float menu_opticaly = 1.f;
    bool menu_show = true;
}

DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;

static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main(int, char**)
{
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"ExentriC Imgui Menu", WS_POPUP, 0, 0, 1920, 1080, nullptr, nullptr, wc.hInstance, nullptr);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   

    ImFontConfig cfg;
    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor;

    font::manrope_widgets = io.Fonts->AddFontFromMemoryTTF(manrope_medium, sizeof(manrope_medium), 18.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::inter_child = io.Fonts->AddFontFromMemoryTTF(inter_bold, sizeof(inter_bold), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::inter_bold = io.Fonts->AddFontFromMemoryTTF(inter_bold, sizeof(inter_bold), 14.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    font::icon_font = io.Fonts->AddFontFromMemoryTTF(icon_font, sizeof(icon_font), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    
    D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
    if (image::background == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, background, sizeof(background), &info, pump, &image::background, 0);
    if (image::logo == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, cheat_logo, sizeof(cheat_logo), &info, pump, &image::logo, 0);
    if (image::user == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, user_panel, sizeof(user_panel), &info, pump, &image::user, 0);

    if (image::aimbot == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, aimbot, sizeof(aimbot), &info, pump, &image::aimbot, 0);
    if (image::visuals == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, visuals, sizeof(visuals), &info, pump, &image::visuals, 0);
    if (image::skins == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, skins, sizeof(skins), &info, pump, &image::skins, 0);
    if (image::misc == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, misc, sizeof(misc), &info, pump, &image::misc, 0);
    if (image::settings == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, settings, sizeof(settings), &info, pump, &image::settings, 0);

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool done = false;
    while (!done)
    {

        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {

                c::accent_color = { var::picker_box::color[0], var::picker_box::color[1], var::picker_box::color[2], 1.f };
                c::accent_color_low = { var::picker_box::color[0], var::picker_box::color[1], var::picker_box::color[2], 0.5f };

            ImGui::GetBackgroundDrawList()->AddImage(image::background, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));

            ImGuiStyle* style = &ImGui::GetStyle();

            style->WindowPadding = ImVec2(0, 0);
            style->ItemSpacing = ImVec2(20, 20);
            style->ScrollbarSize = 8.f;

            ImGui::SetNextWindowSizeConstraints(ImVec2(c::bg::size), ImGui::GetIO().DisplaySize);

            if (GetAsyncKeyState(VK_INSERT) & 0x01) addition::menu_show = !addition::menu_show;

            addition::menu_opticaly = ImLerp(addition::menu_opticaly, addition::menu_show ? 1.f : 0.f, ImGui::GetIO().DeltaTime * 10.f);

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, addition::menu_opticaly);

            if (addition::menu_opticaly > 0.01f) {
                ImGui::Begin("NULL", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
                {
                    const ImVec2& pos = ImGui::GetWindowPos(), spacing = style->ItemSpacing, region = ImGui::GetContentRegionMax();

                    ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + ImVec2(160, region.y), ImGui::GetColorU32(c::bg::background), c::bg::rounding);

                    ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + ImVec2(160, 48), ImGui::GetColorU32(c::bg::Platinum), c::bg::rounding, ImDrawCornerFlags_Top);
                    ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(0, region.y - 48), pos + ImVec2(160, region.y), ImGui::GetColorU32(c::bg::Platinum), c::bg::rounding, ImDrawCornerFlags_Bot);
                    ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(170, 0), pos + ImVec2(region), ImGui::GetColorU32(c::bg::background), c::bg::rounding);
                    ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(170, 0), pos + ImVec2(region.x, 48), ImGui::GetColorU32(c::bg::Platinum), c::bg::rounding, ImDrawCornerFlags_Top);

                    ImGui::GetBackgroundDrawList()->AddLine(pos + ImVec2(170, 48), pos + ImVec2(region.x, 48), ImGui::GetColorU32(c::accent_color), 1.f);
                    ImGui::GetBackgroundDrawList()->AddLine(pos + ImVec2(0, region.y - 48), pos + ImVec2(160, region.y - 48), ImGui::GetColorU32(c::accent_color), 1.f);
                    ImGui::GetBackgroundDrawList()->AddLine(pos + ImVec2(0, 48), pos + ImVec2(160, 48), ImGui::GetColorU32(c::accent_color), 1.f);

                    ImGui::GetBackgroundDrawList()->AddImage(image::logo, pos + (ImVec2(160, 48) - addition::logo_size) / 2, pos + (ImVec2(160, 48) + addition::logo_size) / 2, ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(c::accent_color));
                    ImGui::GetBackgroundDrawList()->AddImage(image::user, pos + ImVec2(region.x - ((spacing.x / 2) + 26), (48 - addition::user_circle_size.x) / 2), pos + ImVec2(region.x - spacing.x / 2, (48 + addition::user_circle_size.x) / 2), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(c::text::text_active));

                    ImGui::PushFont(font::inter_bold);
                    ImGui::GetBackgroundDrawList()->AddText(pos + ImVec2((160 - ImGui::CalcTextSize(addition::cheat_name).x) / 2, region.y - (48 + ImGui::CalcTextSize(addition::cheat_name).y) / 2), ImGui::GetColorU32(c::text::text), addition::cheat_name);

                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(c::text::text));

                    ImGui::RenderTextClipped(pos + ImVec2(170, 0), pos + ImVec2(region.x - spacing.x - 26, 48), addition::user_name, NULL, NULL, ImVec2(1.0f, 0.5f), NULL);

                    ImGui::PopStyleColor();
                    ImGui::PopFont();

                    static int tabs = 0;
                    static int subtabs = 0;

                    ImGui::SetCursorPos(ImVec2(spacing.x, 48 + spacing.y));
                    ImGui::BeginGroup();
                    {
                        const char* tabs_array[] = { "Aimbot", "Visuals", "Skins", "Misc", "Settings" };
                        ID3D11ShaderResourceView* image_array[5] = { image::aimbot, image::visuals, image::skins, image::misc, image::settings };

                        for (int i = 0; i < 5; i++) if (ImGui::Tabs(tabs == i, image_array[i], tabs_array[i], ImVec2(140 - spacing.x, 30))) tabs = i;

                    }
                    ImGui::EndGroup();

                    ImGui::SetCursorPos(ImVec2(170 + spacing.x, 0));
                    ImGui::BeginGroup();
                    {
                        const char* subtabs_array[] = { "Enemy", "Team", "Others" };

                        for (int i = 0; i < 3; i++) {
                            if (ImGui::Subtab(subtabs == i, subtabs_array[i], ImVec2(60, 48))) subtabs = i;
                            ImGui::SameLine(0, 0);
                        }
                    }
                    ImGui::EndGroup();

                    static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;

                    tab_alpha = ImClamp(tab_alpha + (4.f * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);
                    tab_add = ImClamp(tab_add + (std::round(350.f) * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);

                    if (tab_alpha == 0.f && tab_add == 0.f) active_tab = tabs;

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * style->Alpha);

                    ImGui::SetCursorPos(ImVec2(170, 48) + style->ItemSpacing);
                    ImGui::BeginChild("ChildBar", ImVec2(region) - ImVec2(170, 48) - style->ItemSpacing);
                    {
                        if (active_tab == 0) {

                            ImGui::BeginGroup();
                            {
                                ImGui::CustomBeginChild("GENERAL", ImVec2((region.x / 2) - spacing.x - (spacing.x / 2) - (170 / 2), (region.y - 48 - (spacing.y * 4)) / 3));
                                {

                                    ImGui::Keybindbox("Enabled Aimbot", &var::keybind_checkbox::keybind, &var::keybind_checkbox::key, &var::keybind_checkbox::m);

                                    ImGui::KeybindPickerbox("Silent Aim", &var::keybind_pickerbox::checkbox, &var::keybind_pickerbox::key, &var::keybind_pickerbox::m, var::keybind_pickerbox::color_picker, picker_flags);

                                }
                                ImGui::CustomEndChild();

                                ImGui::CustomBeginChild("OTHER", ImVec2((region.x / 2) - spacing.x - (spacing.x / 2) - (170 / 2), (region.y - 48 - (spacing.y * 4)) / 3));
                                {

                                    ImGui::Combo("Combo", &var::combo::select, var::combo::items, IM_ARRAYSIZE(var::combo::items), 3);

                                    ImGui::MultiCombo("Multi-Combo", var::multi_combo::multi_num, var::multi_combo::multi_items, 10);

                                }
                                ImGui::CustomEndChild();

                                ImGui::CustomBeginChild("MISC", ImVec2((region.x / 2) - spacing.x - (spacing.x / 2) - (170 / 2), (region.y - 48 - (spacing.y * 4)) / 3));
                                {

                                    if (ImGui::Button("Button", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 25)));

                                    ImGui::InputTextEx("##NULL", "Enter your text here", var::input_text::input , 16, ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 25), NULL);

                                }
                                ImGui::CustomEndChild();
                            }
                            ImGui::EndGroup();

                            ImGui::SameLine();

                            ImGui::BeginGroup();
                            {
                                ImGui::CustomBeginChild("AIMBOT", ImVec2((region.x / 2) - spacing.x - (spacing.x / 2) - (170 / 2), (region.y - 48 - (spacing.y * 3)) / 2));
                                {

                                    if (ImGui::Button("Succesfully", ImVec2((ImGui::GetContentRegionMax().x - style->WindowPadding.x - spacing.x / 2) / 2, 25))) ImGui::Notification({ ImGuiToastType_Success, 2000, "Successfully!" });
                                    ImGui::SameLine();
                                    if (ImGui::Button("Warning", ImVec2((ImGui::GetContentRegionMax().x - style->WindowPadding.x - spacing.x / 2) / 2, 25))) ImGui::Notification({ ImGuiToastType_Warning, 2000, "Warning!" });

                                    if (ImGui::Button("Error", ImVec2((ImGui::GetContentRegionMax().x - style->WindowPadding.x - spacing.x / 2) / 2, 25))) ImGui::Notification({ ImGuiToastType_Error, 2000, "Error!" });
                                    ImGui::SameLine();
                                    if (ImGui::Button("Info", ImVec2((ImGui::GetContentRegionMax().x - style->WindowPadding.x - spacing.x / 2) / 2, 25))) ImGui::Notification({ ImGuiToastType_Info, 2000, "Notification" });

                                    if (ImGui::Button("Config", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 25))) ImGui::Notification({ ImGuiToastType_Config, 2000, "Config saved successfully!" });

                                }
                                ImGui::CustomEndChild();

                                ImGui::CustomBeginChild("SETTINGS", ImVec2((region.x / 2) - spacing.x - (spacing.x / 2) - (170 / 2), (region.y - 48 - (spacing.y * 3)) / 2));
                                {

                                    ImGui::Keybind("Keybind", &var::keybind::key , &var::keybind::m);

                                    ImGui::Pickerbox("Accent Color", &var::picker_box::accent, var::picker_box::color, picker_flags);

                                    ImGui::SliderInt("Slider Integer", &var::slider::slider_int, 1, 1000);

                                    ImGui::SliderFloat("Slider Float", &var::slider::slider_float, -1.f, 10.f, "%.3f");

                                }
                                ImGui::CustomEndChild();
                            }
                            ImGui::EndGroup();
                        }
                        else if (active_tab == 1) {

                            ImGui::BeginGroup();
                            {
                                ImGui::CustomBeginChild("SETTINGS", ImVec2((region.x / 2) - spacing.x - (spacing.x / 2) - (170 / 2), (region.y - 48 - (spacing.y * 2))));
                                {

                                }
                                ImGui::CustomEndChild();
                            }
                            ImGui::EndGroup();

                            ImGui::SameLine();

                            ImGui::BeginGroup();
                            {
                                ImGui::CustomBeginChild("MISC", ImVec2((region.x / 2) - spacing.x - (spacing.x / 2) - (170 / 2), (region.y - 48 - (spacing.y * 3)) / 2));
                                {

                                }
                                ImGui::CustomEndChild();

                                ImGui::CustomBeginChild("JUST NAME", ImVec2((region.x / 2) - spacing.x - (spacing.x / 2) - (170 / 2), (region.y - 48 - (spacing.y * 3)) / 2));
                                {

                                }
                                ImGui::CustomEndChild();
                            }
                            ImGui::EndGroup();
                        }
                    
                    }
                    ImGui::EndChild();

                    ImGui::PopStyleVar();

                    ImGui::RenderNotifications();
                }
                ImGui::End();
            }

            ImGui::PopStyleVar();

            ImGui::SetNextWindowSizeConstraints(ImVec2(320, 500), ImGui::GetIO().DisplaySize);

            ImGui::Begin("NULL ONE", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
            {
                const ImVec2& pos = ImGui::GetWindowPos(), spacing = style->ItemSpacing, region = ImGui::GetContentRegionMax();

                ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + ImVec2(region), ImGui::GetColorU32(c::bg::background), c::bg::rounding);

                ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(pos + ImVec2(spacing) * 2, pos + ImVec2(region.x - (spacing.x * 2), spacing.y * 2 + 2), ImColor(255, 0, 0, 255), ImColor(0, 255, 0, 255), ImColor(0, 255, 0, 255), ImColor(255, 0, 0, 255), c::bg::rounding);

                ImGui::GetWindowDrawList()->AddRect(pos + ImVec2(spacing), pos + ImVec2(region) - spacing, GetColorU32(c::accent_color));

            }
            ImGui::End();

        }
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}


bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
