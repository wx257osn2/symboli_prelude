#include<will/_windows.hpp>
#include<optional>
#include<filesystem>
#include<string_view>
#include<fstream>
#include<symboli/hook.hpp>
#include<nlohmann/json.hpp>
#include<will/console.hpp>

using namespace std::literals::string_view_literals;

static std::optional<symboli::hook> hook_dll;

struct config_t{
	bool enable_console;
}static config = {
	.enable_console = false
};
static inline void to_json(nlohmann::json& j, const config_t& conf){
	j = nlohmann::json{{"enable_console", conf.enable_console}};
}
static inline void from_json(const nlohmann::json& j, config_t& conf){
	j.at("enable_console").get_to(conf.enable_console);
}

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

will::expected<void> create_console(const TCHAR* console_title)try{
	if(!will::console::attach(ATTACH_PARENT_PROCESS))
		will::console::alloc().value();
	will::console::bind_stdstreams().value();
	will::console::set_output_cp(CP_UTF8).value();
	std::locale::global(std::locale(""));
	will::console::set_title(console_title).value();
	return {};
}catch(...){
	return will::make_unexpected_from_current_exception();
}

static inline BOOL process_attach(HINSTANCE hinst){
	const std::filesystem::path plugin_path{will::get_module_file_name(hinst).value()};
	hook_dll = symboli::hook::create(plugin_path.parent_path()/"symboli_hook.dll").value();
	hook_dll->enqueue_hook<load_library_w>(&::LoadLibraryW).value();
	hook_dll->consume_queue();

	std::ifstream config_file{plugin_path.parent_path()/"symboli_prelude.config"};
	if(config_file.is_open())try{
		nlohmann::json j;
		config_file >> j;
		config = j.get<config_t>();
	}catch(std::exception& e){
		::MessageBoxA(nullptr, e.what(), "Symboli Prelude exception", MB_OK|MB_ICONWARNING|MB_SETFOREGROUND);
	}

	if(config.enable_console){
		create_console(_T("Symboli console")).value();
	}
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
