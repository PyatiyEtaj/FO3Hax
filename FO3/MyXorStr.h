#pragma once
#include <string>
#include <array>
#include <utility>
#include <cstdarg>


namespace detailxorstr {
	const unsigned char key = 0xA1;

	constexpr char enc(char s) {
		return s ^ key;
	}

	char dec(char s) {
		return s ^ key;
	}

	template<size_t N, size_t ... Is>
	__forceinline constexpr decltype(auto) encryption(const char* s, std::index_sequence<Is...>) {
		std::array<char, N> arr = { {enc(s[Is])...} };
		return arr.data();
	}

	template<size_t N>
	__forceinline char* decryption(char* arr) {
		char res[N];
		for (size_t i = 0; i < N; i++)
			res[i] = dec(arr[i]);
		return res;
	}
}

#define xorstr(s) detailxorstr::decryption<sizeof(s)>( \
	detailxorstr::encryption<sizeof(s)>(s, std::make_index_sequence<sizeof(s)>())\
)