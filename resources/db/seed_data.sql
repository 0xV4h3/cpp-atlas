-- =============================================================
-- CppAtlas Quiz Seed Data — C++ Knowledge Base
-- ~50 questions across Beginner → Expert levels
-- References: learncpp.com, studyplan.dev/pro-cpp, cppreference.com
-- =============================================================

-- ─────────────────────────────────────────────
-- TAGS
-- ─────────────────────────────────────────────
INSERT OR IGNORE INTO tags (name, color) VALUES
    ('basics',       '#89D185'),
    ('variables',    '#89D185'),
    ('types',        '#4EC9B0'),
    ('operators',    '#DCDCAA'),
    ('control-flow', '#569CD6'),
    ('functions',    '#DCDCAA'),
    ('arrays',       '#CE9178'),
    ('pointers',     '#F14C4C'),
    ('references',   '#F14C4C'),
    ('strings',      '#CE9178'),
    ('oop',          '#C586C0'),
    ('classes',      '#C586C0'),
    ('inheritance',  '#C586C0'),
    ('templates',    '#BD93F9'),
    ('stl',          '#50FA7B'),
    ('memory',       '#F14C4C'),
    ('smart-pointers','#F14C4C'),
    ('algorithms',   '#50FA7B'),
    ('lambdas',      '#FF79C6'),
    ('move-semantics','#FFB86C'),
    ('concurrency',  '#8BE9FD'),
    ('cpp11',        '#FFB86C'),
    ('cpp14',        '#FFB86C'),
    ('cpp17',        '#FFB86C'),
    ('cpp20',        '#FFB86C'),
    ('best-practices','#89D185');

-- ─────────────────────────────────────────────
-- TOPICS (learncpp.com chapter structure)
-- ─────────────────────────────────────────────
INSERT OR IGNORE INTO topics
    (id, slug, title, description, parent_id, level, difficulty, order_index, icon, ref_url)
VALUES
-- Chapter level (level=0)
(1,  'basics',          'C++ Basics',                  'Fundamental building blocks of C++',         NULL, 0, 1, 1,  '🏁', 'https://www.learncpp.com/cpp-tutorial/introduction-to-cplusplus/'),
(2,  'variables-types', 'Variables and Fundamental Types', 'Variables, data types, initialization',  NULL, 0, 1, 2,  '📦', 'https://www.learncpp.com/cpp-tutorial/introduction-to-variables/'),
(3,  'operators',       'Operators',                   'Arithmetic, logical, bitwise operators',      NULL, 0, 1, 3,  '⚙️', 'https://www.learncpp.com/cpp-tutorial/introduction-to-operators/'),
(4,  'control-flow',    'Control Flow',                'if/else, loops, switch statements',           NULL, 0, 1, 4,  '🔀', 'https://www.learncpp.com/cpp-tutorial/control-flow-introduction/'),
(5,  'functions',       'Functions',                   'Function basics, overloading, parameters',    NULL, 0, 1, 5,  '🔧', 'https://www.learncpp.com/cpp-tutorial/introduction-to-functions/'),
(6,  'arrays-strings',  'Arrays and Strings',          'C-style arrays, std::string, std::array',     NULL, 0, 1, 6,  '📝', 'https://www.learncpp.com/cpp-tutorial/introduction-to-stdstring/'),
(7,  'pointers-refs',   'Pointers and References',     'Raw pointers, references, pointer arithmetic',NULL, 0, 2, 7,  '👉', 'https://www.learncpp.com/cpp-tutorial/introduction-to-pointers/'),
(8,  'oop',             'Object-Oriented Programming', 'Classes, constructors, inheritance, polymorphism', NULL, 0, 2, 8, '🧱', 'https://www.learncpp.com/cpp-tutorial/introduction-to-object-oriented-programming/'),
(9,  'templates',       'Templates',                   'Function templates, class templates, variadic', NULL, 0, 3, 9, '🧬', 'https://www.learncpp.com/cpp-tutorial/function-templates/'),
(10, 'stl',             'Standard Library (STL)',       'Containers, iterators, algorithms',           NULL, 0, 2, 10, '📚', 'https://www.learncpp.com/cpp-tutorial/the-standard-library/'),
(11, 'memory',          'Memory Management',           'Stack, heap, new/delete, smart pointers',     NULL, 0, 3, 11, '🧠', 'https://www.learncpp.com/cpp-tutorial/dynamic-memory-allocation-with-new-and-delete/'),
(12, 'modern-cpp',      'Modern C++ (C++11/14/17/20)', 'Move semantics, lambdas, structured bindings, concepts', NULL, 0, 3, 12, '🚀', 'https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/'),
(13, 'concurrency',     'Concurrency',                 'Threads, mutexes, atomics, async',            NULL, 0, 4, 13, '⚡', 'https://en.cppreference.com/w/cpp/thread');

