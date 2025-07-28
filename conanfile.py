from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout


class PrototypeTradingPlatformRecipe(ConanFile):
    name = "alternate-trading-platform"
    version = "1.0"

    # Binary configuration
    settings = "os", "arch", "compiler", "build_type"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    def requirements(self):
        # Core dependencies for trading platform
        self.requires("fmt/11.1.4", override=True, force=True)  # String formatting
        self.requires("spdlog/1.15.1")  # Logging
        self.requires("nlohmann_json/3.11.3")  # JSON parsing
        self.requires("boost/1.85.0")  # Networking, filesystem, etc.
        
        # Database and storage - using system SQLite for now
        # self.requires("sqlite3/3.46.1")  # SQLite database
        
        # GUI dependencies (for Arthur frontend)
        if self.settings.os != "Linux":  # Skip GUI on headless Linux
            self.requires("sdl/2.30.9")  # SDL for GUI
        
        # Development and debugging
        self.requires("tracy/0.11.1")  # Profiling
        self.requires("magic_enum/0.9.7")  # Enum utilities
        self.requires("yaml-cpp/0.8.0")  # Configuration parsing
        
        # Testing
        self.requires("gtest/1.14.0")  # Unit testing

    def build_requirements(self):
        self.tool_requires("cmake/3.30.1")

    def config_options(self):
        if self.settings.os == "Linux":
            del self.options.fPIC

    def configure(self):
        # Apple Silicon specific configuration
        if self.settings.os == "Macos" and self.settings.arch == "armv8":
            self.output.info("Configuring for Apple Silicon (M1/M2)")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["CMAKE_CXX_STANDARD"] = "20"
        
        # Apple Silicon optimizations
        if self.settings.os == "Macos" and self.settings.arch == "armv8":
            tc.variables["CMAKE_OSX_ARCHITECTURES"] = "arm64"
            tc.variables["CMAKE_CXX_FLAGS"] = "-mcpu=apple-m1"
        
        tc.generate()
