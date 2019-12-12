#include "vector.cpp"
#include "Vector.hpp"
#include <iostream>
#include <dlfcn.h>

vector* vector_create_method()
{
	void *dl_handle;
	vector* (*func)();
	char *error;
 
	dl_handle = dlopen("libvector.so", RTLD_LAZY);
	if (!dl_handle) {
		printf("#%s\n", dlerror());
		return nullptr;
	}
 
	func = (vector* (*)())dlsym(dl_handle, "vector_create");
	error = dlerror();
	if (error != NULL) {
		printf("#%s\n", error);
		return nullptr;
	}
	
	vector* out = func();

	dlclose( dl_handle );
	return out;
}

void vector_push_back_method(vector* argument1, int argument2)
{
	void *dl_handle;
	void (*func)(vector*, int);
	char *error;
 
	dl_handle = dlopen("libvector.so", RTLD_LAZY);
	if (!dl_handle) {
		printf("# %s\n", dlerror());
		return;
	}
 
	func = (void (*)(vector*, int))dlsym(dl_handle, "vector_push_back");
	error = dlerror();
	if (error != NULL) {
		printf("# %s\n", error);
		return;
	}
	
	func(argument1, argument2);

	dlclose( dl_handle );
	return;
}

int& vector_at_method(vector* argument1, int argument2)
{
	void *dl_handle;
	int& (*func)(vector*, int);
	char *error;
 
	dl_handle = dlopen("libvector.so", RTLD_LAZY);
	if (!dl_handle) {
		printf("# %s\n", dlerror());
		exit(-1);
	}
 
	func = (int& (*)(vector*, int))dlsym(dl_handle, "vector_create");
	error = dlerror();
	if (error != NULL) {
		printf("# %s\n", error);
		exit(-1);
	}
	
	int& out = func(argument1, argument2);

	dlclose( dl_handle );
	return out;
}

void vector_destroy_method(vector** argument1)
{
	void *dl_handle;
	void (*func)(vector**);
	char *error;
 
	dl_handle = dlopen("libvector.so", RTLD_LAZY);
	if (!dl_handle) {
		printf("# %s\n", dlerror());
		return;
	}
 
	func = (void (*)(vector**))dlsym(dl_handle, "vector_destroy");
	error = dlerror();
	if (error != NULL) {
		printf("# %s\n", error);
		return;
	}
	
	func(argument1);

	dlclose( dl_handle );
	return;
}

int main() {
    vector* vec = vector_create_method();
    char com = '0';
    int pos, val;
    while(1) {
        std::cin >> com;
        switch(com) {
            case 'p':
                std::cin >> val;
                vector_push_back_method(vec, val);
                break;
            case 'c':
                std::cin >> pos >> val;
                vector_at_method(vec, pos) = val;
                break;
            case 'o':
                std::cin >> pos;
                std::cout << vector_at_method(vec, pos) << '\n';
                break;
            case 'q':
                vector_destroy_method(&vec);
                return 0;
                break;
            default:
                std::cout << "err\n";
                break;
        }
    }
}