-- ─────────────────────────────────────────────
-- QUIZZES (one per major topic)
-- ─────────────────────────────────────────────
INSERT OR IGNORE INTO quizzes (id, title, description, topic_id, difficulty, type, is_active)
VALUES
(1,  'C++ Basics Quiz',           'Test your fundamental C++ knowledge',                      1,  1, 'standard', 1),
(2,  'Variables & Types Quiz',    'Variables, types, initialization and constants',            2,  1, 'standard', 1),
(3,  'Operators Quiz',            'Arithmetic, relational, logical and bitwise operators',     3,  1, 'standard', 1),
(4,  'Control Flow Quiz',         'if/else, loops, switch, break/continue',                   4,  1, 'standard', 1),
(5,  'Functions Quiz',            'Function definitions, overloading, default arguments',      5,  1, 'standard', 1),
(6,  'Arrays & Strings Quiz',     'Arrays, C-strings and std::string',                        6,  1, 'standard', 1),
(7,  'Pointers & References Quiz','Raw pointers, references, pointer arithmetic',              7,  2, 'standard', 1),
(8,  'OOP Quiz',                  'Classes, inheritance, polymorphism, virtual functions',     8,  2, 'standard', 1),
(9,  'Templates Quiz',            'Function & class templates, specialization',               9,  3, 'standard', 1),
(10, 'STL Quiz',                  'Standard containers, iterators, algorithms',               10,  2, 'standard', 1),
(11, 'Memory Management Quiz',    'Stack vs heap, new/delete, smart pointers',               11,  3, 'standard', 1),
(12, 'Modern C++ Quiz',           'C++11/14/17/20 features: lambdas, move semantics, ranges',12,  3, 'standard', 1);

-- ─────────────────────────────────────────────
-- QUESTIONS & OPTIONS
-- ─────────────────────────────────────────────

-- ── Quiz 1: C++ Basics ──────────────────────

INSERT OR IGNORE INTO questions
    (id, quiz_id, topic_id, type, content, explanation, difficulty, points, order_index)
VALUES
(1, 1, 1, 'mcq',
 'What is the correct way to output "Hello, World!" to the console in C++?',
 'std::cout is the standard output stream in C++. The << operator is used to send data to the stream. std::endl flushes the buffer and adds a newline.',
 1, 10, 1),

(2, 1, 1, 'true_false',
 'In C++, the main() function must return an int.',
 'Yes. The C++ standard (ISO C++11 §3.6.1) requires main() to have return type int. Returning 0 conventionally indicates success.',
 1, 10, 2),

(3, 1, 1, 'mcq',
 'Which of the following is a valid single-line comment in C++?',
 '// is the single-line comment syntax in C++. /* */ is used for multi-line comments.',
 1, 10, 3),

(4, 1, 1, 'mcq',
 'What does the #include directive do?',
 '#include is a preprocessor directive that inserts the contents of the specified file into the current file before compilation.',
 1, 10, 4);

INSERT OR IGNORE INTO options (question_id, content, is_correct, order_index) VALUES
(1, 'std::cout << "Hello, World!" << std::endl;', 1, 1),
(1, 'print("Hello, World!");',                   0, 2),
(1, 'Console.WriteLine("Hello, World!");',        0, 3),
(1, 'printf["Hello, World!"];',                   0, 4),

