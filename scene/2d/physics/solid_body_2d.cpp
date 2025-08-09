/**************************************************************************/
/*  character_body_2d.cpp                                                 */
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

#include "solid_body_2d.h"

SolidBody2D::SolidBody2D() :
		PhysicsBody2D(PhysicsServer2D::BODY_MODE_KINEMATIC, PhysicsServer2D::COLLIDER_TYPE_SOLID) {
}

bool SolidBody2D::move_h_exact(int32_t p_amount, const Callable &p_collision_callback, const RID &p_pusher) {
	if (p_amount == 0) {
		return false;
	}
	update_riders();
	if (one_way_collision) {
		move_h_exact_one_way(p_amount, p_collision_callback, p_pusher);
	} else {
		move_h_exact_solid(p_amount, p_collision_callback, p_pusher);
	}
	return false;
}

void SolidBody2D::move_h_exact_solid(int32_t p_amount, const Callable &p_collision_callback, const RID &p_pusher) {
	PhysicsServer2D::CollisionResults r_results;
	if (is_collidable()) {
		if (collides_at_all(Vector2i(p_amount, 0), &r_results, true, PhysicsServer2D::COLLIDER_TYPE_ACTOR | PhysicsServer2D::COLLIDER_TYPE_SIMULATED, true)) {
			for (int i = 0; i < r_results.collision_count; i++) {
				RID other = r_results.colliders[i];
				int local_amount = PhysicsServer2D::get_singleton()->body_push_amount_h(get_rid(), p_amount, other);
				set_collidable(false);
				PhysicsServer2D::get_singleton()->body_move_h_exact(other, local_amount, PhysicsServer2D::get_singleton()->body_get_squish_callable(other), get_rid());
				PhysicsServer2D::get_singleton()->body_set_carry_speed(other, transfer_speed);
				set_collidable(true);
			}
		}
		for (const auto &other : riders) {
			if (r_results.has(other)) {
				// other already handled
				continue;
			}

			set_collidable(false);
			PhysicsServer2D::get_singleton()->body_move_h_exact(other, p_amount, Callable(), get_rid());
			PhysicsServer2D::get_singleton()->body_set_carry_speed(other, transfer_speed);
			set_collidable(true);
		}
	}
	translate(Vector2i(p_amount, 0));
}

void SolidBody2D::move_h_exact_one_way(int32_t p_amount, const Callable &p_collision_callback, const RID &p_pusher) {
	if (is_collidable()) {
		for (const auto &other : riders) {
			set_collidable(false);
			PhysicsServer2D::get_singleton()->body_move_h_exact(other, p_amount, Callable(), get_rid());
			PhysicsServer2D::get_singleton()->body_set_carry_speed(other, transfer_speed);
			set_collidable(true);
		}
	}
	translate(Vector2i(p_amount, 0));
}

bool SolidBody2D::move_v_exact(int32_t p_amount, const Callable &p_collision_callback, const RID &p_pusher) {
	if (p_amount == 0) {
		return false;
	}
	update_riders();
	if (one_way_collision) {
		move_v_exact_one_way(p_amount, p_collision_callback, p_pusher);
	} else {
		move_v_exact_solid(p_amount, p_collision_callback, p_pusher);
	}
	return false;
}

void SolidBody2D::move_v_exact_solid(int32_t p_amount, const Callable &p_collision_callback, const RID &p_pusher) {
	PhysicsServer2D::CollisionResults r_results;
	if (is_collidable()) {
		if (collides_at_all(Vector2i(0, p_amount), &r_results, true, PhysicsServer2D::COLLIDER_TYPE_ACTOR | PhysicsServer2D::COLLIDER_TYPE_SIMULATED, true)) {
			for (int i = 0; i < r_results.collision_count; i++) {
				RID other = r_results.colliders[i];
				int local_amount = PhysicsServer2D::get_singleton()->body_push_amount_v(get_rid(), p_amount, other);
				set_collidable(false);
				PhysicsServer2D::get_singleton()->body_move_v_exact(other, local_amount, PhysicsServer2D::get_singleton()->body_get_squish_callable(other), get_rid());
				PhysicsServer2D::get_singleton()->body_set_carry_speed(other, transfer_speed);
				set_collidable(true);
			}
		}
		for (const auto &other : riders) {
			if (r_results.has(other)) {
				// other already handled
				continue;
			}

			set_collidable(false);
			PhysicsServer2D::get_singleton()->body_move_v_exact(other, p_amount, Callable(), get_rid());
			PhysicsServer2D::get_singleton()->body_set_carry_speed(other, transfer_speed);
			set_collidable(true);
		}
	}
	translate(Vector2i(0, p_amount));
}

