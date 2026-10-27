
asm(".global sceUsbdAllocTransfer\n"
    ".type sceUsbdAllocTransfer @function\n"
    "sceUsbdAllocTransfer:\n");

asm(".global sceUsbdAttachKernelDriver\n"
    ".type sceUsbdAttachKernelDriver @function\n"
    "sceUsbdAttachKernelDriver:\n");

asm(".global sceUsbdBulkTransfer\n"
    ".type sceUsbdBulkTransfer @function\n"
    "sceUsbdBulkTransfer:\n");

asm(".global sceUsbdCancelTransfer\n"
    ".type sceUsbdCancelTransfer @function\n"
    "sceUsbdCancelTransfer:\n");

asm(".global sceUsbdCheckConnected\n"
    ".type sceUsbdCheckConnected @function\n"
    "sceUsbdCheckConnected:\n");

asm(".global sceUsbdClaimInterface\n"
    ".type sceUsbdClaimInterface @function\n"
    "sceUsbdClaimInterface:\n");

asm(".global sceUsbdClearHalt\n"
    ".type sceUsbdClearHalt @function\n"
    "sceUsbdClearHalt:\n");

asm(".global sceUsbdClose\n"
    ".type sceUsbdClose @function\n"
    "sceUsbdClose:\n");

asm(".global sceUsbdControlTransfer\n"
    ".type sceUsbdControlTransfer @function\n"
    "sceUsbdControlTransfer:\n");

asm(".global sceUsbdControlTransferGetData\n"
    ".type sceUsbdControlTransferGetData @function\n"
    "sceUsbdControlTransferGetData:\n");

asm(".global sceUsbdControlTransferGetSetup\n"
    ".type sceUsbdControlTransferGetSetup @function\n"
    "sceUsbdControlTransferGetSetup:\n");

asm(".global sceUsbdDetachKernelDriver\n"
    ".type sceUsbdDetachKernelDriver @function\n"
    "sceUsbdDetachKernelDriver:\n");

asm(".global sceUsbdEventHandlerActive\n"
    ".type sceUsbdEventHandlerActive @function\n"
    "sceUsbdEventHandlerActive:\n");

asm(".global sceUsbdEventHandlingOk\n"
    ".type sceUsbdEventHandlingOk @function\n"
    "sceUsbdEventHandlingOk:\n");

asm(".global sceUsbdExit\n"
    ".type sceUsbdExit @function\n"
    "sceUsbdExit:\n");

asm(".global sceUsbdFillBulkTransfer\n"
    ".type sceUsbdFillBulkTransfer @function\n"
    "sceUsbdFillBulkTransfer:\n");

asm(".global sceUsbdFillControlSetup\n"
    ".type sceUsbdFillControlSetup @function\n"
    "sceUsbdFillControlSetup:\n");

asm(".global sceUsbdFillControlTransfer\n"
    ".type sceUsbdFillControlTransfer @function\n"
    "sceUsbdFillControlTransfer:\n");

asm(".global sceUsbdFillInterruptTransfer\n"
    ".type sceUsbdFillInterruptTransfer @function\n"
    "sceUsbdFillInterruptTransfer:\n");

asm(".global sceUsbdFillIsoTransfer\n"
    ".type sceUsbdFillIsoTransfer @function\n"
    "sceUsbdFillIsoTransfer:\n");

asm(".global sceUsbdFreeConfigDescriptor\n"
    ".type sceUsbdFreeConfigDescriptor @function\n"
    "sceUsbdFreeConfigDescriptor:\n");

asm(".global sceUsbdFreeDeviceList\n"
    ".type sceUsbdFreeDeviceList @function\n"
    "sceUsbdFreeDeviceList:\n");

asm(".global sceUsbdFreeTransfer\n"
    ".type sceUsbdFreeTransfer @function\n"
    "sceUsbdFreeTransfer:\n");

asm(".global sceUsbdGetActiveConfigDescriptor\n"
    ".type sceUsbdGetActiveConfigDescriptor @function\n"
    "sceUsbdGetActiveConfigDescriptor:\n");

asm(".global sceUsbdGetBusNumber\n"
    ".type sceUsbdGetBusNumber @function\n"
    "sceUsbdGetBusNumber:\n");

asm(".global sceUsbdGetConfigDescriptor\n"
    ".type sceUsbdGetConfigDescriptor @function\n"
    "sceUsbdGetConfigDescriptor:\n");

asm(".global sceUsbdGetConfigDescriptorByValue\n"
    ".type sceUsbdGetConfigDescriptorByValue @function\n"
    "sceUsbdGetConfigDescriptorByValue:\n");

asm(".global sceUsbdGetConfiguration\n"
    ".type sceUsbdGetConfiguration @function\n"
    "sceUsbdGetConfiguration:\n");

asm(".global sceUsbdGetDescriptor\n"
    ".type sceUsbdGetDescriptor @function\n"
    "sceUsbdGetDescriptor:\n");

asm(".global sceUsbdGetDevice\n"
    ".type sceUsbdGetDevice @function\n"
    "sceUsbdGetDevice:\n");

asm(".global sceUsbdGetDeviceAddress\n"
    ".type sceUsbdGetDeviceAddress @function\n"
    "sceUsbdGetDeviceAddress:\n");

