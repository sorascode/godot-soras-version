/**************************************************************************/
/*  simulated_body_2d.cpp                                                   */
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

#include "simulated_body_2d.h"

void SimulatedBody2D::_bind_methods() {
	GDVIRTUAL_BIND(_is_riding_solid, "solid");
	GDVIRTUAL_BIND(_is_riding_one_way, "one_way");

	ClassDB::bind_method(D_METHOD("set_ignores_one_way", "enabled"), &SimulatedBody2D::set_ignores_one_way);
	ClassDB::bind_method(D_METHOD("is_ignores_one_way_enabled"), &SimulatedBody2D::is_ignores_one_way_enabled);

	ClassDB::bind_method(D_METHOD("is_riding_solid", "solid"), &SimulatedBody2D::_is_riding_solid);
	ClassDB::bind_method(D_METHOD("is_riding_one_way", "one_way"), &SimulatedBody2D::_is_riding_one_way);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "ignores_one_way"), "set_ignores_one_way", "is_ignores_one_way_enabled");
}

SimulatedBody2D::SimulatedBody2D() :
		PhysicsBody2D(PhysicsServer2D::BODY_MODE_KINEMATIC, PhysicsServer2D::COLLIDER_TYPE_SIMULATED) {
	PhysicsServer2D::get_singleton()->body_set_is_riding_solid(get_rid(), callable_mp(this, &SimulatedBody2D::_is_riding_solid));
	PhysicsServer2D::get_singleton()->body_set_is_riding_one_way(get_rid(), callable_mp(this, &SimulatedBody2D::_is_riding_one_way));
}

bool SimulatedBody2D::on_ground() {
	return collides_at(Vector2i(0, 1)) || (!ignores_one_way && collides_at_all_outside(Vector2i(0, 1), nullptr, PhysicsServer2D::COLLIDER_TYPE_ONE_WAY));
}

void SimulatedBody2D::set_ignores_one_way(bool p_enable) {
	ignores_one_way = p_enable;
}

bool SimulatedBody2D::is_ignores_one_way_enabled() const {
	return ignores_one_way;
}

bool SimulatedBody2D::_is_riding_solid(const RID &p_solid) {
	bool result = false;
	if(GDVIRTUAL_CALL(_is_riding_solid, p_solid, result)) {
		return result;
	}
	result = collides_at_with(Vector2i(0, 1), p_solid);
	return result;
}

bool SimulatedBody2D::_is_riding_one_way(const RID &p_one_way) {
	bool result = false;
	if(GDVIRTUAL_CALL(_is_riding_one_way, p_one_way, result)) {
		return result;
	}
	result = !ignores_one_way && collides_at_with_outside(Vector2i(0, 1), p_one_way);
	return result;
}
