#ifndef ANY_H
#define ANY_H

#include <memory>
#include <stdexcept>

class BadAnyCast : public std::bad_cast {
public:
    const char* what() const noexcept override {
        return "Any instance does not contain that type";
    }
};

class Any {
private:
    template <typename T>
    friend T any_cast(const Any& value);

    class Base {
    public:
        virtual ~Base() = default;

        virtual std::unique_ptr<Base> Clone() const = 0;
    };

    template <typename T>
    class Derived : public Base {
    private:
        T value_;

    public:
        Derived(const T& value) : value_(value) {
        }

        Derived(T&& value) : value_(std::move(value)) {
        }

        const T& GetValue() const {
            return value_;
        }

        T& GetValue() {
            return value_;
        }

        std::unique_ptr<Base> Clone() const override {
            return std::unique_ptr<Base>(new Derived(value_));
        }
    };

    std::unique_ptr<Base> storage_;

    void Copy(const Any& other) {
        if (this == &other) {
            return;
        }

        if (other.storage_ != nullptr) {
            storage_ = other.storage_->Clone();
        } else {
            storage_ = nullptr;
        }
    }

public:
    Any() = default;

    Any(const Any& other) {
        Copy(other);
    }

    Any& operator=(const Any& other) {
        Copy(other);
        return *this;
    }

    Any(Any& other) {
        Copy(other);
    }

    Any& operator=(Any& other) {
        Copy(other);
        return *this;
    }

    Any(Any&& other) {
        storage_ = std::move(other.storage_);
    }

    Any& operator=(Any&& other) {
        storage_ = std::move(other.storage_);
        return *this;
    }

    template <typename T>
    Any(const T& value) : storage_(new Derived<T>(value)) {
    }

    template <typename T>
    Any& operator=(const T& value) {
        storage_ = std::unique_ptr<Base>(new Derived<T>(value));
        return *this;
    }

    template <typename T>
    Any(T&& value) : storage_(new Derived<T>(std::move(value))) {
    }

    template <typename T>
    Any& operator=(T&& value) {
        storage_ = std::unique_ptr<Base>(new Derived<T>(std::move(value)));
        return *this;
    }

    void Swap(Any& other) {
        storage_.swap(other.storage_);
    }

    void Reset() {
        storage_ = nullptr;
    }

    bool HasValue() const {
        return storage_ != nullptr;
    }
};

template <typename T>
T any_cast(const Any& value) {
    const Any::Derived<T>* ptr = dynamic_cast<const Any::Derived<T>*>(value.storage_.get());
    if (ptr == nullptr) {
        throw BadAnyCast();
    }
    return ptr->GetValue();
}


#endif // ANY_H
