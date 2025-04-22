idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES sh1106_i2c
    REQUIRES max30102
)
