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

#include "character_body_2d.h"

void CharacterBody2D::_bind_methods() {
	GDVIRTUAL_BIND(_is_riding_solid, "solid");
	GDVIRTUAL_BIND(_is_riding_one_way, "one_way");
	GDVIRTUAL_BIND(_squish, "move_dir", "amount_moved", "amount_left", "collided_with", "contact_point", "pusher");

	ClassDB::bind_method(D_METHOD("set_ignores_one_way", "enabled"), &CharacterBody2D::set_ignores_one_way);
	ClassDB::bind_method(D_METHOD("is_ignores_one_way_enabled"), &CharacterBody2D::is_ignores_one_way_enabled);

	ClassDB::bind_method(D_METHOD("is_riding_solid", "solid"), &CharacterBody2D::_is_riding_solid);
	ClassDB::bind_method(D_METHOD("is_riding_one_way", "one_way"), &CharacterBody2D::_is_riding_one_way);
	ClassDB::bind_method(D_METHOD("squish", "move_dir", "amount_moved", "amount_left", "collided_with", "contact_point", "pusher"), &CharacterBody2D::_squish);

	ClassDB::bind_method(D_METHOD("set_carry_speed", "speed"), &CharacterBody2D::set_carry_speed);
	ClassDB::bind_method(D_METHOD("get_carry_speed"), &CharacterBody2D::get_carry_speed);

	ClassDB::bind_method(D_METHOD("set_carry_speed_grace", "time"), &CharacterBody2D::set_carry_speed_grace);
	ClassDB::bind_method(D_METHOD("get_carry_speed_grace"), &CharacterBody2D::get_carry_speed_grace);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "ignores_one_way"), "set_ignores_one_way", "is_ignores_one_way_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "carry_speed", PROPERTY_HINT_LAYERS_2D_PHYSICS), "set_carry_speed", "get_carry_speed");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "carry_speed_grace", PROPERTY_HINT_LAYERS_2D_PHYSICS), "set_carry_speed_grace", "get_carry_speed_grace");
}

CharacterBody2D::CharacterBody2D() :
		PhysicsBody2D(PhysicsServer2D::BODY_MODE_KINEMATIC, PhysicsServer2D::COLLIDER_TYPE_ACTOR) {
	PhysicsServer2D::get_singleton()->body_set_is_riding_solid(get_rid(), callable_mp(this, &CharacterBody2D::_is_riding_solid));
	PhysicsServer2D::get_singleton()->body_set_is_riding_one_way(get_rid(), callable_mp(this, &CharacterBody2D::_is_riding_one_way));
	PhysicsServer2D::get_singleton()->body_set_squish(get_rid(), callable_mp(this, &CharacterBody2D::_squish));
	PhysicsServer2D::get_singleton()->body_set_carry_speed_sync_callback(get_rid(), callable_mp(this, &CharacterBody2D::_carry_speed_changed));
}

bool CharacterBody2D::move_h_exact(int32_t p_amount, const Callable &p_callback, const RID &p_pusher) {
	if (p_amount == 0) {
		return false;
	}
	int move_dir = SIGN(p_amount);
	Vector2i move_dir_vector = Vector2i(move_dir, 0);
	int amount_moved = 0;
	PhysicsServer2D::CollisionResult r_result;
	while (p_amount != 0)
	{
		bool colliding = collides_at(move_dir_vector, &r_result);
		if (colliding)
		{
			position_delta.x = 0;
			if (p_callback.is_valid())
			{
				Object *obj = ObjectDB::get_instance(r_result.collider_id);
				Node2D *collider_body = cast_to<Node2D>(obj);

				if (p_callback.get_argument_count() == 7) {
					p_callback.call(move_dir_vector, amount_moved, p_amount, collider_body, r_result.collision_point, r_result.collider, p_pusher);
				} else {
					WARN_PRINT("move_h collision callback does not have the expected number of arguments (expected 7).");
				}
			}
			return true;
		}
		amount_moved += move_dir;
		p_amount -= move_dir;
		translate(move_dir_vector);
	}
	return false;
}

