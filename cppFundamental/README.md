# C++ Fundamentals — Learning Guide

This folder is a **self-contained C++ curriculum** that takes you from Hello World
to GPU-parallel CUDA code. Each file is a standalone annotated program.
Read the code **actively** — every key concept is explained inline where it appears.

---

## How to Build Intuition (the right mindset)

> **Don't just read. Predict, run, break, fix.**

For every file, do this loop:

1. **Read** the file top-to-bottom without running it yet.
2. **Predict** what the output will be. Write it down.
3. **Compile and run** — compare your prediction to the actual output.
4. **Break it** — see the exercise prompts at the bottom of each section.
5. **Fix it** — your understanding is locked in when you fix your own break.

The gap between your prediction and the real output *is* your learning opportunity.

---

## Prerequisites

```bash
# Ubuntu / Debian
sudo apt update
sudo apt install g++ cmake sqlite3 libsqlite3-dev doxygen

# Verify
g++ --version      # need >= 9 for C++17
cmake --version    # need >= 3.16
sqlite3 --version
```

For CUDA (optional — requires an NVIDIA GPU):
```bash
# Install CUDA Toolkit from https://developer.nvidia.com/cuda-downloads
nvcc --version
```

---

## File Map & Learning Path

Work through files **in order**. Each one builds on the previous.

```
01_hello_world.cpp          ← Start here, every time you learn a new language
02_variables.cpp            ← The type system is C++'s superpower
03_data_structures.cpp      ← The STL containers you will use every single day
04_classes.cpp              ← OOP, RAII, smart pointers — core C++ identity
mathutils.h / mathutils.cpp ← The header/implementation contract
05_imports_headers.cpp      ← How the compiler + linker actually work
CMakeLists.txt              ← How real C++ projects are built
07_ui_terminal.cpp          ← Formatted output, colour, safe user input
08_database_sqlite.cpp      ← SQL from C++, parameterised queries, RAII
09_documentation_style.cpp  ← Doxygen, style, commenting discipline
cuda_add.cu                 ← GPU parallel programming — your first kernel
```

---

## Build & Run — Quick Reference

### Single file (no dependencies)
```bash
cd cppFundamental
g++ -std=c++17 -Wall -o hello   01_hello_world.cpp   && ./hello
g++ -std=c++17 -Wall -o vars    02_variables.cpp     && ./vars
g++ -std=c++17 -Wall -o ds      03_data_structures.cpp && ./ds
g++ -std=c++17 -Wall -o classes 04_classes.cpp       && ./classes
g++ -std=c++17 -Wall -o ui      07_ui_terminal.cpp   && ./ui
g++ -std=c++17 -Wall -o docs    09_documentation_style.cpp && ./docs
```

### Multi-file (links mathutils)
```bash
g++ -std=c++17 -Wall -o imports 05_imports_headers.cpp mathutils.cpp && ./imports
```

### Database (needs libsqlite3)
```bash
g++ -std=c++17 -Wall -o db 08_database_sqlite.cpp -lsqlite3 && ./db
```

### CMake (the proper way — use this for real projects)
```bash
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
./hello_world
./imports
cd ..
```

### CUDA (needs nvcc + NVIDIA GPU)
```bash
nvcc -o cuda_add cuda_add.cu && ./cuda_add
```

---

## File-by-File Intuition Guide

---

### `01_hello_world.cpp` — The anatomy of a C++ program

**Core idea:** Every C++ program is a collection of functions. `main()` is where
execution starts. `#include` pulls in pre-written code. The `<<` operator sends
data to a stream (think of a stream as a pipe to the terminal).

**Build the intuition:**
- Why does `main` return `int`? What does the OS do with it?
- What is `std::`? Where does `cout` actually live?
- What is the difference between `"\n"` and `std::endl`? (Hint: buffer flushing)

**Exercise:** Change `endl` to `"\n"` everywhere. Does it still work?
Add a second line that prints your name. Predict the output first.

