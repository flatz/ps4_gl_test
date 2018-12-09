#include "util.h"

#include <stdarg.h>
#include <ctype.h>

#include <kernel_ex.h>

SceKernelModule load_module_from_sandbox(const char* name, size_t args, const void* argp, unsigned int flags, const SceKernelLoadModuleOpt* opts, int* res) {
	char file_path[SCE_KERNEL_MAX_NAME_LENGTH];
	SceKernelModule module;

	snprintf(file_path, sizeof(file_path), "/%s/common/lib/%s", g_sandbox_word, name);

	module = sceKernelLoadStartModule(file_path, args, argp, flags, opts, res);

	return module;
}

bool get_module_base(const char* name, uint64_t* base, uint64_t* size) {
	SceKernelModuleInfo moduleInfo;
	int ret;

	ret = sceKernelGetModuleInfoByName(name, &moduleInfo);
	if (ret) {
		EPRINTF("sceKernelGetModuleInfoByName(%s) failed: 0x%08X\n", name, ret);
		goto err;
	}

	if (base) {
		*base = (uint64_t)moduleInfo.segmentInfo[0].baseAddr;
	}
	if (size) {
		*size = moduleInfo.segmentInfo[0].size;
	}

	return true;

err:
	return false;
}

bool patch_module(const char* name, module_patch_cb_t* cb, void* arg) {
	uint64_t base, size;
	int ret;

	if (!get_module_base(name, &base, &size)) {
		goto err;
	}

	ret = sceKernelMprotect((void*)base, size, SCE_KERNEL_PROT_CPU_READ | SCE_KERNEL_PROT_CPU_WRITE | SCE_KERNEL_PROT_CPU_EXEC);
	if (ret) {
		EPRINTF("sceKernelMprotect(%s) failed: 0x%08X\n", name, ret);
		goto err;
	}

	if (cb) {
		(*cb)(arg, (uint8_t*)base, size);
	}

	return true;

err:
	return false;
}

void send_notify(const char* format, ...) {
	SceNotificationRequest req;
	va_list args;

	memset(&req, 0, sizeof(req));
	{
		req.unk_0x10 = -1;

		va_start(args, format);
		req.buf[0] = '%'; /* XXX: hackity hack */
		vsnprintf((char*)req.buf + 1, 0xB4, format, args);
		va_end(args);
	}

	sceKernelSendNotificationRequest(0, &req, sizeof(req), 1);
}

void hexdump(const void* data, size_t size) {
	const uint8_t* p = (const uint8_t*)data;
	const size_t n = 16;
	size_t i, j, k;
	for (i = 0; i < size; i += n) {
		k = (i + n) <= size ? n : (size - i);
		printf("%8p:", (uint8_t*)data + i);
		for (j = 0; j < k; ++j) {
			printf(" %02x", p[i + j]);
		}
		for (j = k; j < n; ++j) {
			printf("   ");
		}
		printf("  ");
		for (j = 0; j < k; ++j) {
			printf("%c", isprint(p[i + j]) ? p[i + j] : '.');
		}
		for (j = k; j < n; ++j) {
			printf(" ");
		}
		printf("\n");
	}
}
