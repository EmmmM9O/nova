target("async-exampele")
set_kind("binary")
add_cxxflags("-Wall")
add_files("**.cpp")
add_includedirs("$(projectdir)/example/async")
add_includedirs("$(projectdir)/third_party/fmt/include")
add_includedirs("$(projectdir)")
add_deps("nova-core")
set_languages("c++2a")