void SolidBody2D::move_v_exact_one_way(int32_t p_amount, const Callable &p_collision_callback, const RID &p_pusher) {
	PhysicsServer2D::CollisionResults r_results;
	if (is_collidable()) {
		for (const auto &other : riders) {
			set_collidable(false);
			PhysicsServer2D::get_singleton()->body_move_v_exact(other, p_amount, Callable(), get_rid());
			PhysicsServer2D::get_singleton()->body_set_carry_speed(other, transfer_speed);
			set_collidable(true);
		}
		if (p_amount < 0 && collides_at_all(Vector2i(0, p_amount), &r_results, true, PhysicsServer2D::COLLIDER_TYPE_ACTOR | PhysicsServer2D::COLLIDER_TYPE_SIMULATED)) {
			for (int i = 0; i < r_results.collision_count; i++) {
				RID other = r_results.colliders[i];
				if (riders.find(other) != nullptr || collides_at_with(Vector2i(), other)) {
					continue;
				}
				int local_amount = PhysicsServer2D::get_singleton()->body_push_amount_v(get_rid(), p_amount, other);
				set_collidable(false);
				PhysicsServer2D::get_singleton()->body_move_v_exact(other, local_amount, PhysicsServer2D::get_singleton()->body_get_squish_callable(other), get_rid());
				PhysicsServer2D::get_singleton()->body_set_carry_speed(other, transfer_speed);
				set_collidable(true);
			}
		}
	}
	translate(Vector2i(0, p_amount));
}

void SolidBody2D::set_one_way_collision(bool p_enable) {
	one_way_collision = p_enable;
	set_collider_type(p_enable ? PhysicsServer2D::COLLIDER_TYPE_ONE_WAY : PhysicsServer2D::COLLIDER_TYPE_SOLID);
}

bool SolidBody2D::is_one_way_collision_enabled() const {
	return one_way_collision;
}

void SolidBody2D::update_riders() {
	riders.clear();
	if (one_way_collision) {
		PhysicsServer2D::get_singleton()->body_get_riding_bodies_one_way(get_rid(), riders);
	} else {
		PhysicsServer2D::get_singleton()->body_get_riding_bodies_solid(get_rid(), riders);
	}
}

void SolidBody2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_one_way_collision", "enabled"), &SolidBody2D::set_one_way_collision);
	ClassDB::bind_method(D_METHOD("is_one_way_collision_enabled"), &SolidBody2D::is_one_way_collision_enabled);
	ClassDB::bind_method(D_METHOD("set_transfer_speed", "speed"), &SolidBody2D::set_transfer_speed);
	ClassDB::bind_method(D_METHOD("get_transfer_speed"), &SolidBody2D::get_transfer_speed);
	ClassDB::bind_method(D_METHOD("update_riders"), &SolidBody2D::update_riders);
	ClassDB::bind_method(D_METHOD("get_riders"), &SolidBody2D::get_riders);
	ClassDB::bind_method(D_METHOD("has_rider"), &SolidBody2D::has_rider);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "one_way_collision"), "set_one_way_collision", "is_one_way_collision_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "transfer_speed", PROPERTY_HINT_LAYERS_2D_PHYSICS), "set_transfer_speed", "get_transfer_speed");
}

void SolidBody2D::set_transfer_speed(const Vector2 &p_speed) {
	transfer_speed = p_speed;
}

Vector2 SolidBody2D::get_transfer_speed() const {
	return transfer_speed;
}

TypedArray<PhysicsBody2D> SolidBody2D::get_riders() const {
	Array ret;
	for (const RID &body : riders) {
		ObjectID instance_id = PhysicsServer2D::get_singleton()->body_get_object_instance_id(body);
		Object *obj = ObjectDB::get_instance(instance_id);
		PhysicsBody2D *physics_body = Object::cast_to<PhysicsBody2D>(obj);
		ret.append(physics_body);
	}
	return ret;
}

bool SolidBody2D::has_rider() const {
	return riders.size() > 0;
}