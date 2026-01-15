#pragma once

#include "st-ui.h"
#include "mod-data.h"

namespace Menu {
	using ux = StyyxUtil::UIUtil;

	namespace Titles {
		inline std::string MOD_TITLE = "Learn Spells From Staves";
		inline std::string SETTINGS_SEC = "Settings";
	}
	namespace Label {
		
		inline std::string system = FontAwesome::UnicodeToUtf8(0xf390) + " System";
		inline std::string save_settings = FontAwesome::UnicodeToUtf8(0xf0c7) + " Save Settings";
		inline std::string restore_defaults = FontAwesome::UnicodeToUtf8(0xf0e2) + " Reset Settings";
	}
	void __stdcall MenuEventListener(SKSEMenuFramework::Model::EventType eventType);
	void RegisterMenuCloseEvent();
	void TranslateMenuNames();
	void RegisterSpellLearnMenu();
	void RestoreFromSettings();	
	void ResetDefaults();
	void RenderSystem();
	void DrawPerkSelector(bool *a_open);
	void ShowPerkTooltip(RE::BGSPerk* perk);	

	namespace Settings {

		namespace Label {
			inline std::string show_spell_learn_notif = "Show Spell Learn Notification";
			inline std::string only_in_combat = "Only In Combat";
			inline std::string spell_xp_multiplier = "Spell XP Multiplier";
			inline std::string concentration_xp_modifier = "Concentration XP Multiplier";
			inline std::string base_xp_needed_for_spell_learning = "Required XP to Learn Spell";
			inline std::string require_perk = "Requires Perk";
			inline std::string spell_learn_perk = "Spell Learn Perk";
			inline std::string log_to_console = "Console Logging";
		}
		namespace Tool {
			inline std::string show_spell_learn_notif = "Toggle Notification for spells learned";
			inline std::string only_in_combat = "Toggle Spell XP gain only in combat";
			inline std::string spell_xp_multiplier = "Overall multiplier for gained spell XP";
			inline std::string concentration_xp_modifier = "Multiplier for spell XP gained by concentration staves";
			inline std::string base_xp_needed_for_spell_learning = "Amount of XP needed to learn a spell";
			inline std::string require_perk = "Toggle to earn Spell XP only if you have a specific perk";
			inline std::string spell_learn_perk = "Perk required to learn spells from staves if 'Requires Perk' is enabled";
			inline std::string log_to_console = "Log affected spell, acquired XP and remaining XP to the console";
		}

		struct Labels {
			const char* key;
			std::string* value;
		};

		inline Labels kLabels[] = {
			{ "$ShowSpellLearnNotification", &Label::show_spell_learn_notif },
			{ "$OnlyInCombat", &Label::only_in_combat },
			{ "$SpellXPMultiplier", &Label::spell_xp_multiplier },
			{ "$ConcentrationXPModifier", &Label::concentration_xp_modifier },
			{ "$BaseXPNeededForSpellLearning", &Label::base_xp_needed_for_spell_learning },
			{ "$RequirePerk", &Label::require_perk },
			{ "$SpellLearnPerk", &Label::spell_learn_perk },
			{ "$LogToConsole", &Label::log_to_console },

			{ "$ToolShowSpellLearnNotification", &Tool::show_spell_learn_notif },
			{ "$ToolOnlyInCombat", &Tool::only_in_combat },
			{ "$ToolSpellXPMultiplier", &Tool::spell_xp_multiplier },
			{ "$ToolConcentrationXPModifier", &Tool::concentration_xp_modifier },
			{ "$ToolBaseXPNeededForSpellLearning", &Tool::base_xp_needed_for_spell_learning },
			{ "$ToolRequirePerk", &Tool::require_perk },
			{ "$ToolSpellLearnPerk", &Tool::spell_learn_perk },
			{ "$ToolLogToConsole", &Tool::log_to_console }, 
		};

		namespace Var {
			inline bool show_spell_learn_notif;
			inline bool only_in_combat;
			inline float spell_xp_multiplier;
			inline float concentration_xp_modifier;
			inline float base_xp_needed_for_spell_learning;
			inline bool require_perk;
			inline uint32_t spell_learn_perk_id;
			inline RE::BGSPerk* spell_learn_perk = nullptr;
			inline bool log_to_console;
		}
		void RenderOptions();
		void __stdcall RenderSettings();



		
		
	}


}