(3, '// This is a comment',    1, 1),
(3, '## This is a comment',    0, 2),
(3, '** This is a comment **', 0, 3),
(3, '<!-- comment -->',         0, 4),

(4, 'Inserts the content of another file before compilation', 1, 1),
(4, 'Declares a function',                                    0, 2),
(4, 'Imports a Python module',                                0, 3),
(4, 'Links an external library at runtime',                   0, 4);

INSERT OR IGNORE INTO options (question_id, content, is_correct, order_index) VALUES
(2, 'True',  1, 1),
(2, 'False', 0, 2);

-- ── Quiz 2: Variables & Types ────────────────

INSERT OR IGNORE INTO questions
    (id, quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index)
VALUES
(5, 2, 2, 'mcq',
 'What is the output of the following code?',
 'int x = 5;\nint y = 2;\nstd::cout << x / y;',
 'Integer division truncates the fractional part. 5 / 2 = 2 (not 2.5) because both operands are int.',
 1, 10, 1),

(6, 2, 2, 'mcq',
 'Which keyword is used to declare a constant variable in modern C++?',
 NULL,
 'constexpr declares compile-time constants. const declares runtime constants. Both are correct, but constexpr is preferred in modern C++ for values known at compile time.',
 1, 10, 2),

(7, 2, 2, 'true_false',
 'The `auto` keyword in C++ always deduces the type as a reference.',
 NULL,
 'False. auto deduces the value type by default. To deduce a reference, you must explicitly write auto& or auto&&.',
 1, 10, 3),

(8, 2, 2, 'mcq',
 'What is the size of `int` guaranteed by the C++ standard?',
 NULL,
 'The C++ standard only guarantees int is at least 16 bits. In practice it is usually 32 bits on modern platforms, but this is implementation-defined. Use <cstdint> types like int32_t for guaranteed sizes.',
 2, 10, 4);

INSERT OR IGNORE INTO options (question_id, content, is_correct, order_index) VALUES
(5, '2',   1, 1),
(5, '2.5', 0, 2),
(5, '3',   0, 3),
(5, '0',   0, 4),

(6, 'constexpr', 1, 1),
(6, '#define',   0, 2),
(6, 'static',    0, 3),
(6, 'final',     0, 4),

(7, 'True',  0, 1),
(7, 'False', 1, 2),

(8, 'At least 16 bits',  1, 1),
(8, 'Exactly 32 bits',   0, 2),
(8, 'Exactly 64 bits',   0, 3),
(8, 'At least 8 bits',   0, 4);

-- ── Quiz 3: Operators ────────────────────────

INSERT OR IGNORE INTO questions
    (id, quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index)
VALUES
(9, 3, 3, 'mcq',
 'What is the value of `x` after: int x = 10; x += 5;',
 NULL,
 'The += compound assignment operator adds the right operand to the variable and stores the result. 10 + 5 = 15.',
 1, 10, 1),

(10, 3, 3, 'mcq',
 'What does the modulo operator % return?',
 NULL,
 'The % operator returns the remainder of integer division. 17 % 5 = 2 because 17 = 3*5 + 2.',
 1, 10, 2),

(11, 3, 3, 'mcq',
 'What is the result of `true && false` in C++?',
 NULL,
 'The logical AND operator && returns true only if both operands are true. Since one operand is false, the result is false.',
 1, 10, 3);

INSERT OR IGNORE INTO options (question_id, content, is_correct, order_index) VALUES
(9,  '15',    1, 1),
(9,  '10',    0, 2),
(9,  '5',     0, 3),
(9,  '50',    0, 4),

(10, 'The remainder of division',       1, 1),
(10, 'The quotient of division',        0, 2),
(10, 'The product of two numbers',      0, 3),
(10, 'The bitwise OR of two numbers',   0, 4),

(11, 'false', 1, 1),
(11, 'true',  0, 2),
(11, '0',     0, 3),
(11, '1',     0, 4);

-- ── Quiz 4: Control Flow ──────────────────────

