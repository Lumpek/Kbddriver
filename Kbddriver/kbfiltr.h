/*++
Copyright (c) 1997  Microsoft Corporation

Module Name:

    kbfilter.h

Abstract:

    This module contains the common private declarations for the keyboard
    packet filter

Environment:

    kernel mode only

--*/
#ifndef KBFILTER_H
#define KBFILTER_H

#pragma warning(disable:4201)
#include "ntddk.h"
#include "kbdmou.h"
#include <ntddkbd.h>
#include <ntdd8042.h>
#include <wdf.h>
#define NTSTRSAFE_LIB
#include <ntstrsafe.h>
#include <initguid.h>
#include <devguid.h>
#include "public.h"
#pragma warning(default:4201)

#define KBFILTER_POOL_TAG (ULONG) 'tlfK'

#if DBG
#define DebugPrint(_x_) DbgPrint _x_
#else
#define DebugPrint(_x_)
#endif

typedef struct _DEVICE_EXTENSION
{
    WDFDEVICE WdfDevice;
    WDFQUEUE rawPdoQueue;

    // The real connect data that this driver reports to
    CONNECT_DATA UpperConnectData;

    // Cached Keyboard Attributes (for the app)
    KEYBOARD_ATTRIBUTES KeyboardAttributes;

} DEVICE_EXTENSION, * PDEVICE_EXTENSION;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_EXTENSION, FilterGetData)

// Prototypes
DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD KbFilter_EvtDeviceAdd;
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL KbFilter_EvtIoDeviceControlFromRawPdo;
EVT_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL KbFilter_EvtIoInternalDeviceControl;

VOID KbFilter_ServiceCallback(
    IN PDEVICE_OBJECT DeviceObject,
    IN PKEYBOARD_INPUT_DATA InputDataStart,
    IN PKEYBOARD_INPUT_DATA InputDataEnd,
    IN OUT PULONG InputDataConsumed
);

EVT_WDF_REQUEST_COMPLETION_ROUTINE KbFilterRequestCompletionRoutine;

// Sideband (RawPDO) definitions
#define  KBFILTR_DEVICE_ID L"{A65C87F9-BE02-4ed9-92EC-012D416169FA}\\KeyboardFilter\0"
DEFINE_GUID(GUID_DEVINTERFACE_KBFILTER, 0x3fb7299d, 0x6847, 0x4490, 0xb0, 0xc9, 0x99, 0xe0, 0x98, 0x6a, 0xb8, 0x86);

typedef struct _RPDO_DEVICE_DATA
{
    ULONG InstanceNo;
    WDFQUEUE ParentQueue;
} RPDO_DEVICE_DATA, * PRPDO_DEVICE_DATA;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(RPDO_DEVICE_DATA, PdoGetData)

NTSTATUS KbFiltr_CreateRawPdo(WDFDEVICE Device, ULONG InstanceNo);

#endif