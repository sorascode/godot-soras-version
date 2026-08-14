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

#pragma once

#include "scene/2d/physics/kinematic_collision_2d.h"
#include "scene/2d/physics/physics_body_2d.h"
#include "scene/2d/physics/solid_body_2d.h"

class CharacterBody2D : public PhysicsBody2D {
	GDCLASS(CharacterBody2D, PhysicsBody2D);

	bool ignores_one_way = false;

protected:
	void _notification(int p_what);
	static void _bind_methods();

	float carry_speed_grace = 0.16f;

	Vector2 current_carry_speed;
	Vector2 last_carry_speed;
	float carry_speed_timer;

	void set_carry_speed_grace(const float &p_speed);
	float get_carry_speed_grace() const;

	void set_carry_speed(const Vector2 &p_speed);
	Vector2 get_carry_speed() const;

	void _carry_speed_changed(const Vector2 &p_speed);

public:
	bool move_h_exact(int32_t p_amount, const Callable &p_callback = Callable(), const RID &p_pusher = RID()) override;
	bool move_v_exact(int32_t p_amount, const Callable &p_callback = Callable(), const RID &p_pusher = RID()) override;

	bool on_ground() override;

	void set_ignores_one_way(bool p_enable);
	bool is_ignores_one_way_enabled() const;

	bool _is_riding_solid(const RID &p_solid);
	bool _is_riding_one_way(const RID &p_one_way);
	void _squish(const Vector2i &p_move_dir, const int32_t p_amount_moved, const int32_t p_amount_left, Node2D *p_collided_with, const RID &p_collided_with_rid, const Vector2i &p_contact_point, const RID &p_pusher);

	GDVIRTUAL1R(bool, _is_riding_solid, RID)
	GDVIRTUAL1R(bool, _is_riding_one_way, RID)
	GDVIRTUAL7(_squish, Vector2i, int32_t, int32_t, Node2D *, RID, Vector2i, RID)

	CharacterBody2D();
};
