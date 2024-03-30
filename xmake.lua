add_rules("plugin.compile_commands.autoupdate", { outputdir = ".vscode" })
add_rules("mode.debug", "mode.release")
includes("third_party/**.lua")
add_requires("glfw")
set_languages("c++20")
local sys = is_os("windows") and "windows" or is_os("linux") and "linux" 
task("format")
set_category("plugin")
on_run("format")
set_menu({
	usage = "xmake format",
	description = "Format source code",
})
task_end()

target("nova-core")
set_kind("static")
add_cxxflags("-Wall")
add_files("core/**.cpp", sys.."/**.cpp")
add_includedirs("$(projectdir)","$(projectdir)/third_party/fmt/include")
add_deps("fmt")
add_packages("pthread")
set_languages("c++2a")

target("nova-desktop")
set_kind("static")
add_includedirs("$(projectdir)")
add_cxxflags("-Wall")
add_packages("glfw")
add_deps("nova-core")
set_languages("c++2a")
add_files("desktop/**.cpp","desktop/**.c")

includes("example/**.lua")




