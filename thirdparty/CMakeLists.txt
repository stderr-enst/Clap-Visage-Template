include(FetchContent)

FetchContent_Declare(
  visage
  GIT_REPOSITORY https://github.com/VitalAudio/visage.git
  GIT_TAG main
  GIT_SHALLOW TRUE
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(visage)

FetchContent_Declare(
  clap
  GIT_REPOSITORY https://github.com/free-audio/clap
  GIT_TAG 1.2.7
  GIT_SHALLOW TRUE
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(clap)

FetchContent_Declare(
  clap_helpers
  GIT_REPOSITORY https://github.com/free-audio/clap-helpers
  GIT_TAG main
  GIT_SHALLOW TRUE
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(clap_helpers)
