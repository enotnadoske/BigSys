
#include <iostream>
#include <memory>


template <class T>
struct custom_allocator {
    typedef T value_type;
    custom_allocator()= default;
    template <class U> explicit custom_allocator (const custom_allocator<U>&){}

    void deallocate (T* p, std::size_t n) {
        delete p;//про то что должен использоваться перегруженный delete в задании нет ни слова

    }
    void construct (T* p) {
        // initialize memory with placement new
        new((void*)p) T;// необходимо явно указать адрес
    }

    T* allocate (std::size_t n) {
        return reinterpret_cast<T*>( ::operator new(sizeof(T)));
    }



};


class A {
public:
// ...
    static void * operator new(size_t size, void* p) {// в случае когда отсутствует явно указанный адрес - возникает утечка памяти
        std::cout << "operator new!" << std::endl;
        return ::operator new(size,p);
    }
    static void operator delete(void *p) {
        std::cout << "operator delete!" << std::endl;
        return ::operator delete(p);
    }
};
int main() {
    auto sp = std::make_shared<A>();
    custom_allocator<A> alloc;
    auto ptr = std::allocate_shared<A>(alloc);
    ptr.reset();

}

