
asm(".global _sceFiberAttachContextAndRun\n"
    ".type _sceFiberAttachContextAndRun @function\n"
    "_sceFiberAttachContextAndRun:\n");

asm(".global _sceFiberAttachContextAndSwitch\n"
    ".type _sceFiberAttachContextAndSwitch @function\n"
    "_sceFiberAttachContextAndSwitch:\n");

asm(".global _sceFiberGetThreadFramePointerAddress\n"
    ".type _sceFiberGetThreadFramePointerAddress @function\n"
    "_sceFiberGetThreadFramePointerAddress:\n");

asm(".global _sceFiberInitializeImpl\n"
    ".type _sceFiberInitializeImpl @function\n"
    "_sceFiberInitializeImpl:\n");

asm(".global _sceFiberInitializeWithInternalOptionImpl\n"
    ".type _sceFiberInitializeWithInternalOptionImpl @function\n"
    "_sceFiberInitializeWithInternalOptionImpl:\n");

asm(".global sceFiberFinalize\n"
    ".type sceFiberFinalize @function\n"
    "sceFiberFinalize:\n");

asm(".global sceFiberGetInfo\n"
    ".type sceFiberGetInfo @function\n"
    "sceFiberGetInfo:\n");

asm(".global sceFiberGetSelf\n"
    ".type sceFiberGetSelf @function\n"
    "sceFiberGetSelf:\n");

asm(".global sceFiberOptParamInitialize\n"
    ".type sceFiberOptParamInitialize @function\n"
    "sceFiberOptParamInitialize:\n");

asm(".global sceFiberRename\n"
    ".type sceFiberRename @function\n"
    "sceFiberRename:\n");

asm(".global sceFiberReturnToThread\n"
    ".type sceFiberReturnToThread @function\n"
    "sceFiberReturnToThread:\n");

asm(".global sceFiberRun\n"
    ".type sceFiberRun @function\n"
    "sceFiberRun:\n");

asm(".global sceFiberStartContextSizeCheck\n"
    ".type sceFiberStartContextSizeCheck @function\n"
    "sceFiberStartContextSizeCheck:\n");

asm(".global sceFiberStopContextSizeCheck\n"
    ".type sceFiberStopContextSizeCheck @function\n"
    "sceFiberStopContextSizeCheck:\n");

asm(".global sceFiberSwitch\n"
    ".type sceFiberSwitch @function\n"
    "sceFiberSwitch:\n");
