##############################################################
##  THIS CMAKELISTS FILE ONLY LISTS SOURCE FILES            ##
##  IT DOES NOT CONTAIN ANY TARGET TO RUN AND NEVER SHOULD  ##
##  ALL PATH TO A SOURCE FILE MUST BE PRECEDED WITH 'src/'  ##
##############################################################

# Source files related to image processing and manipulation
set(IMG_SRC
    src/image/filter.c
    src/image/image.c
    src/image/image_scaling.c
    src/image/mask.c)

set(IMG_SRC
    src/puissancen/PuissanceN.c
    src/saturation/Saturation.c
    src/rotation/Rotation.c)

# Source files related to the GUI

set(GUI_SRC
    src/gui/gui.c)

# Source files related to video editing
set(VIDEO_SRC "")

# Source files related to drawing
set(DRAWING_SRC
    src/colorimetrie/colorimetrie.c
    src/colorimetrie/stack.c)

# Source files related to helper functions
set(UTILS_SRC
    src/utils/utils.c)

# All source files
set(SRC
    ${IMG_SRC}
    ${GUI_SRC}
    ${VIDEO_SRC}
    ${DRAWING_SRC}
    ${UTILS_SRC})

## Set them to parent scope
set(GUI_SRC ${GUI_SRC} PARENT_SCOPE)
set(IMAGE_SRC ${IMAGE_SRC} PARENT_SCOPE)
set(VIDEO_SRC ${VIDEO_SRC} PARENT_SCOPE)
set(DRAWING_SRC ${DRAWING_SRC} PARENT_SCOPE)
set(UTILS_SRC ${UTILS_SRC} PARENT_SCOPE)
set(SRC ${SRC} PARENT_SCOPE)
