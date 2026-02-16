idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "." "../services"
                    SRCS "../services/wifi.c" "../services/http_client.c" "../services/util.c")