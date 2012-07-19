/*
 * Copyright (C) 2012, Nils Asmussen <nils@os.inf.tu-dresden.de>
 * Economic rights: Technische Universitaet Dresden (Germany)
 *
 * This file is part of NRE (NOVA runtime environment).
 *
 * NRE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * NRE is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details.
 */

#pragma once

#include <arch/Types.h>
#include <stream/OStream.h>
#include <cstring>

namespace nre {

class String {
public:
	explicit String() : _str(0), _len() {
	}
	explicit String(const char *str,size_t len = static_cast<size_t>(-1))
		: _str(), _len() {
		init(str,len);
	}
	explicit String(const String& s)
		: _str(), _len() {
		init(s._str,s._len);
	}
	String& operator=(const String& s) {
		if(&s != this)
			reset(s._str,s._len);
		return *this;
	}
	~String() {
		delete _str;
	}

	const char *str() const {
		return _str ? _str : "";
	}
	size_t length() const {
		return _len;
	}
	void reset(const char *str,size_t len = static_cast<size_t>(-1)) {
		delete _str;
		init(str,len);
	}

private:
	void init(const char *str,size_t len) {
		_len = len == static_cast<size_t>(-1) ? strlen(str) : len;
		_str = new char[_len + 1];
		memcpy(_str,str,_len);
		_str[_len] = '\0';
	}

	char *_str;
	size_t _len;
};

static inline bool operator==(const String &s1,const String &s2) {
	return s1.length() == s2.length() && strcmp(s1.str(),s2.str()) == 0;
}
static inline bool operator!=(const String &s1,const String &s2) {
	return !operator==(s1,s2);
}

static inline OStream &operator <<(OStream &os,const String &str) {
	return os << str.str();
}

}