#include<will/_windows.hpp>
#include<optional>
#include<filesystem>
#include<string_view>
#include<symboli/hook.hpp>

using namespace std::literals::string_view_literals;

static std::optional<symboli::hook> hook_dll;

struct load_library_w : symboli::hook::func<decltype(::LoadLibraryW), load_library_w>{
	static HMODULE WINAPI func(LPCWSTR lib_filename){
		if(lib_filename == L"cri_ware_unity.dll"sv){
			hook_dll->consume_queue();

			hook_dll->remove_hook(::LoadLibraryW);

			return ::LoadLibraryW(lib_filename);
		}
		return orig(lib_filename);
	}
};

static inline BOOL process_attach(HINSTANCE hinst){
	const std::filesystem::path plugin_path{will::get_module_file_name(hinst).value()};
	hook_dll = symboli::hook::create(plugin_path.parent_path()/"symboli_hook.dll").value();
	hook_dll->enqueue_hook<load_library_w>(&::LoadLibraryW).value();
	hook_dll->consume_queue();
	return TRUE;
}

static inline BOOL process_detach(){
	if(hook_dll)
		hook_dll.reset();
	return TRUE;
}

extern "C" auto __declspec(dllexport) hook_instance()->symboli::hook*{
	if(hook_dll)
		return &*hook_dll;
	else
		return nullptr;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID)try{
	switch(fdwReason){
	case DLL_PROCESS_ATTACH:
		return process_attach(hinstDLL);
	case DLL_PROCESS_DETACH:
		return process_detach();
	default:
		return TRUE;
	}
}catch(std::exception& e){
	::MessageBoxA(nullptr, e.what(), "Symboli Prelude exception", MB_OK|MB_ICONERROR|MB_SETFOREGROUND);
	return FALSE;
}
