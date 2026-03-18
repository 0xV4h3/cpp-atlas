-- =============================================================
-- CppAtlas Content Patch: OOP & Templates Pack 03 (Part 4)
-- Date: 2026-03-16
-- Questions: 50 (order_index 451..500)
-- Topics: oop, classes, inheritance, templates, best-practices
--
-- IMPORTANT:
-- - No BEGIN/COMMIT (ContentPatchService handles transaction).
-- - Standardized INSERT INTO questions (12 columns).
-- - SQLite-safe escaping.
-- =============================================================

INSERT OR IGNORE INTO tags (name, color) VALUES
('oop', '#C586C0'),
('classes', '#C586C0'),
('inheritance', '#C586C0'),
('templates', '#BD93F9'),
('best-practices', '#89D185');

INSERT OR IGNORE INTO quizzes (title, description, topic_id, difficulty, type, is_active, created_by, version) VALUES
('Constructor & Encapsulation Mastery Quiz', 'Constructors, access control, invariants, and robust class APIs', 8, 3, 'standard', 1, 0, 1),
('Inheritance Architecture Quiz', 'Hierarchy design, substitution, access, and multiple inheritance reasoning', 8, 4, 'standard', 1, 0, 1),
('Template API Design Quiz', 'Generic constraints, specialization strategy, and maintainable template APIs', 9, 4, 'standard', 1, 0, 1);

