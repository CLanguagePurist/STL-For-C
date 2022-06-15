# Standard Template Library for C Language
[![Chat on Matrix](https://img.shields.io/matrix/stl_for_c:matrix.org)](https://matrix.to/#/#stl_for_c:matrix.org) ![License](https://img.shields.io/github/license/CLanguagePurist/STL-For-C) [![CI](https://github.com/CLanguagePurist/STL-For-C/actions/workflows/main.yml/badge.svg?branch=main)](https://github.com/CLanguagePurist/STL-For-C/actions/workflows/main.yml)

A free library of generic data types for C programming language.

### List of Available Types
1. List/Vector (Non-concurrent)
2. Concurrent Queue
3. Dictionary/Hashtable (Non-concurrent)
4. Event Handler (Non-concurrent)

### Work in Progress Types
1. Concurrent List/Vector
2. Concurrent Stack
3. Concurrent Dictionary/Hashtable
4. Hashset
5. Single Linked List
6. Doubly Linked List
7. Queue
8. Stack
9. Got an idea? Submit a request in project issues!

### Future Ideas
1. Circular buffer
2. Object pooling
3. Event Handler (Concurrent via STL_CThread dispatch, forked from TinyCThread for minor patch fixes)

## Why STL for C?
1. Usable Datatypes for C Programmers
2. Templated data types in a sense that this library leverages preprocessor to allow for data type substitution for each container type avoiding a pointer indirection performance cost that most data container in C are written for.
3. Foreign Function Interface considerations

## How to use this library?

There are 2 approaches you can choose to use which boils down to whether you want documentation or not.

### **The Doxygen Documentation Approach**

WIP. (Script + Text Template)

### **Direct from Source Approach**

#### **List/Vector**

1. For a demonstration of this, create two files: `list_teststruct.c` and `list_teststruct.h`
2. In `list_teststruct.h` file, define a test struct as a typedef:
```c
#ifndef LIST_TESTSTRUCT_H
#define LIST_TESTSTRUCT_H 1
#include <stdint.h>

typedef struct {
    int32_t A;
    int64_t B;
    float C;
} teststruct;
```
3. Then define the macro as followed:
```c
#define LIST_TYPE teststruct
#include "include/list.h"
```
4. Then terminates the header guard by adding the following at the bottom:
```c
#endif
```

5. Now open `list_teststruct.c` and add the followings:
```c
#include "list_teststruct.h"
#define LIST_TYPE teststruct
#include "src/list.c"
```

6. Ensure `list_teststruct.c` get compiled and linked in to your C project and you can include the header file in your C project as needed: 
```c
#include "list_teststruct.h"
```

#### **Dictionary/HashTable**
1. 
## Licensing

This library is licensed under MIT license which you can see [here for more information.](LICENSE) and original ideas/codes are from [CoreCLR Runtime project](https://github.com/dotnet/runtime) for generic data types that are transposed for C programming language and those code are licensed under [MIT license](https://github.com/dotnet/runtime/blob/main/LICENSE.TXT) provided by the Dotnet Foundation and it's contributors.

## Reminder that this project have a warranty disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.