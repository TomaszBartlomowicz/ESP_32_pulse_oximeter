idf_component_register(
    SRCS "sh1106_i2c.c"
    INCLUDE_DIRS "."
    REQUIRES driver  # Dodajemy zależność od komponentu 'driver'
)
