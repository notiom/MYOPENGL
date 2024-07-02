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
    define_values = {"folder": "06_box_plan_sphere"},
)

config_setting(
    name = "use_folder7",
    define_values = {"folder": "07_exercise_mvp"},
)

config_setting(
    name = "use_folder8",
    define_values = {"folder": "08_camera"},
)

config_setting(
    name = "use_folder9",
    define_values = {"folder": "09_camera_class"},
)

config_setting(
    name = "use_folder10",
    define_values = {"folder": "10_light"},
)

config_setting(
    name = "use_folder11",
    define_values = {"folder": "11_earth_and_moon"},
)

config_setting(
    name = "use_folder12",
    define_values = {"folder": "12_basic_light"},
)

config_setting(
    name = "use_folder13",
    define_values = {"folder": "13_Materials"},
)

config_setting(
    name = "use_folder14",
    define_values = {"folder": "14_light_maps"},
)

config_setting(
    name = "use_folder15",
    define_values = {"folder": "15_light_casters"},
)

config_setting(
    name = "use_folder16",
    define_values = {"folder": "16_light_multipile"},
)

# Define the glad library
cc_library(
    name = "libglad",
    srcs = ["include/glad.c"],
    hdrs = glob(["include/*.h"]),
    includes = ["include"],  # This makes sure #include <glad/glad.h> works
    visibility = ["//visibility:public"],
)

cc_library(
    name = "imgui",
    srcs = glob(["include/imgui/*.cpp"]),
    hdrs = glob(["include/imgui/*.h"]),
    includes = ["include/imgui"],  # This makes sure #include <glad/glad.h> works
    visibility = ["//visibility:public"],
)

cc_library(
    name = "geometry",
    hdrs = glob(["include/geometry/*.h"]),
    includes = ["include/geometry"],  # This makes sure #include <glad/glad.h> works
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
        ":use_folder6": glob(["src/06_box_plan_sphere/*.cpp"]),
        ":use_folder7": glob(["src/07_exercise_mvp/*.cpp"]),
        ":use_folder8": glob(["src/08_camera/*.cpp"]),
        ":use_folder9": glob(["src/09_camera_class/*.cpp"]),
        ":use_folder10": glob(["src/10_light/*.cpp"]),
        ":use_folder11": glob(["src/11_earth_and_moon/*.cpp"]),
        ":use_folder12": glob(["src/12_basic_light/*.cpp"]),
        ":use_folder13": glob(["src/13_Materials/*.cpp"]),
        ":use_folder14": glob(["src/14_light_maps/*.cpp"]),
        ":use_folder15": glob(["src/15_light_casters/*.cpp"]),
        ":use_folder16": glob(["src/16_light_multipile/*.cpp"]),
        "//conditions:default": [],  # Default to empty if no folder is specified
    }),
    deps = [":libglad",":imgui",":geometry"],
    copts = [
        "-Iinclude",  # Include the "include" directory for glad and geometry
        "-I/usr/local/include",  # Include system path for glfw
        "-fdiagnostics-color=always",
    ],
    linkopts = [
        "-L/usr/local/lib",  # Link against system libraries
        # "-lGL",
        # "-lGLU",
        # "-lglut",
        "-ldl",
        # "-lGLEW",
        "-lglfw",
        # "-lX11",
        "-lpthread",
        # "-lXrandr",
    ],
    visibility = ["//visibility:public"],
)
