/*
 * PSP Software Development Kit - https://github.com/pspdev
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * main.c - Basic Input demo -- reads from control pad and indicates button
 *          presses.
 *
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 * Copyright (c) 2005 Donour Sizemore <donour@uchicago.edu>
 *
 */
#include <dxlibp.h>
#include <pspctrl.h>
#include <pspdebug.h>
#include <pspkernel.h>
#include <stdlib.h>
#include <string.h>

PSP_MODULE_INFO("TEST", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

void dump_threadstatus(void);

int done = 0;

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common) {
  done = 1;
  return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp) {
  int cbid;

  cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
  sceKernelRegisterExitCallback(cbid);
  sceKernelSleepThreadCB();

  return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void) {
  int thid = 0;

  thid =
      sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
  if (thid >= 0) {
    sceKernelStartThread(thid, 0, 0);
  }

  return thid;
}

int main() {
  SetupCallbacks();
  DxLib_Init();
  screen_setting();
  while (ProcessMessage() != -1) {
    ClearDrawScreen();
    DrawBox(0, 0, 480, 272, DXP_COLOR_WHITE, 1);
    DrawString(0, 0, "こんにちは世界ABCD", DXP_COLOR_BLACK, DXP_COLOR_BLACK);
    ScreenFlip();
  }
  DxLib_End();
  return 0;
}
