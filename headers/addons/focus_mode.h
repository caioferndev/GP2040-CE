#ifndef _FocusMode_H
#define _FocusMode_H

#include "gpaddon.h"
#include "gamepad.h"

#ifndef FOCUS_MODE_ENABLED
#define FOCUS_MODE_ENABLED 0
#endif

#ifndef FOCUS_MODE_PIN
#define FOCUS_MODE_PIN -1
#endif

#ifndef FOCUS_MODE_BUTTON_MASK
#define FOCUS_MODE_BUTTON_MASK 0
#endif

#ifndef FOCUS_MODE_OLED_LOCK_ENABLED
#define FOCUS_MODE_OLED_LOCK_ENABLED 0
#endif

#ifndef FOCUS_MODE_RGB_LOCK_ENABLED
#define FOCUS_MODE_RGB_LOCK_ENABLED 0
#endif

#ifndef FOCUS_MODE_BUTTON_LOCK_ENABLED
#define FOCUS_MODE_BUTTON_LOCK_ENABLED 0
#endif

#ifndef FOCUS_MODE_MACRO_LOCK_ENABLED
#define FOCUS_MODE_MACRO_LOCK_ENABLED 1
#endif

#ifndef FOCUS_MODE_INVERT_SWITCH
#define FOCUS_MODE_INVERT_SWITCH 0
#endif

// FocusMode Module Name
#define FocusModeName "FocusMode"

class FocusModeAddon : public GPAddon {
public:
    virtual bool available();
    virtual void setup();       // FocusMode Setup
    virtual void process();     // FocusMode Process
    virtual void preprocess() {}
    virtual void postprocess(bool sent) {}
    virtual void reinit() {}
    virtual std::string name() { return FocusModeName; }
    static bool isMacroLockEnabled(const FocusModeOptions& options) {
        return options.enabled && options.macroLockEnabled; 
    }
    static bool isFocusModeActive(const FocusModeOptions& options, const Gamepad * gamepad) {
        // focus mode is active if:
        //   1. override is on
        //   2. focus mode pin is pressed
        //   3. if invert switch is on, focus mode pin is NOT pressed
        return options.overrideEnabled ||
            (gamepad->mapFocusMode->pinMask && ((gamepad->debouncedGpio & gamepad->mapFocusMode->pinMask) ^ (options.invertSwitch ? gamepad->mapFocusMode->pinMask : 0)));
    }
private:
    uint32_t buttonLockMask;
    uint32_t invertMask;
};

#endif  // _FocusMode_H_