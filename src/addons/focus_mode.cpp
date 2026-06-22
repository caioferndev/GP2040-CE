#include "addons/focus_mode.h"
#include "storagemanager.h"
#include "hardware/gpio.h"

bool FocusModeAddon::available() {
	const FocusModeOptions& options = Storage::getInstance().getAddonOptions().focusModeOptions;
	return options.enabled && options.buttonLockMask != 0;
}

void FocusModeAddon::setup() {
	const FocusModeOptions& options = Storage::getInstance().getAddonOptions().focusModeOptions;
	buttonLockMask = options.buttonLockMask;
	// lets make this more complicated. make an "invert mask". 
	// if invert switch is enabled, take the pre-existing mask and xor it with the debounced pins.
	Gamepad * gamepad = Storage::getInstance().GetGamepad();
	invertMask = options.invertSwitch ? gamepad->mapFocusMode->pinMask : 0; // gamepad re-inits after profile switch. we can store 
}

void FocusModeAddon::process() {
	// Override Enabled Focus-Mode Toggle OR the pin has been pressed
	const FocusModeOptions& options = Storage::getInstance().getAddonOptions().focusModeOptions;
	Gamepad * gamepad = Storage::getInstance().GetGamepad();
	if (isFocusModeActive(options, gamepad)) {
		if (buttonLockMask & GAMEPAD_MASK_DU) {
			gamepad->state.dpad &= ~GAMEPAD_MASK_UP;
		}
		if (buttonLockMask & GAMEPAD_MASK_DD) {
			gamepad->state.dpad &= ~GAMEPAD_MASK_DOWN;
		}
		if (buttonLockMask & GAMEPAD_MASK_DL) {
			gamepad->state.dpad &= ~GAMEPAD_MASK_LEFT;
		}
		if (buttonLockMask & GAMEPAD_MASK_DR) {
			gamepad->state.dpad &= ~GAMEPAD_MASK_RIGHT;
		}
		gamepad->state.buttons &= ~buttonLockMask;
	}
}