-- -------------------------------------------------------------
-- Existing Quiz 8: OOP Quiz (10 questions, 451..460)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'mcq', 'What is the core purpose of class constructors?', NULL, 'Constructors establish valid initial object state.', 1, 10, 451, 1, 'Initialization responsibility.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-constructors/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=451), 'Initialize object state', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=451), 'Destroy object memory', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=451), 'Instantiate templates at runtime', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=451), 'Replace all setters/getters', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'true_false', 'Encapsulation can reduce accidental misuse of class internals.', NULL, 'True. It narrows mutation paths and preserves invariants.', 1, 10, 452, 1, 'Controlled access.', 'https://www.learncpp.com/cpp-tutorial/the-benefits-of-data-hiding-encapsulation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=452), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=452), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'code_output', 'What is printed?', 'class C{public: C(){std::cout<<"C";}}; int main(){ C a; C b; }', 'Constructor runs per object creation.', 1, 10, 453, 1, 'Two objects.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-classes/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=453), 'CC', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=453), 'C', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=453), 'CCC', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=453), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'mcq', 'What is the main risk of exposing mutable public fields broadly?', NULL, 'It weakens invariants and couples callers to internal representation.', 2, 10, 454, 1, 'API stability concern.', 'https://www.studyplan.dev/intro-to-programming/restricting-access');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=454), 'Breaking invariants and tight coupling', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=454), 'Faster virtual dispatch', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=454), 'Improved specialization automatically', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=454), 'No effect on design', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'multi_select', 'Select valid uses of member initializer lists.', NULL, 'Init-lists are required for const/reference members and useful for direct initialization.', 2, 15, 455, 1, 'Init before body.', 'https://www.studyplan.dev/intro-to-programming/member-initializer-lists');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=455), 'Initialize const member', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=455), 'Initialize reference member', 1, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=455), 'Rebind reference later in body', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=455), 'Avoid extra default-init + assignment in some cases', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'fill_blank', 'A class member declared with keyword static is shared per ____.', NULL, 'Static data members are shared per class.', 1, 10, 456, 1, 'Not per instance.', 'https://www.learncpp.com/cpp-tutorial/static-member-variables/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=456), 'class', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'mcq', 'Friendship in C++ primarily grants what?', NULL, 'Access rights to private/protected members for specified friend entities.', 2, 10, 457, 1, 'Access privilege only.', 'https://www.learncpp.com/cpp-tutorial/friend-classes-and-friend-member-functions/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=457), 'Access to private/protected members', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=457), 'Automatic inheritance', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=457), 'Template instantiation', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=457), 'Automatic polymorphism', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'true_false', 'A constructor may delegate to another constructor in the same class.', NULL, 'True.', 2, 10, 458, 1, 'Delegating constructors.', 'https://www.learncpp.com/cpp-tutorial/delegating-constructors/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=458), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=458), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'mcq', 'What does "has-a" usually suggest in modeling?', NULL, 'Typically composition or aggregation depending on ownership/lifetime.', 2, 10, 459, 1, 'Relation semantics.', 'https://www.learncpp.com/cpp-tutorial/object-relationships/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=459), 'Composition or aggregation', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=459), 'Inheritance only', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=459), 'Friendship only', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=459), 'No relation type', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'code_output', 'What is printed?', 'class T{public: T(){std::cout<<"1";} ~T(){std::cout<<"2";}}; int main(){ T t; }', 'Construction then destruction in scope: 12.', 2, 10, 460, 1, 'Lifetime order.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-destructors/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=460), '12', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=460), '21', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=460), '11', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=460), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- Existing Quiz 9: Templates Quiz (10 questions, 461..470)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'mcq', 'What is template specialization useful for?', NULL, 'Custom behavior for particular type/value combinations.', 2, 10, 461, 1, 'Specific-case tuning.', 'https://www.learncpp.com/cpp-tutorial/template-classes/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=461), 'Custom handling for specific template arguments', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=461), 'Replacing inheritance hierarchy always', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=461), 'Disabling compile checks', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=461), 'Removing need for headers', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'true_false', 'Class templates can have static data members parameterized by template arguments.', NULL, 'True.', 3, 12, 462, 1, 'Per-instantiation static members.', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=462), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=462), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'code_output', 'What is printed?', 'template<typename T> struct P{ static constexpr int k=5; }; int main(){ std::cout<<P<int>::k; }', 'P<int>::k = 5.', 2, 10, 463, 1, 'Template static constexpr.', 'https://www.studyplan.dev/pro-cpp/variable-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=463), '5', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=463), '0', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=463), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=463), 'P', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'mcq', 'What does "partial specialization for pointers" target?', NULL, 'Template patterns where argument itself is pointer type (e.g., T*).', 4, 15, 464, 1, 'Pointer pattern matching.', 'https://www.learncpp.com/cpp-tutorial/partial-template-specialization-for-pointers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=464), 'Pointer-shaped template argument patterns', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=464), 'Only non-type integer parameters', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=464), 'Virtual function dispatch', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=464), 'Macro text substitution', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'multi_select', 'Select maintainable template coding habits.', NULL, 'Clear naming, constraints, and examples improve maintainability.', 3, 15, 465, 1, 'Developer ergonomics.', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=465), 'Use clear names for template params', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=465), 'Document constraints/assumptions', 1, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=465), 'Rely on cryptic implicit behavior', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=465), 'Keep examples close to API docs', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'fill_blank', 'Template definitions are often placed in ____ files so instantiation can see them.', NULL, 'Headers.', 2, 10, 466, 1, 'Definition visibility.', 'https://www.studyplan.dev/pro-cpp/header-file-templates');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=466), 'header', 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=466), 'headers', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'mcq', 'A non-type template parameter must generally be what?', NULL, 'A compile-time constant expression of allowed kind.', 3, 12, 467, 1, 'Compile-time value.', 'https://www.learncpp.com/cpp-tutorial/template-non-type-parameters/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=467), 'Compile-time constant', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=467), 'Runtime mutable variable only', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=467), 'Heap pointer only', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=467), 'Preprocessor token only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'true_false', 'Function templates participate in overload resolution.', NULL, 'True.', 3, 12, 468, 1, 'Template + non-template overload sets.', 'https://www.learncpp.com/cpp-tutorial/function-template-specialization/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=468), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=468), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'code_output', 'What is printed?', 'template<typename T> T f(T v){return v;} int main(){ std::cout<<f<double>(3.5); }', 'Explicit double instantiation prints 3.5 (format-dependent).', 2, 10, 469, 1, 'Explicit template argument.', 'https://www.studyplan.dev/pro-cpp/function-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=469), '3.5', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=469), '3', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=469), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=469), '0.0', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'mcq', 'Educationally, template progression is best when lessons move from ____.', NULL, 'Concrete examples to generic abstraction is easier for learners.', 2, 10, 470, 1, 'Scaffold learning.', 'https://www.learncpp.com/cpp-tutorial/template-classes/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=470), 'Concrete examples to generic forms', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=470), 'Most complex metaprogramming first', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=470), 'No examples at all', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=470), 'Only specialization edge cases', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: Constructor & Encapsulation Mastery Quiz (10 questions, 471..480)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'What is a common constructor best practice?', NULL, 'Initialize all required members into valid state.', 2, 10, 471, 1, 'Invariant at construction.', 'https://www.studyplan.dev/intro-to-programming/constructors');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=471), 'Ensure object starts in valid state', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=471), 'Leave members uninitialized for speed', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=471), 'Expose all state publicly', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=471), 'Avoid constructor parameters forever', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false', 'Encapsulation and clear method contracts can simplify debugging.', NULL, 'True.', 2, 10, 472, 1, 'Narrow mutation paths.', 'https://www.learncpp.com/cpp-tutorial/the-benefits-of-data-hiding-encapsulation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=472), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=472), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output', 'What is printed?', 'class A{int x; public: A():x(7){std::cout<<x;}}; int main(){A a;}', 'x is initialized to 7 in initializer list.', 2, 10, 473, 1, 'Initializer list value.', 'https://www.learncpp.com/cpp-tutorial/constructor-member-initializer-lists/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=473), '7', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=473), '0', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=473), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=473), 'Undefined', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'Why avoid exposing mutable internals directly?', NULL, 'It prevents external code from violating invariants unexpectedly.', 2, 10, 474, 1, 'Protect state integrity.', 'https://www.studyplan.dev/intro-to-programming/restricting-access');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=474), 'To preserve invariants and reduce coupling', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=474), 'To improve template deduction', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=474), 'To disable constructors', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=474), 'To force inheritance', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'multi_select', 'Select useful encapsulation techniques.', NULL, 'Private state, validated mutators, and const-correct query methods are common techniques.', 3, 15, 475, 1, 'API design toolkit.', 'https://www.learncpp.com/cpp-tutorial/the-benefits-of-data-hiding-encapsulation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=475), 'Keep sensitive state private', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=475), 'Validate inputs in mutators', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=475), 'Expose raw internals for convenience', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=475), 'Provide const query methods when possible', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'fill_blank', 'A constructor with no parameters (or all defaulted) is a ____ constructor.', NULL, 'Default constructor.', 1, 10, 476, 1, 'No-arg construction.', 'https://www.learncpp.com/cpp-tutorial/default-constructors-and-default-arguments/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=476), 'default', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'What is an advantage of constructor delegation?', NULL, 'Reduces duplication and centralizes initialization logic.', 2, 10, 477, 1, 'DRY constructors.', 'https://www.learncpp.com/cpp-tutorial/delegating-constructors/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=477), 'Reduces duplicated initialization code', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=477), 'Automatically makes all members public', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=477), 'Disables copy constructor', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=477), 'Forces inheritance', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false', 'A friend declaration should be used sparingly to avoid overexposing internals.', NULL, 'True.', 3, 12, 478, 1, 'Friendship as exception, not default.', 'https://www.learncpp.com/cpp-tutorial/friend-non-member-functions/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=478), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=478), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'Which class API style is generally preferable for maintainable code?', NULL, 'Small, coherent, documented public API with controlled mutators.', 2, 10, 479, 1, 'Surface area and coherence.', 'https://www.studyplan.dev/intro-to-programming/classes');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=479), 'Minimal coherent public API', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=479), 'All members public by default', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=479), 'No invariants documented', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=479), 'Only static globals', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output', 'What is printed?', 'class Q{public: Q(){std::cout<<"Q";}}; int main(){ Q q1; Q q2; Q q3; }', 'Three objects => constructor runs three times.', 1, 10, 480, 1, 'Count constructions.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-classes/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=480), 'QQQ', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=480), 'QQ', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=480), 'Q', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Constructor & Encapsulation Mastery Quiz' ORDER BY id DESC LIMIT 1) AND order_index=480), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: Inheritance Architecture Quiz (10 questions, 481..490)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'What is key criterion before introducing inheritance?', NULL, 'There should be a true is-a substitutable relationship.', 3, 12, 481, 1, 'Substitution test.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-inheritance/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=481), 'Derived must be substitutable for base', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=481), 'Need fewer files only', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=481), 'Prefer inheritance for any reuse', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=481), 'No base interface needed', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false', 'Private base members are directly accessible in derived classes.', NULL, 'False.', 2, 10, 482, 1, 'Private remains inaccessible.', 'https://www.learncpp.com/cpp-tutorial/inheritance-and-access-specifiers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=482), 'True', 0, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=482), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output', 'What is printed?', 'struct B{B(){std::cout<<"B";}}; struct D:B{D(){std::cout<<"D";}}; int main(){D d;}', 'Construction order: B then D.', 2, 10, 483, 1, 'Base before derived.', 'https://www.learncpp.com/cpp-tutorial/order-of-construction-of-derived-classes/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=483), 'BD', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=483), 'DB', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=483), 'B', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=483), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'What does protected inheritance do to inherited public members in external interface?', NULL, 'They become protected in derived external interface.', 4, 15, 484, 1, 'Access transformation.', 'https://www.learncpp.com/cpp-tutorial/inheritance-and-access-specifiers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=484), 'They are exposed as protected', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=484), 'They remain public always', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=484), 'They become private base internals only by language rule', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=484), 'They become static', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1), 8, 'multi_select', 'Select architecture-level inheritance concerns.', NULL, 'Fragile base class, over-coupling, and ambiguous MI can hurt maintainability.', 4, 15, 485, 1, 'Design-level tradeoffs.', 'https://www.learncpp.com/cpp-tutorial/multiple-inheritance/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=485), 'Fragile base assumptions', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=485), 'Ambiguity in multiple inheritance', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=485), 'Clear contracts reduce risk', 1, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=485), 'Inheritance always better than composition', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1), 8, 'fill_blank', 'A function marked = 0 is called ____ virtual function.', NULL, 'Pure virtual.', 2, 10, 486, 1, 'Abstract interface marker.', 'https://www.studyplan.dev/pro-cpp/pure-virtual');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=486), 'pure', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'Which base class feature is important for safe polymorphic deletion?', NULL, 'Virtual destructor.', 3, 12, 487, 1, 'Delete via base pointer safety.', 'https://www.studyplan.dev/pro-cpp/pure-virtual');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=487), 'Virtual destructor', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=487), 'Only static members', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=487), 'Deleted copy constructor', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=487), 'Protected typedef', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false', 'Overriding requires exact signature compatibility (modulo covariant return cases).', NULL, 'True.', 4, 15, 488, 1, 'Signature matters.', 'https://www.studyplan.dev/pro-cpp/pure-virtual');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=488), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=488), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'When inheritance does not reflect true is-a relation, prefer ____.', NULL, 'Prefer composition/aggregation.', 3, 12, 489, 1, 'Modeling correctness.', 'https://www.learncpp.com/cpp-tutorial/object-relationships/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=489), 'Composition or aggregation', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=489), 'More inheritance levels', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=489), 'Friend classes', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=489), 'Global state', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output', 'What is printed?', 'struct B{virtual void g(){std::cout<<"B";}}; struct D:B{void g() override {std::cout<<"D";}}; int main(){ B* p=new D; p->g(); }', 'Virtual dispatch prints D.', 3, 12, 490, 1, 'Base ptr to derived object.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-inheritance/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=490), 'D', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=490), 'B', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=490), 'BD', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Inheritance Architecture Quiz' ORDER BY id DESC LIMIT 1) AND order_index=490), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: Template API Design Quiz (10 questions, 491..500)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq', 'Which approach improves template API readability?', NULL, 'Clear parameter names and concise concepts/constraints.', 2, 10, 491, 1, 'Human-readable generics.', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=491), 'Clear names and documented constraints', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=491), 'Single-letter names for everything', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=491), 'Hidden requirements in comments only', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=491), 'No examples', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1), 9, 'true_false', 'Template aliases can reduce repetition in nested generic types.', NULL, 'True.', 2, 10, 492, 1, 'using improves readability.', 'https://www.studyplan.dev/pro-cpp/type-alias');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=492), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=492), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1), 9, 'code_output', 'What is printed?', 'template<typename T> T plus1(T v){ return v+1; }\nstd::cout<<plus1(8);', 'plus1(8) => 9.', 2, 10, 493, 1, 'Simple deduction.', 'https://www.studyplan.dev/pro-cpp/function-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=493), '9', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=493), '8', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=493), '81', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=493), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq', 'Why might class template partial specialization be preferred over many if constexpr branches in some cases?', NULL, 'It can provide cleaner type-partitioned implementations and clearer diagnostics for distinct forms.', 4, 15, 494, 1, 'Type-form partitioning.', 'https://www.learncpp.com/cpp-tutorial/partial-template-specialization/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=494), 'Cleaner type-pattern specific implementations', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=494), 'Because if constexpr is forbidden in templates', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=494), 'It reduces source code to zero lines', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=494), 'It makes code runtime-only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1), 9, 'multi_select', 'Select practical template API stability tactics.', NULL, 'Stable naming, backward-compatible wrappers, and tests across representative types help API stability.', 3, 15, 495, 1, 'Versioning + compatibility.', 'https://www.learncpp.com/cpp-tutorial/dependencies/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=495), 'Maintain stable public names/signatures where possible', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=495), 'Test with multiple representative types', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=495), 'Break API every patch by default', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=495), 'Document specialization behavior', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1), 9, 'fill_blank', 'Template code is generally instantiated at ____ time.', NULL, 'Compile time.', 1, 10, 496, 1, 'Not runtime.', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=496), 'compile', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq', 'Which choice best improves learner experience for generic code?', NULL, 'Good examples, clear errors, and progressive abstraction help learning.', 2, 10, 497, 1, 'Teaching-oriented API.', 'https://www.learncpp.com/cpp-tutorial/template-classes/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=497), 'Progressive examples with clear constraints', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=497), 'Start only from deepest specialization internals', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=497), 'Hide all diagnostics details', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=497), 'Avoid showing concrete non-template versions', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1), 9, 'true_false', 'Member function templates can coexist with non-template overloads in a class.', NULL, 'True.', 3, 12, 498, 1, 'Overload sets can mix.', 'https://www.studyplan.dev/pro-cpp/member-function-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=498), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=498), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1), 9, 'code_output', 'What is printed?', 'template<typename T> struct W{ T v; }; int main(){ W<int> w{42}; std::cout<<w.v; }', 'Concrete instance stores value 42.', 2, 10, 499, 1, 'Template struct instance.', 'https://www.studyplan.dev/pro-cpp/class-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=499), '42', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=499), '0', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=499), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=499), 'W', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq', 'Best-practice in educational template modules is to optimize for ____.', NULL, 'Comprehension and correctness before cleverness.', 2, 10, 500, 1, 'Educational objective.', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=500), 'Clarity and correctness', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=500), 'Maximum cleverness at all costs', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=500), 'Undocumented behavior', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template API Design Quiz' ORDER BY id DESC LIMIT 1) AND order_index=500), 'No tests', 0, 4);

-- -------------------------------------------------------------
-- Primary question_tags mapping
-- -------------------------------------------------------------
INSERT OR IGNORE INTO question_tags (question_id, tag_id)
SELECT q.id, (SELECT id FROM tags WHERE name = CASE
    WHEN q.order_index BETWEEN 451 AND 460 THEN 'oop'
    WHEN q.order_index BETWEEN 461 AND 470 THEN 'templates'
    WHEN q.order_index BETWEEN 471 AND 480 THEN 'classes'
    WHEN q.order_index BETWEEN 481 AND 490 THEN 'inheritance'
    WHEN q.order_index BETWEEN 491 AND 500 THEN 'templates'
    ELSE 'best-practices'
END)
FROM questions q
WHERE q.order_index BETWEEN 451 AND 500;

-- Secondary tags for balance
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE order_index=455), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=459), (SELECT id FROM tags WHERE name='classes')),
((SELECT id FROM questions WHERE order_index=470), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=475), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=479), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=485), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=489), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=495), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=500), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=487), (SELECT id FROM tags WHERE name='classes'));

-- Derive quiz_tags for affected quizzes
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.order_index BETWEEN 451 AND 500;