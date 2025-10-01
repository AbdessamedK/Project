dofile "GL.lua"

 -- description des projets		 
projects = {
	"tp1",
	"tp2",
	"tp3",
	"tp4",
}

for i, name in ipairs(projects) do
    project(name)
        language "C++"
        kind "ConsoleApp"
        targetdir "bin"
		links { "libgkit3GL" }
		includedirs { ".", "../src", "src" }
		
        files { "projets/" .. name .. ".cpp" }
end

project("portal")
    language "C++"
    kind "ConsoleApp"
    targetdir "bin"
	links { "libgkit3GL" }
	includedirs { ".", "../src", "src" }
		
    files { "portal/*.cpp" }
