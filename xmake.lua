add_rules("mode.debug", "mode.release")

add_requires("glew")
add_requires("opengl", {optional = true})

set_optimize("smallest")

target("simply-remade-nuxi")
    set_kind("binary")
    add_files("src/*.cpp")
    add_files("src/ui/*.cpp")
    set_languages("c++14")
    add_packages("opengl32", "glew")

    add_rules("utils.bin2c", {extensions = {".glsl"}})
    add_files("assets/shaders/*.glsl")

-- Pre-build script to download and build wxwidgets
before_build(function (target)
    local wxwidgets_version = "3.2.2.1"
    local builddir = path.join(os.projectdir(), "build")
    local wxwidgets_builddir = path.join(builddir, "wxwidgets")
    local wxwidgetsinstalldir = path.join(wxwidgets_builddir, "install")
    local wxwidgets_libdir = path.join(wxwidgetsinstalldir, "lib", "vc_x64_lib")
    local wxwidgets_lib = path.join(wxwidgets_libdir, "wxmsw32u_core.lib")
    
    -- Check if wxwidgets is already built
    if os.exists(wxwidgets_lib) then
        print("wxWidgets already built, linking...")
        target:add("includedirs", path.join(wxwidgetsinstalldir, "include"))
        target:add("includedirs", path.join(wxwidgetsinstalldir, "lib", "vc_x64_lib", "mswu"))
        target:add("linkdirs", wxwidgets_libdir)
        if is_plat("windows") then
            target:add("links", 
                "wxmsw32u_core", "wxmsw32u_adv", "wxmsw32u_aui", "wxmsw32u_html", 
                "wxmsw32u_xrc", "wxmsw32u_richtext", "wxmsw32u_stc", "wxbase32u_xml",
                "wxmsw32u_gl",
                "wxbase32u", "wxbase32u_net", "wxregexu", "wxzlib", "wxexpat",
                "wxjpeg", "wxpng", "wxtiff", "wxscintilla")
            target:add("syslinks", "comctl32", "rpcrt4", "shlwapi", "version", "winmm", "wsock32", "uxtheme", "dwmapi", "gdi32", "user32", "advapi32", "shell32", "comdlg32", "ole32", "uuid", "winspool")
        elseif is_plat("linux") then
            target:add("links", "wx_gtk3-" .. wxwidgets_version, "wx_base-" .. wxwidgets_version)
            target:add("syslinks", "gtk-3", "gobject-2.0", "glib-2.0", "pango-1.0", "cairo", "X11", "Xext", "SM", "ICE")
        end
        return
    end
    
    -- Create directories
    os.mkdir(wxwidgets_builddir)
    
    -- Download wxwidgets source via git
    print("Cloning wxWidgets " .. wxwidgets_version .. "...")
    local wxwidgets_srcdir = path.join(wxwidgets_builddir, "wxWidgets-src")
    if not os.exists(path.join(wxwidgets_srcdir, ".git")) then
        os.execv("git", {"clone", "--branch", "v" .. wxwidgets_version, "https://github.com/wxWidgets/wxWidgets.git", wxwidgets_srcdir})
        os.cd(wxwidgets_srcdir)
        os.execv("git", {"submodule", "update", "--init", "--recursive"})
        os.cd(projectdir)
    end
    
    -- Configure with cmake for static build
    print("Configuring wxWidgets with CMake...")
    local cmake_builddir = path.join(wxwidgets_builddir, "cmake-build")
    os.mkdir(cmake_builddir)
    os.cd(cmake_builddir)
    
    local configs = {}
    table.insert(configs, "-S" .. wxwidgets_srcdir)
    table.insert(configs, "-B" .. cmake_builddir)
    table.insert(configs, "-DCMAKE_BUILD_TYPE=Release")
    table.insert(configs, "-DBUILD_SHARED_LIBS=OFF")
    table.insert(configs, "-DwxBUILD_STATIC=ON")
    table.insert(configs, "-DwxBUILD_SAMPLES=OFF")
    table.insert(configs, "-DwxBUILD_TESTS=OFF")
    table.insert(configs, "-DCMAKE_INSTALL_PREFIX=" .. wxwidgetsinstalldir)
    
    if is_plat("windows") then
        table.insert(configs, "-DwxBUILD_TOOLKIT=msw")
        table.insert(configs, "-DwxCOMPILER_MSVC=ON")
    elseif is_plat("linux") then
        table.insert(configs, "-DwxBUILD_TOOLKIT=gtk3")
    end
    
    os.execv("cmake", configs)
    
    -- Build wxwidgets
    print("Building wxWidgets static library...")
    os.execv("cmake", {"--build", cmake_builddir, "--config", "Release", "--parallel"})
    
    -- Install wxwidgets
    print("Installing wxWidgets...")
    os.execv("cmake", {"--install", cmake_builddir})
    
    -- Return to project directory
    os.cd(projectdir)
    
    -- Add wxwidgets to target
    target:add("includedirs", path.join(wxwidgetsinstalldir, "include"))
    target:add("includedirs", path.join(wxwidgetsinstalldir, "lib", "vc_x64_lib", "mswu"))
    target:add("linkdirs", wxwidgets_libdir)
    if is_plat("windows") then
        target:add("links", 
            "wxmsw32u_core", "wxmsw32u_adv", "wxmsw32u_aui", "wxmsw32u_html", 
            "wxmsw32u_xrc", "wxmsw32u_richtext", "wxmsw32u_stc", "wxbase32u_xml",
            "wxbase32u", "wxbase32u_net", "wxregexu", "wxzlib", "wxexpat",
            "wxjpeg", "wxpng", "wxtiff", "wxscintilla")
        target:add("syslinks", "comctl32", "rpcrt4", "shlwapi", "version", "winmm", "wsock32", "uxtheme", "dwmapi")
    elseif is_plat("linux") then
        target:add("links", "wx_gtk3-" .. wxwidgets_version, "wx_base-" .. wxwidgets_version)
        target:add("syslinks", "gtk-3", "gobject-2.0", "glib-2.0", "pango-1.0", "cairo", "X11", "Xext", "SM", "ICE")
    end
end)

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. How to configure output directory?
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--
