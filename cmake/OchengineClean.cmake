function(ochengine_enable_clean source_dir)
    file(
        GLOB_RECURSE generated_files
        CONFIGURE_DEPENDS
        LIST_DIRECTORIES false
        "${source_dir}/compile_commands.json"
        "${source_dir}/*.gch"
        "${source_dir}/*.out"
    )

    file(
        GLOB generated_build_directories
        CONFIGURE_DEPENDS
        LIST_DIRECTORIES true
        "${source_dir}/cmake-build-*"
        "${source_dir}/*/build"
        "${source_dir}/*/cmake-build-*"
        "${source_dir}/*/*/build"
        "${source_dir}/*/*/cmake-build-*"
    )

    list(APPEND generated_build_directories "${source_dir}/build")
    list(REMOVE_ITEM generated_build_directories "${CMAKE_BINARY_DIR}")

    set_property(
        DIRECTORY APPEND
        PROPERTY ADDITIONAL_CLEAN_FILES
            ${generated_files}
            ${generated_build_directories}
    )
endfunction()
