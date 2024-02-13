add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})
add_rules("mode.debug", "mode.release")
add_requires("glfw", "glad", "stb")
set_languages("c++2a")
local sys = is_os("windows") and "Windows" or is_os("macosx") and "Macosx" or is_os("linux") and "Linux"
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
  add_files("src/Framework/**.cpp", "/usr/src/gl.c","src/Platform/"..sys.."/**.cpp")
  --set_policy("build.c++.modules", true)
  add_packages("glfw", "glad", "stb")
  set_languages("c++2a")
target("test")
    set_kind("binary")
  add_cxxflags("-Wall")
    add_files("src/test/**.cpp")
    add_deps("nova-core")
  set_languages("c++2a")
target("linux")
    set_kind("binary")
  add_cxxflags("-Wall")
    add_files("src/Linux/**.cpp")
    add_deps("nova-core")
  set_languages("c++2a")
after_build(function(target)
	local outputdir = target:targetdir() -- 获取输出目录
	local assetsdir = os.args("assets/") -- 获取 assets 目录的绝对路径
	-- 使用系统命令进行复制操作
	os.exec("cp -r " .. assetsdir .. " " .. outputdir)
end)