INSERT OR IGNORE INTO questions
    (id, quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index)
VALUES
(12, 4, 4, 'mcq',
 'How many times does the following loop execute?',
 'for (int i = 0; i < 5; i++) {\n    std::cout << i;\n}',
 'The loop runs while i < 5, starting from 0. So i takes values 0,1,2,3,4 — that is 5 iterations.',
 1, 10, 1),

(13, 4, 4, 'mcq',
 'What is the purpose of the `break` statement in a loop?',
 NULL,
 'break immediately terminates the innermost enclosing loop or switch statement and transfers control to the statement following it.',
 1, 10, 2),

(14, 4, 4, 'true_false',
 'A `do-while` loop always executes its body at least once.',
 NULL,
 'True. In a do-while loop, the condition is checked after the loop body executes, so the body always runs at least once regardless of the condition.',
 1, 10, 3);

INSERT OR IGNORE INTO options (question_id, content, is_correct, order_index) VALUES
(12, '5 times',   1, 1),
(12, '4 times',   0, 2),
(12, '6 times',   0, 3),
(12, '0 times',   0, 4),

(13, 'Exits the current loop immediately',          1, 1),
(13, 'Skips the current iteration and continues',   0, 2),
(13, 'Restarts the loop from the beginning',        0, 3),
(13, 'Pauses execution for one iteration',          0, 4),

(14, 'True',  1, 1),
(14, 'False', 0, 2);

-- ── Quiz 5: Functions ────────────────────────

INSERT OR IGNORE INTO questions
    (id, quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index)
VALUES
(15, 5, 5, 'mcq',
 'What is function overloading?',
 NULL,
 'Function overloading allows multiple functions with the same name but different parameter lists (types or counts). The compiler selects the correct version at compile time based on the arguments provided.',
 1, 10, 1),

(16, 5, 5, 'mcq',
 'What is the output of this code?',
 'int add(int a, int b = 10) {\n    return a + b;\n}\nstd::cout << add(5);',
 'The function has a default argument b=10. Calling add(5) uses a=5 and the default b=10, so the result is 15.',
 2, 10, 2),

(17, 5, 5, 'mcq',
 'What does `pass by reference` mean in C++?',
 NULL,
 'Pass by reference means the function receives an alias (reference) to the original variable, not a copy. Changes inside the function affect the original variable.',
 2, 10, 3);

INSERT OR IGNORE INTO options (question_id, content, is_correct, order_index) VALUES
(15, 'Multiple functions with the same name but different parameters',      1, 1),
(15, 'A function that calls itself',                                         0, 2),
(15, 'A function that returns multiple values',                              0, 3),
(15, 'Defining a function inside another function',                          0, 4),

(16, '15', 1, 1),
(16, '5',  0, 2),
(16, '10', 0, 3),
(16, 'Error: missing argument', 0, 4),

(17, 'The function receives an alias to the original variable',   1, 1),
(17, 'The function receives a copy of the variable',             0, 2),
(17, 'The function receives a pointer to the variable',          0, 3),
(17, 'The variable is passed through a global scope',            0, 4);

-- ── Quiz 6: Arrays & Strings ─────────────────

INSERT OR IGNORE INTO questions
    (id, quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index)
VALUES
(18, 6, 6, 'mcq',
 'What is the index of the last element in an array of size 5?',
 NULL,
 'Arrays in C++ are zero-indexed. For an array of size 5, valid indices are 0,1,2,3,4. The last element is at index 4.',
 1, 10, 1),

(19, 6, 6, 'mcq',
 'Which method returns the length of a std::string?',
 NULL,
 'Both size() and length() return the number of characters in a std::string. They are equivalent. empty() checks if it is empty. find() searches for a substring.',
 1, 10, 2),

(20, 6, 6, 'code_output',
 'What is the output of this code?',
 'std::string s = "Hello";\ns += " World";\nstd::cout << s.size();',
 'The concatenated string "Hello World" has 11 characters. std::string::size() returns the number of characters excluding the null terminator.',
 2, 10, 3);

