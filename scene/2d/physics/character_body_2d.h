/**************************************************************************/
/*  character_body_2d.h                                                   */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef CHARACTER_BODY_2D_H
#define CHARACTER_BODY_2D_H

#include "scene/2d/physics/kinematic_collision_2d.h"
#include "scene/2d/physics/physics_body_2d.h"
#include "scene/2d/physics/solid_body_2d.h"

class CharacterBody2D : public PhysicsBody2D {
	GDCLASS(CharacterBody2D, PhysicsBody2D);

protected:
	static void _bind_methods();

public:
	bool move_h_exact(int32_t p_amount, const Callable &p_callback) override;
	bool move_v_exact(int32_t p_amount, const Callable &p_callback) override;

	bool _is_riding(const RID &p_solid);
	void _squish();

	GDVIRTUAL1R(bool, _is_riding, RID)
	GDVIRTUAL0(_squish)

	CharacterBody2D();
};

#endif // CHARACTER_BODY_2D_H
