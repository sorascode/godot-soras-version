/**************************************************************************/
/*  physics_body_2d.h                                                     */
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

#ifndef PHYSICS_BODY_2D_H
#define PHYSICS_BODY_2D_H

#include "scene/2d/physics/collision_object_2d.h"
#include "scene/2d/physics/kinematic_collision_2d.h"
#include "scene/resources/physics_material.h"
#include "servers/physics_server_2d.h"

class PhysicsBody2D : public CollisionObject2D {
	GDCLASS(PhysicsBody2D, CollisionObject2D);

protected:
	static void _bind_methods();
	PhysicsBody2D(PhysicsServer2D::BodyMode p_mode, PhysicsServer2D::ColliderType p_type);
	Vector2 position_delta;

public:
	void set_position_delta(Vector2 p_value);
	Vector2 get_position_delta() const;

	bool move_h(real_t p_amount, const Callable &p_callback = Callable());
	bool move_v(real_t p_amount, const Callable &p_callback = Callable());
	virtual bool move_h_exact(int32_t p_amount, const Callable &p_callback = Callable(), const RID &p_pusher = RID()) {
		if (p_amount == 0) {
			return false;
		}
		translate(Vector2i(p_amount, 0));
		return false;
	};
	virtual bool move_v_exact(int32_t p_amount, const Callable &p_callback = Callable(), const RID &p_pusher = RID()) {
		if (p_amount == 0) {
			return false;
		}
		translate(Vector2i(0, p_amount));
		return false;
	};
	bool test_move(const Transform2Di &p_from, const Vector2i &p_motion, const Ref<KinematicCollision2D> &r_collision = Ref<KinematicCollision2D>(), bool p_recovery_as_collision = false);
	Vector2i get_gravity() const;

	bool collides_at(const Vector2i &p_delta, PhysicsServer2D::CollisionResult *p_result = nullptr, const int16_t p_collision_type_filter = PhysicsServer2D::DEFAULT_COLLIDER_FILTER);
	virtual bool _collides_at(const Vector2i &p_delta, const Ref<PhysicsCollisionResult2D> &r_result = Ref<PhysicsCollisionResult2D>(), const int16_t p_collision_type_filter = PhysicsServer2D::DEFAULT_COLLIDER_FILTER);

	bool collides_at_all_outside(const Vector2i &p_delta, PhysicsServer2D::CollisionResults *p_result, const int16_t p_collision_type_filter = PhysicsServer2D::DEFAULT_COLLIDER_FILTER);
	bool _collides_at_all_outside(const Vector2i &p_delta, const Ref<PhysicsCollisionResults2D> &r_result = Ref<PhysicsCollisionResults2D>(), const int16_t p_collision_type_filter = PhysicsServer2D::DEFAULT_COLLIDER_FILTER);

	bool collides_at_with(const Vector2i &p_delta, const RID &p_body);

	bool collides_at_with_outside(const Vector2i &p_delta, const RID &p_body);

	bool collides_at_all(const Vector2i &p_delta, PhysicsServer2D::CollisionResults *p_result, const bool p_smear = false, const int16_t p_collision_type_filter = PhysicsServer2D::DEFAULT_COLLIDER_FILTER, const bool p_only_pushable = false);

	bool _collides_at_all(const Vector2i &p_delta, const Ref<PhysicsCollisionResults2D> &r_result = Ref<PhysicsCollisionResults2D>(), const bool p_smear = false, const int16_t p_collision_type_filter = PhysicsServer2D::DEFAULT_COLLIDER_FILTER, const bool p_only_pushable = false);

	virtual bool on_ground();

	TypedArray<PhysicsBody2D> get_collision_exceptions();
	void add_collision_exception_with(Node *p_node); //must be physicsbody
	void remove_collision_exception_with(Node *p_node);

	bool _move_h_exact(int32_t p_amount, const Callable &p_callback = Callable(), const RID &p_pusher = RID());
	bool _move_v_exact(int32_t p_amount, const Callable &p_callback, const RID &p_pusher = RID());

	GDVIRTUAL3R(bool, _move_h_exact, int32_t, Callable, RID)
	GDVIRTUAL3R(bool, _move_v_exact, int32_t, Callable, RID)
};

#endif // PHYSICS_BODY_2D_H
