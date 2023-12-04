template <class T>
class UniquePtr {
public:
    UniquePtr() noexcept : ptr(nullptr) {}

    UniquePtr(const UniquePtr& o) = delete;
    UniquePtr& operator=(const UniquePtr& o) = delete;
    
    UniquePtr(UniquePtr&& o): ptr(o.ptr) {
        o.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& o) noexcept{          
        if (this != std::addressof(o)) {
            reset();
            ptr = o.ptr;
            o.ptr = nullptr;
        }
        return *this;
    }

    void reset() {
        delete ptr;
        ptr = nullptr;
    }

    UniquePtr(T* p) noexcept : ptr(p) {}
    
    T* operator->() { 
        return ptr;
    }
    
    T& operator*() { 
        return *ptr;
     }
    
    ~UniquePtr() noexcept{ 
        reset();
    }
    
private:
    T* ptr;
};


struct RefCntBlock
{
    size_t strong, weak;
};

template <class T>
class WeakPtr;

template <class T>
class SharedPtr {   
    friend class WeakPtr<T>;
public:
    SharedPtr() noexcept: ptr(nullptr), ref_count(nullptr) {}

    SharedPtr(const SharedPtr& o) noexcept: ptr(o.ptr), ref_count(o.ref_count) {
        Inc();
    }

    SharedPtr& operator=(const SharedPtr& o) noexcept{ 
        if (ref_count != o.ref_count)
        {
            this->~SharedPtr();
        }

        ptr = o.ptr;
        ref_count = o.ref_count;
        Inc();
        return *this;
    }
    
    SharedPtr(SharedPtr&& o) noexcept: ptr(o.ptr), ref_count(o.ref_count) {
        Inc();
        o.Reset();
    }
    
    SharedPtr& operator=(SharedPtr&& o) noexcept
    {
        if (ref_count != o.ref_count)
        {
            this->~SharedPtr();
        }

        ptr = o.ptr;
        ref_count = o.ref_count;
        Inc();
        o.Reset();
        return *this;
    }

    SharedPtr(T* p) noexcept : ptr(p), ref_count(new RefCntBlock())
    {
        ref_count->strong++;
    }
    
    // Implementation below
    SharedPtr(const WeakPtr<T>& o) noexcept ;
    
    // Replaces pointer with nullptr
    void Reset() noexcept
    {
        this->~SharedPtr();

        ref_count = nullptr;
        ptr = nullptr;
    }
    
    T* operator->() { 
        return ptr;
    }
    
    T& operator*() {
        return *ptr;
    }
    
    ~SharedPtr () {
        Reset();
    }
    
private:
    T* ptr;
    RefCntBlock* ref_count;

    void Inc() noexcept
    {
        if (ref_count != nullptr)
        {
            ref_count->strong++;
        }
    }
};

template <class T>
class WeakPtr {
    friend class SharedPtr<T>;
public:
    WeakPtr() : ptr(nullptr), ref_count(nullptr) {}

    WeakPtr(const WeakPtr& o): ptr(o.ptr), ref_count(o.ref_count) {}

    WeakPtr& operator=(const WeakPtr& o) {
        if (this != std::addressof(o)) {
            Reset();
            ptr = o.ptr;
            ref_count = o.ref_count;
        }
        return *this;
    }
    
    WeakPtr(WeakPtr&& o) noexcept : ptr(o.ptr), ref_count(o.ref_count)
    {
        Inc();
        o.Reset();
    };
    
    WeakPtr& operator=(WeakPtr&& o) noexcept
    {
        if (ref_count != o.ref_count)
        {
            this->~WeakPtr();
        }

        ref_count = o.ref_count;
        ptr = o.ptr;
        Inc();
        o.Reset();
        return *this;
    }
    
    WeakPtr(const SharedPtr<T>& o) noexcept : ptr(o.ptr), ref_count(o.ref_count)
    {
        Inc();
    };
    
    WeakPtr& operator=(const SharedPtr<T>& o) noexcept
    {
        if (ref_count != o.ref_count)
        {
            this->~WeakPtr();
        }

        ref_count = o.ref_count;
        ptr = o.ptr;
        Inc();
        return *this;
    }

    // Replaces pointer with nullptr
    void Reset() noexcept
    {
        this->~WeakPtr();

        ptr = nullptr;
        ref_count = nullptr;
    }
    
    bool Expired() const {
        return (ref_count == nullptr || ref_count->strong == 0);
    }
    
    SharedPtr<T> Lock() const {
        if (Expired()) {
            return SharedPtr<T>();
        }
        return SharedPtr<T>(*this);
    }
        
    ~WeakPtr () noexcept
    {
        if (ref_count != nullptr)
        {
            ref_count->weak--;
            if (ref_count->strong == 0 & ref_count->weak == 0)
            {
                delete ref_count;
            }
        }
    }
    
private:
    T* ptr;
    RefCntBlock* ref_count;

    void Inc() noexcept
    {
        if (ref_count != nullptr)
        {
            ref_count->weak++;
        }
    }
};

template <class T>
SharedPtr<T>::SharedPtr(const WeakPtr<T>& o)noexcept : ptr(o.ptr), ref_count(o.ref_count)
{
    Inc();
}
