#pragma once

namespace MOD {
	struct SpellLearner : REX::TSingleton<SpellLearner> {

	private:
		RE::ActorMagicCaster* caster;
		static void SpellCast(RE::ActorMagicCaster* a_this, bool a_doCast, std::uint32_t a_arg2, RE::MagicItem* a_spell);
		static void UpdateCaster(RE::ActorMagicCaster* a_this, float a_delta);		
		
		static RE::SpellItem* FindMatchingSpellWithEffectFromList(RE::EffectSetting* a_effect);

		static inline float elapsed;
		static inline float frameCount;

		static inline bool CombatCheck(RE::Actor* actor);

		// Hooks
		static inline REL::THookVFT _SpellCastHook{ RE::ActorMagicCaster::VTABLE[0], 0x9, SpellCast };
		static inline REL::THookVFT _UpdateCasterHook{ RE::ActorMagicCaster::VTABLE[0], 0x1D, UpdateCaster };
	};
	struct SpellLearnData : REX::TSingleton<SpellLearnData> {
		RE::SpellItem* learn_spell;
		float a_spell_xp{};
		
		std::unordered_map<RE::SpellItem*, float> spell_xp_map;
		void PopulateXPMapOnStart();
		void AddXPToSpell(RE::SpellItem* a_spell);
		void LearnSpell(RE::Actor* actor, RE::SpellItem* a_spell);
		bool CanEarnSpellXP(RE::Actor* actor) const;
	};
}