INSERT OR IGNORE INTO options (question_id, content, is_correct, order_index) VALUES
(18, '4', 1, 1),
(18, '5', 0, 2),
(18, '6', 0, 3),
(18, '0', 0, 4),

(19, 'size() or length()', 1, 1),
(19, 'count()',            0, 2),
(19, 'capacity()',         0, 3),
(19, 'len()',              0, 4),

(20, '11', 1, 1),
(20, '10', 0, 2),
(20, '12', 0, 3),
(20, '5',  0, 4);

-- ── Quiz 7: Pointers & References ───────────

INSERT OR IGNORE INTO questions
    (id, quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index)
VALUES
(21, 7, 7, 'mcq',
 'What does the dereference operator * do when applied to a pointer?',
 NULL,
 'The dereference operator * accesses the value stored at the memory address held by the pointer. It gives you the object the pointer points to.',
 2, 10, 1),

(22, 7, 7, 'mcq',
 'What is the value of `b` after this code runs?',
 'int a = 10;\nint& b = a;\na = 20;',
 'b is a reference (alias) to a. When a changes to 20, b also reflects that change because they refer to the same memory location.',
 2, 10, 2),

(23, 7, 7, 'true_false',
 'A reference in C++ can be rebound to refer to a different variable after initialization.',
 NULL,
 'False. Unlike pointers, references in C++ cannot be rebound. Once a reference is initialized to refer to a variable, it always refers to that same variable for its entire lifetime.',
 2, 10, 3),

(24, 7, 7, 'mcq',
 'What is a null pointer in C++?',
 NULL,
 'A null pointer is a pointer that does not point to any valid memory address. In modern C++, nullptr (introduced in C++11) is the preferred way to represent a null pointer, replacing the old NULL macro.',
 2, 10, 4);

INSERT OR IGNORE INTO options (question_id, content, is_correct, order_index) VALUES
(21, 'Accesses the value at the memory address the pointer holds',   1, 1),
(21, 'Gets the memory address of the pointer itself',                0, 2),
(21, 'Multiplies the pointer value by 2',                            0, 3),
(21, 'Declares a new pointer variable',                              0, 4),

(22, '20', 1, 1),
(22, '10', 0, 2),
(22, '0',  0, 3),
(22, 'Undefined behavior', 0, 4),

(23, 'True',  0, 1),
(23, 'False', 1, 2),

(24, 'A pointer that does not point to any valid memory address',     1, 1),
(24, 'A pointer to the value 0',                                     0, 2),
(24, 'A pointer that has been deleted',                              0, 3),
(24, 'A pointer to the stack',                                       0, 4);

-- ── Quiz 8: OOP ──────────────────────────────

INSERT OR IGNORE INTO questions
    (id, quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index)
VALUES
(25, 8, 8, 'mcq',
 'What access specifier makes class members accessible only within the class itself?',
 NULL,
 'private members are accessible only from within the class itself and its friends. protected allows access in derived classes as well. public allows access from anywhere.',
 1, 10, 1),

(26, 8, 8, 'mcq',
 'What is a constructor in C++?',
 NULL,
 'A constructor is a special member function that is automatically called when an object is created. It initializes the object''s data members and has the same name as the class with no return type.',
 1, 10, 2),

(27, 8, 8, 'mcq',
 'What keyword is used to enable runtime polymorphism in C++?',
 NULL,
 'The virtual keyword declares a virtual function. When called through a base class pointer or reference, the most-derived override is invoked at runtime (dynamic dispatch via vtable).',
 2, 10, 3),

(28, 8, 8, 'true_false',
 'A class can inherit from multiple base classes in C++.',
 NULL,
 'True. C++ supports multiple inheritance, allowing a class to inherit from more than one base class. This can lead to the diamond problem, which can be solved using virtual inheritance.',
 2, 10, 4);

INSERT OR IGNORE INTO options (question_id, content, is_correct, order_index) VALUES
(25, 'private',    1, 1),
(25, 'public',     0, 2),
(25, 'protected',  0, 3),
(25, 'internal',   0, 4),

