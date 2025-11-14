function(enable_sanitizers TARGET)
    message(STATUS "Enabling sanitizers for ${TARGET}")
    target_compile_options(
        ${TARGET} 
        PRIVATE
            -fsanitize=signed-integer-overflow -fsanitize=shift
            -fsanitize=leak -fsanitize=address -fsanitize=bounds
            -fsanitize=integer-divide-by-zero -fsanitize=float-divide-by-zero
            -fsanitize=return -fsanitize=null -fsanitize=undefined
    )
    target_link_options(
        ${TARGET} 
        PRIVATE 
            -fsanitize=address -fsanitize=bounds -fsanitize=undefined
    )
endfunction()