---

### `02_variables.cpp` — The type system

**Core idea:** C++ is *statically typed* — the compiler knows the type of every
variable at compile time, which lets it catch bugs and optimise aggressively.
Unlike Python, there is no runtime type guessing.

**Key mental model — integer division trap:**
```
int a = 7, b = 2;
a / b  →  3    (not 3.5!)   ← truncates toward zero
```
Cast one operand to `double` first. This trips up every Python/JS programmer.

**Build the intuition:**
- What is the size of `int` vs `long long`? Why does size matter for GPU code?
- When would you use `constexpr` over `const`?
- What does `auto` actually do — does it make C++ dynamically typed? (No — why not?)

**Exercise:**
```cpp
auto x = 7 / 2;          // What type is x? What value?
auto y = 7.0 / 2;        // Different. Why?
auto z = 7 / 2.0f;       // What precision is z?
```

---

### `03_data_structures.cpp` — The STL containers

**Core idea:** The Standard Template Library gives you containers that are
correct, fast, and well-tested. Choosing the right container is a major
part of writing good C++.

**Key mental model — complexity table:**

| Container | Access | Search | Insert/Delete |
|---|---|---|---|
| `vector` | O(1) | O(n) | O(1) back, O(n) front |
| `map` | O(log n) | O(log n) | O(log n) |
| `unordered_map` | O(1) avg | O(1) avg | O(1) avg |
| `set` | — | O(log n) | O(log n) |

**Build the intuition:**
- Why is `vector` the default choice over raw arrays?
- Why does `map` print keys in sorted order but `unordered_map` doesn't?
- What happens when you insert a duplicate key into a `map`?

**Exercise:** Count word frequencies in a sentence using an `unordered_map`.
Then print them sorted alphabetically (hint: copy into a `map`).

---

### `04_classes.cpp` — OOP + RAII + Smart Pointers

**Core idea:** C++ classes are more than Python classes — they control *memory
lifetime* through constructors and destructors (RAII). When a variable goes
out of scope, its destructor runs automatically. This is how C++ avoids
garbage collection while still being memory-safe.

**Key mental model — RAII:**
```
{
    Dog d("Rex", 3);    ← constructor fires: memory allocated
    d.describe();
}                       ← destructor fires: memory released automatically
```
Smart pointers (`unique_ptr`, `shared_ptr`) apply this same principle to
heap memory — they delete themselves when they go out of scope.

**Build the intuition:**
- What is the difference between `virtual` and non-virtual methods?
- What breaks if you forget `virtual` on the base class destructor?
- Why can you pass a `Dog*` wherever `Animal*` is expected?
- Why prefer `make_unique<Dog>()` over `new Dog()`?

**Exercise:** Add a `Parrot` class that inherits from `Animal`. Override
`sound()`. Add it to the `zoo` vector and call `describe()` polymorphically.

---

### `mathutils.h` + `mathutils.cpp` + `05_imports_headers.cpp` — The build model

**Core idea:** C++ compilation is a two-step process:
1. **Compile** each `.cpp` file independently → produces `.o` object files
2. **Link** all `.o` files together → produces the executable

The `.h` header is the *contract* (what exists). The `.cpp` is the
*implementation* (how it works). Any file that `#include`s the header
can use the functions — it never needs to see the `.cpp`.

**Key mental model — why `#pragma once`?**
If file A includes B.h, and C.h also includes B.h, and A also includes C.h,
then without a guard, B.h gets included twice → compiler error (redefinition).
`#pragma once` tells the compiler: include this file only once, no matter what.

**Build the intuition:**
- What does the linker actually do? Why does linking fail with "undefined reference"?
- What would go wrong if you put the `factorial` body in the `.h` file? (Try it)
- Why is `using namespace std;` dangerous in a header file?

