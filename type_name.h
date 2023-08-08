#ifndef TYPE_NAME_H
#define TYPE_NAME_H

#include <string_view>

template<typename T>
constexpr auto type_name() {
#if defined( __clang__ )
  std::string_view name = __PRETTY_FUNCTION__;
  constexpr std::string_view prefix = "auto type_name() [T = ";
  constexpr std::string_view suffix = "]";
#elif defined( __GNUC__ )
  std::string_view name = __PRETTY_FUNCTION__;
  constexpr std::string_view prefix = "constexpr auto type_name() [with T = ";
  constexpr std::string_view suffix = "]";
#elif defined( _MSC_VER )
  std::string_view name = __FUNCSIG__;
  constexpr std::string_view prefix = "auto __cdecl type_name<";
  constexpr std::string_view suffix = ">(void)";
#endif
  name.remove_prefix( prefix.size() );
  name.remove_suffix( suffix.size() );
  return name;
}

#endif /* TYPE_NAME_H */
