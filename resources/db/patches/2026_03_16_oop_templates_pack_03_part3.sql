-- =============================================================
-- CppAtlas Content Patch: OOP & Templates Pack 03 (Part 3)
-- Date: 2026-03-16
-- Questions: 50 (order_index 401..450)
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
('Composition & Relationships Quiz', 'Composition/aggregation/association and class collaboration patterns', 8, 2, 'standard', 1, 0, 1),
('Advanced Inheritance Cases Quiz', 'Construction order, access transformations, multiple inheritance caveats', 8, 4, 'standard', 1, 0, 1),
('Template Specialization Lab Quiz', 'Function/class specialization, partial specialization, and generic APIs', 9, 4, 'standard', 1, 0, 1);

-- -------------------------------------------------------------
-- Existing Quiz 8: OOP Quiz (10 questions, 401..410)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'mcq', 'Which statement best describes composition?', NULL, 'Composition is a strong has-a relationship where owner controls contained object lifetime.', 2, 10, 401, 1, 'Strong ownership.', 'https://www.learncpp.com/cpp-tutorial/composition/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=401), 'Strong has-a with owned lifetime', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=401), 'is-a inheritance relationship', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=401), 'Friendship-only relation', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=401), 'Template parameter relation', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'true_false', 'Association usually implies strict ownership and forced shared lifetime.', NULL, 'False. Association is typically non-owning and lifetimes can be independent.', 2, 10, 402, 1, 'Compare with composition.', 'https://www.learncpp.com/cpp-tutorial/association/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=402), 'True', 0, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=402), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'code_output', 'What is printed?', 'class A{public: A(){std::cout<<"A";}}; class B{A a; public: B(){std::cout<<"B";}}; int main(){B b;}', 'Member subobjects construct before owner body => AB.', 3, 12, 403, 1, 'Member construction order.', 'https://www.learncpp.com/cpp-tutorial/composition/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=403), 'AB', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=403), 'BA', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=403), 'A', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=403), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'mcq', 'What does aggregation typically model?', NULL, 'A has-a relationship where part may outlive whole and ownership is weaker.', 2, 10, 404, 1, 'Independent lifetime possible.', 'https://www.learncpp.com/cpp-tutorial/aggregation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=404), 'Weak has-a with potentially independent lifetime', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=404), 'Guaranteed same lifetime always', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=404), 'Pure virtual requirement', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=404), 'Static inheritance', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'multi_select', 'Select valid OOP design best practices.', NULL, 'Cohesion, clear interfaces, and encapsulation support maintainable design.', 2, 15, 405, 1, 'API and invariants.', 'https://www.studyplan.dev/intro-to-programming/classes');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=405), 'Keep class responsibilities focused', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=405), 'Expose all internals as public', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=405), 'Protect invariants through methods', 1, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=405), 'Use meaningful method names', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'fill_blank', 'A function allowed to access private members without being class member may be declared as ____.', NULL, 'That is a friend function.', 2, 10, 406, 1, 'Access grant keyword.', 'https://www.learncpp.com/cpp-tutorial/friend-non-member-functions/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=406), 'friend', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'mcq', 'Which statement about static member functions is true?', NULL, 'They do not have this pointer and can access only static members directly.', 3, 12, 407, 1, 'No implicit object context.', 'https://www.learncpp.com/cpp-tutorial/static-member-variables/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=407), 'They have no this pointer', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=407), 'They can directly access non-static members without object', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=407), 'They cannot be public', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=407), 'They require virtual keyword', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'true_false', 'Class invariants should be preserved after every public method call (unless explicitly documented otherwise).', NULL, 'True. This supports correctness and predictability.', 3, 12, 408, 1, 'Design-by-contract mindset.', 'https://www.learncpp.com/cpp-tutorial/the-benefits-of-data-hiding-encapsulation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=408), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=408), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'mcq', 'Which relationship best fits "University has Departments that may exist independently in DB model"?', NULL, 'Aggregation is often used when parts can exist independently.', 3, 12, 409, 1, 'Independent lifecycle clue.', 'https://www.learncpp.com/cpp-tutorial/aggregation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=409), 'Aggregation', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=409), 'Composition', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=409), 'Friendship', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=409), 'Template specialization', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(8, 8, 'code_output', 'What is printed?', 'class N{public: N(){std::cout<<"N";}}; int main(){ N n1; N n2; }', 'Constructor runs for each object -> NN.', 1, 10, 410, 1, 'Two objects.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-classes/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=410), 'NN', 1, 1),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=410), 'N', 0, 2),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=410), 'N2', 0, 3),
((SELECT id FROM questions WHERE quiz_id=8 AND order_index=410), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- Existing Quiz 9: Templates Quiz (10 questions, 411..420)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'mcq', 'What is class template instantiation?', NULL, 'Creation of a concrete class type from template with specific arguments.', 2, 10, 411, 1, 'Template + arguments => concrete type.', 'https://www.learncpp.com/cpp-tutorial/template-classes/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=411), 'Creating a concrete type from template arguments', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=411), 'Runtime reflection process', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=411), 'Declaring virtual function', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=411), 'Preprocessor include expansion', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'true_false', 'A class template specialization may completely replace behavior for one exact type.', NULL, 'True, that is full specialization.', 3, 12, 412, 1, 'Exact match override.', 'https://www.learncpp.com/cpp-tutorial/class-template-specialization/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=412), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=412), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'code_output', 'What is printed?', 'template<typename T> T twice(T x){ return x+x; }\nstd::cout<<twice(5);', 'twice(5) with int gives 10.', 2, 10, 413, 1, 'Simple function template.', 'https://www.studyplan.dev/pro-cpp/function-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=413), '10', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=413), '25', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=413), '5', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=413), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'mcq', 'Which statement about variable templates is correct?', NULL, 'They define variable families parameterized by template arguments.', 3, 12, 414, 1, 'Template variables.', 'https://www.studyplan.dev/pro-cpp/variable-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=414), 'They create parameterized variable declarations', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=414), 'They are runtime dynamic variables only', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=414), 'They cannot be constexpr', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=414), 'They replace class templates entirely', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'multi_select', 'Select valid template specialization statements.', NULL, 'Full and partial specialization apply to class templates; function templates use overloading plus explicit specialization.', 4, 15, 415, 1, 'Know class vs function distinctions.', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=415), 'Class templates support partial specialization', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=415), 'Function templates can be explicitly specialized', 1, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=415), 'Function templates support partial specialization syntax', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=415), 'Specialization choice is compile-time', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'fill_blank', 'Template code clarity often improves with type ____ declarations (using).', NULL, 'Type aliases via using improve readability.', 2, 10, 416, 1, 'using Name = ...;', 'https://www.studyplan.dev/pro-cpp/type-alias');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=416), 'alias', 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=416), 'aliases', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'mcq', 'What is a key risk of template-heavy APIs for newcomers?', NULL, 'Verbose diagnostics and complex type chains can harm learnability.', 3, 12, 417, 1, 'Educational ergonomics.', 'https://www.learncpp.com/cpp-tutorial/dependencies/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=417), 'Hard-to-read error diagnostics and complexity', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=417), 'No compile-time checking', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=417), 'No code reuse', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=417), 'No inlining possible', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'true_false', 'Template definitions hidden only in .cpp may cause linker/instantiation issues in common workflows.', NULL, 'True for many ordinary template usage patterns.', 3, 12, 418, 1, 'Definition visibility.', 'https://www.studyplan.dev/pro-cpp/header-file-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=418), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=418), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'code_output', 'What is printed?', 'template<typename T,int N> struct A{ static constexpr int v=N; }; int main(){ std::cout<<A<int,3>::v; }', 'Non-type template parameter N is 3.', 3, 12, 419, 1, 'Type + non-type parameter.', 'https://www.learncpp.com/cpp-tutorial/template-non-type-parameters/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=419), '3', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=419), '0', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=419), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=419), 'int', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(9, 9, 'mcq', 'Best-practice for template API evolution in a teaching project?', NULL, 'Add complexity incrementally and preserve stable simple usage paths.', 2, 10, 420, 1, 'Layer complexity progressively.', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=420), 'Introduce advanced features incrementally', 1, 1),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=420), 'Break public template API every release', 0, 2),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=420), 'Prefer undocumented implicit behavior', 0, 3),
((SELECT id FROM questions WHERE quiz_id=9 AND order_index=420), 'Remove beginner examples', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: Composition & Relationships Quiz (10 questions, 421..430)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'Which relationship is strongest in terms of ownership?', NULL, 'Composition is strongest ownership relation.', 2, 10, 421, 1, 'Owner controls member lifetime.', 'https://www.learncpp.com/cpp-tutorial/composition/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=421), 'Composition', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=421), 'Association', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=421), 'Friend class', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=421), 'Type alias', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false', 'Association often models a "uses/knows about" relationship without ownership.', NULL, 'True.', 2, 10, 422, 1, 'Non-owning relation.', 'https://www.learncpp.com/cpp-tutorial/association/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=422), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=422), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output', 'What is printed?', 'struct Engine{Engine(){std::cout<<"E";}}; struct Car{Engine e; Car(){std::cout<<"C";}}; int main(){Car c;}', 'Engine member constructs before Car body -> EC.', 2, 10, 423, 1, 'Member construction order.', 'https://www.learncpp.com/cpp-tutorial/composition/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=423), 'EC', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=423), 'CE', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=423), 'E', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=423), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'Aggregation differs from composition because aggregated part ____.', NULL, 'can exist independently from whole object.', 2, 10, 424, 1, 'Lifecycle clue.', 'https://www.learncpp.com/cpp-tutorial/aggregation/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=424), 'can outlive the whole', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=424), 'must always be private member', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=424), 'must be static', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=424), 'cannot be referenced', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1), 8, 'multi_select', 'Select reasonable criteria for choosing composition over inheritance.', NULL, 'Prefer composition when relation is has-a and behavior should be assembled, not specialized by subtype.', 3, 15, 425, 1, 'Has-a vs is-a test.', 'https://www.learncpp.com/cpp-tutorial/object-relationships/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=425), 'Relationship is has-a, not is-a', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=425), 'Need to reuse behavior by embedding components', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=425), 'Want deep fragile inheritance hierarchy', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=425), 'Need loose coupling between parts', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1), 8, 'fill_blank', 'A relationship where objects cooperate but neither owns the other is often called ____.', NULL, 'Association.', 2, 10, 426, 1, 'Non-owning relation.', 'https://www.learncpp.com/cpp-tutorial/association/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=426), 'association', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'Which class design helps testability most?', NULL, 'Small cohesive classes with injected collaborators are easier to test.', 3, 12, 427, 1, 'Isolation and seams.', 'https://www.studyplan.dev/intro-to-programming/classes');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=427), 'Small cohesive classes with clear boundaries', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=427), 'Single giant god class', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=427), 'All methods static globally', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=427), 'No encapsulation', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false', 'Object relationships directly impact maintainability and coupling in large systems.', NULL, 'True.', 2, 10, 428, 1, 'Architecture consequence.', 'https://www.learncpp.com/cpp-tutorial/object-relationships/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=428), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=428), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'What is a practical anti-pattern in class relationships?', NULL, 'Circular dependencies and over-coupling hinder evolvability.', 3, 12, 429, 1, 'Dependency direction matters.', 'https://www.learncpp.com/cpp-tutorial/dependencies/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=429), 'Tight cyclic dependencies between classes', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=429), 'Clear ownership and interfaces', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=429), 'Encapsulation with stable API', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=429), 'Documented invariants', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output', 'What is printed?', 'struct L{L(){std::cout<<"L";}}; struct O{L l; O(){std::cout<<"O";}}; int main(){ O o; }', 'Member object constructs first => LO.', 2, 10, 430, 1, 'Construction sequence.', 'https://www.learncpp.com/cpp-tutorial/composition/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=430), 'LO', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=430), 'OL', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=430), 'L', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Composition & Relationships Quiz' ORDER BY id DESC LIMIT 1) AND order_index=430), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: Advanced Inheritance Cases Quiz (10 questions, 431..440)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'In derived construction, initialization order is determined by ____.', NULL, 'Base classes first (declaration order), then members (declaration order), then body.', 4, 15, 431, 1, 'Not by textual order in init list.', 'https://www.learncpp.com/cpp-tutorial/order-of-construction-of-derived-classes/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=431), 'Class declaration/member declaration order rules', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=431), 'Only constructor body statement order', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=431), 'Linker order', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=431), 'Random compiler choice', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false', 'Public inheritance is generally appropriate when derived is substitutable for base.', NULL, 'True (LSP perspective).', 3, 12, 432, 1, 'Substitutability principle.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-inheritance/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=432), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=432), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output', 'What is printed?', 'struct B{ B(){std::cout<<"B";} }; struct D:B{ D(){std::cout<<"D";} }; int main(){ D d; }', 'Base then derived: BD.', 2, 10, 433, 1, 'Ctor chain.', 'https://www.learncpp.com/cpp-tutorial/constructors-and-initialization-of-derived-classes/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=433), 'BD', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=433), 'DB', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=433), 'B', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=433), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'What challenge is common with multiple inheritance?', NULL, 'Ambiguity/diamond complexity in member resolution and base layout.', 4, 15, 434, 1, 'Diamond problem concept.', 'https://www.learncpp.com/cpp-tutorial/multiple-inheritance/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=434), 'Ambiguity and diamond-related complexity', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=434), 'No constructor execution', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=434), 'Template argument deduction failure only', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=434), 'Destructors cannot exist', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1), 8, 'multi_select', 'Select robust inheritance practices.', NULL, 'Prefer clear virtual interfaces, avoid fragile base assumptions, and keep hierarchies shallow when possible.', 4, 15, 435, 1, 'Maintainability of hierarchies.', 'https://www.learncpp.com/cpp-tutorial/inheritance-and-access-specifiers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=435), 'Use virtual destructor in polymorphic bases', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=435), 'Keep base contracts explicit', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=435), 'Rely on undocumented override behavior', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=435), 'Prefer composition if relation is not truly is-a', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1), 8, 'fill_blank', 'Access specifier in inheritance declaration that preserves base public API visibility is ____ inheritance.', NULL, 'Public inheritance.', 2, 10, 436, 1, 'Most common OO subtype form.', 'https://www.learncpp.com/cpp-tutorial/inheritance-and-access-specifiers/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=436), 'public', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'What does protected member allow?', NULL, 'Accessible in class and derived classes (subject to context).', 2, 10, 437, 1, 'Between private and public.', 'https://www.studyplan.dev/intro-to-programming/protected-members');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=437), 'Access from class and derived classes', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=437), 'Access from any free function', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=437), 'No access for derived classes', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=437), 'Only compile-time constants can be protected', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false', 'If base class method is not virtual, overriding in derived gives runtime polymorphism through base pointer.', NULL, 'False. Without virtual, static binding through base type occurs.', 3, 12, 438, 1, 'virtual keyword is key.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-inheritance/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=438), 'True', 0, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=438), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq', 'Which is a safer guideline for base classes used polymorphically?', NULL, 'Design stable interface and ensure proper virtual destructor semantics.', 3, 12, 439, 1, 'Lifetime + interface contract.', 'https://www.studyplan.dev/pro-cpp/pure-virtual');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=439), 'Provide virtual destructor and clear contract', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=439), 'Avoid documenting override expectations', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=439), 'Expose private internals as public', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=439), 'Prefer deep inheritance by default', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output', 'What is printed?', 'struct B{virtual void f(){std::cout<<"B";}}; struct D:B{void f() override{std::cout<<"D";}}; int main(){ D d; B& b=d; b.f(); }', 'Virtual dispatch via reference calls D::f.', 3, 12, 440, 1, 'Reference polymorphism.', 'https://www.learncpp.com/cpp-tutorial/basic-inheritance-in-c/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=440), 'D', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=440), 'B', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=440), 'BD', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Advanced Inheritance Cases Quiz' ORDER BY id DESC LIMIT 1) AND order_index=440), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: Template Specialization Lab Quiz (10 questions, 441..450)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq', 'What is explicit specialization?', NULL, 'Providing dedicated implementation for a specific template argument set.', 3, 12, 441, 1, 'Exact case override.', 'https://www.learncpp.com/cpp-tutorial/function-template-specialization/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=441), 'Custom implementation for exact template args', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=441), 'Generic fallback implementation only', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=441), 'Runtime branch selection', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=441), 'Linker aliasing trick', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1), 9, 'true_false', 'Partial specialization can match pointer forms such as T* patterns in class templates.', NULL, 'True.', 4, 15, 442, 1, 'Pattern matching on args.', 'https://www.learncpp.com/cpp-tutorial/partial-template-specialization-for-pointers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=442), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=442), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1), 9, 'code_output', 'What is printed?', 'template<typename T> T id(T x){return x;} int main(){ std::cout<<id(9); }', 'Template deduces int; prints 9.', 2, 10, 443, 1, 'Deduction example.', 'https://www.studyplan.dev/pro-cpp/function-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=443), '9', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=443), '0', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=443), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=443), 'id', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq', 'Why can templates improve code reuse?', NULL, 'One generic implementation can serve multiple types with compile-time type checking.', 2, 10, 444, 1, 'Generic + type-safe.', 'https://www.studyplan.dev/pro-cpp/class-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=444), 'Generic logic reused for many types', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=444), 'They remove compile-time checks', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=444), 'They force runtime casts', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=444), 'They only work for ints', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1), 9, 'multi_select', 'Select template API quality factors.', NULL, 'Readable diagnostics, clear constraints, and predictable behavior are key.', 3, 15, 445, 1, 'Developer experience.', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=445), 'Clear constraints and assumptions', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=445), 'Readable compiler diagnostics where possible', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=445), 'Undocumented specialization precedence', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=445), 'Consistent naming and examples', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1), 9, 'fill_blank', 'When compiler infers T from arguments in template call, this is called template argument ____.', NULL, 'Deduction.', 2, 10, 446, 1, 'Inference process.', 'https://www.studyplan.dev/pro-cpp/function-templates');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=446), 'deduction', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq', 'Which file organization is most common for template definitions?', NULL, 'Headers are common so definitions are visible at instantiation sites.', 2, 10, 447, 1, 'Visibility requirement.', 'https://www.studyplan.dev/pro-cpp/header-file-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=447), 'Header-visible definitions', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=447), 'Only opaque binary blobs', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=447), 'Only linker scripts', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=447), 'Only generated JSON', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1), 9, 'true_false', 'Template code and inheritance hierarchies should both favor clarity over cleverness in educational platforms.', NULL, 'True.', 2, 10, 448, 1, 'Pedagogy-first.', 'https://www.learncpp.com/cpp-tutorial/template-classes/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=448), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=448), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1), 9, 'code_output', 'What is printed?', 'template<typename T> struct B{ T x; }; int main(){ B<int> b{7}; std::cout<<b.x; }', 'Instantiated class stores int 7.', 2, 10, 449, 1, 'Class template instance data.', 'https://www.studyplan.dev/pro-cpp/class-templates');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=449), '7', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=449), '0', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=449), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=449), 'B', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq', 'A practical "best-practices" goal for templates in this project is to ____.', NULL, 'Provide understandable, well-scaffolded generic examples and avoid unnecessary complexity.', 2, 10, 450, 1, 'Education + maintainability.', 'https://cppreference.com/w/cpp/language/templates.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=450), 'Favor clarity and progressive complexity', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=450), 'Maximize inscrutable metaprogramming by default', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=450), 'Avoid documenting assumptions', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Template Specialization Lab Quiz' ORDER BY id DESC LIMIT 1) AND order_index=450), 'Rely on undefined behavior examples', 0, 4);

-- -------------------------------------------------------------
-- Primary question_tags mapping
-- -------------------------------------------------------------
INSERT OR IGNORE INTO question_tags (question_id, tag_id)
SELECT q.id, (SELECT id FROM tags WHERE name = CASE
    WHEN q.order_index BETWEEN 401 AND 410 THEN 'oop'
    WHEN q.order_index BETWEEN 411 AND 420 THEN 'templates'
    WHEN q.order_index BETWEEN 421 AND 430 THEN 'classes'
    WHEN q.order_index BETWEEN 431 AND 440 THEN 'inheritance'
    WHEN q.order_index BETWEEN 441 AND 450 THEN 'templates'
    ELSE 'best-practices'
END)
FROM questions q
WHERE q.order_index BETWEEN 401 AND 450;

-- Secondary tags for balance
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE order_index=405), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=409), (SELECT id FROM tags WHERE name='classes')),
((SELECT id FROM questions WHERE order_index=420), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=425), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=429), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=435), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=439), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=445), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=450), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=437), (SELECT id FROM tags WHERE name='classes'));

-- Derive quiz_tags for affected quizzes
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.order_index BETWEEN 401 AND 450;