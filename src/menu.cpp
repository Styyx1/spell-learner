#include "menu.h"

#include "config.h"

// Templates

template <class T>
bool SettingSlider(const char* label, T& slider_var, const T min, const T max, const char* fmt,
                   REX::TTomlSetting<T>& setting, const char* help)
{
    ImGuiMCP::SetNextItemWidth(200.f);
    bool changed =
        ImGuiMCP::SliderScalar(label, ImGuiMCP::ImGuiDataType_::ImGuiDataType_Float, &slider_var, &min, &max, fmt);
    if (changed)
    {
        setting.SetValue(slider_var);
    }
    ImGuiMCP::SameLine();
    StyyxUtil::SKSEMenuUtil::HelpMarker(help);
    return changed;
}

template <class T>
bool SettingSliderINT(const char* label, T& slider_var, const T min, const T max, const char* fmt,
                      REX::TTomlSetting<T>& setting, const char* help)
{

    ImGuiMCP::SetNextItemWidth(200.f);
    bool changed =
        ImGuiMCP::SliderScalar(label, ImGuiMCP::ImGuiDataType_::ImGuiDataType_S32, &slider_var, &min, &max, fmt);
    if (changed)
    {
        setting.SetValue(slider_var);
    }
    ImGuiMCP::SameLine();
    StyyxUtil::SKSEMenuUtil::HelpMarker(help);
    return changed;
}

inline bool SettingCheckbox(const char* label, bool& value_var, REX::TOML::Setting<bool>& setting, const char* help)
{
    bool changed = ImGuiMCP::Checkbox(label, &value_var);
    if (changed)
    {
        setting.SetValue(value_var);
    }
    ImGuiMCP::SameLine();
    StyyxUtil::SKSEMenuUtil::HelpMarker(help);
    return changed;
}

