#include "addons/focus_mode.h"
#include "storagemanager.h"
#include "hardware/gpio.h"

uint32_t FocusModeAddon::invertSwitchMask = 0;

bool FocusModeAddon::isMacroLockEnabled(const FocusModeOptions& options) {
	return options.enabled && options.macroLockEnabled; 
}

bool FocusModeAddon::isFocusModeActive(const FocusModeOptions& options, const Gamepad * gamepad) {
	return options.overrideEnabled ||
		(gamepad->mapFocusMode->pinMask && ((gamepad->debouncedGpio & gamepad->mapFocusMode->pinMask) ^ (FocusModeAddon::invertSwitchMask)));
}

bool FocusModeAddon::available() {
	const FocusModeOptions& options = Storage::getInstance().getAddonOptions().focusModeOptions;
	return options.enabled && options.buttonLockMask != 0;
}

void FocusModeAddon::setup() {
	const FocusModeOptions& options = Storage::getInstance().getAddonOptions().focusModeOptions;

	// gamepad re-inits after profile switch. we can store masks to avoid re-calculation
	dpadLockMask = 0;
	dpadLockMask |= options.buttonLockMask & GAMEPAD_MASK_DU ? GAMEPAD_MASK_UP : 0;
	dpadLockMask |= options.buttonLockMask & GAMEPAD_MASK_DD ? GAMEPAD_MASK_DOWN : 0;
	dpadLockMask |= options.buttonLockMask & GAMEPAD_MASK_DL ? GAMEPAD_MASK_LEFT : 0;
	dpadLockMask |= options.buttonLockMask & GAMEPAD_MASK_DR ? GAMEPAD_MASK_RIGHT : 0;

	// webconfig stores buttons to mask. invert to mask incoming bits
	dpadLockMask = ~dpadLockMask;
	buttonLockMask = ~options.buttonLockMask; 

	Gamepad * gamepad = Storage::getInstance().GetGamepad();
	FocusModeAddon::invertSwitchMask = options.invertSwitch ? gamepad->mapFocusMode->pinMask : 0; 
}

void FocusModeAddon::process() {
	const FocusModeOptions& options = Storage::getInstance().getAddonOptions().focusModeOptions;
	Gamepad * gamepad = Storage::getInstance().GetGamepad();
	if (isFocusModeActive(options, gamepad)) {
		gamepad->state.dpad &= dpadLockMask;
		gamepad->state.buttons &= buttonLockMask;
	}
}