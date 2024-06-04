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
    define_values = {"folder": "03_transfrom"},
)

config_setting(
    name = "use_folder4",
    define_values = {"folder": "04_use_plane_geometry"},
)

config_setting(
    name = "use_folder5",
    define_values = {"folder": "05_model_view_project"},
)

config_setting(
    name = "use_folder6",
    define_values = {"folder": "06_camera"},
)


# Define the glad library
cc_library(
    name = "glad",
    srcs = ["include/glad.c"],
    hdrs = glob(["include/*.h"]),
    includes = ["include"],  # This makes sure #include <glad/glad.h> works
    visibility = ["//visibility:public"],
)

cc_library(
    name = "imgui",
    srcs = glob(["include/imgui/*.cpp"]),
    hdrs = glob(["include/imgui/*.h"]),
    includes = ["include"],  # This makes sure #include <glad/glad.h> works
    visibility = ["//visibility:public"],
)

cc_library(
    name = "geometry",
    hdrs = glob(["include/geometry/*.h"]),
    includes = ["include"],  # This makes sure #include <glad/glad.h> works
    visibility = ["//visibility:public"],
)


# Define the main binary with conditional srcs
cc_binary(
    name = "main",
    srcs = select({
        ":use_folder1": glob(["src/01_hello_triangle/*.cpp"]),
        ":use_folder2": glob(["src/02_load_texture/*.cpp"]),
        ":use_folder3": glob(["src/03_transfrom/*.cpp"]),
        ":use_folder4": glob(["src/04_use_plane_geometry/*.cpp"]),
        ":use_folder5": glob(["src/05_model_view_project/*.cpp"]),
        ":use_folder6": glob(["src/06_camera/*.cpp"]), 
        "//conditions:default": [],  # Default to empty if no folder is specified
    }),
    deps = [":glad",":imgui",":geometry"],
    copts = [
        "-Iinclude",  # Include the "include" directory for glad and geometry
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
