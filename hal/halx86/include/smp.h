/*
 * PROJECT:     ReactOS Kernel
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Header File for SMP support
 * COPYRIGHT:   Copyright 2021 Justin Miller <justinmiller100@gmail.com>
 */

#pragma once

#define AFFINITY_MASK(Id) ((KAFFINITY)1 << (Id))

/* Helper to find the lowest CPU in a KAFFINITY */
#ifdef _WIN64
#define BitScanForwardAffinity BitScanForward64
#else
#define BitScanForwardAffinity BitScanForward
#endif

/* This table is filled for each physical processor on system */
typedef struct _PROCESSOR_IDENTITY
{
    UCHAR ProcessorId;
    UCHAR LapicId;
    BOOLEAN ProcessorStarted;
    BOOLEAN BSPCheck;
    PKPRCB ProcessorPrcb;

} PROCESSOR_IDENTITY, *PPROCESSOR_IDENTITY;

/* This table is counter of the overall APIC constants acquired from madt */
typedef struct _HALP_APIC_INFO_TABLE
{
    ULONG ApicMode;
    ULONG ProcessorCount; /* Count of all physical cores, This includes BSP */
    ULONG IOAPICCount;
    ULONG LocalApicPA;                // The 32-bit physical address at which each processor can access its local interrupt controller
    ULONG IoApicVA[256];
    ULONG IoApicPA[256];
    ULONG IoApicIrqBase[256]; // Global system interrupt base

} HALP_APIC_INFO_TABLE, *PHALP_APIC_INFO_TABLE;

VOID
HalpParseApicTables(
    _In_ PLOADER_PARAMETER_BLOCK LoaderBlock);

VOID
HalpSetupProcessorsTable(
    _In_ UINT32 NTProcessorNumber);

VOID
HalpPrintApicTables(VOID);

/* APIC specific functions inside apic/apicsmp.c */

VOID
ApicStartApplicationProcessor(
    _In_ ULONG NTProcessorNumber,
    _In_ PHYSICAL_ADDRESS StartupLoc);

VOID
NTAPI
HalpRequestIpi(
    _In_ KAFFINITY TargetProcessors);

VOID
NTAPI
HalpBroadcastIpiSpecifyVector(
    _In_ UCHAR Vector,
    _In_ BOOLEAN IncludeSelf);

VOID
NTAPI
HalRequestIpiSpecifyVector(
    _In_ KAFFINITY TargetSet,
    _In_ UCHAR Vector);

#ifdef _M_AMD64

NTHALAPI
VOID
NTAPI
HalpSendNMI(
    _In_ KAFFINITY TargetSet);

NTHALAPI
VOID
NTAPI
HalpSendSoftwareInterrupt(
    _In_ KAFFINITY TargetSet,
    _In_ KIRQL Irql);

#endif // _M_AMD64