namespace Menu
{
void TranslateMenuNames()
{
    std::string system;
    SKSE::Translation::Translate("$System", system);
    Label::system = FontAwesome::UnicodeToUtf8(0xf390) + " " + system;
    std::string save_settings;
    SKSE::Translation::Translate("$SaveSettings", save_settings);
    Label::save_settings = FontAwesome::UnicodeToUtf8(0xf0c7) + " " + save_settings;
    std::string restore_settings;
    SKSE::Translation::Translate("$RestoreDefaults", restore_settings);
    Label::restore_defaults = FontAwesome::UnicodeToUtf8(0xf0e2) + " " + restore_settings;

    for (auto& entry : Menu::Settings::kLabels)
    {
        std::string translated;
        if (SKSE::Translation::Translate(entry.key, translated))
        {
            *entry.value = translated;
        }
    }

    REX::INFO("Completed translation for menu");
}

void RegisterSpellLearnMenu()
{
    if (!SKSEMenuFramework::IsInstalled())
    {
        return;
    }
    SKSEMenuFramework::SetSection(Titles::MOD_TITLE);
    SKSEMenuFramework::AddSectionItem(Titles::SETTINGS_SEC, Settings::RenderSettings);
    RegisterMenuCloseEvent();
    RestoreFromSettings();
}
void RestoreFromSettings()
{
    using set = Config::Settings;
    using namespace Settings::Var;

    show_spell_learn_notif            = set::show_spell_learn_notif.GetValue();
    only_in_combat                    = set::only_in_combat.GetValue();
    spell_xp_multiplier               = set::spell_xp_multiplier.GetValue();
    concentration_xp_modifier         = set::concentration_xp_modifier.GetValue();
    base_xp_needed_for_spell_learning = set::base_xp_needed_for_spell_learning.GetValue();
    require_perk                      = set::require_perk.GetValue();
    log_to_console                    = set::log_to_console.GetValue();
}
void ResetDefaults()
{
    using set = Config::Settings;
    using namespace Settings::Var;

    show_spell_learn_notif            = true;
    only_in_combat                    = false;
    spell_xp_multiplier               = 1.0f;
    concentration_xp_modifier         = 1.0f;
    base_xp_needed_for_spell_learning = 100.0f;
    require_perk                      = false;
    log_to_console                    = false;


    set::show_spell_learn_notif.SetValue(show_spell_learn_notif);
    set::only_in_combat.SetValue(only_in_combat);
    set::spell_xp_multiplier.SetValue(spell_xp_multiplier);
    set::concentration_xp_modifier.SetValue(concentration_xp_modifier);
    set::base_xp_needed_for_spell_learning.SetValue(base_xp_needed_for_spell_learning);
    set::require_perk.SetValue(require_perk);
    set::log_to_console.SetValue(log_to_console);
    // perk reset
    set::spell_learn_perk.SetValue("Skyrim.esm|0x108a44");
    auto form                       = FormUtil::GetFormFromString(set::spell_learn_perk.GetValue());
    Config::Forms::spell_learn_perk = form ? form->As<RE::BGSPerk>() : nullptr;
    spell_learn_perk                = Config::Forms::spell_learn_perk;
}
void RenderSystem()
{
    ImGuiMCP::NewLine();
    ImGuiMCP::SeparatorText(Label::system.c_str());

    if (ImGuiMCP::Button(Label::save_settings.c_str()))
    {
        Config::Settings::GetSingleton()->UpdateSettings(true);
    }

    ImGuiMCP::SameLine();
    if (ImGuiMCP::Button(Label::restore_defaults.c_str()))
    {
        ResetDefaults();
    }
}
void DrawPerkSelector(bool* a_open)
{

    ImGuiMCP::Begin("Perk Selector", a_open, ImGuiWindowFlags_NoCollapse);

    static char perkFilter[64] = "";

    ImGuiMCP::InputText("Filter", perkFilter, sizeof(perkFilter));

    ImGuiMCP::Text("Registered perks: %zu", Config::Forms::perk_init_map.size());
    ImGuiMCP::Separator();

    ImGuiMCP::BeginChild("perk_list", ImVec2(600, 600));

    for (auto& perk : Config::Forms::perk_init_map)
    {
        if (!perk)
            continue;

        const char* name = perk->GetName();
        if (!name || name[0] == '\0')
            continue;

        if (perkFilter[0] != '\0' && std::string(name).find(perkFilter) == std::string::npos)
            continue;

        bool isSelected = Settings::Var::spell_learn_perk == perk;

        if (ImGuiMCP::Selectable(name, isSelected))
        {
            Settings::Var::spell_learn_perk = perk;

            if (perk)
            {
                Config::Forms::spell_learn_perk = Settings::Var::spell_learn_perk;
                auto file                       = perk->GetFile();
                auto fileName                   = file ? file->GetFilename() : "NO FILE FOUND";
                auto localID                    = perk->GetLocalFormID();
                std::string configWrite         = std::format("{}|0x{:X}", fileName, localID);
                Config::Settings::spell_learn_perk.SetValue(configWrite);
                REX::INFO("Found Perk: {} from {}", Settings::Var::spell_learn_perk->GetName(), configWrite);
            }
            if (a_open)
            {
                *a_open = false;
            }
        }
        if (ImGuiMCP::IsItemHovered(ImGuiHoveredFlags_::ImGuiHoveredFlags_AllowWhenOverlapped))
        {
            ShowPerkTooltip(perk);
        }
    }
    ImGuiMCP::EndChild();
    ImGuiMCP::Separator();

    if (Settings::Var::spell_learn_perk)
    {
        ImGuiMCP::Text("Selected:");
        ImGuiMCP::Text("Name: %s", Settings::Var::spell_learn_perk->GetName());
        ImGuiMCP::Text("FormID: %08X", Settings::Var::spell_learn_perk->GetFormID());
    }
    else
    {
        ImGuiMCP::TextDisabled("No perk selected");
    }
    ImGuiMCP::End();
}
void ShowPerkTooltip(RE::BGSPerk* perk)
{
    if (ImGuiMCP::BeginItemTooltip())
    {
        ImGuiMCP::PushTextWrapPos(ImGuiMCP::GetFontSize() * 35.0f);
        ImGuiMCP::Text("Name: %s", perk->GetName());
        ImGuiMCP::Text("FormID: %08X", perk->GetFormID());
        ImGuiMCP::PopTextWrapPos();
        ImGuiMCP::EndTooltip();
    }
}
} // namespace Menu

