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

#include <Types.h>

class ResourceSpace {
public:
	explicit ResourceSpace(cap_t portal,unsigned type)
		: _pt(portal), _type(type) {
	}

	void allocate(uintptr_t base,size_t size,unsigned rights,uintptr_t target = 0);

private:
	ResourceSpace(const ResourceSpace&);
	ResourceSpace& operator=(const ResourceSpace&);

private:
	cap_t _pt;
	unsigned _type;
};