bool CharacterBody2D::move_v_exact(int32_t p_amount, const Callable &p_callback, const RID &p_pusher) {
	if (p_amount == 0) {
		return false;
	}
	int move_dir = SIGN(p_amount);
	Vector2i move_dir_vector = Vector2i(0, move_dir);
	int amount_moved = 0;
	PhysicsServer2D::CollisionResult r_result;
	PhysicsServer2D::CollisionResults r_results;
	while (p_amount != 0)
	{
		bool colliding = collides_at(move_dir_vector, &r_result);
		if (colliding)
		{
			position_delta.y = 0;
			if (p_callback.is_valid())
			{
				Object *obj = ObjectDB::get_instance(r_result.collider_id);
				Node2D *collider_body = cast_to<Node2D>(obj);
				if (p_callback.get_argument_count() == 7) {
					p_callback.call(move_dir_vector, amount_moved, p_amount, collider_body, r_result.collision_point, r_result.collider, p_pusher);
				} else {
					WARN_PRINT("move_v collision callback does not have the expected number of arguments (expected 7).");
				}
			}
			return true;
		}
		if (p_amount > 0 && !ignores_one_way) {
			colliding = collides_at_all_outside(move_dir_vector, &r_results, PhysicsServer2D::COLLIDER_TYPE_ONE_WAY);
			if (colliding)
			{
				position_delta.y = 0;
				if (p_callback.is_valid())
				{
					Object *obj = ObjectDB::get_instance(r_results.collider_ids[0]);
					Node2D *collider_body = cast_to<Node2D>(obj);
					if (p_callback.get_argument_count() == 7) {
						p_callback.call(move_dir_vector, amount_moved, p_amount, collider_body, r_results.collision_points[0], r_results.colliders[0], p_pusher);
					} else {
						WARN_PRINT("move_v collision callback does not have the expected number of arguments (expected 7).");
					}
				}
				return true;
			}
		}
		amount_moved += move_dir;
		p_amount -= move_dir;
		translate(move_dir_vector);
	}
	return false;
}

bool CharacterBody2D::on_ground() {
	return collides_at(Vector2i(0, 1)) || (!ignores_one_way && collides_at_all_outside(Vector2i(0, 1), nullptr, PhysicsServer2D::COLLIDER_TYPE_ONE_WAY));
}

void CharacterBody2D::set_ignores_one_way(bool p_enable) {
	ignores_one_way = p_enable;
}

bool CharacterBody2D::is_ignores_one_way_enabled() const {
	return ignores_one_way;
}

bool CharacterBody2D::_is_riding_solid(const RID &p_solid) {
	bool result = false;
	if(GDVIRTUAL_CALL(_is_riding_solid, p_solid, result)) {
		return result;
	}
	result = collides_at_with(Vector2i(0, 1), p_solid);
	return result;
}

bool CharacterBody2D::_is_riding_one_way(const RID &p_one_way) {
	bool result = false;
	if(GDVIRTUAL_CALL(_is_riding_one_way, p_one_way, result)) {
		return result;
	}
	result = !ignores_one_way && collides_at_with_outside(Vector2i(0, 1), p_one_way);
	return result;
}

void CharacterBody2D::_squish(const Vector2i &p_move_dir, const int32_t p_amount_moved, const int32_t p_amount_left, const RID &p_collided_with, const Vector2i &p_contact_point, const RID &p_pusher) {
	GDVIRTUAL_CALL(_squish, p_move_dir, p_amount_moved, p_amount_left, p_collided_with, p_contact_point, p_pusher);
}

void CharacterBody2D::set_carry_speed(const Vector2 &p_speed){
	_carry_speed_changed(p_speed);
	if (current_carry_speed != p_speed) {
		PhysicsServer2D::get_singleton()->body_set_carry_speed(get_rid(), p_speed);
	}
}

Vector2 CharacterBody2D::get_carry_speed() const {
	if (current_carry_speed.is_zero_approx()) {
		return last_carry_speed;
	}
	return current_carry_speed;
}

void CharacterBody2D::set_carry_speed_grace(const float &p_time){
	carry_speed_grace = p_time;
}

float CharacterBody2D::get_carry_speed_grace() const {
	return carry_speed_grace;
}

void CharacterBody2D::_carry_speed_changed(const Vector2 &p_speed){
	current_carry_speed = p_speed;
	if (!p_speed.is_zero_approx() && carry_speed_grace > 0.0f) {
		last_carry_speed = p_speed;
		carry_speed_timer = carry_speed_grace;
	}
}

void CharacterBody2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			_carry_speed_changed(Vector2());
			if (carry_speed_timer > 0.0f) {
				carry_speed_timer -= get_physics_process_delta_time();
				if (carry_speed_timer <= 0.0f) {
					last_carry_speed = Vector2();
				}
			}
		} break;
		case NOTIFICATION_READY: {
			set_physics_process_internal(true);
		} break;
	}
}