**Exercise:** Add a `stdDev()` function to `mathutils.h` and `mathutils.cpp`.
Declare it in the header, implement it in the `.cpp`, call it from `05`.

---

### `CMakeLists.txt` — Project management

**Core idea:** `make` by itself is too manual. CMake is the meta-build system —
it generates the right Makefiles (or Ninja files, or VS project files) from a
single description. This is how virtually all professional C++ projects are built.

**Key commands to memorise:**
```bash
cmake ..                         # configure
cmake -DCMAKE_BUILD_TYPE=Release ..   # release build (-O3)
cmake -DCMAKE_BUILD_TYPE=Debug ..     # debug build (-g, sanitisers)
make -j$(nproc)                  # parallel build
make VERBOSE=1                   # see exact compiler flags
```

**Build the intuition:**
- What is the difference between `add_executable` and `add_library`?
- What does `-Wall -Wextra` do? Why are warnings valuable?
- What is AddressSanitizer (`-fsanitize=address`)? When would you use it?

**Exercise:** Add a new executable `ui` for `07_ui_terminal.cpp` to the
`CMakeLists.txt`. Build it with CMake and run it.

---

### `07_ui_terminal.cpp` — Terminal UI

**Core idea:** `std::cout` is a stream — you can manipulate it with format flags
from `<iomanip>` to produce aligned tables, padded numbers, and scientific notation.
ANSI escape codes let you add colour to any Linux/macOS terminal.

**ANSI escape code anatomy:**
```
\033[31m   →  ESC [ 31 m   →  red text
\033[0m    →  ESC [ 0  m   →  reset all formatting
```

**Build the intuition:**
- What happens if you forget `<< Color::RESET` at the end of a coloured print?
- Why must `std::cin.clear()` be called after bad integer input?
- `std::cout.flush()` vs `std::endl` — when does flushing actually matter?

**Exercise:** Add a 4th menu option "Add score" that asks for a name and score
using `getIntInput`, stores it in a local `vector<Row>`, then prints the table.

---

### `08_database_sqlite.cpp` — Database connections

