
asm(".global sceJpegDecCreate\n"
    ".type sceJpegDecCreate @function\n"
    "sceJpegDecCreate:\n");

asm(".global sceJpegDecDecode\n"
    ".type sceJpegDecDecode @function\n"
    "sceJpegDecDecode:\n");

asm(".global sceJpegDecDecodeWithInputControl\n"
    ".type sceJpegDecDecodeWithInputControl @function\n"
    "sceJpegDecDecodeWithInputControl:\n");

asm(".global sceJpegDecDelete\n"
    ".type sceJpegDecDelete @function\n"
    "sceJpegDecDelete:\n");

asm(".global sceJpegDecParseHeader\n"
    ".type sceJpegDecParseHeader @function\n"
    "sceJpegDecParseHeader:\n");

asm(".global sceJpegDecQueryMemorySize\n"
    ".type sceJpegDecQueryMemorySize @function\n"
    "sceJpegDecQueryMemorySize:\n");
