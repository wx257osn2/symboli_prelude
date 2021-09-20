#pragma once
#include<will/_windows.hpp>
#include<functional>
#include<type_traits>
#include"prelude_version.hpp"

namespace symboli{

class prelude : will::module_handle{
	prelude(will::module_handle&& mod, void (*enqueue_task)(std::function<void()>), void (*hook)(void*, void*, void**), void (*diagnostic)(const char*, const char*), unsigned int major_version, unsigned int minor_version, unsigned int patch_version) : will::module_handle{std::move(mod)}, enqueue_task_{enqueue_task}, hook_{hook}, diagnostic_{diagnostic}, major_version_{major_version}, minor_version_{minor_version}, patch_version_{patch_version}{}
	void (*enqueue_task_)(std::function<void()>);
	void (*hook_)(void*, void*, void**);
	void (*diagnostic_)(const char*, const char*);
	unsigned int major_version_;
	unsigned int minor_version_;
	unsigned int patch_version_;
	static bool version_check(unsigned int major, unsigned int minor, unsigned int patch){
		if(major != SYMBOLI_PRELUDE_EXPECTED_VERSION_MAJOR)
			return false;
		if(minor < SYMBOLI_PRELUDE_EXPECTED_VERSION_MINOR)
			return false;
		if(minor == SYMBOLI_PRELUDE_EXPECTED_VERSION_MINOR && patch < SYMBOLI_PRELUDE_EXPECTED_VERSION_PATCH)
			return false;
		return true;
	}
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
		const auto major_version = module->get_proc_address<int()>("major_version");
		if(!major_version)
			return will::make_unexpected(major_version.error());
		const auto minor_version = module->get_proc_address<int()>("minor_version");
		if(!minor_version)
			return will::make_unexpected(minor_version.error());
		const auto patch_version = module->get_proc_address<int()>("patch_version");
		if(!patch_version)
			return will::make_unexpected(patch_version.error());
		const unsigned int major = (*major_version)();
		const unsigned int minor = (*minor_version)();
		const unsigned int patch = (*patch_version)();
		if(!version_check(major, minor, patch))
			return will::make_unexpected<will::winapi_last_error>(_T("symboli::prelude::version_check"), PEERDIST_ERROR_VERSION_UNSUPPORTED);
		return prelude{std::move(*module), *enqueue_task, *hook, *diagnostic, major, minor, patch};
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

	unsigned int major_version()const{
		return major_version_;
	}
	unsigned int minor_version()const{
		return minor_version_;
	}
	unsigned int patch_version()const{
		return patch_version_;
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
