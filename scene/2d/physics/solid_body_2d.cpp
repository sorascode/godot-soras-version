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

bool SolidBody2D::move_h_exact(int32_t amount, const Callable &collision_callback) {
	update_riders();
	translate(Vector2i(amount, 0));
	return false;
}

bool SolidBody2D::move_v_exact(int32_t amount, const Callable &collision_callback) {
	update_riders();
	translate(Vector2i(0, amount));
	return false;
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

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "one_way_collision"), "set_one_way_collision", "is_one_way_collision_enabled");
}
