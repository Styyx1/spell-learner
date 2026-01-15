#include "config.h"
#include "spelllearner.h"
#include "serialisation.h"
#include "menu.h"
#include "mod-data.h"

void Listener(SKSE::MessagingInterface::Message* a_msg)
{
	if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
		const auto forms = Config::Forms::GetSingleton();
		forms->LoadForms();
		Config::SpellData::GetSingleton()->PopulateAllowSet();
		forms->AddToSpellSet();
		MOD::SpellLearnData::GetSingleton()->PopulateXPMapOnStart();
		Menu::TranslateMenuNames();
	}
}

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);

	Config::Settings::GetSingleton()->UpdateSettings(false);
	Menu::RegisterSpellLearnMenu();
	SKSE::GetMessagingInterface()->RegisterListener(Listener);
	if (auto serialization = SKSE::GetSerializationInterface())
	{
		serialization->SetUniqueID(Serialisation::ID);
		serialization->SetSaveCallback(&Serialisation::SaveCallback);
		serialization->SetLoadCallback(&Serialisation::LoadCallback);
		serialization->SetRevertCallback(&Serialisation::RevertCallback);
	}
	return true;
}
