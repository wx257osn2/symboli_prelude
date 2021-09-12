#pragma once
#include<will/_windows.hpp>
#include<functional>
#include<type_traits>

namespace symboli{

struct prelude : will::module_handle{
	prelude(will::module_handle&& mod, void (*enqueue_task)(std::function<void()>), void (*hook)(void*, void*, void**), void (*diagnostic)(const char*, const char*)) : will::module_handle{std::move(mod)}, enqueue_task_{enqueue_task}, hook_{hook}, diagnostic_{diagnostic}{}
	void (*enqueue_task_)(std::function<void()>);
	void (*hook_)(void*, void*, void**);
	void (*diagnostic_)(const char*, const char*);
public:
	prelude(prelude&&) = default;
	prelude& operator=(prelude&&) = default;
	static will::expected<prelude, will::winapi_last_error> create(const std::filesystem::path& dll_path = "symboli_prelude.dll"){
		auto module = will::load_library(will::tchar::to_tstring(dll_path));
		if(!module)
			return will::make_unexpected(module.error());
		const auto enqueue_task = module->get_proc_address<void(std::function<void()>)>("?enqueue_task@@YAXV?$function@$$A6AXXZ@std@@@Z");
		if(!enqueue_task)
			return will::make_unexpected(enqueue_task.error());
		const auto hook = module->get_proc_address<void(void*, void*, void**)>("?hook@@YAXPEAX0PEAPEAX@Z");
		if(!hook)
			return will::make_unexpected(hook.error());
		const auto diagnostic = module->get_proc_address<void(const char*, const char*)>("diagnostic");
		if(!diagnostic)
			return will::make_unexpected(diagnostic.error());
		return prelude{std::move(*module), *enqueue_task, *hook, *diagnostic};
	}
	template<typename T, typename U>
	will::expected<void> hook(U* f)const try{
		static_assert(std::is_same<T::func_type, U>::value);
		hook_(static_cast<void*>(f), static_cast<void*>(&T::func), &reinterpret_cast<void*&>(T::original_proc));
		return {};
	}catch(...){
		return will::make_unexpected_from_current_exception();
	}

	void enqueue_task(std::function<void()> f)const{
		enqueue_task_(std::move(f));
	}

	void diagnostic(const char* module, const char* message)const{
		diagnostic_(module, message);
	}

	template<bool Opt, typename J, typename T>
	void config_read(const char* diagnostic_module, const J& j, const typename J::object_t::key_type& key, T& t)try{
		if constexpr(Opt)
			if(!j.contains(key))
				return;
		j.at(key).get_to(t);
	}catch(std::exception& e){
		diagnostic(diagnostic_module, e.what());
	}
};

template<typename, typename>
struct hook_func;

template<typename T, typename Ret, typename... Args>
struct hook_func<Ret(Args...), T>{
	using func_type = Ret(Args...);
	using func_pointer_type = func_type*;
	static Ret orig(Args... args){
		return original_proc(args...);
	}
private:
	static inline func_pointer_type original_proc;
	template<typename U, typename V>friend will::expected<void> prelude::hook<U, V>(V*)const;
};

}
