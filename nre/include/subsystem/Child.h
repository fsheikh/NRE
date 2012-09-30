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

#include <kobj/Pd.h>
#include <kobj/GlobalThread.h>
#include <kobj/Sc.h>
#include <kobj/Pt.h>
#include <kobj/UserSm.h>
#include <kobj/Gsi.h>
#include <kobj/Ports.h>
#include <subsystem/ChildMemory.h>
#include <mem/RegionManager.h>
#include <util/BitField.h>
#include <util/SList.h>
#include <RCU.h>
#include <String.h>

namespace nre {

class OStream;
class Child;
class ChildManager;

OStream &operator<<(OStream &os,const Child &c);

class Child : public RCUObject {
	friend class ChildManager;
	friend OStream &operator<<(OStream &os,const Child &c);

	class SchedEntity : public nre::SListItem {
	public:
		explicit SchedEntity(const nre::String &name,cpu_t cpu,capsel_t cap)
			: nre::SListItem(), _name(name), _cpu(cpu), _cap(cap) {
		}

		const nre::String &name() const {
			return _name;
		}
		cpu_t cpu() const {
			return _cpu;
		}
		capsel_t cap() const {
			return _cap;
		}

	private:
		nre::String _name;
		cpu_t _cpu;
		capsel_t _cap;
	};

public:
	typedef capsel_t id_type;

	id_type id() const {
		return _id;
	}
	cpu_t cpu() const {
		return _ec->cpu();
	}
	capsel_t pd() const {
		return _pd->sel();
	}
	const String &cmdline() const {
		return _cmdline;
	}
	ChildMemory &reglist() {
		return _regs;
	}
	const ChildMemory &reglist() const {
		return _regs;
	}
	RegionManager &io() {
		return _io;
	}
	const RegionManager &io() const {
		return _io;
	}
	uintptr_t entry() const {
		return _entry;
	}
	uintptr_t stack() const {
		return _stack;
	}
	uintptr_t utcb() const {
		return _utcb;
	}
	uintptr_t hip() const {
		return _hip;
	}
	BitField<Hip::MAX_GSIS> &gsis() {
		return _gsis;
	}
	const BitField<Hip::MAX_GSIS> &gsis() const {
		return _gsis;
	}
	SList<SchedEntity> &scs() {
		return _scs;
	}
	const SList<SchedEntity> &scs() const {
		return _scs;
	}

private:
	explicit Child(ChildManager *cm,id_type id,const char *cmdline)
			: RCUObject(), _cm(cm), _id(id), _cmdline(cmdline), _started(), _pd(), _ec(),
			  _pts(), _ptcount(), _regs(), _io(), _scs(), _gsis(),
			  _gsi_caps(CapSelSpace::get().allocate(Hip::MAX_GSIS)), _gsi_next(), _entry(),
			  _main(), _stack(), _utcb(), _hip(), _last_fault_addr(), _last_fault_cpu(), _sm() {
	}
	virtual ~Child() {
		for(size_t i = 0; i < _ptcount; ++i)
			delete _pts[i];
		delete[] _pts;
		if(_ec)
			delete _ec;
		if(_pd)
			delete _pd;
		release_gsis();
		release_ports();
		release_scs();
		release_regs();
		CapSelSpace::get().free(_gsi_caps,Hip::MAX_GSIS);
	}

	void add_sc(const String &name,cpu_t cpu,capsel_t sc) {
		ScopedLock<UserSm> guard(&_sm);
		_scs.append(new SchedEntity(name,cpu,sc));
	}
	void remove_sc(capsel_t sc) {
		ScopedLock<UserSm> guard(&_sm);
		for(SList<SchedEntity>::iterator it = _scs.begin(); it != _scs.end(); ++it) {
			if(it->cap() == sc) {
				_scs.remove(&*it);
				delete &*it;
				break;
			}
		}
	}

	void release_gsis();
	void release_ports();
	void release_scs();
	void release_regs();

	Child(const Child&);
	Child& operator=(const Child&);

	ChildManager *_cm;
	id_type _id;
	String _cmdline;
	bool _started;
	Pd *_pd;
	GlobalThread *_ec;
	Pt **_pts;
	size_t _ptcount;
	ChildMemory _regs;
	RegionManager _io;
	SList<SchedEntity> _scs;
	BitField<Hip::MAX_GSIS> _gsis;
	capsel_t _gsi_caps;
	capsel_t _gsi_next;
	uintptr_t _entry;
	uintptr_t _main;
	uintptr_t _stack;
	uintptr_t _utcb;
	uintptr_t _hip;
	uintptr_t _last_fault_addr;
	cpu_t _last_fault_cpu;
	UserSm _sm;
};

}
