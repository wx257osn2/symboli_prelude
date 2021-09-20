#pragma once
#include<will/_windows.hpp>
#include<cstdint>
#include<cstddef>
#include<ranges>

namespace symboli::il2cpp{

inline namespace data_type{

namespace UnityEngine{

using Font = void*;
enum FontStyle : std::int32_t{};
enum TextAnchor : std::int32_t{};
enum VerticalWrapMode : std::int32_t{};
enum HorizontalWrapMode : std::int32_t{};
struct Color{float r, g, b, a;};
struct Vector2{float x, y;};
struct Vector3{float x, y, z;};
struct TextGenerationSettings{
	Font font;
	Color color;
	std::int32_t fontSize;
	float lineSpacing;
	bool richText;
	float scaleFactor;
	FontStyle fontStyle;
	TextAnchor textAnchor;
	bool alignByGeometry;
	bool resizeTextForBestFit;
	std::int32_t resizeTextMinSize;
	std::int32_t resizeTextMaxSize;
	bool updateBounds;
	VerticalWrapMode verticalOverflow;
	HorizontalWrapMode horizontalOverflow;
	Vector2 generationExtents;
	Vector2 pivot;
	bool generateOutOfBounds;
};

}

struct Resolution{int width, height, herz;};
struct Il2CppString{
	void* Empty;
	void* WhiteChars;
	std::int32_t length;
	wchar_t start_char[1];
};
enum Il2CppTypeEnum : std::uint8_t{
	IL2CPP_TYPE_END         = 0x00,
	IL2CPP_TYPE_VOID        = 0x01,
	IL2CPP_TYPE_BOOLEAN     = 0x02,
	IL2CPP_TYPE_CHAR        = 0x03,
	IL2CPP_TYPE_I1          = 0x04,
	IL2CPP_TYPE_U1          = 0x05,
	IL2CPP_TYPE_I2          = 0x06,
	IL2CPP_TYPE_U2          = 0x07,
	IL2CPP_TYPE_I4          = 0x08,
	IL2CPP_TYPE_U4          = 0x09,
	IL2CPP_TYPE_I8          = 0x0a,
	IL2CPP_TYPE_U8          = 0x0b,
	IL2CPP_TYPE_R4          = 0x0c,
	IL2CPP_TYPE_R8          = 0x0d,
	IL2CPP_TYPE_STRING      = 0x0e,
	IL2CPP_TYPE_PTR         = 0x0f,
	IL2CPP_TYPE_BYREF       = 0x10,
	IL2CPP_TYPE_VALUETYPE   = 0x11,
	IL2CPP_TYPE_CLASS       = 0x12,
	IL2CPP_TYPE_VAR         = 0x13,
	IL2CPP_TYPE_ARRAY       = 0x14,
	IL2CPP_TYPE_GENERICINST = 0x15,
	IL2CPP_TYPE_TYPEDBYREF  = 0x16,
	IL2CPP_TYPE_I           = 0x18,
	IL2CPP_TYPE_U           = 0x19,
	IL2CPP_TYPE_FNPTR       = 0x1b,
	IL2CPP_TYPE_OBJECT      = 0x1c,
	IL2CPP_TYPE_SZARRAY     = 0x1d,
	IL2CPP_TYPE_MVAR        = 0x1e,
	IL2CPP_TYPE_CMOD_REQD   = 0x1f,
	IL2CPP_TYPE_CMOD_OPT    = 0x20,
	IL2CPP_TYPE_INTERNAL    = 0x21,
	IL2CPP_TYPE_MODIFIER    = 0x40,
	IL2CPP_TYPE_SENTINEL    = 0x41,
	IL2CPP_TYPE_PINNED      = 0x45,
	IL2CPP_TYPE_ENUM        = 0x55
};
struct Il2CppType{
	void* dummy;
	unsigned int attrs : 16;
	Il2CppTypeEnum type;
	unsigned int num_mods : 6;
	unsigned int byref : 1;
	unsigned int pinned : 1;
};
struct ParameterInfo{
	const char* name;
	std::int32_t position;
	std::uint32_t token;
	const Il2CppType* parameter_type;
};
struct MethodInfo{
	std::uintptr_t methodPointer;
	std::uintptr_t invoker_method;
	const char* name;
	std::uintptr_t klass;
	std::uintptr_t return_type;
	const ParameterInfo* parameters;
	std::uintptr_t methodDefinition;
	std::uintptr_t genericContainer;
	std::uint32_t token;
	std::uint16_t flags;
	std::uint16_t iflags;
	std::uint16_t slot;
	std::uint8_t parameters_count;
	std::uint8_t is_generic : 1;
	std::uint8_t is_inflated : 1;
	std::uint8_t wrapper_type : 1;
	std::uint8_t is_marshaled_from_native : 1;
};
struct Il2CppObject{
	union {
		void* klass;
		void* vtable;
	};
	void* monitor;
};
#pragma warning(push)
#pragma warning(disable:4200)
struct Il2CppArraySize{
	Il2CppObject obj;
	void* bounds;
	std::uintptr_t max_length;
	alignas(8) void* vector[0];
};
#pragma warning(pop)

static constexpr std::size_t kIl2CppSizeOfArray = offsetof(Il2CppArraySize, vector);

}

class module{
	module(
		data_type::Il2CppString* (*string_new_utf16)(const wchar_t*, unsigned int),
		data_type::Il2CppString* (*string_new)(const char*),
		void* (*domain_get)(),
		void* (*domain_assembly_open)(void*, const char*),
		void* (*assembly_get_image)(void*),
		void* (*class_from_name)(void*, const char*, const char*),
		data_type::MethodInfo* (*class_get_methods)(void*, void**),
		data_type::MethodInfo* (*class_get_method_from_name)(void*, const char*, int),
		void* (*method_get_param)(const data_type::MethodInfo*, std::uint32_t),
		void* (*object_new)(void*),
		void* (*resolve_icall)(const char*),
		void* (*array_new)(void*, std::uintptr_t),
		void* (*thread_attach)(void*),
		void (*thread_detach)(void*)):
		string_new_utf16_{string_new_utf16},
		string_new_{string_new},
		domain_get_{domain_get},
		domain_assembly_open_{domain_assembly_open},
		assembly_get_image_{assembly_get_image},
		class_from_name_{class_from_name},
		class_get_methods_{class_get_methods},
		class_get_method_from_name_{class_get_method_from_name},
		method_get_param_{method_get_param},
		object_new_{object_new},
		resolve_icall_{resolve_icall},
		array_new_{array_new},
		thread_attach_{thread_attach},
		thread_detach_{thread_detach}{}
	data_type::Il2CppString* (*string_new_utf16_)(const wchar_t*, unsigned int);
	data_type::Il2CppString* (*string_new_)(const char*);
	void* (*domain_get_)();
	void* (*domain_assembly_open_)(void*, const char*);
	void* (*assembly_get_image_)(void*);
	void* (*class_from_name_)(void*, const char*, const char*);
	data_type::MethodInfo* (*class_get_methods_)(void*, void**);
	data_type::MethodInfo* (*class_get_method_from_name_)(void*, const char*, int);
	void* (*method_get_param_)(const data_type::MethodInfo*, std::uint32_t);
	void* (*object_new_)(void*);
	void* (*resolve_icall_)(const char*);
	void* (*array_new_)(void*, std::uintptr_t);
	void* (*thread_attach_)(void*);
	void (*thread_detach_)(void*);
public:
	struct domain{};
	struct assembly{};
	struct image{};
	struct klass{};
	struct thread{};
	static will::expected<module, will::winapi_last_error> create(LPCTSTR module_name){
		auto mod = will::get_module_handle(module_name);
		if(!mod)
			return will::make_unexpected(mod.error());
#define SYMBOLI_IL2CPP_MODULE_RESOLVE_IMPORT(type, name) \
		auto name = mod->get_proc_address<type>("il2cpp_" #name); \
		if(!name) \
			return will::make_unexpected(name.error());
		SYMBOLI_IL2CPP_MODULE_RESOLVE_IMPORT(data_type::Il2CppString*(const wchar_t*, unsigned int), string_new_utf16)
		SYMBOLI_IL2CPP_MODULE_RESOLVE_IMPORT(data_type::Il2CppString*(const char*), string_new)
		SYMBOLI_IL2CPP_MODULE_RESOLVE_IMPORT(void*(), domain_get)
		SYMBOLI_IL2CPP_MODULE_RESOLVE_IMPORT(void*(void*, const char*), domain_assembly_open)
		SYMBOLI_IL2CPP_MODULE_RESOLVE_IMPORT(void*(void*), assembly_get_image)
		SYMBOLI_IL2CPP_MODULE_RESOLVE_IMPORT(void*(void*, const char*, const char*), class_from_name)
		SYMBOLI_IL2CPP_MODULE_RESOLVE_IMPORT(data_type::MethodInfo*(void*, void**), class_get_methods)
		SYMBOLI_IL2CPP_MODULE_RESOLVE_IMPORT(data_type::MethodInfo*(void*, const char*, int), class_get_method_from_name)
		SYMBOLI_IL2CPP_MODULE_RESOLVE_IMPORT(void*(const MethodInfo*, std::uint32_t), method_get_param)
		SYMBOLI_IL2CPP_MODULE_RESOLVE_IMPORT(void*(void*), object_new)
		SYMBOLI_IL2CPP_MODULE_RESOLVE_IMPORT(void*(const char*), resolve_icall)
		SYMBOLI_IL2CPP_MODULE_RESOLVE_IMPORT(void*(void*, std::uintptr_t), array_new)
		SYMBOLI_IL2CPP_MODULE_RESOLVE_IMPORT(void*(void*), thread_attach)
		SYMBOLI_IL2CPP_MODULE_RESOLVE_IMPORT(void(void*), thread_detach)
		return module{
			*string_new_utf16,
			*string_new,
			*domain_get,
			*domain_assembly_open,
			*assembly_get_image,
			*class_from_name,
			*class_get_methods,
			*class_get_method_from_name,
			*method_get_param,
			*object_new,
			*resolve_icall,
			*array_new,
			*thread_attach,
			*thread_detach
		};
	}
	module(const module&) = default;
	module(module&&) = default;
	data_type::Il2CppString* string_new_utf16(const wchar_t* str, unsigned int length)const{
		return string_new_utf16_(str, length);
	}
	data_type::Il2CppString* string_new(const char* str)const{
		return string_new_(str);
	}
	domain* domain_get()const{
		return static_cast<domain*>(domain_get_());
	}
	assembly* domain_assembly_open(domain* domain, const char* name)const{
		return static_cast<assembly*>(domain_assembly_open_(static_cast<void*>(domain), name));
	}
	image* assembly_get_image(assembly* assembly)const{
		return static_cast<image*>(assembly_get_image_(static_cast<void*>(assembly)));
	}
	klass* class_from_name(image* image, const char* namespaze, const char* name)const{
		return static_cast<klass*>(class_from_name_(static_cast<void*>(image), namespaze, name));
	}
	data_type::MethodInfo* class_get_methods(klass* klass, void** iter)const{
		return class_get_methods_(static_cast<void*>(klass), iter);
	}
	data_type::MethodInfo* class_get_method_from_name(klass* klass, const char* name, int args_count)const{
		return class_get_method_from_name_(static_cast<void*>(klass), name, args_count);
	}
	void* method_get_param(const data_type::MethodInfo* method, std::uint32_t index)const{
		return method_get_param_(method, index);
	}
	void* object_new(klass* klass)const{
		return object_new_(static_cast<void*>(klass));
	}
	void* resolve_icall(const char* name)const{
		return resolve_icall_(name);
	}
	void* array_new(klass* klass, std::uintptr_t count)const{
		return array_new_(static_cast<void*>(klass), count);
	}
	thread* thread_attach(domain* domain)const{
		return static_cast<thread*>(thread_attach_(static_cast<void*>(domain)));
	}
	void thread_detach(thread* thread)const{
		return thread_detach_(static_cast<void*>(thread));
	}
};

namespace detail{

template<typename Base>
struct utility_operator{
	friend auto operator->*(const module& lhs, const Base& rhs){
		return rhs(lhs);
	}
};

}

struct get_class : detail::utility_operator<get_class>{
	const char* assembly_name;
	const char* namespaze;
	const char* klass_name;
	constexpr explicit get_class(const char* assembly_name, const char* namespaze, const char* klass_name)noexcept:assembly_name{assembly_name}, namespaze{namespaze}, klass_name{klass_name}{}
	constexpr get_class(const get_class&)noexcept = default;
	constexpr get_class(get_class&&)noexcept = default;
	module::klass* operator()(const module& il2cpp)const{
		auto domain = il2cpp.domain_get();
		auto assembly = il2cpp.domain_assembly_open(domain, assembly_name);
		auto image = il2cpp.assembly_get_image(assembly);
		return il2cpp.class_from_name(image, namespaze, klass_name);
	}
};

struct get_method_info : detail::utility_operator<get_method_info>{
	const char* assembly_name;
	const char* namespaze;
	const char* klass_name;
	const char* name;
	int args_count;
	constexpr explicit get_method_info(const char* assembly_name, const char* namespaze, const char* klass_name, const char* name, int args_count)noexcept:assembly_name{assembly_name}, namespaze{namespaze}, klass_name{klass_name}, name{name}, args_count{args_count}{}
	constexpr get_method_info(const get_method_info&)noexcept = default;
	constexpr get_method_info(get_method_info&&)noexcept = default;
	data_type::MethodInfo* operator()(const module& il2cpp)const{
		auto klass = il2cpp->*get_class(assembly_name, namespaze, klass_name);
		return il2cpp.class_get_method_from_name(klass, name, args_count);
	}
};

struct get_method_infos : detail::utility_operator<get_method_infos>{
	const char* assembly_name;
	const char* namespaze;
	const char* klass_name;
	constexpr explicit get_method_infos(const char* assembly_name, const char* namespaze, const char* klass_name)noexcept:assembly_name{assembly_name}, namespaze{namespaze}, klass_name{klass_name}{}
	constexpr get_method_infos(const get_method_infos&)noexcept = default;
	constexpr get_method_infos(get_method_infos&&)noexcept = default;
	class iterator{
		const data_type::MethodInfo* method = nullptr;
		module::klass* klass;
		void* iter;
		const module* il2cpp;
		iterator(const data_type::MethodInfo* method, module::klass* klass, void* iter, const module& il2cpp)noexcept:method{method}, klass{klass}, iter{iter}, il2cpp{&il2cpp}{}
		friend get_method_infos;
	public:
		using value_type = const data_type::MethodInfo*;
		using difference_type = std::ptrdiff_t;
		constexpr iterator()noexcept = default;
		constexpr iterator(const iterator&)noexcept = default;
		constexpr iterator(iterator&&)noexcept = default;
		iterator& operator=(const iterator&)noexcept = default;
		iterator& operator=(iterator&&)noexcept = default;
		value_type operator*()const noexcept{return method;}
		iterator& operator++(){
			method = il2cpp->class_get_methods(klass, &iter);
			return *this;
		}
		iterator operator++(int){
			auto it = *this;
			++*this;
			return it;
		}
		bool operator==(const iterator& rhs)const noexcept{
			return this->method == rhs.method;
		}
		bool operator!=(const iterator& rhs)const noexcept{
			return !(*this == rhs);
		}
	};
	auto operator()(const module& il2cpp)const{
		auto klass = il2cpp->*get_class(assembly_name, namespaze, klass_name);
		void* iter;
		auto method = il2cpp.class_get_methods(klass, &iter);
		iterator beg{method, klass, iter, il2cpp};
		return std::ranges::subrange(beg, iterator{});
	}
};

}