(26, 'A special function called when an object is created',         1, 1),
(26, 'A function that destroys an object',                         0, 2),
(26, 'A static function belonging to the class',                   0, 3),
(26, 'A function that copies an object',                           0, 4),

(27, 'virtual',   1, 1),
(27, 'override',  0, 2),
(27, 'abstract',  0, 3),
(27, 'dynamic',   0, 4),

(28, 'True',  1, 1),
(28, 'False', 0, 2);

-- ── Quiz 9: Templates ────────────────────────

INSERT OR IGNORE INTO questions
    (id, quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index)
VALUES
(29, 9, 9, 'mcq',
 'What is a function template in C++?',
 NULL,
 'A function template is a blueprint for generating functions. The compiler generates specific function instances (specializations) based on the types provided at the call site. This enables generic programming.',
 2, 10, 1),

(30, 9, 9, 'mcq',
 'What does template specialization allow you to do?',
 NULL,
 'Template specialization allows you to provide a custom implementation of a template for specific types. Full specialization handles one specific type; partial specialization handles a subset of types.',
 3, 10, 2),

(31, 9, 9, 'mcq',
 'What is `typename` used for inside a template?',
 NULL,
 'typename has two roles in templates: it introduces a type parameter (template <typename T>) and it disambiguates dependent names — telling the compiler a dependent name is a type (typename T::value_type).',
 3, 10, 3);

INSERT OR IGNORE INTO options (question_id, content, is_correct, order_index) VALUES
(29, 'A blueprint for creating generic functions that work with any type',  1, 1),
(29, 'A function that creates objects',                                     0, 2),
(29, 'A virtual function in a template class',                              0, 3),
(29, 'A function pointer stored in a template',                             0, 4),

(30, 'Provide a custom implementation for specific types',                              1, 1),
(30, 'Prevent a template from being instantiated',                                     0, 2),
(30, 'Add more type parameters to an existing template',                               0, 3),
(30, 'Override a virtual function in a template class',                                0, 4),

(31, 'Introduce a type template parameter and disambiguate dependent type names',    1, 1),
(31, 'Specify that the template is type-safe',                                        0, 2),
(31, 'Declare a runtime type check',                                                  0, 3),
(31, 'Mark a function as returning a type',                                           0, 4);

-- ── Quiz 10: STL ─────────────────────────────

INSERT OR IGNORE INTO questions
    (id, quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index)
VALUES
(32, 10, 10, 'mcq',
 'Which STL container provides O(1) average-time lookup by key?',
 NULL,
 'std::unordered_map uses a hash table, providing O(1) average-case lookup, insertion, and deletion. std::map uses a balanced BST with O(log n). std::vector is a sequence container with O(n) lookup.',
 2, 10, 1),

(33, 10, 10, 'mcq',
 'What does `std::sort` require of the range it sorts?',
 NULL,
 'std::sort requires RandomAccessIterators (like those of std::vector or std::deque). It uses an introsort algorithm (typically). std::list has its own sort() member because it does not support random access.',
 2, 10, 2),

(34, 10, 10, 'mcq',
 'What is the difference between `std::vector::size()` and `std::vector::capacity()`?',
 NULL,
 'size() returns the number of elements currently stored. capacity() returns the number of elements the vector can hold without reallocating. capacity() >= size() always.',
 2, 10, 3),

(35, 10, 10, 'mcq',
 'Which algorithm finds the first element satisfying a predicate?',
 NULL,
 'std::find_if searches a range and returns an iterator to the first element for which the predicate returns true. std::count_if counts such elements. std::all_of checks if all elements satisfy the predicate.',
 2, 10, 4);

INSERT OR IGNORE INTO options (question_id, content, is_correct, order_index) VALUES
(32, 'std::unordered_map', 1, 1),
(32, 'std::map',           0, 2),
(32, 'std::vector',        0, 3),
(32, 'std::list',          0, 4),

(33, 'Random access iterators (e.g., std::vector, std::deque)',  1, 1),
(33, 'Any iterator type',                                        0, 2),
(33, 'Only forward iterators',                                   0, 3),
(33, 'Bidirectional iterators (e.g., std::list)',                0, 4),

