#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>

/* Configuración del módulo de la PSP */
PSP_MODULE_INFO("AsteroidsClassic", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

#define printf pspDebugScreenPrintf

int running = 1;

/* Callback de salida mediante el botón HOME */
int exit_callback(int arg1, int arg2, void *common)
{
    running = 0;
    return 0;
}

int CallbackThread(SceSize args, void *argp)
{
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

int SetupCallbacks(void)
{
    int thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
    if (thid >= 0)
    {
        sceKernelStartThread(thid, 0, 0);
    }
    return thid;
}

int main()
{
    SceCtrlData pad;

    pspDebugScreenInit();
    SetupCallbacks();

    /* Configuración de lectura de controles:
       - Ciclo de muestreo: 0 (estándar)
       - Modo: PSP_CTRL_MODE_ANALOG para incluir soporte del stick analógico */
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    printf("=========================================\n");
    printf("   Asteroids Classic 2D - PSP 1000       \n");
    printf("=========================================\n");
    printf("Controles de prueba:\n");
    printf(" - Flechas (D-Pad): Direccion\n");
    printf(" - Cruz (X): Disparo / Accion\n");
    printf(" - START + SELECT: Salir\n");
    printf("=========================================\n\n");

    while (running)
    {
        // Lee el estado actual de los controles de la PSP
        sceCtrlReadBufferPositive(&pad, 1);

        // Limpia un área pequeña para refrescar lecturas en pantalla (coordenadas de texto)
        pspDebugScreenSetXY(0, 10);

        // Lectura de botones
        if (pad.Buttons & PSP_CTRL_UP)
            printf("Boton presionado: ARRIBA    \n");
        else if (pad.Buttons & PSP_CTRL_DOWN)
            printf("Boton presionado: ABAJO     \n");
        else if (pad.Buttons & PSP_CTRL_LEFT)
            printf("Boton presionado: IZQUIERDA \n");
        else if (pad.Buttons & PSP_CTRL_RIGHT)
            printf("Boton presionado: DERECHA   \n");
        else if (pad.Buttons & PSP_CTRL_CROSS)
            printf("Boton presionado: CRUZ (X)  \n");
        else
            printf("Esperando entrada...        \n");

        // Condición rápida de salida: START + SELECT al mismo tiempo
        if ((pad.Buttons & PSP_CTRL_START) && (pad.Buttons & PSP_CTRL_SELECT))
        {
            printf("\nSalida forzada por combinacion de botones.\n");
            running = 0;
        }

        // Espera de refresco vertical a 60 FPS
        sceDisplayWaitVblankStart();
    }

    printf("\nSaliendo al XMB...");
    sceKernelDelayThread(1000000);
    sceKernelExitGame();

    return 0;
}