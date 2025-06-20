# Copyright (c) 2025 by Tim Janke. All rights reserved.

function(install_hooks)
    set(PRE_COMMIT_SCRIPT ${CMAKE_SOURCE_DIR}/scripts/hooks/pre-commit)
    set(GIT_HOOK_DIR ${CMAKE_SOURCE_DIR}/.git/hooks)
    set(PRE_COMMIT_HOOK ${GIT_HOOK_DIR}/pre-commit)
    if(EXISTS ${GIT_HOOK_DIR} AND EXISTS ${PRE_COMMIT_SCRIPT})
        message(STATUS "Configuring git pre-commit hook from ${PRE_COMMIT_SCRIPT}")
        file(COPY ${PRE_COMMIT_SCRIPT} DESTINATION ${GIT_HOOK_DIR} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE USE_SOURCE_PERMISSIONS)
        # The USE_SOURCE_PERMISSIONS might not work reliably across systems/filesystems for +x.
        # Add execute permission explicitly if needed, but file(COPY ...) should handle it.
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${PRE_COMMIT_SCRIPT} ${PRE_COMMIT_HOOK})
        execute_process(COMMAND chmod +x ${PRE_COMMIT_HOOK}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE CHMOD_RESULT
                ERROR_QUIET)
        if(NOT CHMOD_RESULT EQUAL 0)
            message(WARNING "Failed to make pre-commit hook executable: ${PRE_COMMIT_HOOK}")
        endif()
    else()
        if(NOT EXISTS ${GIT_HOOK_DIR})
            message(STATUS ".git/hooks directory not found. Skipping pre-commit hook configuration. (This is normal if not in a git repository)")
        endif()
        if(NOT EXISTS ${PRE_COMMIT_SCRIPT})
            message(WARNING "Pre-commit script not found: ${PRE_COMMIT_SCRIPT}. Cannot configure pre-commit hook.")
        endif()
    endif()
endfunction()