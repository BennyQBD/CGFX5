#include "common.hpp"
#include "memory.hpp"
#include <new>

static void* newFunc(std::size_t size)
{
	void* p = nullptr;
	while((p = Memory::malloc(size)) == nullptr) {
		void (*l_handler)() = std::set_new_handler(NULL);
		std::set_new_handler(l_handler);
		if (l_handler == NULL) {
			return nullptr;
			
		}
		
		l_handler();
	}
	return p;
}

static void deleteFunc(void* p)
{
	if(p == nullptr) {
		return;
	}
	Memory::free(p);
}

void* operator new(std::size_t size)
{	
	void* result = newFunc(size);
	if(result == nullptr) {
		throw std::bad_alloc();
	}
	return result;
}

void* operator new (std::size_t size, const std::nothrow_t& nothrow_value) throw()
{
	return newFunc(size);
}

void operator delete(void * p) throw()
{
	deleteFunc(p);
}
void* operator new[](std::size_t size)
{
    void* result = newFunc(size);
	if(result == nullptr) {
		throw std::bad_alloc();
	}
	return result;
}
void operator delete[](void *p) throw()
{
	deleteFunc(p);
}
