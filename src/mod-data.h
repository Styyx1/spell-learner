#pragma once

namespace MOD {
	inline constexpr std::string_view TOML_PATH_DEFAULT = "Data/SKSE/Plugins/spell-learner.toml";
	inline constexpr std::string_view TOML_PATH_CUSTOM = "Data/SKSE/Plugins/spell-learner_custom.toml";
	inline constexpr std::string_view JSON_PATH_DEFAULT = "Data/SKSE/Plugins/spell-learner-spells.json";
	inline constexpr std::string_view JSON_PATH_CUSTOM = "Data/SKSE/Plugins/spell-learner-spells_custom.json";
	inline constexpr std::string_view JSON_FOLDER = "Data/SKSE/Plugins/SpellsToLearn/";
	static inline constexpr std::string_view SPELL_LEARNING{ "Spell Learning" };

	inline constexpr std::string_view MOD_NAME = "spell-learner";

	struct TranslateMenu : REX::Singleton<TranslateMenu> {

		static inline std::unordered_map<std::string, std::string> menu_translation_map;
		static inline void GetMenuTranslationMap() {
			using namespace SKSE::Translation;

			ParseTranslation(MOD::MOD_NAME.data());

			std::string system;
			Translate("$System", system);
			menu_translation_map["$System"] = system;
			std::string save_settings;
			Translate("$SaveSettings", save_settings);
			menu_translation_map["$SaveSettings"] = save_settings;
			std::string restore_defaults;
			Translate("$RestoreDefaults", restore_defaults);
			menu_translation_map["$RestoreDefaults"] = restore_defaults;

			std::string label_show_spell_learn_notif;
			Translate("$ShowSpellLearnNotification", label_show_spell_learn_notif);
			menu_translation_map["$ShowSpellLearnNotification"] = label_show_spell_learn_notif;
			std::string label_only_in_combat;
			Translate("$OnlyInCombat", label_only_in_combat);
			menu_translation_map["$OnlyInCombat"] = label_only_in_combat;
			std::string label_spell_xp_multiplier;
			Translate("$SpellXPMultiplier", label_spell_xp_multiplier);
			menu_translation_map["$SpellXPMultiplier"] = label_spell_xp_multiplier;
			std::string label_concentration_xp_modifier;
			Translate("$ConcentrationXPModifier", label_concentration_xp_modifier);
			menu_translation_map["$ConcentrationXPModifier"] = label_concentration_xp_modifier;
			std::string label_base_xp_needed_for_spell_learning;
			Translate("$BaseXPNeededForSpellLearning", label_base_xp_needed_for_spell_learning);
			menu_translation_map["$BaseXPNeededForSpellLearning"] = label_base_xp_needed_for_spell_learning;
			std::string label_require_perk;
			Translate("$RequirePerk", label_require_perk);
			menu_translation_map["$RequirePerk"] = label_require_perk;

			std::string tool_show_spell_learn_notif;
			Translate("$ToolShowSpellLearnNotification", tool_show_spell_learn_notif);
			menu_translation_map["$ToolShowSpellLearnNotification"] = tool_show_spell_learn_notif;
			std::string tool_only_in_combat;
			Translate("$ToolOnlyInCombat", tool_only_in_combat);
			menu_translation_map["$ToolOnlyInCombat"] = tool_only_in_combat;
			std::string tool_spell_xp_multiplier;
			Translate("$ToolSpellXPMultiplier", tool_spell_xp_multiplier);
			menu_translation_map["$ToolSpellXPMultiplier"] = tool_spell_xp_multiplier;
			std::string tool_concentration_xp_modifier;
			Translate("$ToolConcentrationXPModifier", tool_concentration_xp_modifier);
			menu_translation_map["$ToolConcentrationXPModifier"] = tool_concentration_xp_modifier;
			std::string tool_base_xp_needed_for_spell_learning;
			Translate("$ToolBaseXPNeededForSpellLearning", tool_base_xp_needed_for_spell_learning);
			menu_translation_map["$ToolBaseXPNeededForSpellLearning"] = tool_base_xp_needed_for_spell_learning;
			std::string tool_require_perk;
			Translate("$ToolRequirePerk", tool_require_perk);
			menu_translation_map["$ToolRequirePerk"] = tool_require_perk;

			std::string log_to_console;
			Translate("$LogToConsole", log_to_console);
			menu_translation_map["$LogToConsole"] = log_to_console;
			std::string tool_log_to_console;
			Translate("$ToolLogToConsole", tool_log_to_console);
			menu_translation_map["$ToolLogToConsole"] = tool_log_to_console;

			REX::INFO("Completed translation for menu and stored it in map");

		};
	};

	
}