(34, 'size() = current element count; capacity() = reserved space before realloc',  1, 1),
(34, 'They are always equal',                                                        0, 2),
(34, 'capacity() = current element count; size() = reserved space',                 0, 3),
(34, 'size() includes null terminator; capacity() does not',                         0, 4),

(35, 'std::find_if',  1, 1),
(35, 'std::count_if', 0, 2),
(35, 'std::all_of',   0, 3),
(35, 'std::search',   0, 4);

-- ── Quiz 11: Memory Management ───────────────

INSERT OR IGNORE INTO questions
    (id, quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index)
VALUES
(36, 11, 11, 'mcq',
 'What is a memory leak?',
 NULL,
 'A memory leak occurs when dynamically allocated memory (via new/malloc) is never released (via delete/free), causing the program to consume more and more memory over time.',
 2, 10, 1),

(37, 11, 11, 'mcq',
 'Which smart pointer represents exclusive ownership with no sharing?',
 NULL,
 'std::unique_ptr (C++11) represents exclusive ownership. It cannot be copied, only moved. When it goes out of scope, the owned resource is automatically deleted. Use std::shared_ptr for shared ownership.',
 2, 10, 2),

(38, 11, 11, 'mcq',
 'What is the Rule of Five in C++?',
 NULL,
 'The Rule of Five states: if you define any of the five special member functions (destructor, copy constructor, copy assignment, move constructor, move assignment), you should define or explicitly delete all five, because they are interdependent.',
 3, 10, 3),

(39, 11, 11, 'true_false',
 'std::shared_ptr uses reference counting to manage the lifetime of a shared resource.',
 NULL,
 'True. std::shared_ptr maintains a control block with a reference count. When the last shared_ptr owning the resource is destroyed, the reference count drops to zero and the resource is deleted.',
 2, 10, 4);

INSERT OR IGNORE INTO options (question_id, content, is_correct, order_index) VALUES
(36, 'Dynamically allocated memory that is never released',    1, 1),
(36, 'Accessing memory outside array bounds',                  0, 2),
(36, 'Using a pointer after it has been deleted',              0, 3),
(36, 'Stack overflow due to deep recursion',                   0, 4),

(37, 'std::unique_ptr', 1, 1),
(37, 'std::shared_ptr', 0, 2),
(37, 'std::weak_ptr',   0, 3),
(37, 'std::auto_ptr',   0, 4),

(38, 'If you define any special member function, define or delete all five',            1, 1),
(38, 'Always define exactly five member functions in every class',                      0, 2),
(38, 'Use exactly five classes in a well-designed program',                             0, 3),
(38, 'A function template must have at most five type parameters',                      0, 4),

(39, 'True',  1, 1),
(39, 'False', 0, 2);

-- ── Quiz 12: Modern C++ ──────────────────────

INSERT OR IGNORE INTO questions
    (id, quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index)
VALUES
(40, 12, 12, 'mcq',
 'What is a lambda expression in C++?',
 NULL,
 'A lambda expression (C++11) is an anonymous function object that can capture variables from its enclosing scope. Syntax: [capture](params) -> return_type { body }.',
 2, 10, 1),

(41, 12, 12, 'mcq',
 'What does `std::move` actually do to its argument?',
 NULL,
 'std::move does NOT move anything. It is a cast that converts its argument to an rvalue reference, enabling move semantics. The actual move is performed by the move constructor or move assignment operator.',
 3, 10, 2),

(42, 12, 12, 'mcq',
 'What is the purpose of `[[nodiscard]]` attribute in C++17?',
 NULL,
 '[[nodiscard]] tells the compiler to warn if the return value of a function is discarded (ignored) by the caller. Useful for functions where ignoring the return value is likely a bug (e.g., error codes, allocated resources).',
 3, 10, 3),

(43, 12, 12, 'mcq',
 'What does structured bindings (C++17) allow?',
 'auto [x, y] = std::pair{1, 2};',
 'Structured bindings (C++17) allow you to decompose an object into its constituent parts and bind them to named variables. Works with pairs, tuples, arrays, and structs/classes with accessible members.',
 2, 10, 4),

