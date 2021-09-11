#pragma once
#include<will/_windows.hpp>
#include<symboli/hook.hpp>

namespace symboli{

struct prelude : will::module_handle{
	symboli::hook* hook_;
	prelude(will::module_handle&& mod, symboli::hook* hook) : will::module_handle{std::move(mod)}, hook_{hook}{}
public:
	prelude(prelude&&) = default;
	prelude& operator=(prelude&&) = default;
	static will::expected<prelude, will::winapi_last_error> create(const std::filesystem::path& dll_path = "symboli_prelude.dll"){
		auto module = will::load_library(will::tchar::to_tstring(dll_path));
		if(!module)
			return will::make_unexpected(module.error());
		symboli::hook* hook;
		{
			const auto hook_instance = module->get_proc_address<symboli::hook*()>("hook_instance");
			if(!hook_instance)
				return will::make_unexpected(hook_instance.error());
			hook = (*hook_instance)();
			if(!hook)
				return will::make_unexpected<will::winapi_last_error>(_T(__FUNCTION__), ERROR_INVALID_HANDLE);
		}
		return prelude{std::move(*module), hook};
	}
	template<typename T, typename U>
	will::expected<void> hook(U* f){
		return hook->enqueue_hook<T>(f);
	}
};

}
