load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

# Define config settings for each folder
config_setting(
    name = "use_folder1",
    define_values = {"folder": "01_hello_triangle"},
)

config_setting(
    name = "use_folder2",
    define_values = {"folder": "02_load_texture"},
)

config_setting(
    name = "use_folder3",
    define_values = {"folder": "03_transform"},
)

# Define the glad library
cc_library(
    name = "glad",
    srcs = ["include/glad.c"],
    hdrs = glob(["include/*.h"]),
    includes = ["include"],  # This makes sure #include <glad/glad.h> works
    visibility = ["//visibility:public"],
)

# Define the main binary with conditional srcs
cc_binary(
    name = "main",
    srcs = select({
        ":use_folder1": glob(["src/01_hello_triangle/*.cpp"]),
        ":use_folder2": glob(["src/02_load_texture/*.cpp"]),
        ":use_folder3": glob(["src/03_transform/*.cpp"]),
        "//conditions:default": [],  # Default to empty if no folder is specified
    }),
    deps = [":glad"],
    copts = [
        "-Iinclude",  # Include the "include" directory for glad
        "-I/usr/local/include",  # Include system path for glfw
        "-fdiagnostics-color=always",
    ],
    linkopts = [
        "-L/usr/local/lib",  # Link against system libraries
        "-lGL",
        "-lGLU",
        "-lglut",
        "-ldl",
        "-lGLEW",
        "-lglfw",
        "-lX11",
        "-lpthread",
        "-lXrandr",
    ],
    visibility = ["//visibility:public"],
)