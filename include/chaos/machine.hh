#ifndef CHAOS_MACHINE_HH
#define CHAOS_MACHINE_HH

#include <iostream>
#include <stdexcept>
#include <vector>
#include <limits>

#include "chaos/types.hh"

namespace chaos { //::chaos ////////////////////////////////////////////////////

template <class engine> class machine : public virtual engine {
private:
	// memory
	std::vector<typename engine::size_cell> key;

public:
	explicit machine(std::vector<typename engine::size_cell> key = {});

	// configuration
	void set_key(std::vector<typename engine::size_cell>);
	void set_space(size_t);
	void set_time(size_t);

	// cleaning
	void reset(void);

	// API
	constexpr static typename engine::size_pull max(void) {
		return std::numeric_limits<typename engine::size_pull>::max();
	}
	constexpr static typename engine::size_pull min(void) {
		return std::numeric_limits<typename engine::size_pull>::lowest();
	}
	friend machine &operator<<(machine &instance,
	                           const typename engine::size_push &block) {
		instance.push(block);
		return instance;
	}
	friend machine &operator>>(machine &instance,
	                           typename engine::size_pull &block) {
		block = instance.pull();
		return instance;
	}
	inline typename engine::size_pull operator()(void) noexcept {
		return this->pull();
	}
};

template <class engine>
machine<engine>::machine(std::vector<typename engine::size_cell> key) {
	// check if there is a initial secret key
	this->key = key.size() != 0 ? key : this->__default_key;
	// configure space and time parameter
	this->set_space(this->key.size());
	this->set_time(this->__default_time);
	// set starting variable
	this->reset();
}

template <class engine>
void machine<engine>::set_key(std::vector<typename engine::size_cell> key) {
	// check if received key is not empty
	this->key = key.size() != 0 ? key : this->__default_key;
	this->set_space(this->key.size()); // set new space parameter
	this->__set_key(key);              // pass via algorithm
};

template <class engine> void machine<engine>::set_time(size_t value) {
	this->__set_time(value); // pass via algorithm
};

template <class engine> void machine<engine>::set_space(size_t value) {
	this->__set_space(value); // pass via algorithm
};

template <class engine> void machine<engine>::reset(void) {
	// call reset function in algorithm
	this->__reset();
	// replace memory with our key
	this->__set_key(this->key);
}

} //::chaos ////////////////////////////////////////////////////////////////////

#endif // CHAOS_MACHINE_HH
