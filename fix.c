#ifdef NVML_VERSION

#include <nvml.h>
#include <dlfcn.h>
#include <stddef.h>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define NVML_FILENAME "libnvidia-ml.so." TOSTRING(NVML_VERSION)

static typeof(nvmlInit_v2) *real_nvmlInit_v2;
static typeof(nvmlDeviceGetHandleByIndex_v2) *real_nvmlDeviceGetHandleByIndex_v2;
static typeof(nvmlDeviceGetHandleByPciBusId_v2) *real_nvmlDeviceGetHandleByPciBusId_v2;
static typeof(nvmlDeviceGetHandleBySerial) *real_nvmlDeviceGetHandleBySerial;
static typeof(nvmlDeviceGetHandleByUUID) *real_nvmlDeviceGetHandleByUUID;

void fixDevice(nvmlDevice_t *);

nvmlReturn_t nvmlInit_v2() {
    void *realNVML = dlopen(NVML_FILENAME, RTLD_NOW);
    if (!(real_nvmlInit_v2 = dlsym(realNVML, "nvmlInit_v2"))) {
        return NVML_ERROR_FUNCTION_NOT_FOUND;
    }
    if (!(real_nvmlDeviceGetHandleByIndex_v2 = dlsym(realNVML, "nvmlDeviceGetHandleByIndex_v2"))) {
        return NVML_ERROR_FUNCTION_NOT_FOUND;
    }
    if (!(real_nvmlDeviceGetHandleByPciBusId_v2 = dlsym(realNVML, "nvmlDeviceGetHandleByPciBusId_v2"))) {
        return NVML_ERROR_FUNCTION_NOT_FOUND;
    }
    if (!(real_nvmlDeviceGetHandleBySerial = dlsym(realNVML, "nvmlDeviceGetHandleBySerial"))) {
        return NVML_ERROR_FUNCTION_NOT_FOUND;
    }
    if (!(real_nvmlDeviceGetHandleByUUID = dlsym(realNVML, "nvmlDeviceGetHandleByUUID"))) {
        return NVML_ERROR_FUNCTION_NOT_FOUND;
    }
    return real_nvmlInit_v2();
}

nvmlReturn_t nvmlDeviceGetHandleByIndex_v2(unsigned int index, nvmlDevice_t *device) {
    nvmlReturn_t result;
    if (!real_nvmlDeviceGetHandleByIndex_v2) {
        return NVML_ERROR_UNINITIALIZED;
    }
    result = real_nvmlDeviceGetHandleByIndex_v2(index, device);
    if (result != NVML_SUCCESS) {
        return result;
    }
    fixDevice(device);
    return NVML_SUCCESS;
};

nvmlReturn_t nvmlDeviceGetHandleByPciBusId_v2(const char *pciBusId, nvmlDevice_t *device) {
    nvmlReturn_t result;
    if (!real_nvmlDeviceGetHandleByIndex_v2) {
        return NVML_ERROR_UNINITIALIZED;
    }
    result = real_nvmlDeviceGetHandleByPciBusId_v2(pciBusId, device);
    if (result != NVML_SUCCESS) {
        return result;
    }
    fixDevice(device);
    return NVML_SUCCESS;
};

nvmlReturn_t nvmlDeviceGetHandleBySerial(const char *serial, nvmlDevice_t *device) {
    nvmlReturn_t result;
    if (!real_nvmlDeviceGetHandleByIndex_v2) {
        return NVML_ERROR_UNINITIALIZED;
    }
    result = real_nvmlDeviceGetHandleBySerial(serial, device);
    if (result != NVML_SUCCESS) {
        return result;
    }
    fixDevice(device);
    return NVML_SUCCESS;
};

nvmlReturn_t nvmlDeviceGetHandleByUUID(const char *uuid, nvmlDevice_t *device) {
    nvmlReturn_t result;
    if (!real_nvmlDeviceGetHandleByIndex_v2) {
        return NVML_ERROR_UNINITIALIZED;
    }
    result = real_nvmlDeviceGetHandleByUUID(uuid, device);
    if (result != NVML_SUCCESS) {
        return result;
    }
    fixDevice(device);
    return NVML_SUCCESS;
};

void fixDevice(nvmlDevice_t *device) {
    //TODO: nvmlDevice_t?
}

#else
#error "Missing NVML_VERSION definition"
#endif