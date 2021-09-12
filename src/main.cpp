#include<will/_windows.hpp>
#include<optional>
#include<filesystem>
#include<string_view>
#include<fstream>
#include<nlohmann/json.hpp>
#include<will/console.hpp>
#include<mutex>
#include<MinHook.h>
#include<functional>
#pragma comment(lib, "minhook.x64.lib")

using namespace std::literals::string_view_literals;

static std::pair<std::vector<std::function<void()>>, std::mutex> entries;

void __declspec(dllexport) enqueue_task(std::function<void()> f){
	const std::lock_guard<std::mutex> lock{entries.second};
	entries.first.emplace_back(std::move(f));
}

static inline void consume_tasks(){
	const std::lock_guard<std::mutex> lock{entries.second};
	for(auto&& x : entries.first)try{
		x();
	}catch(std::exception& e){
		::MessageBoxA(nullptr, e.what(), "Symboli Prelude exception", MB_OK|MB_ICONWARNING|MB_SETFOREGROUND);
	}
	entries.first.clear();
}

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

static inline std::string to_string(::MH_STATUS stat){
	switch(stat){
	case ::MH_OK: return "Successful.";
	case ::MH_ERROR_ALREADY_INITIALIZED: return "MinHook is already initialized.";
	case ::MH_ERROR_NOT_INITIALIZED: return "MinHook is not initialized yet, or already uninitialized.";
	case ::MH_ERROR_ALREADY_CREATED: return "The hook for the specified target function is already created.";
	case ::MH_ERROR_NOT_CREATED: return "The hook for the specified target function is not created yet.";
	case ::MH_ERROR_ENABLED: return "The hook for the specified target function is already enabled.";
	case ::MH_ERROR_DISABLED: return "The hook for the specified target function is no enabled yet, or already disabled.";
	case ::MH_ERROR_NOT_EXECUTABLE: return "The specified pointer is invalid. It points the address of non-allocated and/or non-executable region.";
	case ::MH_ERROR_UNSUPPORTED_FUNCTION: return "The specified target function cannot be hooked.";
	case ::MH_ERROR_MEMORY_ALLOC: return "Failed to allocate memory.";
	case ::MH_ERROR_MEMORY_PROTECT: return "Failed to change the memory protection.";
	case ::MH_ERROR_MODULE_NOT_FOUND: return "The specified module is not loaded.";
	case ::MH_ERROR_FUNCTION_NOT_FOUND: return "The specified function is not found.";
	default: return "Unknown error.";
	}
}

void __declspec(dllexport) hook(void* target, void* detour, void** original){
	auto stat = ::MH_CreateHook(target, detour, original);
	if(stat != ::MH_OK)
		throw std::runtime_error("MH_CreateHook(): " + to_string(stat));
	stat = ::MH_EnableHook(target);
	if(stat != ::MH_OK)
		throw std::runtime_error("MH_EnableHook(): " + to_string(stat));
}

static inline void remove_hook(void* func){
	auto stat = ::MH_DisableHook(func);
	if(stat != MH_OK)
		throw std::runtime_error("MH_DisableHook(): " + to_string(stat));
	stat = ::MH_RemoveHook(func);
	if(stat != MH_OK)
		throw std::runtime_error("MH_RemoveHook(): " + to_string(stat));
}

static HMODULE (WINAPI *load_library_w_orig)(LPCWSTR);
static HMODULE WINAPI load_library_w(LPCWSTR lib_filename){
	if(lib_filename == L"cri_ware_unity.dll"sv){
		consume_tasks();

		remove_hook(::LoadLibraryW);

		return ::LoadLibraryW(lib_filename);
	}
	return load_library_w_orig(lib_filename);
}

static inline will::expected<void> create_console(const TCHAR* console_title)try{
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
	const auto stat = ::MH_Initialize();
	if(stat == ::MH_ERROR_ALREADY_INITIALIZED)
		return TRUE;
	if(stat != ::MH_OK)
		throw std::runtime_error("MH_Initialize(): " + to_string(stat));

	hook(&::LoadLibraryW, &load_library_w, &reinterpret_cast<void*&>(load_library_w_orig));

	const std::filesystem::path plugin_path{will::get_module_file_name(hinst).value()};

	std::ifstream config_file{plugin_path.parent_path()/"symboli_prelude.config"};
	if(config_file.is_open())try{
		nlohmann::json j;
		config_file >> j;
		config = j.get<config_t>();
	}catch(std::exception& e){
		::MessageBoxA(nullptr, e.what(), "Symboli Prelude exception", MB_OK|MB_ICONWARNING|MB_SETFOREGROUND);
	}

	if(config.enable_console)
		create_console(_T("Symboli console")).value();
	return TRUE;
}

static inline BOOL process_detach(){
	auto stat = ::MH_DisableHook(MH_ALL_HOOKS);
	if(stat != ::MH_OK)
		throw std::runtime_error("MH_DisableHook(): " + to_string(stat));
	stat = ::MH_Uninitialize();
	if(stat != ::MH_OK)
		throw std::runtime_error("MH_Uninitialize(): " + to_string(stat));
	return TRUE;
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