void Menu::Settings::RenderOptions()
{
    // https://pavetr1337.github.io/colorpicker-for-imgui/
    ImGuiMCP::TextColored(ImVec4(0.961f, 0.616f, 0.2f, 1.f), Titles::MOD_TITLE.c_str());
    using set = Config::Settings;

    SettingCheckbox(Label::show_spell_learn_notif.c_str(), Var::show_spell_learn_notif, set::show_spell_learn_notif,
                    Tool::show_spell_learn_notif.c_str());
    ImGuiMCP::SameLine();
    SettingCheckbox(Label::only_in_combat.c_str(), Var::only_in_combat, set::only_in_combat,
                    Tool::only_in_combat.c_str());

    SettingSlider(Label::spell_xp_multiplier.c_str(), Var::spell_xp_multiplier, 0.1f, 5.0f, "%.2f",
                  set::spell_xp_multiplier, Tool::spell_xp_multiplier.c_str());
    SettingSlider(Label::concentration_xp_modifier.c_str(), Var::concentration_xp_modifier, 0.1f, 5.0f, "%.2f",
                  set::concentration_xp_modifier, Tool::concentration_xp_modifier.c_str());
    SettingSlider(Label::base_xp_needed_for_spell_learning.c_str(), Var::base_xp_needed_for_spell_learning, 10.0f,
                  1000.0f, "%.0f", set::base_xp_needed_for_spell_learning,
                  Tool::base_xp_needed_for_spell_learning.c_str());

    SettingCheckbox(Label::require_perk.c_str(), Var::require_perk, set::require_perk, Tool::require_perk.c_str());
    ImGuiMCP::SameLine();
    SettingCheckbox(Label::log_to_console.c_str(), Var::log_to_console, set::log_to_console,
                    Tool::log_to_console.c_str());

    if (Config::Forms::spell_learn_perk)
    {
        Var::spell_learn_perk = Config::Forms::spell_learn_perk;
    }

    std::string PerkName = Var::spell_learn_perk ? Var::spell_learn_perk->GetName() : "No Perk Selected";
    ImGuiMCP::Text(PerkName.c_str());

    static bool showPerkSelector = false;
    if (ImGuiMCP::Button("Search For Perk"))
    {
        showPerkSelector = true;
    }
    if (showPerkSelector)
    {
        DrawPerkSelector(&showPerkSelector);
    }
}

void __stdcall Menu::Settings::RenderSettings()
{
    using set = Config::Settings;
    FontAwesome::PushSolid();
    RenderOptions();
    ImGuiMCP::NewLine();
    RenderSystem();
    FontAwesome::Pop();
}

void __stdcall Menu::MenuEventListener(SKSEMenuFramework::Model::EventType eventType)
{
    static bool saving = true;
    if (eventType == SKSEMenuFramework::Model::EventType::kCloseMenu)
    {
        if (saving)
        {
            saving = false;
            REX::INFO("Auto-saving settings on menu close");
            auto dummy_set = Config::Settings::base_xp_needed_for_spell_learning.GetValue();
            Config::Settings::base_xp_needed_for_spell_learning.SetValue(dummy_set);
            Config::Settings::GetSingleton()->UpdateSettings(true);
        }
    }
}
SKSEMenuFramework::Model::Event* event;
void Menu::RegisterMenuCloseEvent()
{
    event = new SKSEMenuFramework::Model::Event(MenuEventListener, 1);
    if (event)
    {
        REX::INFO("Registered skse menu open/close event");
    }
}
