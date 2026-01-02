# Learn Spells From Staves

Plugin to allow learning spells from staves you use

### Requirements
* [XMake](https://xmake.io) [2.8.2+]
* C++23 Compiler (MSVC, Clang-CL)

## Getting Started
```bat
git clone --recurse-submodules https://github.com/Styyx1/spell-learner
cd spell-learner
```

### Pre-Build Instructions
Until libxse's commonlib updates, you need to change RE::SendHUDMessage::ShowHUDMessage to have the correct IDs. SE and AE IDs are switched up.
here's how it should look:
```cpp
void SendHUDMessage::ShowHUDMessage(const char* a_notification, const char* a_soundToPlay, bool a_cancelIfAlreadyQueued)
{
	using func_t = decltype(&SendHUDMessage::ShowHUDMessage);
	static REL::Relocation<func_t> func{ RELOCATION_ID(52050, 52933) };
	return func(a_notification, a_soundToPlay, a_cancelIfAlreadyQueued);
}
```
If you don't make this change, the game will crash upon learning spells

### Build
To build the project, run the following command:
```bat
xmake build
```

> ***Note:*** *This will generate a `build/windows/` directory in the **project's root directory** with the build output.*

### Build Output (Optional)
If you want to redirect the build output, set one of or both of the following environment variables:

- Path to a Skyrim install folder: `XSE_TES5_GAME_PATH`

- Path to a Mod Manager mods folder: `XSE_TES5_MODS_PATH`

### Project Generation (Optional)
If you want to generate a Visual Studio project, run the following command:
```bat
xmake project -k vsxmake
```

> ***Note:*** *This will generate a `vsxmakeXXXX/` directory in the **project's root directory** using the latest version of Visual Studio installed on the system.*

### Upgrading Packages (Optional)
If you want to upgrade the project's dependencies, run the following commands:
```bat
xmake repo --update
xmake require --upgrade
```