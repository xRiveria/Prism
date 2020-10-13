#pragma once

//Sourced from GLFW.h.
/* Printable Keys */

//This #ifdef solution won't with save files across platforms. We can serialize strings and do lookups to circumvent this. Alternatively, setup a lookup function with a key table whenever a key is used.
#ifdef PRISM_PLATFORM_WINDOWS  
#define PRISM_KEY_SPACE              32  //The numbers line up with ASCII code. This means that we can just convert the numbers to a char and we can print the character to the console. 
#define PRISM_KEY_APOSTROPHE         39  /* ' */
#define PRISM_KEY_COMMA              44  /* , */
#define PRISM_KEY_MINUS              45  /* - */
#define PRISM_KEY_PERIOD             46  /* . */
#define PRISM_KEY_SLASH              47  /* / */
#define PRISM_KEY_0                  48
#define PRISM_KEY_1                  49
#define PRISM_KEY_2                  50
#define PRISM_KEY_3                  51
#define PRISM_KEY_4                  52
#define PRISM_KEY_5                  53
#define PRISM_KEY_6                  54
#define PRISM_KEY_7                  55
#define PRISM_KEY_8                  56
#define PRISM_KEY_9                  57
#define PRISM_KEY_SEMICOLON          59  /* ; */
#define PRISM_KEY_EQUAL              61  /* = */
#define PRISM_KEY_A                  65
#define PRISM_KEY_B                  66
#define PRISM_KEY_C                  67
#define PRISM_KEY_D                  68
#define PRISM_KEY_E                  69
#define PRISM_KEY_F                  70
#define PRISM_KEY_G                  71
#define PRISM_KEY_H                  72
#define PRISM_KEY_I                  73
#define PRISM_KEY_J                  74
#define PRISM_KEY_K                  75
#define PRISM_KEY_L                  76
#define PRISM_KEY_M                  77
#define PRISM_KEY_N                  78
#define PRISM_KEY_O                  79
#define PRISM_KEY_P                  80
#define PRISM_KEY_Q                  81
#define PRISM_KEY_R                  82
#define PRISM_KEY_S                  83
#define PRISM_KEY_T                  84
#define PRISM_KEY_U                  85
#define PRISM_KEY_V                  86
#define PRISM_KEY_W                  87
#define PRISM_KEY_X                  88
#define PRISM_KEY_Y                  89
#define PRISM_KEY_Z                  90
#define PRISM_KEY_LEFT_BRACKET       91  /* [ */
#define PRISM_KEY_BACKSLASH          92  /* \ */
#define PRISM_KEY_RIGHT_BRACKET      93  /* ] */
#define PRISM_KEY_GRAVE_ACCENT       96  /* ` */
#define PRISM_KEY_WORLD_1            161 /* non-US #1 */
#define PRISM_KEY_WORLD_2            162 /* non-US #2 */

/* Function Keys */

#define PRISM_KEY_ESCAPE             256
#define PRISM_KEY_ENTER              257
#define PRISM_KEY_TAB                258
#define PRISM_KEY_BACKSPACE          259
#define PRISM_KEY_INSERT             260
#define PRISM_KEY_DELETE             261
#define PRISM_KEY_RIGHT              262
#define PRISM_KEY_LEFT               263
#define PRISM_KEY_DOWN               264
#define PRISM_KEY_UP                 265
#define PRISM_KEY_PAGE_UP            266
#define PRISM_KEY_PAGE_DOWN          267
#define PRISM_KEY_HOME               268
#define PRISM_KEY_END                269
#define PRISM_KEY_CAPS_LOCK          280
#define PRISM_KEY_SCROLL_LOCK        281
#define PRISM_KEY_NUM_LOCK           282
#define PRISM_KEY_PRINT_SCREEN       283
#define PRISM_KEY_PAUSE              284
#define PRISM_KEY_F1                 290
#define PRISM_KEY_F2                 291
#define PRISM_KEY_F3                 292
#define PRISM_KEY_F4                 293
#define PRISM_KEY_F5                 294
#define PRISM_KEY_F6                 295
#define PRISM_KEY_F7                 296
#define PRISM_KEY_F8                 297
#define PRISM_KEY_F9                 298
#define PRISM_KEY_F10                299
#define PRISM_KEY_F11                300
#define PRISM_KEY_F12                301
#define PRISM_KEY_F13                302
#define PRISM_KEY_F14                303
#define PRISM_KEY_F15                304
#define PRISM_KEY_F16                305
#define PRISM_KEY_F17                306
#define PRISM_KEY_F18                307
#define PRISM_KEY_F19                308
#define PRISM_KEY_F20                309
#define PRISM_KEY_F21                310
#define PRISM_KEY_F22                311
#define PRISM_KEY_F23                312
#define PRISM_KEY_F24                313
#define PRISM_KEY_F25                314
#define PRISM_KEY_KP_0               320
#define PRISM_KEY_KP_1               321
#define PRISM_KEY_KP_2               322
#define PRISM_KEY_KP_3               323
#define PRISM_KEY_KP_4               324
#define PRISM_KEY_KP_5               325
#define PRISM_KEY_KP_6               326
#define PRISM_KEY_KP_7               327
#define PRISM_KEY_KP_8               328
#define PRISM_KEY_KP_9               329
#define PRISM_KEY_KP_DECIMAL         330
#define PRISM_KEY_KP_DIVIDE          331
#define PRISM_KEY_KP_MULTIPLY        332
#define PRISM_KEY_KP_SUBTRACT        333
#define PRISM_KEY_KP_ADD             334
#define PRISM_KEY_KP_ENTER           335
#define PRISM_KEY_KP_EQUAL           336
#define PRISM_KEY_LEFT_SHIFT         340
#define PRISM_KEY_LEFT_CONTROL       341
#define PRISM_KEY_LEFT_ALT           342
#define PRISM_KEY_LEFT_SUPER         343
#define PRISM_KEY_RIGHT_SHIFT        344
#define PRISM_KEY_RIGHT_CONTROL      345
#define PRISM_KEY_RIGHT_ALT          346
#define PRISM_KEY_RIGHT_SUPER        347
#define PRISM_KEY_MENU               348

#endif