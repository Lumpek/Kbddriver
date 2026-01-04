#include <windows.h>
#include <setupapi.h>
#include <initguid.h>
#include <iostream>
#include <vector>

DEFINE_GUID(GUID_DEVINTERFACE_KBFILTER, 0x3fb7299d, 0x6847, 0x4490, 0xb0, 0xc9, 0x99, 0xe0, 0x98, 0x6a, 0xb8, 0x86);

#include "public.h" 

// Helper to find device path
std::wstring GetDevicePath(const GUID& InterfaceGuid) {
    HDEVINFO hDevInfo = SetupDiGetClassDevs(&InterfaceGuid, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
    if (hDevInfo == INVALID_HANDLE_VALUE) return L"";
    SP_DEVICE_INTERFACE_DATA deviceInterfaceData = { 0 };
    deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    if (!SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &InterfaceGuid, 0, &deviceInterfaceData)) { SetupDiDestroyDeviceInfoList(hDevInfo); return L""; }
    DWORD requiredSize = 0;
    SetupDiGetDeviceInterfaceDetail(hDevInfo, &deviceInterfaceData, NULL, 0, &requiredSize, NULL);
    std::vector<BYTE> buffer(requiredSize);
    PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)buffer.data();
    detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
    if (SetupDiGetDeviceInterfaceDetail(hDevInfo, &deviceInterfaceData, detailData, requiredSize, NULL, NULL)) { SetupDiDestroyDeviceInfoList(hDevInfo); return std::wstring(detailData->DevicePath); }
    SetupDiDestroyDeviceInfoList(hDevInfo); return L"";
}

int main() {
    std::cout << "--- Keyboard Filter Controller ---\n";
    std::wstring devicePath = GetDevicePath(GUID_DEVINTERFACE_KBFILTER);
    if (devicePath.empty()) { std::cerr << "Driver not found.\n"; return 1; }

    HANDLE hDevice = CreateFile(devicePath.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (hDevice == INVALID_HANDLE_VALUE) { std::cerr << "Open failed.\n"; return 1; }

    while (true) {
        int prob, mode;
        std::cout << "\nSelect Mode:\n 0: Normal\n 1: Chaos (Letters + Backspace)\n 2: Drop letters\n 3: Drop only Space\n -1: Exit\n> ";
        std::cin >> mode;
        if (mode == -1) break;

        if (mode != 0) {
            std::cout << "Probability (0-100): ";
            std::cin >> prob;
        } else {
            prob = 0;
        }

        KB_CONFIG config;
        config.Probability = (ULONG)prob;
        config.Mode = (ULONG)mode;
        DWORD bytes;

        if (DeviceIoControl(hDevice, IOCTL_SET_PROBABILITY, &config, sizeof(config), NULL, 0, &bytes, NULL))
            std::cout << "Config sent.\n";
        else
            std::cerr << "Error: " << GetLastError() << "\n";
    }
    CloseHandle(hDevice);
    return 0;
}