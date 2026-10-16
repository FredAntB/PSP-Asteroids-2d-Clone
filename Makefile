# Nombre del ejecutable final
TARGET = AsteroidsClassic

# Archivos de objeto a compilar (compila todo lo que esté en src/)
OBJS = src/main.c

# Opciones de compilación (Banderas para el compilador C)
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

# Archivos de recursos opcionales para el XMB de la PSP
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Asteroids Classic 2D v0.1
# PSP_EBOOT_ICON = icon0.png  # Descomenta cuando tengas un icono flotante 144x80
# PSP_EBOOT_PIC1 = pic1.png  # Descomenta cuando tengas un fondo 480x272

# Inclusión de las reglas por defecto del PSP SDK
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
