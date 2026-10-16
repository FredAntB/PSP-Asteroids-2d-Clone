#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>

/* Configuración del módulo de la PSP */
PSP_MODULE_INFO("AsteroidsClassic", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

/* Simplificación para imprimir en pantalla */
#define printf pspDebugScreenPrintf

/* Variable global para controlar el bucle principal del juego */
int running = 1;

/* Callback de salida: Se activa cuando el usuario presiona el botón HOME y confirma salir */
int exit_callback(int arg1, int arg2, void *common) {
    running = 0;
    return 0;
}

/* Hilo del Callback: Registra el evento de salida en el sistema */
int CallbackThread(SceSize args, void *argp) {
    int cbid;

    cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);

    sceKernelSleepThreadCB();
    return 0;
}

/* Configura e inicia el hilo de los callbacks */
int SetupCallbacks(void) {
    int thid = 0;

    thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
    if (thid >= 0) {
        sceKernelStartThread(thid, 0, 0);
    }

    return thid;
}

/* Punto de entrada principal */
int main() {
    // Inicializa la pantalla de depuración de texto de la PSP
    pspDebugScreenInit();
    
    // Configura los callbacks del sistema
    SetupCallbacks();

    printf("=========================================\n");
    printf("   Asteroids Classic 2D - PSP 1000       \n");
    printf("=========================================\n");
    printf("Presiona HOME para salir del juego.\n\n");
    printf("Iniciando bucle principal...\n");

    /* Bucle principal del juego (Game Loop) */
    while (running) {
        // Por ahora, solo simulamos el loop imprimiendo un indicador visual
        printf(".");
        
        // Espera un frame (~16.67ms para 60fps) para no saturar la CPU de la PSP
        sceDisplayWaitVblankStart();
    }

    /* Limpieza y salida formal al menú de la PSP (XMB) */
    printf("\nSaliendo al XMB...");
    sceKernelDelayThread(1000000); // Espera 1 segundo antes de cerrar
    sceKernelExitGame();

    return 0;
}
