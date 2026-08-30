-- include subprojects
includes("lib/commonlibsse", "extern/styyx-utils")

local MOD_NAME = "styyx-spell-learner"
local MOD_VERSION = "1.1.0"
local MOD_DESC = "learn spells from staves"

-- set project constants
set_project(MOD_NAME)
set_version(MOD_VERSION)
set_license("GPL-3.0")
set_languages("c++23")
set_warnings("allextra")
set_config("commonlib_toml", true)
set_config("commonlib_json", true)
set_config("use-fui", true)

-- add common rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- define targets
target(MOD_NAME)
    add_deps("styyx-util")
    add_rules("commonlibsse.plugin", {
        name = MOD_NAME,
        author = "styyx",
        description = MOD_DESC
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")
