#include "spelllearner.h"
#include "REX/TSingleton.h"
#include "REX/TTomlSetting.h"
#include "config.h"

namespace MOD {

	void SpellLearner::SpellCast(RE::ActorMagicCaster* a_this, bool a_doCast, std::uint32_t a_arg2, RE::MagicItem* a_spell)
	{
		_SpellCastHook(a_this, a_doCast, a_arg2, a_spell);

		if (!a_spell || !a_doCast)
			return;

		if (a_spell->GetSpellType() == RE::MagicSystem::SpellType::kStaffEnchantment) {
			auto actor = a_this->GetCasterAsActor();
			if (!actor)
				return;

			if (actor->IsPlayerRef()) {
				if (a_spell) {
					RE::SpellItem* spell{ nullptr };
					auto data = REX::TSingleton<SpellLearnData>::GetSingleton();

					if (!data->CanEarnSpellXP(actor))
						return;

					for (auto& effect : a_spell->effects) {
						if (effect->baseEffect) {
							spell = FindMatchingSpellWithEffectFromList(effect->baseEffect);
							if (spell) {
								data->AddXPToSpell(spell);
								break;
							}
						}
					}
					if (spell) {
						data->LearnSpell(actor, spell);
					}
				}
			}
		}
	}

	void SpellLearner::UpdateCaster(RE::ActorMagicCaster* a_this, float a_delta)
	{
		auto curr_spell = a_this->currentSpell;
		if (curr_spell) {
			bool conc_spell = a_this->currentSpell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration;
			if (conc_spell) {
				if (auto actor = a_this->GetCasterAsActor(); actor && actor->IsPlayerRef()) {
					elapsed += a_delta;

					if (elapsed >= 1.0f) {
						elapsed = 0.0f;

						RE::SpellItem* spell = nullptr;
						auto data = REX::TSingleton<SpellLearnData>::GetSingleton();

						if (!data->CanEarnSpellXP(actor))
							return;

						if(!CombatCheck(actor))
							return;

						for (auto& effect : curr_spell->effects) {
							if (effect->baseEffect) {
								spell = FindMatchingSpellWithEffectFromList(effect->baseEffect);
								if (spell) {
									data->AddXPToSpell(spell);
									data->LearnSpell(actor, spell);
									break;
								}
							}
						}
					}
				}
			}
		}		
		return _UpdateCasterHook(a_this, a_delta);
	}

	RE::SpellItem* SpellLearner::FindMatchingSpellWithEffectFromList(RE::EffectSetting* a_effect)
	{
		if (Config::SpellData::spell_allow_set.empty()) {
			return nullptr;
		}
		for (auto& spell : Config::SpellData::spell_allow_set) {
			for (auto& effect : spell->effects) {
				if (effect->baseEffect == a_effect) {
					return spell;
				}
			}
		}
		return nullptr;
	}
	inline bool SpellLearner::CombatCheck(RE::Actor* actor)
	{
		if(Config::Settings::only_in_combat.GetValue())
		{
			if (!actor->IsInCombat()) {
				return false;
			}
		}
		return true;
	}
	void SpellLearnData::PopulateXPMapOnStart() {
		for (auto& spell : Config::SpellData::spell_allow_set) {
			spell_xp_map[spell] = 0.f;
		}
	}
	void SpellLearnData::AddXPToSpell(RE::SpellItem* a_spell)
	{
		if (!a_spell) {
			return;
		}

		auto it = spell_xp_map.find(a_spell);
		if (it == spell_xp_map.end()) {
			return;
		}
		if (it->second > Config::Settings::base_xp_needed_for_spell_learning.GetValue())
			return;
		double xp_to_add = 1.0 * Config::Settings::spell_xp_multiplier.GetValue();
		if (a_spell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration) {
			xp_to_add *= Config::Settings::concentration_xp_modifier.GetValue();
		}
		it->second += xp_to_add;
		if (Config::Settings::log_to_console.GetValue()) {
			std::string msg = std::format("{} has now {} xp, use it {} more times to learn the spell", a_spell->GetName(), it->second, Config::Settings::base_xp_needed_for_spell_learning.GetValue() - it->second);
			RE::ConsoleLog::GetSingleton()->Print(msg.c_str());
		}
	}
	void SpellLearnData::LearnSpell(RE::Actor* actor, RE::SpellItem* a_spell) {
		if (!actor || !a_spell)
			return;

		if (actor->HasSpell(a_spell)) {
			spell_xp_map.erase(a_spell);
			return;
		}

		auto it = spell_xp_map.find(a_spell);
		if (it == spell_xp_map.end()) {
			return;
		}
		if (it->second >= Config::Settings::base_xp_needed_for_spell_learning.GetValue()) {
			actor->AddSpell(a_spell);
			std::string message = std::format("You have learned {}", a_spell->GetName());
			RE::SendHUDMessage::ShowHUDMessage(message.c_str());
			spell_xp_map.erase(it);
		}
	}

	bool SpellLearnData::CanEarnSpellXP(RE::Actor* actor) const
	{
		if (Config::Settings::require_perk.GetValue()) {

			if(!Config::Forms::spell_learn_perk)
				return false;

			if (!actor->HasPerk(Config::Forms::spell_learn_perk)) {
				return false;
			}

		}
		return true;
	}

}