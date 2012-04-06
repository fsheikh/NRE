/*
 * TODO comment me
 *
 * Copyright (C) 2012, Nils Asmussen <nils@os.inf.tu-dresden.de>
 * Economic rights: Technische Universitaet Dresden (Germany)
 *
 * This file is part of NUL.
 *
 * NUL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * NUL is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details.
 */

#pragma once

#include <ex/Exception.h>

class AssertException : public Exception {
public:
	explicit AssertException(const char *code,const char *file,int line) throw()
		: Exception(), _code(code), _file(file), _line(line) {
	}
	AssertException(const AssertException& ae)
		: Exception(ae), _code(ae._code), _file(ae._file), _line(ae._line) {
	}
	AssertException& operator=(const AssertException& ae) {
		if(&ae == this)
			return *this;
		Exception::operator=(ae);
		_code = ae._code;
		_file = ae._file;
		_line = ae._line;
		return *this;
	}
	virtual ~AssertException() throw() {
	}

	const char *code() const throw() {
		return _code;
	}
	const char *file() const throw() {
		return _file;
	}
	int line() const throw() {
		return _line;
	}

	virtual void print(Format& fmt) const {
		fmt.print("Assert '%s' failed in %s, line %d\n",code(),file(),line());
		print_backtrace(fmt);
	}

private:
	const char *_code;
	const char *_file;
	int _line;
};
