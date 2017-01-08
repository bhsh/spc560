# List of library files exported by the component.
LIB_C_SRC       += .\components\spc560pxx_low_level_drivers_component_rla/lib/src/spc5_lld.c \
                   .\components\spc560pxx_low_level_drivers_component_rla/lib/src/spc5_edma.c \
                   .\components\spc560pxx_low_level_drivers_component_rla/lib/src/serial_lld.c \
                   .\components\spc560pxx_low_level_drivers_component_rla/lib/src/pit_lld.c \
                   .\components\spc560pxx_low_level_drivers_component_rla/lib/src/stm_lld.c \

LIB_CPP_SRC     +=

LIB_ASM_SRC     +=

LIB_INCLUDES    += .\components\spc560pxx_low_level_drivers_component_rla/lib/include

APP_C_SRC       += \
                   .\components\spc560pxx_low_level_drivers_component_rla/cfg/pit_lld_cfg.c \
                   .\components\spc560pxx_low_level_drivers_component_rla/cfg/stm_lld_cfg.c \

APP_CPP_SRC     +=

APP_ASM_SRC     +=

APP_INCLUDES    += .\components\spc560pxx_low_level_drivers_component_rla/cfg
