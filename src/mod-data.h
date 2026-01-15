#pragma once

namespace MOD {
	inline constexpr std::string_view TOML_PATH_DEFAULT = "Data/SKSE/Plugins/spell-learner.toml";
	inline constexpr std::string_view TOML_PATH_CUSTOM = "Data/SKSE/Plugins/spell-learner_custom.toml";
	inline constexpr std::string_view JSON_PATH_DEFAULT = "Data/SKSE/Plugins/spell-learner-spells.json";
	inline constexpr std::string_view JSON_PATH_CUSTOM = "Data/SKSE/Plugins/spell-learner-spells_custom.json";
	inline constexpr std::string_view JSON_FOLDER = "Data/SKSE/Plugins/SpellsToLearn/";
	static inline constexpr std::string_view SPELL_LEARNING{ "Spell Learning" };

	inline constexpr std::string_view SPERG_PATCH = "sperg-spell-learner.esp";
	inline constexpr RE::FormID SPERG_SPELL_LIST = 0x1;

	inline constexpr std::string_view MOD_NAME = "spell-learner";
	
}
