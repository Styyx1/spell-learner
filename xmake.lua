-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibsse")
includes("extern/styyx-utils")

-- set project
set_project("spell-learner")
set_version("1.0.0")
set_license("GPL-3.0")

-- set defaults
set_languages("c++23")
set_warnings("allextra")

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

set_config("commonlib_json", true)
set_config("commonlib_toml", true)

-- set policies
set_policy("package.requires_lock", true)

-- targets
target("spell-learner")
    -- add dependencies to target
    add_deps("commonlibsse")
    add_deps("styyx-util")

    -- add commonlibsse plugin
    add_rules("commonlibsse.plugin", {
        name = "spell-learner",
        author = "styyx",
        description = "Learn spells from staves"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")
