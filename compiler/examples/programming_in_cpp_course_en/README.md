# MaiaCpp Course Examples — Programming in C++ (English)

## Overview

This directory contains **48 C++ examples** from a C++ programming course, converted from Portuguese to English. The programs cover fundamental to intermediate C++ concepts and are organized into 13 thematic categories.

Each category corresponds to a numbered directory. Programs can be compiled and run natively with **g++**, and optionally transpiled through the **MaiaCpp → MaiaC** pipeline.

---

## Directory Structure

```
programming_in_cpp_course_en/
  01_basics/               — Data types, I/O, operators
  02_control_flow/         — Conditionals, loops
  03_functions/            — Function definitions, examples
  04_classes/              — Classes, constructors, destructors
  05_inheritance/          — Single and multi-level inheritance
  06_polymorphism/         — Virtual methods, polymorphism
  07_overloading/          — Function and operator overloading
  08_templates/            — Function templates
  09_pointers/             — Pointers and pointer arithmetic
  10_strings/              — String operations, vowels, palindromes
  11_exceptions/           — Exception handling (try/catch/throw)
  12_namespaces/           — Namespaces
  13_misc/                 — Command-line arguments
  build_all.sh             — Build all programs (g++ + MaiaCpp)
  run_all.sh               — Run all programs and compare outputs
  README.md                — This file
  DIAGNOSTICS.md           — MaiaCpp compatibility analysis
  FINAL_RESULTS.md         — Summary of build and run results
```

---

## Programs by Category

### 01_basics
| File | Description | Interactive |
|------|-------------|-------------|
| `hello_world.cpp` | Hello World — first C++ program | No |
| `data_types.cpp` | Primitive data types (int, float, double, char) | No |
| `data_input.cpp` | Reading user input with cin | Yes |
| `operators.cpp` | Arithmetic, relational, and logical operators | No |

### 02_control_flow
| File | Description | Interactive |
|------|-------------|-------------|
| `conditional_statements.cpp` | if/else and switch statements | Yes |
| `conditionals_with_loops.cpp` | Combining conditionals with while loops | Yes |
| `loop_structures.cpp` | while, do-while, and for loops | No |

### 03_functions
| File | Description | Interactive |
|------|-------------|-------------|
| `functions.cpp` | Defining and calling functions | No |
| `functions_v2.cpp` | Functions with arrays as parameters | No |
| `celsius.cpp` | Fahrenheit to Celsius converter | Yes |
| `celsius_formatted.cpp` | Celsius converter with formatted output | Yes |
| `mph.cpp` | Km/h to mph converter | Yes |
| `bmi_function.cpp` | BMI (Body Mass Index) calculator | Yes |
| `whr.cpp` | WHR (Waist-to-Hip Ratio) calculator | Yes |
| `position.cpp` | Uniform motion position formula | Yes |

### 04_classes
| File | Description | Interactive |
|------|-------------|-------------|
| `class_model.cpp` | Basic class definition (Rectangle) | No |
| `constructors_and_destructors.cpp` | Constructors and destructors | No |
| `multiplication_table.cpp` | Multiplication table using a class | Yes |
| `fibonacci.cpp` | Fibonacci sequence using a class | Yes |

### 05_inheritance
| File | Description | Interactive |
|------|-------------|-------------|
| `inheritance.cpp` | Simple inheritance (Polygon → Rectangle/Triangle) | No |
| `automobiles.cpp` | Inheritance with vehicle classes | No |
| `automobiles_v2.cpp` | Extended vehicle class with options | Yes |
| `dinosaurs.cpp` | Dinosaur hierarchy with templates | No |
| `dinosaurs_v2.cpp` | Extended dinosaur hierarchy with LifeForm base | No |
| `humanity.cpp` | Multi-level human ancestry hierarchy | No |

### 06_polymorphism
| File | Description | Interactive |
|------|-------------|-------------|
| `polymorphism.cpp` | Virtual methods and polymorphism | No |
| `game.cpp` | Text adventure game with full class hierarchy | Yes |

### 07_overloading
| File | Description | Interactive |
|------|-------------|-------------|
| `overloading.cpp` | Function overloading (square of int/float) | Yes |
| `function_overloading.cpp` | Function overloading (Fahrenheit to Celsius) | Yes |
| `operator_overloading.cpp` | Operator overloading (Vector +) | No |
| `vector.cpp` | Extended Vector class with + and − operators | No |

### 08_templates
| File | Description | Interactive |
|------|-------------|-------------|
| `templates.cpp` | Generic tmax<T> function template | No |

### 09_pointers
| File | Description | Interactive |
|------|-------------|-------------|
| `pointers.cpp` | Pointers, pointer arithmetic, and to_uppercase | Yes |
| `pointers_v2.cpp` | Pointers with character arrays | Yes |
| `pointers_to_arrays.cpp` | Pointers to integer arrays | No |
| `pointers_to_various_types.cpp` | Pointers to int, float, and char | No |

### 10_strings
| File | Description | Interactive |
|------|-------------|-------------|
| `vowels.cpp` | Count vowels in a name (function version) | Yes |
| `vowels_v2.cpp` | Count vowels using a loop | Yes |
| `vowels_with_pointers.cpp` | Count vowels with and without repetition (pointer version) | Yes |
| `vowels_combined.cpp` | Count vowels with and without repetition (array version) | Yes |
| `vowels_no_repeat.cpp` | Count unique vowels only | Yes |
| `palindrome.cpp` | Palindrome check (inline) | Yes |
| `palindrome_function.cpp` | Palindrome check (function version) | Yes |
| `palindrome_v3.cpp` | Palindrome check (optimized while-loop version) | Yes |

### 11_exceptions
| File | Description | Interactive |
|------|-------------|-------------|
| `exceptions.cpp` | try/catch/throw with int and string exceptions | No |

### 12_namespaces
| File | Description | Interactive |
|------|-------------|-------------|
| `namespace.cpp` | Custom namespace with constants | No |
| `namespaces.cpp` | Namespace with printf formatting | No |

### 13_misc
| File | Description | Interactive |
|------|-------------|-------------|
| `command_line_args.cpp` | Reading argc, argv, and environment variables | No |

---

## Building

### g++ only (recommended for course use)
```bash
cd programming_in_cpp_course_en
bash build_all.sh
```

### With filter (specific category)
```bash
bash build_all.sh 01      # only 01_basics
bash build_all.sh functions
```

### With verbose output
```bash
bash build_all.sh --verbose
```

### Build and run with comparison
```bash
bash build_all.sh && bash run_all.sh
```

---

## Running

```bash
bash run_all.sh           # run all categories
bash run_all.sh 08        # only 08_templates
```

For interactive programs that require user input, place an `input.txt` file in the same directory as the `.cpp` file. The `run_all.sh` script will feed its contents to stdin automatically.

**Example** — `03_functions/celsius/input.txt`:
```
100
```

---

## MaiaCpp Compatibility

See [DIAGNOSTICS.md](DIAGNOSTICS.md) for a full analysis of which programs successfully transpile through MaiaCpp and which are blocked by known backend limitations.

---

## Original Source

These programs were translated from the Portuguese originals in:
```
maiacpp/compiler/examples/programming_in_cpp_course/
```

Translation includes: comments, string literals, variable names, class/function names, and file names.
