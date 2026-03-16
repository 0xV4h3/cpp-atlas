-- =============================================================
-- CppAtlas Content Patch: Foundations Pack 01 (Part 2)
-- Date: 2026-03-16
-- Questions: 50 (order_index 1201..1250)
-- Topics: basics, variables, types, operators, control-flow
-- =============================================================

INSERT OR IGNORE INTO tags (name, color) VALUES
('basics', '#89D185'),
('variables', '#89D185'),
('types', '#4EC9B0'),
('operators', '#DCDCAA'),
('control-flow', '#569CD6'),
('best-practices', '#89D185');

INSERT OR IGNORE INTO quizzes (title, description, topic_id, difficulty, type, is_active, created_by, version) VALUES
('Program Structure Basics Quiz', 'Statements, translation units, headers, preprocessor, namespaces', 1, 1, 'standard', 1, 0, 1),
('Variables & Literals Deep Dive Quiz', 'Initialization forms, constants, literals, conversions, precedence', 2, 2, 'standard', 1, 0, 1),
('Control Flow Intensive Quiz', 'if/switch/loops/goto edge-cases and common pitfalls', 4, 2, 'standard', 1, 0, 1),
('Operators Drill Quiz', 'Precedence, associativity, comma, remainder, relational nuances', 3, 2, 'standard', 1, 0, 1),
('Random & Loop Patterns Quiz', 'while/do-while/for and random generation basics', 4, 2, 'standard', 1, 0, 1);

