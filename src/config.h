#pragma once
#include "mod-data.h"

namespace Config {
	using namespace MOD;

	struct Settings : REX::Singleton<Settings> {
		static inline REX::TOML::Bool show_spell_learn_notif{ SPELL_LEARNING, "bShowXPNotification", true };
		static inline REX::TOML::Bool only_in_combat{ SPELL_LEARNING, "bOnlyInCombat", false };
		static inline REX::TOML::F32 spell_xp_multiplier{ SPELL_LEARNING, "fSpellXPMultiplier", 1.0f };
		static inline REX::TOML::F32 concentration_xp_modifier{ SPELL_LEARNING, "fConcentrationXPModifier", 1.0f };
		static inline REX::TOML::F32 base_xp_needed_for_spell_learning{ SPELL_LEARNING, "fBaseXPForSpellLearning", 100.0f };
		static inline REX::TOML::Bool require_perk{ SPELL_LEARNING, "bRequiresPerk", false };
		static inline REX::TOML::Str spell_learn_perk{ SPELL_LEARNING, "sSpellLearnPerk", std::string("Skyrim.esm|0x108a44") };
		static inline REX::TOML::Bool log_to_console{ SPELL_LEARNING, "bLogToConsole", false };

		void UpdateSettings(bool a_save) {
			const auto toml = REX::TOML::SettingStore::GetSingleton();
			toml->Init(TOML_PATH_DEFAULT.data(), TOML_PATH_CUSTOM.data());
			if (!a_save) {
				toml->Load();
			}
			else {
				toml->Save();
			}
		};
	};

	struct SpellData : REX::Singleton<SpellData> {

		static inline std::unordered_set<RE::SpellItem*> spell_allow_set;
		
		void PopulateAllowSet(bool rebuild = false) {
			if (rebuild)
				spell_allow_set.clear();

			auto dataHandler = RE::TESDataHandler::GetSingleton();
			for (auto book : dataHandler->GetFormArray<RE::TESObjectBOOK>()) {
				if (book->TeachesSpell()) {
					auto spell = book->data.teaches.spell;
					if (spell) {
						if (MagicUtil::IsSpellPlayable(spell)) {
							spell_allow_set.insert(spell);
						}
					}
				}
			}
			REX::INFO("Registered {} teachable spells from books", spell_allow_set.size());
			return;
		};
	};	

	struct Forms : REX::Singleton<Forms> {

		static inline RE::BGSPerk* spell_learn_perk{ nullptr };
		static inline std::unordered_map<RE::BGSPerk*, bool> perk_init_map;

		static inline RE::BGSListForm* SPELL_LIST_SPERG{ nullptr };

		bool IsSpergPatchActive;

		void AddToSpellSet() const {
			if (IsSpergPatchActive) {
				const auto size_default = SpellData::spell_allow_set.size();
				if (Forms::SPELL_LIST_SPERG) {
					Forms::SPELL_LIST_SPERG->ForEachForm([&](RE::TESForm* a_formInList) {
						auto spell = a_formInList->As<RE::SpellItem>();
						if (spell)
						{
							SpellData::spell_allow_set.insert(spell);
							REX::DEBUG("insterted {}", spell->GetName());
							return RE::BSContainer::ForEachResult::kContinue;
						}
						return RE::BSContainer::ForEachResult::kContinue;
						});
					REX::INFO("Registered {} from SPERG patch", static_cast<int>(SpellData::spell_allow_set.size()) - static_cast<int>(size_default));
				}
			}
		}

		void LoadForms() {

			auto dataHandler = RE::TESDataHandler::GetSingleton();

			IsSpergPatchActive = false;
			if (MiscUtil::IsModLoaded(MOD::SPERG_PATCH)) {
				SPELL_LIST_SPERG = dataHandler->LookupForm<RE::BGSListForm>(MOD::SPERG_SPELL_LIST, SPERG_PATCH);
				if (SPELL_LIST_SPERG) {
					IsSpergPatchActive = true;
					
				}
			}


			for (auto& perk : dataHandler->GetFormArray<RE::BGSPerk>()) {
				RE::BSString desc;
				perk->GetDescription(desc, perk);
				if (perk->GetPlayable() && !desc.empty() && perk->GetName() != "") {
					perk_init_map[perk] = true;
					
				}				
			}
			REX::INFO("Loaded {} playable perks most likely part of skill trees", perk_init_map.size());

			if (Settings::spell_learn_perk.GetValue().empty()) {
				Settings::require_perk.SetValue(false);
				REX::WARN("Spell Learn Perk setting is empty, no perk will be required for spell learning.");
				return;
			}
			
			auto form = FormUtil::GetFormFromString(Settings::spell_learn_perk.GetValue());
			if(!form)
			{
				Settings::require_perk.SetValue(false);
				REX::WARN("Spell Learn Perk can not be found, check your setting in the .toml file. It needs to be \"Modname.esp|FormID\" or EditorID of the perk");
				return;
			}

			spell_learn_perk = form->As<RE::BGSPerk>();
			if(!spell_learn_perk || !spell_learn_perk->Is(RE::FormType::Perk)) {
				Settings::require_perk.SetValue(false);
				REX::WARN("Spell Learn Perk is not a perk, check your setting in the .toml file. It needs to be \"Modname.esp|FormID\" or EditorID of the perk");
				return;
			}

			//probably not necessary but i learned about one-line null checks, so why not use it 
			const auto file = spell_learn_perk->GetFile();
			auto fileName = file ? file->GetFilename() : "NO FILE FOUND";
			auto localID = spell_learn_perk->GetLocalFormID();
			std::string configWrite = std::format("{}|0x{:X}", fileName, localID);
			REX::INFO("Loaded Spell Learn Perk: {} from {}", spell_learn_perk->GetName(), configWrite);
		};
	};
};





