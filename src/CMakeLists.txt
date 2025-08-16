add_subdirectory(core)
add_subdirectory(usecases)
add_subdirectory(adapters)

# Headless test application
add_subdirectory(headless)

# GUI
add_subdirectory(gui)

# Targets depending on GUI
add_subdirectory(plugin-clap)
add_subdirectory(standalone)