-- 1201..1210 Program Structure Basics Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Program Structure Basics Quiz' ORDER BY id DESC LIMIT 1), 1, 'mcq', 'What is a statement in C++?', NULL, 'A statement is an instruction that performs an action.', 1, 10, 1201, 1, 'Semicolon-terminated actions are common.', 'https://www.learncpp.com/cpp-tutorial/statements-and-the-structure-of-a-program/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1201), 'An instruction executed by the program', 1, 1),
((SELECT id FROM questions WHERE order_index=1201), 'A header include path only', 0, 2),
((SELECT id FROM questions WHERE order_index=1201), 'A namespace alias only', 0, 3),
((SELECT id FROM questions WHERE order_index=1201), 'A linker directive', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Program Structure Basics Quiz' ORDER BY id DESC LIMIT 1), 1, 'true_false', 'A C++ program can be split across multiple source files.', NULL, 'True.', 1, 10, 1202, 1, 'Translation units are linked together.', 'https://www.learncpp.com/cpp-tutorial/programs-with-multiple-code-files/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1202), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1202), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Program Structure Basics Quiz' ORDER BY id DESC LIMIT 1), 1, 'code_output', 'What is printed?', '#include <iostream>\nint main(){ std::cout << "Hi"; }', 'Outputs Hi.', 1, 10, 1203, 1, 'Basic iostream usage.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-iostream-cout-cin-and-endl/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1203), 'Hi', 1, 1),
((SELECT id FROM questions WHERE order_index=1203), 'hi', 0, 2),
((SELECT id FROM questions WHERE order_index=1203), 'Nothing', 0, 3),
((SELECT id FROM questions WHERE order_index=1203), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Program Structure Basics Quiz' ORDER BY id DESC LIMIT 1), 1, 'mcq', 'What is the purpose of header files?', NULL, 'Headers expose declarations for reuse across source files.', 1, 10, 1204, 1, 'Declarations vs definitions.', 'https://www.learncpp.com/cpp-tutorial/header-files/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1204), 'Share declarations between files', 1, 1),
((SELECT id FROM questions WHERE order_index=1204), 'Store only runtime logs', 0, 2),
((SELECT id FROM questions WHERE order_index=1204), 'Replace linker', 0, 3),
((SELECT id FROM questions WHERE order_index=1204), 'Allocate memory', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Program Structure Basics Quiz' ORDER BY id DESC LIMIT 1), 1, 'multi_select', 'Select valid preprocessor directives.', NULL, '#include, #define, #if are preprocessor directives.', 2, 15, 1205, 1, 'Starts with #.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-the-preprocessor/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1205), '#include', 1, 1),
((SELECT id FROM questions WHERE order_index=1205), '#define', 1, 2),
((SELECT id FROM questions WHERE order_index=1205), '#if', 1, 3),
((SELECT id FROM questions WHERE order_index=1205), 'namespace', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Program Structure Basics Quiz' ORDER BY id DESC LIMIT 1), 1, 'fill_blank', 'C++ standard library symbols like cout are in namespace ____.', NULL, 'std.', 1, 10, 1206, 1, 'std:: prefix.', 'https://www.learncpp.com/cpp-tutorial/naming-collisions-and-an-introduction-to-namespaces/');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Program Structure Basics Quiz' ORDER BY id DESC LIMIT 1), 1, 'mcq', 'Why use namespaces?', NULL, 'They avoid naming collisions and organize symbols.', 1, 10, 1207, 1, 'Scope management.', 'https://www.learncpp.com/cpp-tutorial/naming-collisions-and-an-introduction-to-namespaces/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1207), 'Prevent name collisions and organize code', 1, 1),
((SELECT id FROM questions WHERE order_index=1207), 'Force dynamic allocation', 0, 2),
((SELECT id FROM questions WHERE order_index=1207), 'Replace classes', 0, 3),
((SELECT id FROM questions WHERE order_index=1207), 'Disable linking', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Program Structure Basics Quiz' ORDER BY id DESC LIMIT 1), 1, 'true_false', 'Header guards help prevent multiple inclusion problems.', NULL, 'True.', 2, 10, 1208, 1, 'One-definition issues.', 'https://www.learncpp.com/cpp-tutorial/header-files/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1208), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1208), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Program Structure Basics Quiz' ORDER BY id DESC LIMIT 1), 1, 'mcq', 'What phase handles #include expansion?', NULL, 'Preprocessing phase.', 2, 10, 1209, 1, 'Before compilation proper.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-the-preprocessor/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1209), 'Preprocessor stage', 1, 1),
((SELECT id FROM questions WHERE order_index=1209), 'Linker stage', 0, 2),
((SELECT id FROM questions WHERE order_index=1209), 'Runtime loader only', 0, 3),
((SELECT id FROM questions WHERE order_index=1209), 'Debugger stage', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Program Structure Basics Quiz' ORDER BY id DESC LIMIT 1), 1, 'code_output', 'What is printed?', 'int main(){ int x{1}; x = x + 2; std::cout << x; }', 'Assignment updates x to 3.', 1, 10, 1210, 1, 'Basic statements and assignment.', 'https://www.learncpp.com/cpp-tutorial/variable-assignment-and-initialization/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1210), '3', 1, 1),
((SELECT id FROM questions WHERE order_index=1210), '2', 0, 2),
((SELECT id FROM questions WHERE order_index=1210), '1', 0, 3),
((SELECT id FROM questions WHERE order_index=1210), 'Compile error', 0, 4);

-- 1211..1220 Variables & Literals Deep Dive Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Variables & Literals Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 2, 'mcq', 'Which initialization form uses braces?', NULL, 'List-initialization uses {}.', 1, 10, 1211, 1, 'Brace initialization.', 'https://www.learncpp.com/cpp-tutorial/variable-assignment-and-initialization/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1211), 'int x{42};', 1, 1),
((SELECT id FROM questions WHERE order_index=1211), 'int x = ;', 0, 2),
((SELECT id FROM questions WHERE order_index=1211), 'x int 42;', 0, 3),
((SELECT id FROM questions WHERE order_index=1211), 'int{ x } = 42;', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Variables & Literals Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 2, 'true_false', 'A named constant should generally be declared const or constexpr.', NULL, 'True.', 1, 10, 1212, 1, 'Intent + safety.', 'https://www.learncpp.com/cpp-tutorial/constant-variables-named-constants/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1212), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1212), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Variables & Literals Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 2, 'code_output', 'What is printed?', 'int a{10};\nint b{3};\nstd::cout << a % b;', 'Remainder of 10 divided by 3 is 1.', 2, 10, 1213, 1, 'Modulo operator.', 'https://www.learncpp.com/cpp-tutorial/remainder-and-exponentiation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1213), '1', 1, 1),
((SELECT id FROM questions WHERE order_index=1213), '3', 0, 2),
((SELECT id FROM questions WHERE order_index=1213), '0', 0, 3),
((SELECT id FROM questions WHERE order_index=1213), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Variables & Literals Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 3, 'mcq', 'What does operator precedence determine?', NULL, 'The order in which operators bind in an expression.', 2, 10, 1214, 1, 'Binding before evaluation sequencing nuances.', 'https://www.learncpp.com/cpp-tutorial/operator-precedence-and-associativity/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1214), 'How expression operators group/bind by default', 1, 1),
((SELECT id FROM questions WHERE order_index=1214), 'Variable lifetime', 0, 2),
((SELECT id FROM questions WHERE order_index=1214), 'Header inclusion order', 0, 3),
((SELECT id FROM questions WHERE order_index=1214), 'Namespace visibility', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Variables & Literals Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 3, 'multi_select', 'Select literal kinds valid in C++.', NULL, 'Integer, floating, character, and string literals are all fundamental literal categories.', 2, 15, 1215, 1, 'Literal taxonomy.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-literals-and-operators/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1215), '123', 1, 1),
((SELECT id FROM questions WHERE order_index=1215), '3.14', 1, 2),
((SELECT id FROM questions WHERE order_index=1215), '''A''', 1, 3),
((SELECT id FROM questions WHERE order_index=1215), 'identifierOnly', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Variables & Literals Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 2, 'fill_blank', 'Keyword often used for compile-time constant variables is _____.', NULL, 'constexpr.', 2, 10, 1216, 1, 'Modern constant style.', 'https://www.learncpp.com/cpp-tutorial/constant-variables-named-constants/');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Variables & Literals Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 3, 'mcq', 'What is special about comma operator in expressions?', NULL, 'It evaluates left expression then right and yields right expression value.', 3, 12, 1217, 1, 'Rare operator; avoid overuse.', 'https://www.learncpp.com/cpp-tutorial/the-comma-operator/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1217), 'Evaluates left then right, result is right operand', 1, 1),
((SELECT id FROM questions WHERE order_index=1217), 'Always separates function arguments only', 0, 2),
((SELECT id FROM questions WHERE order_index=1217), 'Equivalent to logical AND', 0, 3),
((SELECT id FROM questions WHERE order_index=1217), 'Forbidden in C++ expressions', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Variables & Literals Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 3, 'true_false', 'Relational comparisons on floating-point values can be tricky due to precision.', NULL, 'True.', 2, 10, 1218, 1, 'Precision/rounding concerns.', 'https://www.learncpp.com/cpp-tutorial/relational-operators-and-floating-point-comparisons/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1218), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1218), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Variables & Literals Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 3, 'mcq', 'Which expression groups as (a + (b * c)) by precedence?', NULL, 'Multiplication has higher precedence than addition.', 2, 10, 1219, 1, 'Standard arithmetic precedence.', 'https://www.learncpp.com/cpp-tutorial/operator-precedence-and-associativity/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1219), 'a + b * c', 1, 1),
((SELECT id FROM questions WHERE order_index=1219), '(a + b) * c', 0, 2),
((SELECT id FROM questions WHERE order_index=1219), 'a * b + c (same grouping)', 0, 3),
((SELECT id FROM questions WHERE order_index=1219), 'a, b, c', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Variables & Literals Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 2, 'code_output', 'What is printed?', 'int x = 2, y = 3;\nstd::cout << x + y * 2;', 'y*2 is 6, then +2 => 8.', 1, 10, 1220, 1, 'Apply precedence.', 'https://www.learncpp.com/cpp-tutorial/operator-precedence-and-associativity/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1220), '8', 1, 1),
((SELECT id FROM questions WHERE order_index=1220), '10', 0, 2),
((SELECT id FROM questions WHERE order_index=1220), '7', 0, 3),
((SELECT id FROM questions WHERE order_index=1220), 'Compile error', 0, 4);

-- 1221..1230 Operators Drill Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Operators Drill Quiz' ORDER BY id DESC LIMIT 1), 3, 'mcq', 'What does x += y mean?', NULL, 'Equivalent to x = x + y (with usual compound semantics).', 1, 10, 1221, 1, 'Compound assignment.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-literals-and-operators/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1221), 'Add y to x and store back in x', 1, 1),
((SELECT id FROM questions WHERE order_index=1221), 'Compare x and y', 0, 2),
((SELECT id FROM questions WHERE order_index=1221), 'Initialize x and y', 0, 3),
((SELECT id FROM questions WHERE order_index=1221), 'Bit-shift x by y only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Operators Drill Quiz' ORDER BY id DESC LIMIT 1), 3, 'true_false', 'The remainder operator % is defined for integer operands.', NULL, 'True (built-in integer arithmetic context).', 2, 10, 1222, 1, 'Integer math operator.', 'https://www.learncpp.com/cpp-tutorial/remainder-and-exponentiation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1222), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1222), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Operators Drill Quiz' ORDER BY id DESC LIMIT 1), 3, 'code_output', 'What is printed?', 'int a=7, b=4;\nstd::cout << a - b;', 'Simple subtraction: 3.', 1, 10, 1223, 1, 'Arithmetic basics.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-literals-and-operators/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1223), '3', 1, 1),
((SELECT id FROM questions WHERE order_index=1223), '11', 0, 2),
((SELECT id FROM questions WHERE order_index=1223), '28', 0, 3),
((SELECT id FROM questions WHERE order_index=1223), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Operators Drill Quiz' ORDER BY id DESC LIMIT 1), 3, 'mcq', 'What is associativity?', NULL, 'It determines grouping order when operators have same precedence.', 2, 10, 1224, 1, 'Left-to-right vs right-to-left grouping.', 'https://www.learncpp.com/cpp-tutorial/operator-precedence-and-associativity/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1224), 'Rule for grouping operators of same precedence', 1, 1),
((SELECT id FROM questions WHERE order_index=1224), 'Variable storage duration', 0, 2),
((SELECT id FROM questions WHERE order_index=1224), 'Namespace linkage policy', 0, 3),
((SELECT id FROM questions WHERE order_index=1224), 'Header include order', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Operators Drill Quiz' ORDER BY id DESC LIMIT 1), 3, 'multi_select', 'Select relational operators.', NULL, '<, <=, >, >=, ==, != are relational/equality comparisons.', 1, 15, 1225, 1, 'Comparison operators.', 'https://www.learncpp.com/cpp-tutorial/relational-operators-and-floating-point-comparisons/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1225), '<', 1, 1),
((SELECT id FROM questions WHERE order_index=1225), '>=', 1, 2),
((SELECT id FROM questions WHERE order_index=1225), '==', 1, 3),
((SELECT id FROM questions WHERE order_index=1225), '+=', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Operators Drill Quiz' ORDER BY id DESC LIMIT 1), 3, 'fill_blank', 'Operator with highest common arithmetic precedence among + and * is ____.', NULL, '*.', 1, 10, 1226, 1, 'Multiplication binds tighter.', 'https://www.learncpp.com/cpp-tutorial/operator-precedence-and-associativity/');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Operators Drill Quiz' ORDER BY id DESC LIMIT 1), 3, 'mcq', 'What does (x, y) evaluate to with comma operator?', NULL, 'It evaluates to y value after evaluating x first.', 3, 12, 1227, 1, 'Comma operator semantics.', 'https://www.learncpp.com/cpp-tutorial/the-comma-operator/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1227), 'Value of y', 1, 1),
((SELECT id FROM questions WHERE order_index=1227), 'Value of x', 0, 2),
((SELECT id FROM questions WHERE order_index=1227), 'Sum x+y', 0, 3),
((SELECT id FROM questions WHERE order_index=1227), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Operators Drill Quiz' ORDER BY id DESC LIMIT 1), 3, 'true_false', 'For floating point, direct equality comparison can be unreliable in some cases.', NULL, 'True.', 2, 10, 1228, 1, 'Binary floating-point representation.', 'https://www.learncpp.com/cpp-tutorial/relational-operators-and-floating-point-comparisons/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1228), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1228), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Operators Drill Quiz' ORDER BY id DESC LIMIT 1), 3, 'mcq', 'Remainder of 17 % 5 is?', NULL, '17 = 3*5 + 2.', 1, 10, 1229, 1, 'Modulo remainder.', 'https://www.learncpp.com/cpp-tutorial/remainder-and-exponentiation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1229), '2', 1, 1),
((SELECT id FROM questions WHERE order_index=1229), '3', 0, 2),
((SELECT id FROM questions WHERE order_index=1229), '5', 0, 3),
((SELECT id FROM questions WHERE order_index=1229), '1', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Operators Drill Quiz' ORDER BY id DESC LIMIT 1), 3, 'code_output', 'What is printed?', 'int x=1; int y=2; std::cout << (x<y);', 'Comparison true prints 1 by default bool formatting.', 1, 10, 1230, 1, 'bool stream format.', 'https://www.learncpp.com/cpp-tutorial/relational-operators-and-floating-point-comparisons/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1230), '1', 1, 1),
((SELECT id FROM questions WHERE order_index=1230), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=1230), 'true (text)', 0, 3),
((SELECT id FROM questions WHERE order_index=1230), 'Compile error', 0, 4);

-- 1231..1240 Control Flow Intensive Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Control Flow Intensive Quiz' ORDER BY id DESC LIMIT 1), 4, 'mcq', 'Which loop checks condition before each iteration?', NULL, 'while and for are pre-condition loops.', 1, 10, 1231, 1, 'Pre-check loop type.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-loops-and-while-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1231), 'while loop', 1, 1),
((SELECT id FROM questions WHERE order_index=1231), 'do-while only', 0, 2),
((SELECT id FROM questions WHERE order_index=1231), 'goto loop', 0, 3),
((SELECT id FROM questions WHERE order_index=1231), 'switch loop', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Control Flow Intensive Quiz' ORDER BY id DESC LIMIT 1), 4, 'true_false', 'A do-while loop executes body at least once.', NULL, 'True.', 1, 10, 1232, 1, 'Condition checked after body.', 'https://www.learncpp.com/cpp-tutorial/do-while-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1232), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1232), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Control Flow Intensive Quiz' ORDER BY id DESC LIMIT 1), 4, 'code_output', 'What is printed?', 'int i=0;\nwhile(i<3){ std::cout<<i; ++i; }', 'Prints 0,1,2 => 012.', 1, 10, 1233, 1, 'Counter progression.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-loops-and-while-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1233), '012', 1, 1),
((SELECT id FROM questions WHERE order_index=1233), '123', 0, 2),
((SELECT id FROM questions WHERE order_index=1233), '01', 0, 3),
((SELECT id FROM questions WHERE order_index=1233), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Control Flow Intensive Quiz' ORDER BY id DESC LIMIT 1), 4, 'mcq', 'What is common if-statement pitfall?', NULL, 'Using assignment (=) instead of equality (==) in condition is a classic bug.', 2, 10, 1234, 1, 'Condition correctness.', 'https://www.learncpp.com/cpp-tutorial/common-if-statement-problems/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1234), 'Using = instead of == in condition', 1, 1),
((SELECT id FROM questions WHERE order_index=1234), 'Using braces', 0, 2),
((SELECT id FROM questions WHERE order_index=1234), 'Using bool condition', 0, 3),
((SELECT id FROM questions WHERE order_index=1234), 'Using else branch', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Control Flow Intensive Quiz' ORDER BY id DESC LIMIT 1), 4, 'multi_select', 'Select valid loop statements.', NULL, 'for, while, do-while are loop constructs.', 1, 15, 1235, 1, 'Core loop trio.', 'https://www.studyplan.dev/intro-to-programming/loops');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1235), 'for', 1, 1),
((SELECT id FROM questions WHERE order_index=1235), 'while', 1, 2),
((SELECT id FROM questions WHERE order_index=1235), 'do-while', 1, 3),
((SELECT id FROM questions WHERE order_index=1235), 'switch-only-loop', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Control Flow Intensive Quiz' ORDER BY id DESC LIMIT 1), 4, 'fill_blank', 'Statement that exits current loop immediately is ____.', NULL, 'break.', 1, 10, 1236, 1, 'Not continue.', 'https://www.learncpp.com/cpp-tutorial/break-and-continue/');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Control Flow Intensive Quiz' ORDER BY id DESC LIMIT 1), 4, 'mcq', 'What does continue do?', NULL, 'Skips remaining current iteration and proceeds with next iteration.', 1, 10, 1237, 1, 'Loop iteration control.', 'https://www.learncpp.com/cpp-tutorial/break-and-continue/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1237), 'Skips rest of current iteration', 1, 1),
((SELECT id FROM questions WHERE order_index=1237), 'Ends function immediately', 0, 2),
((SELECT id FROM questions WHERE order_index=1237), 'Exits entire program', 0, 3),
((SELECT id FROM questions WHERE order_index=1237), 'Restarts application', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Control Flow Intensive Quiz' ORDER BY id DESC LIMIT 1), 4, 'true_false', 'goto exists in C++ but should generally be used sparingly.', NULL, 'True.', 2, 10, 1238, 1, 'Readability concerns.', 'https://www.learncpp.com/cpp-tutorial/goto-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1238), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1238), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Control Flow Intensive Quiz' ORDER BY id DESC LIMIT 1), 4, 'mcq', 'How many times runs: for(int i=1;i<=3;++i) ?', NULL, 'i takes 1,2,3 => 3 iterations.', 1, 10, 1239, 1, 'Inclusive bound.', 'https://www.learncpp.com/cpp-tutorial/for-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1239), '3', 1, 1),
((SELECT id FROM questions WHERE order_index=1239), '2', 0, 2),
((SELECT id FROM questions WHERE order_index=1239), '4', 0, 3),
((SELECT id FROM questions WHERE order_index=1239), '0', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Control Flow Intensive Quiz' ORDER BY id DESC LIMIT 1), 4, 'code_output', 'What is printed?', 'int x=2;\nif(x>3) std::cout<<"A"; else std::cout<<"B";', 'Condition false, prints B.', 1, 10, 1240, 1, 'if/else branch.', 'https://www.learncpp.com/cpp-tutorial/if-statements-and-blocks/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1240), 'B', 1, 1),
((SELECT id FROM questions WHERE order_index=1240), 'A', 0, 2),
((SELECT id FROM questions WHERE order_index=1240), 'AB', 0, 3),
((SELECT id FROM questions WHERE order_index=1240), 'Compile error', 0, 4);

-- 1241..1250 Random & Loop Patterns Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Random & Loop Patterns Quiz' ORDER BY id DESC LIMIT 1), 4, 'mcq', 'Why avoid rand() in modern C++ educational examples?', NULL, 'Modern <random> facilities provide better quality and control.', 2, 10, 1241, 1, 'Prefer modern random utilities.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-random-number-generation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1241), 'Modern random facilities are better controlled/quality', 1, 1),
((SELECT id FROM questions WHERE order_index=1241), 'rand() does not compile in C++', 0, 2),
((SELECT id FROM questions WHERE order_index=1241), 'rand() is always cryptographically secure', 0, 3),
((SELECT id FROM questions WHERE order_index=1241), 'No headers needed for rand()', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Random & Loop Patterns Quiz' ORDER BY id DESC LIMIT 1), 4, 'true_false', 'A while loop may execute zero times.', NULL, 'True, if condition is initially false.', 1, 10, 1242, 1, 'Pre-condition loop.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-loops-and-while-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1242), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1242), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Random & Loop Patterns Quiz' ORDER BY id DESC LIMIT 1), 4, 'code_output', 'What is printed?', 'int i=0;\ndo { std::cout<<i; ++i; } while(i<1);', 'Body executes once before condition check => prints 0.', 1, 10, 1243, 1, 'Post-condition loop.', 'https://www.learncpp.com/cpp-tutorial/do-while-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1243), '0', 1, 1),
((SELECT id FROM questions WHERE order_index=1243), '1', 0, 2),
((SELECT id FROM questions WHERE order_index=1243), '01', 0, 3),
((SELECT id FROM questions WHERE order_index=1243), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Random & Loop Patterns Quiz' ORDER BY id DESC LIMIT 1), 4, 'mcq', 'Which loop construct is often convenient when iteration count is known?', NULL, 'for loop is idiomatic for counted iteration.', 1, 10, 1244, 1, 'Init/condition/increment in one header.', 'https://www.learncpp.com/cpp-tutorial/for-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1244), 'for loop', 1, 1),
((SELECT id FROM questions WHERE order_index=1244), 'goto loop', 0, 2),
((SELECT id FROM questions WHERE order_index=1244), 'switch loop', 0, 3),
((SELECT id FROM questions WHERE order_index=1244), 'namespace loop', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Random & Loop Patterns Quiz' ORDER BY id DESC LIMIT 1), 4, 'multi_select', 'Select valid control-flow statements.', NULL, 'if, else, switch, break, continue, goto are control-flow constructs/statements.', 2, 15, 1245, 1, 'Execution path control.', 'https://www.studyplan.dev/pro-cpp/conditionals-loops');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1245), 'if', 1, 1),
((SELECT id FROM questions WHERE order_index=1245), 'switch', 1, 2),
((SELECT id FROM questions WHERE order_index=1245), 'continue', 1, 3),
((SELECT id FROM questions WHERE order_index=1245), 'typedef', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Random & Loop Patterns Quiz' ORDER BY id DESC LIMIT 1), 4, 'fill_blank', 'Pseudo-random generation utilities are provided in header <____>.', NULL, 'random.', 2, 10, 1246, 1, 'C++ random header.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-random-number-generation/');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Random & Loop Patterns Quiz' ORDER BY id DESC LIMIT 1), 4, 'mcq', 'What is a common while-loop bug?', NULL, 'Forgetting to update loop variable can cause infinite loop.', 2, 10, 1247, 1, 'Progress toward termination.', 'https://www.studyplan.dev/intro-to-programming/loops');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1247), 'Not updating loop control variable', 1, 1),
((SELECT id FROM questions WHERE order_index=1247), 'Using braces', 0, 2),
((SELECT id FROM questions WHERE order_index=1247), 'Using integer counter', 0, 3),
((SELECT id FROM questions WHERE order_index=1247), 'Using < operator', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Random & Loop Patterns Quiz' ORDER BY id DESC LIMIT 1), 4, 'true_false', 'goto can reduce readability if overused.', NULL, 'True.', 2, 10, 1248, 1, 'Structured control flow preferred.', 'https://www.learncpp.com/cpp-tutorial/goto-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1248), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1248), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Random & Loop Patterns Quiz' ORDER BY id DESC LIMIT 1), 4, 'mcq', 'In if/else chain, how many branches execute per evaluation?', NULL, 'At most one matching branch in a standard if/else-if/else chain.', 1, 10, 1249, 1, 'First true branch wins.', 'https://www.learncpp.com/cpp-tutorial/if-statements-and-blocks/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1249), 'At most one branch', 1, 1),
((SELECT id FROM questions WHERE order_index=1249), 'All branches always', 0, 2),
((SELECT id FROM questions WHERE order_index=1249), 'Exactly two branches', 0, 3),
((SELECT id FROM questions WHERE order_index=1249), 'None even if condition true', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Random & Loop Patterns Quiz' ORDER BY id DESC LIMIT 1), 4, 'code_output', 'What is printed?', 'for(int i=0;i<3;++i){ if(i==1) continue; std::cout<<i; }', 'i=0 prints 0, i=1 skipped, i=2 prints 2 => 02.', 2, 10, 1250, 1, 'continue skips current iteration tail.', 'https://www.learncpp.com/cpp-tutorial/break-and-continue/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1250), '02', 1, 1),
((SELECT id FROM questions WHERE order_index=1250), '012', 0, 2),
((SELECT id FROM questions WHERE order_index=1250), '01', 0, 3),
((SELECT id FROM questions WHERE order_index=1250), 'Compile error', 0, 4);

-- Primary question_tags for 1201..1250
INSERT OR IGNORE INTO question_tags (question_id, tag_id)
SELECT q.id, t.id
FROM questions q
JOIN tags t ON t.name = CASE
    WHEN q.order_index BETWEEN 1201 AND 1210 THEN 'basics'
    WHEN q.order_index BETWEEN 1211 AND 1220 THEN 'variables'
    WHEN q.order_index BETWEEN 1221 AND 1230 THEN 'operators'
    WHEN q.order_index BETWEEN 1231 AND 1240 THEN 'control-flow'
    WHEN q.order_index BETWEEN 1241 AND 1250 THEN 'control-flow'
    ELSE 'best-practices'
END
WHERE q.order_index BETWEEN 1201 AND 1250;

-- Secondary tags
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE order_index=1204), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1206), (SELECT id FROM tags WHERE name='basics')),
((SELECT id FROM questions WHERE order_index=1212), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1215), (SELECT id FROM tags WHERE name='types')),
((SELECT id FROM questions WHERE order_index=1218), (SELECT id FROM tags WHERE name='types')),
((SELECT id FROM questions WHERE order_index=1225), (SELECT id FROM tags WHERE name='types')),
((SELECT id FROM questions WHERE order_index=1234), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1238), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1241), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1246), (SELECT id FROM tags WHERE name='basics'));

-- derive quiz_tags
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.order_index BETWEEN 1201 AND 1250;