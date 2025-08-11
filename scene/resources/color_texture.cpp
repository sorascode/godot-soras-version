/**************************************************************************/
/*  color_texture.cpp                                                  */
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

#include "color_texture.h"

#include "core/math/geometry_2d.h"

ColorTexture::ColorTexture() {
	_queue_update();
}

ColorTexture::~ColorTexture() {
	if (texture.is_valid()) {
		ERR_FAIL_NULL(RenderingServer::get_singleton());
		RS::get_singleton()->free(texture);
	}
}

void ColorTexture::set_color(const Color &p_color) {
	if (color == p_color) {
		return;
	}
	color = p_color;
	_queue_update();
	emit_changed();
}

Color ColorTexture::get_color() const {
	return color;
}

void ColorTexture::_queue_update() {
	if (update_pending) {
		return;
	}
	update_pending = true;
	callable_mp(this, &ColorTexture::update_now).call_deferred();
}

void ColorTexture::_update() const {
	update_pending = false;

	Ref<Image> image;
	image.instantiate();

	image->initialize_data(width, height, false, (use_hdr) ? Image::FORMAT_RGBAF : Image::FORMAT_RGBA8);
	image->fill(color);

	if (texture.is_valid()) {
		RID new_texture = RS::get_singleton()->texture_2d_create(image);
		RS::get_singleton()->texture_replace(texture, new_texture);
	} else {
		texture = RS::get_singleton()->texture_2d_create(image);
	}
	RS::get_singleton()->texture_set_path(texture, get_path());
}

void ColorTexture::set_width(int p_width) {
	ERR_FAIL_COND_MSG(p_width <= 0 || p_width > 16384, "Texture dimensions have to be within 1 to 16384 range.");
	width = p_width;
	_queue_update();
	emit_changed();
}

int ColorTexture::get_width() const {
	return width;
}

void ColorTexture::set_height(int p_height) {
	ERR_FAIL_COND_MSG(p_height <= 0 || p_height > 16384, "Texture dimensions have to be within 1 to 16384 range.");
	height = p_height;
	_queue_update();
	emit_changed();
}
int ColorTexture::get_height() const {
	return height;
}

void ColorTexture::set_use_hdr(bool p_enabled) {
	if (p_enabled == use_hdr) {
		return;
	}

	use_hdr = p_enabled;
	_queue_update();
	emit_changed();
}

bool ColorTexture::is_using_hdr() const {
	return use_hdr;
}

RID ColorTexture::get_rid() const {
	if (!texture.is_valid()) {
		texture = RS::get_singleton()->texture_2d_placeholder_create();
	}
	return texture;
}

Ref<Image> ColorTexture::get_image() const {
	update_now();
	if (!texture.is_valid()) {
		return Ref<Image>();
	}
	return RenderingServer::get_singleton()->texture_2d_get(texture);
}

void ColorTexture::update_now() const {
	if (update_pending) {
		_update();
	}
}

void ColorTexture::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_color", "color"), &ColorTexture::set_color);
	ClassDB::bind_method(D_METHOD("get_color"), &ColorTexture::get_color);

	ClassDB::bind_method(D_METHOD("set_width", "width"), &ColorTexture::set_width);
	ClassDB::bind_method(D_METHOD("set_height", "height"), &ColorTexture::set_height);

	ClassDB::bind_method(D_METHOD("set_use_hdr", "enabled"), &ColorTexture::set_use_hdr);
	ClassDB::bind_method(D_METHOD("is_using_hdr"), &ColorTexture::is_using_hdr);

	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "width", PROPERTY_HINT_RANGE, "1,2048,or_greater,suffix:px"), "set_width", "get_width");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "height", PROPERTY_HINT_RANGE, "1,2048,or_greater,suffix:px"), "set_height", "get_height");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_hdr"), "set_use_hdr", "is_using_hdr");
}
