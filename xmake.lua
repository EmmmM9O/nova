add_rules("plugin.compile_commands.autoupdate", { outputdir = ".vscode" })
add_rules("mode.debug", "mode.release")
add_requires("glfw", "stb", "fmt")
set_languages("c++2a")
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
add_files("core/**.cpp", sys .. "/**.cpp")
add_includedirs("$(projectdir)")
add_packages("stb", "fmt","pthread")
set_languages("c++2a")

target("nova-desktop")
set_kind("static")
add_includedirs("$(projectdir)")
add_cxxflags("-Wall")
add_packages("glfw")
add_deps("nova-core")
set_languages("c++2a")
add_files("desktop/**.cpp","include/glad.c")

target("test")
set_kind("binary")
add_includedirs("$(projectdir)")
add_cxxflags("-Wall")
add_files("test/**.cpp")
add_deps("nova-core", "nova-desktop")
set_languages("c++2a")
--[[
target("android")
set_kind("binary")
add_includedirs("$(projectdir)")
add_cxxflags("-Wall")
add_files("andorid/csrc/**.cpp")
add_deps("nova-core")
set_languages("c++2a")
]]--
--[[
target("LinuxDesktop")
set_kind("binary")
add_includedirs("$(projectdir)")
add_cxxflags("-Wall")
add_files("linux/**.cpp")
add_deps("nova-core","nova-desktop")
set_languages("c++2a")
]]
--
after_build(function(target)
	local outputdir = target:targetdir() -- 获取输出目录
	local assetsdir = os.args("assets/") -- 获取 assets 目录的绝对路径
	-- 使用系统命令进行复制操作
	os.exec("cp -r " .. assetsdir .. " " .. outputdir)
end)
