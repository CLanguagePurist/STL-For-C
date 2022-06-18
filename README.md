# Standard Template Library for C
[![Chat on Matrix](https://img.shields.io/matrix/stl_for_c:matrix.org)](https://matrix.to/#/#stl_for_c:matrix.org) ![License](https://img.shields.io/github/license/CLanguagePurist/STL-For-C) [![CI](https://github.com/CLanguagePurist/STL-For-C/actions/workflows/main.yml/badge.svg?branch=main)](https://github.com/CLanguagePurist/STL-For-C/actions/workflows/main.yml)

A free library of generic data types for C programming language and any other programming languages.

### List of Available Types
1. List/Vector (Non-concurrent)
2. Concurrent Queue
3. Concurrent Stack
4. Dictionary/Hashtable (Non-concurrent)
5. Event Handler (Non-concurrent)

### Work in Progress Types
1. Concurrent List/Vector
3. Concurrent Dictionary/Hashtable
4. Hashset
5. Single Linked List
6. Doubly Linked List
7. Queue
8. Stack
9. Got an idea? Submit an idea in [discussion](https://github.com/CLanguagePurist/STL-For-C/discussions)!

## Why STL for C?
1. Usable Data Containers for C Programming Language with or without thread safety
2. Templated data types in a sense that this library leverages preprocessor to allow for data type substitution for each container type avoiding a pointer indirection performance cost that most data container in C are written for.
3. Foreign Function Interface considerations, other programming languages can use this too!
4. "Why isn't something like this made already?"

## How to use this library?

#[**Read the Wiki**](https://github.com/CLanguagePurist/STL-For-C/wiki)

## Licensing

This library is licensed under MIT license which you can see [here for more information.](LICENSE) and original ideas/codes are from [CoreCLR Runtime project](https://github.com/dotnet/runtime) for generic data types that are transposed for C programming language and those code are licensed under [MIT license](https://github.com/dotnet/runtime/blob/main/LICENSE.TXT) provided by the Dotnet Foundation and it's contributors.

**Warranty Disclaimer**

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.