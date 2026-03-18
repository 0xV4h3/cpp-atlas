-- =============================================================
-- CppAtlas Content Patch: Foundations Pack 01 (Part 3)
-- Date: 2026-03-16
-- Questions: 50 (order_index 1251..1300)
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
('Foundations Syntax Review Quiz', 'Program structure, declarations, namespaces, and preprocessor basics', 1, 1, 'standard', 1, 0, 1),
('Types & Constants Intensive Quiz', 'Type categories, literals, constants, and safe initialization patterns', 2, 2, 'standard', 1, 0, 1),
('Expression Semantics Quiz', 'Operators, precedence, associativity, comma and comparison behavior', 3, 2, 'standard', 1, 0, 1),
('Conditional Logic Workshop Quiz', 'if/switch branching, common pitfalls and readable conditions', 4, 2, 'standard', 1, 0, 1),
('Looping & Flow Practice Quiz', 'while/do-while/for/goto and iteration control in practical tasks', 4, 2, 'standard', 1, 0, 1);

-- 1251..1260 Foundations Syntax Review Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Foundations Syntax Review Quiz' ORDER BY id DESC LIMIT 1), 1, 'mcq', 'What is the role of the preprocessor in C++ build pipeline?', NULL, 'It processes directives like include/define before compilation.', 1, 10, 1251, 1, 'Runs before compiler proper.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-the-preprocessor/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1251), 'Handles directives before compilation', 1, 1),
((SELECT id FROM questions WHERE order_index=1251), 'Performs runtime optimization', 0, 2),
((SELECT id FROM questions WHERE order_index=1251), 'Links object files', 0, 3),
((SELECT id FROM questions WHERE order_index=1251), 'Executes source code', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Foundations Syntax Review Quiz' ORDER BY id DESC LIMIT 1), 1, 'true_false', 'A declaration tells the compiler about an entity''s name and type.', NULL, 'True.', 1, 10, 1252, 1, 'Declaration vs definition.', 'https://www.learncpp.com/cpp-tutorial/statements-and-the-structure-of-a-program/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1252), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1252), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Foundations Syntax Review Quiz' ORDER BY id DESC LIMIT 1), 1, 'code_output', 'What is printed?', '#include <iostream>\nint main(){ std::cout << "C++"; }', 'Outputs C++.', 1, 10, 1253, 1, 'Literal output.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-iostream-cout-cin-and-endl/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1253), 'C++', 1, 1),
((SELECT id FROM questions WHERE order_index=1253), 'c++', 0, 2),
((SELECT id FROM questions WHERE order_index=1253), 'Nothing', 0, 3),
((SELECT id FROM questions WHERE order_index=1253), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Foundations Syntax Review Quiz' ORDER BY id DESC LIMIT 1), 1, 'mcq', 'Why are header guards useful?', NULL, 'They prevent multiple inclusion of same header in one translation unit.', 2, 10, 1254, 1, 'ODR-related safety.', 'https://www.learncpp.com/cpp-tutorial/header-files/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1254), 'Prevent duplicate header inclusion issues', 1, 1),
((SELECT id FROM questions WHERE order_index=1254), 'Speed up CPU at runtime', 0, 2),
((SELECT id FROM questions WHERE order_index=1254), 'Replace namespaces', 0, 3),
((SELECT id FROM questions WHERE order_index=1254), 'Allocate stack memory', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Foundations Syntax Review Quiz' ORDER BY id DESC LIMIT 1), 1, 'multi_select', 'Select valid reasons to use namespaces.', NULL, 'They reduce collisions and help organize code logically.', 2, 15, 1255, 1, 'Code organization/scoping.', 'https://www.learncpp.com/cpp-tutorial/naming-collisions-and-an-introduction-to-namespaces/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1255), 'Avoid symbol name collisions', 1, 1),
((SELECT id FROM questions WHERE order_index=1255), 'Group related declarations', 1, 2),
((SELECT id FROM questions WHERE order_index=1255), 'Force dynamic linking only', 0, 3),
((SELECT id FROM questions WHERE order_index=1255), 'Improve readability in large codebases', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Foundations Syntax Review Quiz' ORDER BY id DESC LIMIT 1), 1, 'fill_blank', 'Directive used to include declarations from another file is #_____.', NULL, 'include.', 1, 10, 1256, 1, 'Preprocessor include directive.', 'https://www.learncpp.com/cpp-tutorial/header-files/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
VALUES
((SELECT id FROM questions WHERE order_index=1256), 'include', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Foundations Syntax Review Quiz' ORDER BY id DESC LIMIT 1), 1, 'mcq', 'What is one benefit of splitting program into multiple code files?', NULL, 'Improves modularity, maintainability, and build organization.', 1, 10, 1257, 1, 'Separation of concerns.', 'https://www.learncpp.com/cpp-tutorial/programs-with-multiple-code-files/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1257), 'Better modular organization and reuse', 1, 1),
((SELECT id FROM questions WHERE order_index=1257), 'Disables compiler errors', 0, 2),
((SELECT id FROM questions WHERE order_index=1257), 'Removes need for declarations', 0, 3),
((SELECT id FROM questions WHERE order_index=1257), 'Prevents use of headers', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Foundations Syntax Review Quiz' ORDER BY id DESC LIMIT 1), 1, 'true_false', 'Using namespace std; in global scope of headers is generally discouraged.', NULL, 'True.', 2, 10, 1258, 1, 'Pollution of global namespace.', 'https://www.learncpp.com/cpp-tutorial/naming-collisions-and-an-introduction-to-namespaces/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1258), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1258), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Foundations Syntax Review Quiz' ORDER BY id DESC LIMIT 1), 1, 'mcq', 'What does #define typically do?', NULL, 'Defines a preprocessor macro replacement.', 2, 10, 1259, 1, 'Text substitution stage.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-the-preprocessor/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1259), 'Creates macro substitution', 1, 1),
((SELECT id FROM questions WHERE order_index=1259), 'Creates class instance', 0, 2),
((SELECT id FROM questions WHERE order_index=1259), 'Allocates heap memory', 0, 3),
((SELECT id FROM questions WHERE order_index=1259), 'Declares namespace', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Foundations Syntax Review Quiz' ORDER BY id DESC LIMIT 1), 1, 'code_output', 'What is printed?', 'int main(){ int x{2}; { int x{5}; std::cout<<x; } }', 'Inner block variable shadows outer x, prints 5.', 2, 10, 1260, 1, 'Block scope shadowing.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-objects-and-variables/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1260), '5', 1, 1),
((SELECT id FROM questions WHERE order_index=1260), '2', 0, 2),
((SELECT id FROM questions WHERE order_index=1260), '25', 0, 3),
((SELECT id FROM questions WHERE order_index=1260), 'Compile error', 0, 4);

-- 1261..1270 Types & Constants Intensive Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Types & Constants Intensive Quiz' ORDER BY id DESC LIMIT 1), 2, 'mcq', 'What is variable initialization?', NULL, 'Assigning initial value to variable at creation.', 1, 10, 1261, 1, 'Value at declaration time.', 'https://www.learncpp.com/cpp-tutorial/variable-assignment-and-initialization/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1261), 'Giving variable an initial value', 1, 1),
((SELECT id FROM questions WHERE order_index=1261), 'Changing namespace', 0, 2),
((SELECT id FROM questions WHERE order_index=1261), 'Linking object files', 0, 3),
((SELECT id FROM questions WHERE order_index=1261), 'Calling delete', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Types & Constants Intensive Quiz' ORDER BY id DESC LIMIT 1), 2, 'true_false', 'const variable cannot be reassigned after initialization.', NULL, 'True.', 1, 10, 1262, 1, 'Immutability contract.', 'https://www.learncpp.com/cpp-tutorial/constant-variables-named-constants/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1262), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1262), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Types & Constants Intensive Quiz' ORDER BY id DESC LIMIT 1), 2, 'code_output', 'What is printed?', 'const int x{9};\nstd::cout << x;', 'Constant value x is 9.', 1, 10, 1263, 1, 'Const still readable.', 'https://www.learncpp.com/cpp-tutorial/constant-variables-named-constants/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1263), '9', 1, 1),
((SELECT id FROM questions WHERE order_index=1263), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=1263), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE order_index=1263), 'undefined', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Types & Constants Intensive Quiz' ORDER BY id DESC LIMIT 1), 2, 'mcq', 'Which is a floating-point literal?', NULL, 'Literal with decimal point is floating-point (default double).', 1, 10, 1264, 1, 'Decimal notation.', 'https://www.studyplan.dev/intro-to-programming/types-literals');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1264), '2.75', 1, 1),
((SELECT id FROM questions WHERE order_index=1264), '42', 0, 2),
((SELECT id FROM questions WHERE order_index=1264), '''c''', 0, 3),
((SELECT id FROM questions WHERE order_index=1264), '"hello"', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Types & Constants Intensive Quiz' ORDER BY id DESC LIMIT 1), 2, 'multi_select', 'Select valid initialization styles in modern C++.', NULL, 'Copy, direct, and list initialization are all valid forms.', 2, 15, 1265, 1, 'int x = 1; int y(2); int z{3};', 'https://www.learncpp.com/cpp-tutorial/variable-assignment-and-initialization/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1265), 'int a = 1;', 1, 1),
((SELECT id FROM questions WHERE order_index=1265), 'int b(2);', 1, 2),
((SELECT id FROM questions WHERE order_index=1265), 'int c{3};', 1, 3),
((SELECT id FROM questions WHERE order_index=1265), 'int d = ;', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Types & Constants Intensive Quiz' ORDER BY id DESC LIMIT 1), 2, 'fill_blank', 'Named compile-time constant keyword is often _____.', NULL, 'constexpr.', 2, 10, 1266, 1, 'Compile-time const expression.', 'https://www.learncpp.com/cpp-tutorial/constant-variables-named-constants/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
VALUES
((SELECT id FROM questions WHERE order_index=1266), 'constexpr', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Types & Constants Intensive Quiz' ORDER BY id DESC LIMIT 1), 3, 'mcq', 'What is an object in C++ basics context?', NULL, 'Region of storage with a type and associated value/lifetime.', 2, 10, 1267, 1, 'Storage + type notion.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-objects-and-variables/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1267), 'Typed region of storage', 1, 1),
((SELECT id FROM questions WHERE order_index=1267), 'Preprocessor macro only', 0, 2),
((SELECT id FROM questions WHERE order_index=1267), 'Header include guard', 0, 3),
((SELECT id FROM questions WHERE order_index=1267), 'Namespace block', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Types & Constants Intensive Quiz' ORDER BY id DESC LIMIT 1), 2, 'true_false', 'Uninitialized local variables may contain indeterminate values.', NULL, 'True.', 2, 10, 1268, 1, 'Initialize before use.', 'https://www.learncpp.com/cpp-tutorial/variable-assignment-and-initialization/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1268), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1268), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Types & Constants Intensive Quiz' ORDER BY id DESC LIMIT 1), 2, 'mcq', 'Why prefer descriptive variable names?', NULL, 'Improves readability, maintainability, and intent clarity.', 1, 10, 1269, 1, 'Naming is design communication.', 'https://www.studyplan.dev/pro-cpp/variables-and-types');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1269), 'Better readability and intent communication', 1, 1),
((SELECT id FROM questions WHERE order_index=1269), 'Forces faster machine code always', 0, 2),
((SELECT id FROM questions WHERE order_index=1269), 'Removes need for comments entirely', 0, 3),
((SELECT id FROM questions WHERE order_index=1269), 'Disables warnings', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Types & Constants Intensive Quiz' ORDER BY id DESC LIMIT 1), 2, 'code_output', 'What is printed?', 'int value{4};\nvalue = value + 6;\nstd::cout << value;', 'Assignment updates value to 10.', 1, 10, 1270, 1, 'Update then print.', 'https://www.learncpp.com/cpp-tutorial/variable-assignment-and-initialization/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1270), '10', 1, 1),
((SELECT id FROM questions WHERE order_index=1270), '46', 0, 2),
((SELECT id FROM questions WHERE order_index=1270), '4', 0, 3),
((SELECT id FROM questions WHERE order_index=1270), 'Compile error', 0, 4);

-- 1271..1280 Expression Semantics Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Expression Semantics Quiz' ORDER BY id DESC LIMIT 1), 3, 'mcq', 'What is operator associativity used for?', NULL, 'Determines grouping direction for operators of same precedence.', 2, 10, 1271, 1, 'Same precedence tie-breaker.', 'https://www.learncpp.com/cpp-tutorial/operator-precedence-and-associativity/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1271), 'Grouping direction for same-precedence operators', 1, 1),
((SELECT id FROM questions WHERE order_index=1271), 'Variable declaration scope', 0, 2),
((SELECT id FROM questions WHERE order_index=1271), 'Namespace import behavior', 0, 3),
((SELECT id FROM questions WHERE order_index=1271), 'Preprocessor include order', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Expression Semantics Quiz' ORDER BY id DESC LIMIT 1), 3, 'true_false', 'In expression a + b * c, multiplication is evaluated before addition by precedence.', NULL, 'True.', 1, 10, 1272, 1, 'Arithmetic precedence.', 'https://www.learncpp.com/cpp-tutorial/operator-precedence-and-associativity/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1272), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1272), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Expression Semantics Quiz' ORDER BY id DESC LIMIT 1), 3, 'code_output', 'What is printed?', 'int x=3;\nint y=4;\nstd::cout << (x<y);', '3<4 is true => bool prints 1 by default.', 1, 10, 1273, 1, 'Relational result is bool.', 'https://www.learncpp.com/cpp-tutorial/relational-operators-and-floating-point-comparisons/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1273), '1', 1, 1),
((SELECT id FROM questions WHERE order_index=1273), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=1273), 'true (text always)', 0, 3),
((SELECT id FROM questions WHERE order_index=1273), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Expression Semantics Quiz' ORDER BY id DESC LIMIT 1), 3, 'mcq', 'What is remainder of 19 % 4?', NULL, '19 = 4*4 + 3.', 1, 10, 1274, 1, 'Remainder arithmetic.', 'https://www.learncpp.com/cpp-tutorial/remainder-and-exponentiation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1274), '3', 1, 1),
((SELECT id FROM questions WHERE order_index=1274), '4', 0, 2),
((SELECT id FROM questions WHERE order_index=1274), '2', 0, 3),
((SELECT id FROM questions WHERE order_index=1274), '1', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Expression Semantics Quiz' ORDER BY id DESC LIMIT 1), 3, 'multi_select', 'Select true statements about comma operator.', NULL, 'Comma operator evaluates left then right; result is right operand value.', 3, 15, 1275, 1, 'Rare but valid operator.', 'https://www.learncpp.com/cpp-tutorial/the-comma-operator/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1275), 'Evaluates left operand first', 1, 1),
((SELECT id FROM questions WHERE order_index=1275), 'Result value is right operand', 1, 2),
((SELECT id FROM questions WHERE order_index=1275), 'Same as argument separator in all contexts', 0, 3),
((SELECT id FROM questions WHERE order_index=1275), 'Can appear in expressions', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Expression Semantics Quiz' ORDER BY id DESC LIMIT 1), 3, 'fill_blank', 'Comparison operators like < and > return type ____ in C++.', NULL, 'bool.', 1, 10, 1276, 1, 'Boolean result.', 'https://www.learncpp.com/cpp-tutorial/relational-operators-and-floating-point-comparisons/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
VALUES
((SELECT id FROM questions WHERE order_index=1276), 'bool', 1),
((SELECT id FROM questions WHERE order_index=1276), 'boolean', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Expression Semantics Quiz' ORDER BY id DESC LIMIT 1), 3, 'mcq', 'What is a safer approach for floating-point equality checks in many practical cases?', NULL, 'Compare with tolerance (epsilon) instead of exact equality in many numeric contexts.', 3, 12, 1277, 1, 'Precision-aware comparisons.', 'https://www.learncpp.com/cpp-tutorial/relational-operators-and-floating-point-comparisons/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1277), 'Use tolerance-based comparison', 1, 1),
((SELECT id FROM questions WHERE order_index=1277), 'Always use exact == for floats', 0, 2),
((SELECT id FROM questions WHERE order_index=1277), 'Cast to int first always', 0, 3),
((SELECT id FROM questions WHERE order_index=1277), 'Avoid floats entirely in C++', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Expression Semantics Quiz' ORDER BY id DESC LIMIT 1), 3, 'true_false', 'Parentheses can be used to make expression grouping explicit regardless of precedence.', NULL, 'True.', 1, 10, 1278, 1, 'Readability and correctness aid.', 'https://www.learncpp.com/cpp-tutorial/operator-precedence-and-associativity/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1278), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1278), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Expression Semantics Quiz' ORDER BY id DESC LIMIT 1), 3, 'mcq', 'Expression (a=b) assigns b to a and yields what value?', NULL, 'Assignment expression yields assigned value (with type of lhs).', 3, 12, 1279, 1, 'Assignment is expression too.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-literals-and-operators/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1279), 'The assigned value', 1, 1),
((SELECT id FROM questions WHERE order_index=1279), 'Always 0', 0, 2),
((SELECT id FROM questions WHERE order_index=1279), 'Always 1', 0, 3),
((SELECT id FROM questions WHERE order_index=1279), 'No value (void)', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Expression Semantics Quiz' ORDER BY id DESC LIMIT 1), 3, 'code_output', 'What is printed?', 'int a=5,b=2;\nstd::cout << a/b;', 'Integer division truncates toward zero => 2.', 1, 10, 1280, 1, 'Integer arithmetic semantics.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-literals-and-operators/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1280), '2', 1, 1),
((SELECT id FROM questions WHERE order_index=1280), '2.5', 0, 2),
((SELECT id FROM questions WHERE order_index=1280), '3', 0, 3),
((SELECT id FROM questions WHERE order_index=1280), 'Compile error', 0, 4);

-- 1281..1290 Conditional Logic Workshop Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Conditional Logic Workshop Quiz' ORDER BY id DESC LIMIT 1), 4, 'mcq', 'Which branch executes if if-condition is false and else exists?', NULL, 'else branch executes.', 1, 10, 1281, 1, 'Basic branch logic.', 'https://www.learncpp.com/cpp-tutorial/if-statements-and-blocks/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1281), 'else branch', 1, 1),
((SELECT id FROM questions WHERE order_index=1281), 'if branch', 0, 2),
((SELECT id FROM questions WHERE order_index=1281), 'Both branches', 0, 3),
((SELECT id FROM questions WHERE order_index=1281), 'No branch ever', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Conditional Logic Workshop Quiz' ORDER BY id DESC LIMIT 1), 4, 'true_false', 'An assignment inside if-condition may be intentional but can also be bug-prone.', NULL, 'True.', 2, 10, 1282, 1, 'Watch for = vs == confusion.', 'https://www.learncpp.com/cpp-tutorial/common-if-statement-problems/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1282), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1282), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Conditional Logic Workshop Quiz' ORDER BY id DESC LIMIT 1), 4, 'code_output', 'What is printed?', 'int n=0;\nif(n) std::cout<<"T"; else std::cout<<"F";', 'Zero converts to false, so else prints F.', 1, 10, 1283, 1, 'Boolean conversion of integers.', 'https://www.learncpp.com/cpp-tutorial/if-statements-and-blocks/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1283), 'F', 1, 1),
((SELECT id FROM questions WHERE order_index=1283), 'T', 0, 2),
((SELECT id FROM questions WHERE order_index=1283), 'TF', 0, 3),
((SELECT id FROM questions WHERE order_index=1283), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Conditional Logic Workshop Quiz' ORDER BY id DESC LIMIT 1), 4, 'mcq', 'What is key benefit of braces in if/else blocks?', NULL, 'They avoid ambiguity and improve maintainability when adding statements.', 1, 10, 1284, 1, 'Safer multi-statement blocks.', 'https://www.learncpp.com/cpp-tutorial/if-statements-and-blocks/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1284), 'Clear block scope and safer edits', 1, 1),
((SELECT id FROM questions WHERE order_index=1284), 'Required only for single statement', 0, 2),
((SELECT id FROM questions WHERE order_index=1284), 'Disable warnings', 0, 3),
((SELECT id FROM questions WHERE order_index=1284), 'Increase integer range', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Conditional Logic Workshop Quiz' ORDER BY id DESC LIMIT 1), 4, 'multi_select', 'Select common if-statement mistakes.', NULL, 'Using = instead of ==, dangling else confusion, missing braces in complex flows are common pitfalls.', 2, 15, 1285, 1, 'Classic pitfalls.', 'https://www.learncpp.com/cpp-tutorial/common-if-statement-problems/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1285), 'Using = accidentally in condition', 1, 1),
((SELECT id FROM questions WHERE order_index=1285), 'Misreading nested else association', 1, 2),
((SELECT id FROM questions WHERE order_index=1285), 'Using explicit braces', 0, 3),
((SELECT id FROM questions WHERE order_index=1285), 'Not testing edge conditions', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Conditional Logic Workshop Quiz' ORDER BY id DESC LIMIT 1), 4, 'fill_blank', 'Keyword that starts alternative branch after if is ____.', NULL, 'else.', 1, 10, 1286, 1, 'if ... else.', 'https://www.learncpp.com/cpp-tutorial/if-statements-and-blocks/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
VALUES
((SELECT id FROM questions WHERE order_index=1286), 'else', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Conditional Logic Workshop Quiz' ORDER BY id DESC LIMIT 1), 4, 'mcq', 'switch is most suitable when branching on what kind of cases?', NULL, 'Discrete known constant cases of integral/enumeration expression.', 2, 10, 1287, 1, 'Case labels are constants.', 'https://www.learncpp.com/cpp-tutorial/switch-statement-basics/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1287), 'Discrete constant integral/enumeration cases', 1, 1),
((SELECT id FROM questions WHERE order_index=1287), 'Arbitrary floating expressions only', 0, 2),
((SELECT id FROM questions WHERE order_index=1287), 'String pattern matching by default', 0, 3),
((SELECT id FROM questions WHERE order_index=1287), 'Dynamic runtime case labels', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Conditional Logic Workshop Quiz' ORDER BY id DESC LIMIT 1), 4, 'true_false', 'Without break, switch may fall through to next case.', NULL, 'True.', 2, 10, 1288, 1, 'Fallthrough semantics.', 'https://www.learncpp.com/cpp-tutorial/switch-statement-basics/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1288), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1288), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Conditional Logic Workshop Quiz' ORDER BY id DESC LIMIT 1), 4, 'mcq', 'goto is usually discouraged mainly because it can harm what?', NULL, 'Readability and maintainability via tangled control flow.', 2, 10, 1289, 1, 'Structured flow preferred.', 'https://www.learncpp.com/cpp-tutorial/goto-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1289), 'Readability and maintainability', 1, 1),
((SELECT id FROM questions WHERE order_index=1289), 'Compilation speed only', 0, 2),
((SELECT id FROM questions WHERE order_index=1289), 'Integer precision', 0, 3),
((SELECT id FROM questions WHERE order_index=1289), 'Namespace support', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Conditional Logic Workshop Quiz' ORDER BY id DESC LIMIT 1), 4, 'code_output', 'What is printed?', 'int x=3;\nif(x>0){ if(x>2) std::cout<<"A"; else std::cout<<"B"; }', 'x>0 and x>2 true => prints A.', 2, 10, 1290, 1, 'Nested if flow.', 'https://www.learncpp.com/cpp-tutorial/if-statements-and-blocks/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1290), 'A', 1, 1),
((SELECT id FROM questions WHERE order_index=1290), 'B', 0, 2),
((SELECT id FROM questions WHERE order_index=1290), 'AB', 0, 3),
((SELECT id FROM questions WHERE order_index=1290), 'Compile error', 0, 4);

-- 1291..1300 Looping & Flow Practice Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Looping & Flow Practice Quiz' ORDER BY id DESC LIMIT 1), 4, 'mcq', 'Which loop naturally places init/condition/increment together?', NULL, 'for loop.', 1, 10, 1291, 1, 'Counted iteration style.', 'https://www.learncpp.com/cpp-tutorial/for-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1291), 'for loop', 1, 1),
((SELECT id FROM questions WHERE order_index=1291), 'while only', 0, 2),
((SELECT id FROM questions WHERE order_index=1291), 'do-while only', 0, 3),
((SELECT id FROM questions WHERE order_index=1291), 'goto', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Looping & Flow Practice Quiz' ORDER BY id DESC LIMIT 1), 4, 'true_false', 'continue in a for-loop still performs increment step before next condition check.', NULL, 'True.', 2, 10, 1292, 1, 'Flow goes to loop increment.', 'https://www.learncpp.com/cpp-tutorial/break-and-continue/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1292), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1292), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Looping & Flow Practice Quiz' ORDER BY id DESC LIMIT 1), 4, 'code_output', 'What is printed?', 'for(int i=0;i<4;++i){ if(i==2) break; std::cout<<i; }', 'Prints 0 and 1 then breaks => 01.', 2, 10, 1293, 1, 'break exits loop.', 'https://www.learncpp.com/cpp-tutorial/break-and-continue/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1293), '01', 1, 1),
((SELECT id FROM questions WHERE order_index=1293), '0123', 0, 2),
((SELECT id FROM questions WHERE order_index=1293), '012', 0, 3),
((SELECT id FROM questions WHERE order_index=1293), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Looping & Flow Practice Quiz' ORDER BY id DESC LIMIT 1), 4, 'mcq', 'Main distinction between while and do-while?', NULL, 'while checks before body; do-while checks after body.', 1, 10, 1294, 1, 'Pre-check vs post-check.', 'https://www.learncpp.com/cpp-tutorial/do-while-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1294), 'do-while executes body before condition check', 1, 1),
((SELECT id FROM questions WHERE order_index=1294), 'while always executes at least once', 0, 2),
((SELECT id FROM questions WHERE order_index=1294), 'They are identical always', 0, 3),
((SELECT id FROM questions WHERE order_index=1294), 'for cannot be converted to while', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Looping & Flow Practice Quiz' ORDER BY id DESC LIMIT 1), 4, 'multi_select', 'Select loop termination best practices.', NULL, 'Use clear conditions, ensure progression, and test boundary cases.', 2, 15, 1295, 1, 'Avoid infinite loops.', 'https://www.studyplan.dev/pro-cpp/conditionals-loops');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1295), 'Ensure loop variable changes toward termination', 1, 1),
((SELECT id FROM questions WHERE order_index=1295), 'Use clear and testable conditions', 1, 2),
((SELECT id FROM questions WHERE order_index=1295), 'Depend on hidden side-effects only', 0, 3),
((SELECT id FROM questions WHERE order_index=1295), 'Verify edge bounds', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Looping & Flow Practice Quiz' ORDER BY id DESC LIMIT 1), 4, 'fill_blank', 'Loop statement that may execute zero times due to precondition is ____.', NULL, 'while.', 1, 10, 1296, 1, 'Condition checked first.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-loops-and-while-statements/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
VALUES
((SELECT id FROM questions WHERE order_index=1296), 'while', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Looping & Flow Practice Quiz' ORDER BY id DESC LIMIT 1), 4, 'mcq', 'What often makes goto especially problematic in larger code?', NULL, 'Non-local jumps make control flow hard to trace/debug.', 2, 10, 1297, 1, 'Spaghetti flow risk.', 'https://www.learncpp.com/cpp-tutorial/goto-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1297), 'Hard-to-follow non-local control flow', 1, 1),
((SELECT id FROM questions WHERE order_index=1297), 'Compilation impossible', 0, 2),
((SELECT id FROM questions WHERE order_index=1297), 'No labels allowed', 0, 3),
((SELECT id FROM questions WHERE order_index=1297), 'For-loops stop working', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Looping & Flow Practice Quiz' ORDER BY id DESC LIMIT 1), 4, 'true_false', 'for(;;) creates an infinite loop unless broken/returned/etc.', NULL, 'True.', 2, 10, 1298, 1, 'No termination condition.', 'https://www.learncpp.com/cpp-tutorial/for-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1298), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1298), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Looping & Flow Practice Quiz' ORDER BY id DESC LIMIT 1), 4, 'mcq', 'What is reliable way to control pseudo-random generation reproducibility in examples?', NULL, 'Use deterministic seeding strategy/known seed for reproducible sequence.', 3, 12, 1299, 1, 'Deterministic seed for teaching/testing.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-random-number-generation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1299), 'Use controlled deterministic seed', 1, 1),
((SELECT id FROM questions WHERE order_index=1299), 'Use uninitialized memory as seed', 0, 2),
((SELECT id FROM questions WHERE order_index=1299), 'Never seed generator', 0, 3),
((SELECT id FROM questions WHERE order_index=1299), 'Randomly rewrite source code', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Looping & Flow Practice Quiz' ORDER BY id DESC LIMIT 1), 4, 'code_output', 'What is printed?', 'int i=3;\nwhile(i>0){ std::cout<<i; --i; }', 'Prints 3 then 2 then 1 => 321.', 1, 10, 1300, 1, 'Countdown loop.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-loops-and-while-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1300), '321', 1, 1),
((SELECT id FROM questions WHERE order_index=1300), '123', 0, 2),
((SELECT id FROM questions WHERE order_index=1300), '32', 0, 3),
((SELECT id FROM questions WHERE order_index=1300), 'Compile error', 0, 4);

-- Primary tags
INSERT OR IGNORE INTO question_tags (question_id, tag_id)
SELECT q.id, t.id
FROM questions q
JOIN tags t ON t.name = CASE
    WHEN q.order_index BETWEEN 1251 AND 1260 THEN 'basics'
    WHEN q.order_index BETWEEN 1261 AND 1270 THEN 'variables'
    WHEN q.order_index BETWEEN 1271 AND 1280 THEN 'operators'
    WHEN q.order_index BETWEEN 1281 AND 1290 THEN 'control-flow'
    WHEN q.order_index BETWEEN 1291 AND 1300 THEN 'control-flow'
    ELSE 'best-practices'
END
WHERE q.order_index BETWEEN 1251 AND 1300;

-- Secondary tags
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE order_index=1255), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1262), (SELECT id FROM tags WHERE name='types')),
((SELECT id FROM questions WHERE order_index=1264), (SELECT id FROM tags WHERE name='types')),
((SELECT id FROM questions WHERE order_index=1268), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1277), (SELECT id FROM tags WHERE name='types')),
((SELECT id FROM questions WHERE order_index=1285), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1289), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1295), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1299), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1275), (SELECT id FROM tags WHERE name='basics'));

-- Derive quiz_tags
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.order_index BETWEEN 1251 AND 1300;