(44, 12, 12, 'mcq',
 'What is a fold expression in C++17?',
 NULL,
 'A fold expression allows a variadic template parameter pack to be reduced using a binary operator. Example: (args + ...) expands to arg0 + arg1 + ... + argN.',
 4, 10, 5),

(45, 12, 12, 'true_false',
 'In C++11 and later, range-based for loops can iterate over any container that has begin() and end() member functions or free functions.',
 NULL,
 'True. The range-based for loop (for (auto& x : range)) works with any type that provides begin() and end() — either as member functions or as free functions found via ADL (Argument-Dependent Lookup).',
 2, 10, 6);

INSERT OR IGNORE INTO options (question_id, content, is_correct, order_index) VALUES
(40, 'An anonymous function object that can capture variables from its enclosing scope', 1, 1),
(40, 'A function that automatically infers its return type',                              0, 2),
(40, 'A function defined inside a class',                                                 0, 3),
(40, 'A recursive function with a fixed point',                                           0, 4),

(41, 'Casts its argument to an rvalue reference, enabling move semantics',              1, 1),
(41, 'Physically moves the data to a new memory location',                              0, 2),
(41, 'Copies the argument and marks the copy as movable',                               0, 3),
(41, 'Destroys the argument after copying',                                             0, 4),

(42, 'Warns when the return value of the function is ignored by the caller',            1, 1),
(42, 'Marks a function as pure (no side effects)',                                      0, 2),
(42, 'Prevents the function from being called without arguments',                       0, 3),
(42, 'Forces the compiler to inline the function',                                      0, 4),

(43, 'Decompose an object into named variables bound to its members',                   1, 1),
(43, 'Bind a reference to a structure without copying',                                 0, 2),
(43, 'Create a struct from individual variables',                                       0, 3),
(43, 'Unpack function arguments at the call site',                                      0, 4),

(44, 'Reduce a variadic parameter pack with a binary operator',                         1, 1),
(44, 'Create a fold (functional construct) over iterators',                             0, 2),
(44, 'Fold a template class hierarchy into a single type',                              0, 3),
(44, 'A shorthand for std::accumulate over a range',                                    0, 4),

(45, 'True',  1, 1),
(45, 'False', 0, 2);

-- ─────────────────────────────────────────────
-- QUESTION-TAG ASSOCIATIONS
-- ─────────────────────────────────────────────
-- Map tag names to IDs (for reference when inserting):
-- basics=1, variables=2, types=3, operators=4, control-flow=5,
-- functions=6, arrays=7, pointers=8, references=9, strings=10,
-- oop=11, classes=12, inheritance=13, templates=14, stl=15,
-- memory=16, smart-pointers=17, algorithms=18, lambdas=19,
-- move-semantics=20, concurrency=21, cpp11=22, cpp14=23,
-- cpp17=24, cpp20=25, best-practices=26

INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
-- Basics
(1, 1), (2, 1), (3, 1), (4, 1),
-- Variables & Types
(5, 2), (5, 3), (6, 2), (6, 3), (7, 2), (8, 3),
-- Operators
(9, 4), (10, 4), (11, 4),
-- Control Flow
(12, 5), (13, 5), (14, 5),
-- Functions
(15, 6), (16, 6), (17, 6),
-- Arrays & Strings
(18, 7), (19, 10), (20, 10),
-- Pointers & References
(21, 8), (22, 9), (23, 9), (24, 8),
-- OOP
(25, 11), (25, 12), (26, 12), (27, 11), (28, 13),
-- Templates
(29, 14), (30, 14), (31, 14),
-- STL
(32, 15), (33, 15), (33, 18), (34, 15), (35, 15), (35, 18),
-- Memory
(36, 16), (37, 16), (37, 17), (38, 16), (38, 26), (39, 17),
-- Modern C++
(40, 19), (40, 22), (41, 20), (41, 22), (42, 24), (43, 24),
(44, 24), (45, 22);