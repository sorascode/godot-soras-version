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

#ifndef SOLID_BODY_2D_H
#define SOLID_BODY_2D_H

#include "scene/2d/physics/kinematic_collision_2d.h"
#include "scene/2d/physics/physics_body_2d.h"

class SolidBody2D : public PhysicsBody2D {
	GDCLASS(SolidBody2D, PhysicsBody2D);

	bool one_way_collision = false;
	List<RID> riders;

protected:
	static void _bind_methods();

	Vector2 transfer_speed;

public:
	bool move_h_exact(int32_t p_amount, const Callable &p_collision_callback = Callable(), const RID &p_pusher = RID()) override;
	bool move_v_exact(int32_t p_amount, const Callable &p_collision_callback = Callable(), const RID &p_pusher = RID()) override;

	bool move_h_collide(real_t p_amount, const Callable &p_callback = Callable());
	bool move_v_collide(real_t p_amount, const Callable &p_callback = Callable());

	bool move_h_exact_collide(int32_t p_amount, const Callable &p_collision_callback = Callable(), const RID &p_pusher = RID());
	bool move_v_exact_collide(int32_t p_amount, const Callable &p_collision_callback = Callable(), const RID &p_pusher = RID());

	void set_one_way_collision(bool p_enable);
	bool is_one_way_collision_enabled() const;

	void update_riders();

	void set_transfer_speed(const Vector2 &p_speed);
	Vector2 get_transfer_speed() const;

	TypedArray<PhysicsBody2D> get_riders() const;
	bool has_rider() const;

	SolidBody2D();

	bool _move_h_exact_collide(int32_t p_amount, const Callable &p_callback = Callable(), const RID &p_pusher = RID());
	bool _move_v_exact_collide(int32_t p_amount, const Callable &p_callback = Callable(), const RID &p_pusher = RID());

	GDVIRTUAL3R(bool, _move_h_exact_collide, int32_t, Callable, RID)
	GDVIRTUAL3R(bool, _move_v_exact_collide, int32_t, Callable, RID)
private:
	void move_h_exact_solid(int32_t p_amount, const Callable &p_collision_callback = Callable(), const RID &p_pusher = RID());
	void move_h_exact_one_way(int32_t p_amount, const Callable &p_collision_callback = Callable(), const RID &p_pusher = RID());
	void move_v_exact_solid(int32_t p_amount, const Callable &p_collision_callback = Callable(), const RID &p_pusher = RID());
	void move_v_exact_one_way(int32_t p_amount, const Callable &p_collision_callback = Callable(), const RID &p_pusher = RID());
};

#endif // SOLID_BODY_2D_H
