idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES "${requires}"
                    PRIV_REQUIRES "${priv_requires}"
                    EMBED_TXTFILES  "styles.css" "script.js" "index.html")