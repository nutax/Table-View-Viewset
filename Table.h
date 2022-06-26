#pragma once
#include <tuple>
#include <array>
#include <utility>

template<unsigned CAPACITY, typename... Ts>
class Table {
private:
	unsigned sz = 0;
	std::tuple<std::array<Ts, CAPACITY>..., > t;
public:
	unsigned* select_sz() { return &sz; }

	template<typename T>
	T* select() { return std::get<std::array<T, CAPACITY>>(t).data(); }

	template<typename... As>
	void insert(As... args) { ((std::get<std::array<As, CAPACITY>>(t)[sz] = args), ...); sz++; }

	void erase(unsigned i) { sz--;  ((std::get<std::array<Ts, CAPACITY>>(t)[i] = std::get<std::array<Ts, CAPACITY>>(t)[sz]), ...); }

	unsigned size() const { return sz; }
};


template<typename... Ts>
class View {
private:
	unsigned* sz;
	std::tuple<Ts* ..., > t;
public:
	template<typename T>
	T* select() { return std::get<T*>(t); }

	unsigned size() const { return *sz; }

	template<typename Table>
	static View make(Table table) {
		View view;
		sz = table.select_sz();
		((std::get<Ts*>(t) = table.select<Ts>()), ...);
		return view;
	}

	template<typename... Tables>
	static auto make_viewset(Tables... tables) {
		std::array<View, sizeof...(Tables)> viewset = {View::make(tables)..., };
		return viewset;
	}
};