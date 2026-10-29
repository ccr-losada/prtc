
#include <cstdio>
#include <cstdlib>
#include "types.h"

void error(const char* message) {
    printf("\033[1;31mFAILURE\033[0m: %s\n");
    exit(1);
}

template<typename T>
struct Stack {
    T* data = nullptr;
    UInt32 cap = 0;
    UInt32 count = 0;

    void alloc(UInt32 new_cap) {
        void* alloc = std::realloc(data, sizeof(T) * new_cap);
        if (alloc == nullptr) error("Failed allocation.");
        data = reinterpret_cast<T*>(alloc);
        cap = new_cap;
        count = 0;
    }

    void free(void) {
        std::free(data);
        cap = 0;
        count = 0;
    }

    void push(const T& item) {
        if (count + 1 > cap) alloc(cap * 2);
        data[count] = item;
        count += 1;
    }
};


struct Str {
    const char* data;
    UInt16 size;

    Str(const char* cstr) {
        data = cstr;
        size = 0;
        while (data[size] != '\0') size += 1;
    }
    Str(const char* str, UInt16 str_size) {
        data = str;
        size = str_size;
    }
};

bool str_equal(Str a, Str b) {
    if (a.size != b.size) return false;
    UInt16 i = 0;
    for (UInt16 i = 0; i < a.size; i++)
        if (a.data[i] != b.data[i]) return false;
    return true;
}

UInt64 hash_str(Str str) {
    // 64bit FNV-1a implementation.
    constexpr UInt64 FNV_PRIME = 1099511628211;
    constexpr UInt64 FNV_OFFSET = 14695981039346656037ull;

    UInt64 hash = FNV_OFFSET;
    for (UInt16 i = 0; i < str.size; i++) {
        hash ^= str.data[i];
        hash *= FNV_PRIME;                
    }
    return hash;
}

struct Entry {
    UInt16 key_offset;
    UInt16 key_size;
    UInt32 data;
};

#define HASHMAP_MAX_LOAD 0.75
struct Hashmap {
    Stack<char> keys;
    Entry* entries = nullptr;
    UInt32 cap = 0;
    UInt32 count = 0;

    void alloc(UInt32 init_cap) {
        if (init_cap < count) error("Allocating less then is stored.");    

        void* entries_alloc = new [init_cap]Entry ;
        if (entries_alloc == nullptr) error("Failed allocation.");
        count = 0;
        cap = init_cap;
        keys.alloc(init_cap * 2);
    }

    void free(void) {
        keys.free();
        std::free(entries);
        cap = 0;
        count = 0;
    }

    Entry* find(Str key) {
        UInt32 index = hash_str(key) % cap;
        while (true) {
            Entry* entry = &entries[index];
            Str entry_key = Str(&keys.data[entry->key_offset], entry->key_size);
            if (entry->key_size == 0 || str_equal(key, entry_key)) return entry;
            index = (index + 1) % cap;
        }
    }

    void set(Str key, UInt32 data) {

    }

    UInt32* get(Str key) {
        return nullptr;
    };
};

Int32 main(void) {
    Hashmap hm;
    hm.alloc(1024);

    hm.set


    hm.free();
    return 0;
}