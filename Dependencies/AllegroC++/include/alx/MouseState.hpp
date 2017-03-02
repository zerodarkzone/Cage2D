#ifndef ALX_MOUSESTATE_HPP
#define ALX_MOUSESTATE_HPP


#include <allegro5/allegro.h>
#include <alx/Value.hpp>


namespace alx {


/**
	Value-based wrapper class around ALLEGRO_MOUSE_STATE.
 */
class MouseState : public Value<ALLEGRO_MOUSE_STATE> {
public:
	/**
		The default constructor.
	 */
	MouseState() {
	}

	/**
		constructor from state.
		@param state state.
	 */
	MouseState(const ALLEGRO_MOUSE_STATE &state) : Value(state) {
	}

	/**
		Retrieves the mouse state.
	 */
	void retrieve() {
		al_get_mouse_state(&get());
	}

	ALLEGRO_MOUSE_STATE retrieve() const{
		ALLEGRO_MOUSE_STATE state;
		al_get_mouse_state(&state);
		return state;
	}

	/**
		Returns the given axis' value.
		@param axis asix index.
		@return the given axis' value.
	 */
	int getAxis(int axis) const {
		return al_get_mouse_state_axis(&get(), axis);
	}

	/**
		Checks if a button was down in this state.
		@param button button index; 1 for first button.
		@return true if the key was down.
	 */
	bool isButtonDown(int button) const {
		return al_mouse_button_down(&get(), button);
	}

	/**
		Returns the mouse x.
		@return the mouse x.
	 */
	int getMouseX() const {		
		return retrieve().x;
	}

	/**
		Returns the mouse y.
		@return the mouse y.
	 */
	int getMouseY() const {		
		return retrieve().y;
	}

	/**
		Returns the mouse w.
		@return the mouse w.
	 */
	int getMouseW() const {		
		return retrieve().w;
	}

	/**
		Returns the mouse z.
		@return the mouse z.
	 */
	int getMouseZ() const {
		return retrieve().z;
	}

	/**
		Returns the mouse buttons .
		@return the mouse x.
	 */
	int getMouseButtons() const {		
		return retrieve().buttons;
	}
};


} //namespace alx


#endif //ALX_MOUSESTATE_HPP
