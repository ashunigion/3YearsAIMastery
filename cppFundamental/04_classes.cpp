// =============================================================================
// 04_classes.cpp — struct, class, constructors, methods, inheritance, virtual
// =============================================================================
//
// COMPILE:  g++ -std=c++17 -o classes 04_classes.cpp
// RUN:      ./classes
//
// In C++:
//   struct  → members are PUBLIC by default   (use for plain data bundles)
//   class   → members are PRIVATE by default  (use when you add behavior)
// Both can have constructors, methods, and inheritance.
// =============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <memory>   // std::unique_ptr, std::shared_ptr

// =============================================================================
// 1. STRUCT — plain data bundle
// =============================================================================
struct Point {
    double x;
    double y;

    // Member function
    double distanceFromOrigin() const {   // 'const' = does NOT modify *this
        return std::sqrt(x*x + y*y);
    }
};

// =============================================================================
// 2. CLASS — encapsulation + behavior
// =============================================================================
class Animal {
private:                            // ← inaccessible from outside
    std::string name_;
    int         age_;

public:                             // ← accessible from outside
    // ── Constructor ──────────────────────────────────────────────────────────
    // Initialiser list  (name_{n}, age_{a})  is faster than assigning
    // inside the body; always prefer it.
    Animal(const std::string& name, int age)
        : name_(name), age_(age)
    {
        std::cout << "Animal created: " << name_ << "\n";
    }

    // ── Destructor — called automatically when the object goes out of scope ──
    ~Animal()
    {
        std::cout << "Animal destroyed: " << name_ << "\n";
    }

    // ── Getters (const member functions) ─────────────────────────────────────
    const std::string& name() const { return name_; }
    int                age()  const { return age_;  }

    // ── Setter ───────────────────────────────────────────────────────────────
    void setAge(int a) {
        if (a >= 0) age_ = a;    // basic validation
    }

    // ── Virtual method — can be overridden by derived classes ─────────────────
    virtual std::string sound() const {
        return "(generic animal sound)";
    }

    // ── Non-virtual method ────────────────────────────────────────────────────
    void describe() const {
        std::cout << name_ << " (age " << age_ << ") says: " << sound() << "\n";
    }
};

// =============================================================================
// 3. INHERITANCE — Dog IS an Animal
// =============================================================================
class Dog : public Animal {
private:
    std::string breed_;

public:
    Dog(const std::string& name, int age, const std::string& breed)
        : Animal(name, age),   // call parent constructor first
          breed_(breed)
    {}

    // ── Override virtual method ───────────────────────────────────────────────
    std::string sound() const override {  // 'override' = compiler checks this
        return "Woof!";
    }

    std::string breed() const { return breed_; }
};

class Cat : public Animal {
public:
    Cat(const std::string& name, int age) : Animal(name, age) {}

    std::string sound() const override { return "Meow!"; }
};

// =============================================================================
// 4. ABSTRACT CLASS — pure virtual function (cannot be instantiated directly)
// =============================================================================
class Shape {
public:
    virtual double area()      const = 0;   // = 0 → pure virtual
    virtual double perimeter() const = 0;
    virtual ~Shape() = default;             // always give base classes virtual dtors
};

class Circle : public Shape {
    double radius_;
public:
    Circle(double r) : radius_(r) {}
    double area()      const override { return 3.14159265 * radius_ * radius_; }
    double perimeter() const override { return 2 * 3.14159265 * radius_; }
};

class Rectangle : public Shape {
    double w_, h_;
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area()      const override { return w_ * h_; }
    double perimeter() const override { return 2 * (w_ + h_); }
};

// =============================================================================
// 5. TEMPLATES — generic programming
// =============================================================================
template <typename T>
T maxOf(T a, T b) {
    return (a > b) ? a : b;
}

// =============================================================================
// MAIN
// =============================================================================
int main()
{
    // ── Struct usage ─────────────────────────────────────────────────────────
    Point p{3.0, 4.0};
    std::cout << "Distance from origin: " << p.distanceFromOrigin() << "\n";  // 5

    // ── Stack-allocated objects (destructor called at end of scope) ───────────
    {
        Animal a("Generic", 5);
        a.describe();
    }  // ← destructor fires here automatically

    // ── Inheritance + polymorphism ────────────────────────────────────────────
    Dog dog("Rex", 3, "Labrador");
    Cat cat("Whiskers", 2);

    dog.describe();   // Rex (age 3) says: Woof!
    cat.describe();   // Whiskers (age 2) says: Meow!

    // ── Polymorphism via pointer to base class ─────────────────────────────────
    // Animal* can point to any derived class — sound() resolves at runtime (vtable)
    std::vector<Animal*> zoo = { &dog, &cat };
    for (Animal* animal : zoo)
        animal->describe();

    // ── Smart pointers (modern C++ — prefer over raw new/delete) ─────────────
    // unique_ptr: sole owner, deleted automatically when out of scope
    auto fido = std::make_unique<Dog>("Fido", 1, "Poodle");
    fido->describe();

    // shared_ptr: reference-counted, deleted when last owner goes away
    std::shared_ptr<Shape> circle    = std::make_shared<Circle>(5.0);
    std::shared_ptr<Shape> rectangle = std::make_shared<Rectangle>(4.0, 6.0);

    for (auto& shape : std::vector<std::shared_ptr<Shape>>{circle, rectangle}) {
        std::cout << "Area: " << shape->area()
                  << "  Perimeter: " << shape->perimeter() << "\n";
    }

    // ── Templates ────────────────────────────────────────────────────────────
    std::cout << "max(3,7)   = " << maxOf(3, 7)     << "\n";
    std::cout << "max(a,z)   = " << maxOf('a','z')  << "\n";
    std::cout << "max(3.1,2.9)=" << maxOf(3.1, 2.9) << "\n";

    return 0;
}

// =============================================================================
// KEY TAKEAWAYS
//   - struct vs class: only difference is default access (public vs private)
//   - Always use initialiser lists in constructors: Animal(n, a) : name_(n), age_(a)
//   - virtual + override = runtime polymorphism (vtable dispatch)
//   - Pure virtual (= 0) → abstract class, cannot be instantiated
//   - NEVER use raw new/delete in modern C++ → use smart pointers
//     unique_ptr → single owner | shared_ptr → shared ownership
//   - template <typename T> → write code once, works for any type
// =============================================================================