asm(".global sceUsbdGetDeviceDescriptor\n"
    ".type sceUsbdGetDeviceDescriptor @function\n"
    "sceUsbdGetDeviceDescriptor:\n");

asm(".global sceUsbdGetDeviceList\n"
    ".type sceUsbdGetDeviceList @function\n"
    "sceUsbdGetDeviceList:\n");

asm(".global sceUsbdGetDeviceSpeed\n"
    ".type sceUsbdGetDeviceSpeed @function\n"
    "sceUsbdGetDeviceSpeed:\n");

asm(".global sceUsbdGetIsoPacketBuffer\n"
    ".type sceUsbdGetIsoPacketBuffer @function\n"
    "sceUsbdGetIsoPacketBuffer:\n");

asm(".global sceUsbdGetMaxIsoPacketSize\n"
    ".type sceUsbdGetMaxIsoPacketSize @function\n"
    "sceUsbdGetMaxIsoPacketSize:\n");

asm(".global sceUsbdGetMaxPacketSize\n"
    ".type sceUsbdGetMaxPacketSize @function\n"
    "sceUsbdGetMaxPacketSize:\n");

asm(".global sceUsbdGetStringDescriptor\n"
    ".type sceUsbdGetStringDescriptor @function\n"
    "sceUsbdGetStringDescriptor:\n");

asm(".global sceUsbdGetStringDescriptorAscii\n"
    ".type sceUsbdGetStringDescriptorAscii @function\n"
    "sceUsbdGetStringDescriptorAscii:\n");

asm(".global sceUsbdHandleEvents\n"
    ".type sceUsbdHandleEvents @function\n"
    "sceUsbdHandleEvents:\n");

asm(".global sceUsbdHandleEventsLocked\n"
    ".type sceUsbdHandleEventsLocked @function\n"
    "sceUsbdHandleEventsLocked:\n");

asm(".global sceUsbdHandleEventsTimeout\n"
    ".type sceUsbdHandleEventsTimeout @function\n"
    "sceUsbdHandleEventsTimeout:\n");

asm(".global sceUsbdInit\n"
    ".type sceUsbdInit @function\n"
    "sceUsbdInit:\n");

asm(".global sceUsbdInterruptTransfer\n"
    ".type sceUsbdInterruptTransfer @function\n"
    "sceUsbdInterruptTransfer:\n");

asm(".global sceUsbdKernelDriverActive\n"
    ".type sceUsbdKernelDriverActive @function\n"
    "sceUsbdKernelDriverActive:\n");

asm(".global sceUsbdLockEventWaiters\n"
    ".type sceUsbdLockEventWaiters @function\n"
    "sceUsbdLockEventWaiters:\n");

asm(".global sceUsbdLockEvents\n"
    ".type sceUsbdLockEvents @function\n"
    "sceUsbdLockEvents:\n");

asm(".global sceUsbdOpen\n"
    ".type sceUsbdOpen @function\n"
    "sceUsbdOpen:\n");

asm(".global sceUsbdOpenDeviceWithVidPid\n"
    ".type sceUsbdOpenDeviceWithVidPid @function\n"
    "sceUsbdOpenDeviceWithVidPid:\n");

asm(".global sceUsbdRefDevice\n"
    ".type sceUsbdRefDevice @function\n"
    "sceUsbdRefDevice:\n");

asm(".global sceUsbdReleaseInterface\n"
    ".type sceUsbdReleaseInterface @function\n"
    "sceUsbdReleaseInterface:\n");

asm(".global sceUsbdResetDevice\n"
    ".type sceUsbdResetDevice @function\n"
    "sceUsbdResetDevice:\n");

asm(".global sceUsbdSetConfiguration\n"
    ".type sceUsbdSetConfiguration @function\n"
    "sceUsbdSetConfiguration:\n");

asm(".global sceUsbdSetInterfaceAltSetting\n"
    ".type sceUsbdSetInterfaceAltSetting @function\n"
    "sceUsbdSetInterfaceAltSetting:\n");

asm(".global sceUsbdSetIsoPacketLengths\n"
    ".type sceUsbdSetIsoPacketLengths @function\n"
    "sceUsbdSetIsoPacketLengths:\n");

asm(".global sceUsbdSubmitTransfer\n"
    ".type sceUsbdSubmitTransfer @function\n"
    "sceUsbdSubmitTransfer:\n");

asm(".global sceUsbdTryLockEvents\n"
    ".type sceUsbdTryLockEvents @function\n"
    "sceUsbdTryLockEvents:\n");

asm(".global sceUsbdUnlockEventWaiters\n"
    ".type sceUsbdUnlockEventWaiters @function\n"
    "sceUsbdUnlockEventWaiters:\n");

asm(".global sceUsbdUnlockEvents\n"
    ".type sceUsbdUnlockEvents @function\n"
    "sceUsbdUnlockEvents:\n");

asm(".global sceUsbdUnrefDevice\n"
    ".type sceUsbdUnrefDevice @function\n"
    "sceUsbdUnrefDevice:\n");

asm(".global sceUsbdWaitForEvent\n"
    ".type sceUsbdWaitForEvent @function\n"
    "sceUsbdWaitForEvent:\n");
