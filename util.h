#pragma once

#include "common.h"

#include <kernel_ex.h>

SceKernelModule load_module_from_sandbox(const char* name, size_t args, const void* argp, unsigned int flags, const SceKernelLoadModuleOpt* opts, int* res);

bool get_module_base(const char* name, uint64_t* base, uint64_t* size);

typedef void module_patch_cb_t(void* arg, uint8_t* base, uint64_t size);
bool patch_module(const char* name, module_patch_cb_t* cb, void* arg);

void hexdump(const void* data, size_t size);

void send_notify(const char* format, ...);