**Core idea:** SQL injection (OWASP #3) is one of the most exploited
vulnerabilities. The rule is simple: **never build SQL by concatenating
strings**. Always use `?` placeholders and bind parameters separately.

**Safe vs unsafe:**
```cpp
// UNSAFE — never do this:
std::string sql = "SELECT * FROM users WHERE name = '" + userInput + "'";

// SAFE — always do this:
sqlite3_prepare_v2(db, "SELECT * FROM users WHERE name = ?", -1, &stmt, nullptr);
sqlite3_bind_text(stmt, 1, userInput.c_str(), -1, SQLITE_STATIC);
```

**Build the intuition:**
- Why does the `Database` class delete its copy constructor?
- What is `:memory:` as a database path? When would you use it?
- What is a transaction (`BEGIN; ... COMMIT;`)? Why does it matter for batch inserts?

**Exercise:** Wrap multiple inserts in a transaction:
```cpp
db.execute("BEGIN;");
for (auto& user : users) db.insertUser(...);
db.execute("COMMIT;");
```
Time it with and without the transaction on 1000 inserts.

---

### `09_documentation_style.cpp` — Documentation & Style

**Core idea:** Code is read far more often than it is written. Documentation
written *at the point of writing* is infinitely easier than writing it later.
Doxygen reads `///` comments and generates HTML/PDF docs automatically.

**The three levels of comments:**
| Level | When to use |
|---|---|
| `/// @brief` Doxygen | Public APIs — every function someone else will call |
| `// WHY comment` | Non-obvious logic — explain *why*, not *what* |
| No comment | Self-evident code — `i++` does not need a comment |

**Build the intuition:**
- What is the difference between documenting *what* vs documenting *why*?
- When does a `TODO` comment become technical debt?
- What does `const` after a method signature mean? (e.g. `double area() const`)

**Exercise:** Generate Doxygen HTML:
```bash
doxygen -g Doxyfile
# Edit Doxyfile: set INPUT = . and EXTRACT_ALL = YES
doxygen Doxyfile
open html/index.html   # or xdg-open html/index.html on Linux
```

---

### `cuda_add.cu` — GPU Parallel Programming

**Core idea:** The GPU has thousands of cores, each running the same kernel
function on a different piece of data. Your job as the programmer is to:
1. Allocate memory **on the GPU** (`cudaMalloc`)
2. Copy data **from CPU RAM to GPU VRAM** (`cudaMemcpy` HostToDevice)
3. Launch the kernel — every thread computes one element
4. Copy results **back from GPU VRAM to CPU RAM** (`cudaMemcpy` DeviceToHost)
5. Free GPU memory (`cudaFree`)

**The thread index formula — memorise this:**
```
idx = blockIdx.x * blockDim.x + threadIdx.x
```

**Why `if (idx < N)`:**
GPU thread counts must be multiples of the block size, but your array size N
usually isn't. You launch *extra* threads to fill the last block. Without the
guard, those extra threads write past the end of the array → memory corruption.

```
N = 9, threadsPerBlock = 4:
  Blocks launched: ceil(9/4) = 3
  Total threads:   3 × 4 = 12
  Threads 9,10,11 → NO array slot → must be guarded
```

**Build the intuition:**
- Why can't a GPU kernel `printf` to your screen directly? (Try it — it actually works in CUDA, but why is it expensive?)
- What is a "warp"? Why should `threadsPerBlock` be a multiple of 32?
- What happens if you forget `cudaDeviceSynchronize()` before the final `cudaMemcpy`?
- What is the difference between shared memory and global memory on a GPU?

**Exercise 1:** Modify the kernel to multiply instead of add: `C[idx] = A[idx] * B[idx]`.

**Exercise 2:** Change `N = 9` (not a multiple of 4). Update `numBlocks`
with the ceiling formula. Verify all 9 elements are correct.

**Exercise 3:** Add proper `CUDA_CHECK()` around every CUDA call (already in the file).
Deliberately pass a null pointer to `cudaMemcpy` — observe the error message.

---

## The Full Mental Model: How C++ Fits Together

```
Source files (.cpp)
      │
      │  #include → copies header text into .cpp
      ▼
  Preprocessor
      │
      ▼
   Compiler (g++ / nvcc)
      │  one .o per .cpp
      ▼
  Object files (.o)
      │
      │  resolves function references across .o files
      ▼
    Linker
      │
      ▼
  Executable (your program)
```

CMake automates all of this — it figures out which files changed, what order
to compile them, and which libraries to link.

---

## Progression Checkpoint

After working through all files you should be able to:

- [ ] Write a C++ class with a constructor, destructor, and virtual method
- [ ] Choose the right STL container for a given problem
- [ ] Split a project into `.h` + `.cpp` + link with CMake
- [ ] Write a safe SQL query using parameterised statements
- [ ] Write a Doxygen-formatted function comment
- [ ] Explain what `idx = blockIdx.x * blockDim.x + threadIdx.x` computes
- [ ] Explain why `if (idx < N)` is necessary in a CUDA kernel
- [ ] Trace the full data flow: CPU array → GPU VRAM → kernel → CPU array

---

## Recommended Next Steps

| Topic | What to explore |
|---|---|
| Memory & Pointers | References vs pointers, `const T*` vs `T* const`, move semantics (`std::move`) |
| Modern C++ | Lambda functions, `std::function`, range algorithms (`std::transform`) |
| Concurrency | `std::thread`, `std::mutex`, `std::atomic` |
| Testing | GoogleTest (gtest) — add a `tests/` folder to the CMake project |
| GPU (CUDA) | Shared memory, 2D grids, cuBLAS, Thrust library |
| Profiling | `gprof`, Valgrind (`memcheck`), CUDA Nsight |
