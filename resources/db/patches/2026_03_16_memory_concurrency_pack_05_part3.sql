-- =============================================================
-- CppAtlas Content Patch: Memory & Concurrency Pack 05 (Part 3)
-- Date: 2026-03-16
-- Questions: 50 (order_index 1006..1055)
--
-- Rules:
-- - No BEGIN/COMMIT (managed by ContentPatchService)
-- - Standardized INSERT INTO questions (12 columns)
-- - SQLite-safe quoting
-- - Only NEW quizzes in this part
-- =============================================================

INSERT OR IGNORE INTO tags (name, color) VALUES
('memory', '#F14C4C'),
('smart-pointers', '#F14C4C'),
('concurrency', '#8BE9FD'),
('best-practices', '#89D185');

-- New quizzes for part3
INSERT OR IGNORE INTO quizzes (title, description, topic_id, difficulty, type, is_active, created_by, version) VALUES
('Concurrent Memory Safety Quiz', 'Ownership, lifetime, and synchronization interactions in multithreaded code', 13, 4, 'standard', 1, 0, 1),
('Futures Promises Async Mastery Quiz', 'Future/promise/async contracts and safe asynchronous result handling', 13, 4, 'standard', 1, 0, 1),
('Advanced Mutex Patterns Quiz', 'Timed/shared/recursive mutex scenarios and lock strategy tradeoffs', 13, 4, 'standard', 1, 0, 1),
('Atomic Correctness Workshop Quiz', 'Atomic operations, atomic_ref, and data-race-free design patterns', 13, 5, 'standard', 1, 0, 1),
('Resource Ownership in Systems Quiz', 'RAII, copy/move ownership transfer, and architecture-level best practices', 11, 4, 'standard', 1, 0, 1);

-- -------------------------------------------------------------
-- A) Concurrent Memory Safety Quiz (1006..1015)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Concurrent Memory Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Why is shared mutable state dangerous across threads without synchronization?',
 NULL,
 'It can produce data races and undefined behavior.',
 3, 12, 1006, 1,
 'Race condition root cause.',
 'https://cppreference.com/w/cpp/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1006), 'It can cause data races/undefined behavior', 1, 1),
((SELECT id FROM questions WHERE order_index=1006), 'It improves determinism by default', 0, 2),
((SELECT id FROM questions WHERE order_index=1006), 'It automatically locks memory', 0, 3),
((SELECT id FROM questions WHERE order_index=1006), 'It disables thread interleaving', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Concurrent Memory Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'Using shared_ptr alone always makes object internals thread-safe.',
 NULL,
 'False. shared_ptr manages ownership, not internal data synchronization.',
 3, 12, 1007, 1,
 'Ownership vs synchronization.',
 'https://www.learncpp.com/cpp-tutorial/stdshared_ptr/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1007), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=1007), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Concurrent Memory Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::shared_ptr<int> p = std::make_shared<int>(5);\nstd::shared_ptr<int> q = p;\nstd::cout << p.use_count();',
 'Two shared owners exist, so use_count is 2.',
 2, 10, 1008, 1,
 'Reference count check.',
 'https://www.studyplan.dev/pro-cpp/shared-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1008), '2', 1, 1),
((SELECT id FROM questions WHERE order_index=1008), '1', 0, 2),
((SELECT id FROM questions WHERE order_index=1008), '0', 0, 3),
((SELECT id FROM questions WHERE order_index=1008), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Concurrent Memory Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Which is best for non-owning observation of shared object in graph-like structures?',
 NULL,
 'std::weak_ptr helps avoid ownership cycles.',
 3, 12, 1009, 1,
 'Observer pointer role.',
 'https://www.studyplan.dev/pro-cpp/weak-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1009), 'std::weak_ptr', 1, 1),
((SELECT id FROM questions WHERE order_index=1009), 'std::unique_ptr', 0, 2),
((SELECT id FROM questions WHERE order_index=1009), 'std::thread', 0, 3),
((SELECT id FROM questions WHERE order_index=1009), 'std::lock_guard', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Concurrent Memory Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'multi_select',
 'Select good concurrent memory safety habits.',
 NULL,
 'Clear ownership, minimal sharing, synchronization, and RAII are core habits.',
 4, 15, 1010, 1,
 'Safety-first architecture.',
 'https://www.studyplan.dev/pro-cpp/smart-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1010), 'Minimize shared mutable state', 1, 1),
((SELECT id FROM questions WHERE order_index=1010), 'Use RAII wrappers for resources', 1, 2),
((SELECT id FROM questions WHERE order_index=1010), 'Rely on timing instead of synchronization', 0, 3),
((SELECT id FROM questions WHERE order_index=1010), 'Document ownership/lifetime contracts', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Concurrent Memory Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'fill_blank',
 'A race-free increment of shared integer can use std::atomic<___>.',
 NULL,
 'int.',
 2, 10, 1011, 1,
 'Atomic scalar type.',
 'https://cppreference.com/w/cpp/atomic/atomic.html');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Concurrent Memory Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Which issue appears when two shared_ptr objects own each other directly?',
 NULL,
 'Circular ownership keeps refcount >0, causing leak.',
 3, 12, 1012, 1,
 'Cycle leak diagnosis.',
 'https://www.learncpp.com/cpp-tutorial/circular-dependency-issues-with-stdshared_ptr-and-stdweak_ptr/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1012), 'Reference cycle memory leak', 1, 1),
((SELECT id FROM questions WHERE order_index=1012), 'Automatic deadlock resolution', 0, 2),
((SELECT id FROM questions WHERE order_index=1012), 'Compile-time ownership error always', 0, 3),
((SELECT id FROM questions WHERE order_index=1012), 'Forced stack allocation', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Concurrent Memory Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'Atomic operations remove need to reason about object lifetime.',
 NULL,
 'False. Atomics synchronize access, not object lifetime ownership.',
 4, 15, 1013, 1,
 'Synchronization != lifetime mgmt.',
 'https://cppreference.com/w/cpp/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1013), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=1013), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Concurrent Memory Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'In concurrent design, which API style reduces misuse most?',
 NULL,
 'Strong types with explicit ownership (value, unique_ptr, shared_ptr, ref) reduce ambiguity.',
 3, 12, 1014, 1,
 'Encode ownership in interfaces.',
 'https://www.studyplan.dev/pro-cpp/smart-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1014), 'Explicit ownership in type signatures', 1, 1),
((SELECT id FROM questions WHERE order_index=1014), 'Raw pointers everywhere', 0, 2),
((SELECT id FROM questions WHERE order_index=1014), 'Hidden implicit ownership rules', 0, 3),
((SELECT id FROM questions WHERE order_index=1014), 'No lifetime documentation', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Concurrent Memory Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::atomic<int> n{0};\n++n; ++n; ++n;\nstd::cout << n.load();',
 'Three increments produce 3.',
 2, 10, 1015, 1,
 'Atomic counter.',
 'https://cppreference.com/w/cpp/atomic/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1015), '3', 1, 1),
((SELECT id FROM questions WHERE order_index=1015), '2', 0, 2),
((SELECT id FROM questions WHERE order_index=1015), '1', 0, 3),
((SELECT id FROM questions WHERE order_index=1015), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- B) Futures Promises Async Mastery Quiz (1016..1025)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Futures Promises Async Mastery Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What links std::promise and std::future?',
 NULL,
 'They share asynchronous result state: promise sets, future gets.',
 2, 10, 1016, 1,
 'Producer/consumer pair.',
 'https://cppreference.com/w/cpp/thread/promise.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1016), 'Shared state for result/exception transfer', 1, 1),
((SELECT id FROM questions WHERE order_index=1016), 'Mutex lock ownership', 0, 2),
((SELECT id FROM questions WHERE order_index=1016), 'Atomic compare-exchange loop', 0, 3),
((SELECT id FROM questions WHERE order_index=1016), 'Container iteration', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Futures Promises Async Mastery Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'future::get can be called repeatedly to retrieve the same stored value multiple times.',
 NULL,
 'False for std::future; get() consumes value (shared_future supports repeated gets).',
 4, 15, 1017, 1,
 'Single-consumption future.',
 'https://cppreference.com/w/cpp/thread/future.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1017), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=1017), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Futures Promises Async Mastery Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'auto fut = std::async(std::launch::deferred, []{ return 12; });\nstd::cout << fut.get();',
 'Deferred callable runs when get() invoked, returning 12.',
 3, 12, 1018, 1,
 'Deferred launch behavior.',
 'https://cppreference.com/w/cpp/thread/async.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1018), '12', 1, 1),
((SELECT id FROM questions WHERE order_index=1018), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=1018), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE order_index=1018), 'undefined', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Futures Promises Async Mastery Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What does future::wait_for provide?',
 NULL,
 'Timeout-based waiting for readiness status.',
 3, 12, 1019, 1,
 'Time-bounded wait.',
 'https://cppreference.com/w/cpp/thread/future.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1019), 'Wait with duration timeout semantics', 1, 1),
((SELECT id FROM questions WHERE order_index=1019), 'Immediate forced cancellation', 0, 2),
((SELECT id FROM questions WHERE order_index=1019), 'Thread creation', 0, 3),
((SELECT id FROM questions WHERE order_index=1019), 'Mutex recursive lock', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Futures Promises Async Mastery Quiz' ORDER BY id DESC LIMIT 1), 13, 'multi_select',
 'Select valid async error-handling statements.',
 NULL,
 'Exceptions from async tasks can propagate through future::get; promise can set_exception.',
 4, 15, 1020, 1,
 'Result + exception channels.',
 'https://cppreference.com/w/cpp/thread/future.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1020), 'future::get may rethrow task exception', 1, 1),
((SELECT id FROM questions WHERE order_index=1020), 'promise can set_exception', 1, 2),
((SELECT id FROM questions WHERE order_index=1020), 'async exceptions are always ignored', 0, 3),
((SELECT id FROM questions WHERE order_index=1020), 'Structured error flow should be documented', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Futures Promises Async Mastery Quiz' ORDER BY id DESC LIMIT 1), 13, 'fill_blank',
 'Future operation to retrieve result value is ____().',
 NULL,
 'get',
 1, 10, 1021, 1,
 'Consume result.',
 'https://cppreference.com/w/cpp/thread/future.html');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Futures Promises Async Mastery Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Which launch policy may start task on separate thread immediately?',
 NULL,
 'std::launch::async.',
 2, 10, 1022, 1,
 'Eager async execution.',
 'https://cppreference.com/w/cpp/thread/async.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1022), 'std::launch::async', 1, 1),
((SELECT id FROM questions WHERE order_index=1022), 'std::launch::deferred only', 0, 2),
((SELECT id FROM questions WHERE order_index=1022), 'std::defer_lock', 0, 3),
((SELECT id FROM questions WHERE order_index=1022), 'std::adopt_lock', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Futures Promises Async Mastery Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'promise::set_value can be called multiple times on same promise successfully.',
 NULL,
 'False; setting already-satisfied promise state is error.',
 3, 12, 1023, 1,
 'Single fulfillment rule.',
 'https://cppreference.com/w/cpp/thread/promise.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1023), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=1023), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Futures Promises Async Mastery Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Best practice for async API consumers?',
 NULL,
 'Explicitly handle readiness, timeout, and exceptions; avoid indefinite blocking without policy.',
 3, 12, 1024, 1,
 'Robust async consumption.',
 'https://cppreference.com/w/cpp/thread/future.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1024), 'Handle timeout/readiness/exception paths explicitly', 1, 1),
((SELECT id FROM questions WHERE order_index=1024), 'Assume immediate completion always', 0, 2),
((SELECT id FROM questions WHERE order_index=1024), 'Ignore get() exceptions', 0, 3),
((SELECT id FROM questions WHERE order_index=1024), 'Never use wait_for', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Futures Promises Async Mastery Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::promise<int> p; auto f = p.get_future(); p.set_value(6); std::cout << f.get();',
 'Value set through promise read via future => 6.',
 2, 10, 1025, 1,
 'Promise to future path.',
 'https://cppreference.com/w/cpp/thread/promise.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1025), '6', 1, 1),
((SELECT id FROM questions WHERE order_index=1025), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=1025), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE order_index=1025), 'undefined', 0, 4);

-- -------------------------------------------------------------
-- C) Advanced Mutex Patterns Quiz (1026..1035)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Mutex Patterns Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Which mutex supports both recursion and timed lock attempts?',
 NULL,
 'recursive_timed_mutex.',
 4, 15, 1026, 1,
 'Recursive + timeout combo.',
 'https://cppreference.com/w/cpp/thread/recursive_timed_mutex.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1026), 'std::recursive_timed_mutex', 1, 1),
((SELECT id FROM questions WHERE order_index=1026), 'std::recursive_mutex', 0, 2),
((SELECT id FROM questions WHERE order_index=1026), 'std::shared_mutex', 0, 3),
((SELECT id FROM questions WHERE order_index=1026), 'std::mutex', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Mutex Patterns Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'std::lock_guard supports deferred locking via defer_lock tag.',
 NULL,
 'False. defer_lock is for unique_lock/scoped patterns, not lock_guard.',
 3, 12, 1027, 1,
 'Wrapper capability differences.',
 'https://cppreference.com/w/cpp/thread/lock_guard.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1027), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=1027), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Mutex Patterns Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::timed_mutex m;\nbool ok = m.try_lock_for(std::chrono::milliseconds(1));\nif(ok){ std::cout<<"1"; m.unlock(); } else { std::cout<<"0"; }',
 'On uncontended mutex, try_lock_for typically succeeds quickly -> prints 1.',
 4, 15, 1028, 1,
 'Timed lock attempt.',
 'https://cppreference.com/w/cpp/thread/timed_mutex.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1028), 'Usually 1 on uncontended lock', 1, 1),
((SELECT id FROM questions WHERE order_index=1028), 'Always 0', 0, 2),
((SELECT id FROM questions WHERE order_index=1028), 'Compile error always', 0, 3),
((SELECT id FROM questions WHERE order_index=1028), 'Always deadlock', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Mutex Patterns Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What advantage does scoped_lock have over naive manual multi-locking?',
 NULL,
 'It provides deadlock-avoidance locking of multiple mutexes in one RAII object.',
 4, 15, 1029, 1,
 'Deadlock-safe multi-lock pattern.',
 'https://cppreference.com/w/cpp/thread/scoped_lock.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1029), 'Deadlock-avoidant multi-mutex RAII locking', 1, 1),
((SELECT id FROM questions WHERE order_index=1029), 'Automatically makes code lock-free', 0, 2),
((SELECT id FROM questions WHERE order_index=1029), 'Only works with one mutex', 0, 3),
((SELECT id FROM questions WHERE order_index=1029), 'Replaces condition_variable', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Mutex Patterns Quiz' ORDER BY id DESC LIMIT 1), 13, 'multi_select',
 'Select reader-writer locking related types.',
 NULL,
 'shared_mutex/shared_timed_mutex and shared_lock are reader-writer related.',
 4, 15, 1030, 1,
 'Shared read ownership patterns.',
 'https://cppreference.com/w/cpp/thread/shared_mutex.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1030), 'std::shared_mutex', 1, 1),
((SELECT id FROM questions WHERE order_index=1030), 'std::shared_timed_mutex', 1, 2),
((SELECT id FROM questions WHERE order_index=1030), 'std::shared_lock', 1, 3),
((SELECT id FROM questions WHERE order_index=1030), 'std::atomic_ref', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Mutex Patterns Quiz' ORDER BY id DESC LIMIT 1), 13, 'fill_blank',
 'Tag used to indicate lock ownership already acquired is std::adopt____.',
 NULL,
 'lock.',
 3, 12, 1031, 1,
 'Lock tag type.',
 'https://cppreference.com/w/cpp/thread/lock_tag_t.html');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Mutex Patterns Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What is a major risk with recursive_mutex overuse?',
 NULL,
 'Can hide design issues and make lock hierarchy reasoning harder.',
 4, 15, 1032, 1,
 'Use only where justified.',
 'https://cppreference.com/w/cpp/thread/recursive_mutex.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1032), 'Can mask design/locking structure problems', 1, 1),
((SELECT id FROM questions WHERE order_index=1032), 'Always faster than mutex', 0, 2),
((SELECT id FROM questions WHERE order_index=1032), 'Eliminates deadlocks in all cases', 0, 3),
((SELECT id FROM questions WHERE order_index=1032), 'Cannot be unlocked', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Mutex Patterns Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'Lock wrappers (RAII) reduce risk of forgetting unlock on exception paths.',
 NULL,
 'True.',
 2, 10, 1033, 1,
 'Exception safety via RAII.',
 'https://cppreference.com/w/cpp/thread/lock_guard.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1033), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1033), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Mutex Patterns Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Which type is suitable for timed wait on mutex with normal ownership semantics?',
 NULL,
 'timed_mutex.',
 3, 12, 1034, 1,
 'Time-bounded lock acquisition.',
 'https://cppreference.com/w/cpp/thread/timed_mutex.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1034), 'std::timed_mutex', 1, 1),
((SELECT id FROM questions WHERE order_index=1034), 'std::shared_lock', 0, 2),
((SELECT id FROM questions WHERE order_index=1034), 'std::future', 0, 3),
((SELECT id FROM questions WHERE order_index=1034), 'std::atomic<int>', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Advanced Mutex Patterns Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::mutex a,b; std::scoped_lock lk(a,b); std::cout<<"ok";',
 'scoped_lock acquires both mutexes safely; prints ok.',
 3, 12, 1035, 1,
 'Multi-lock RAII in one line.',
 'https://cppreference.com/w/cpp/thread/scoped_lock.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1035), 'ok', 1, 1),
((SELECT id FROM questions WHERE order_index=1035), 'Compile error always', 0, 2),
((SELECT id FROM questions WHERE order_index=1035), 'deadlock always', 0, 3),
((SELECT id FROM questions WHERE order_index=1035), '0', 0, 4);

-- -------------------------------------------------------------
-- D) Atomic Correctness Workshop Quiz (1036..1045)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Correctness Workshop Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What does fetch_add return?',
 NULL,
 'Returns previous value before addition.',
 3, 12, 1036, 1,
 'Read-modify-write return semantics.',
 'https://cppreference.com/w/cpp/atomic/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1036), 'Old value before increment/addition', 1, 1),
((SELECT id FROM questions WHERE order_index=1036), 'New value always', 0, 2),
((SELECT id FROM questions WHERE order_index=1036), 'Boolean success flag only', 0, 3),
((SELECT id FROM questions WHERE order_index=1036), 'Pointer to atomic object', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Correctness Workshop Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'Atomic operations on one variable automatically synchronize all unrelated shared data safely.',
 NULL,
 'False. Need proper synchronization design; atomics on one variable do not magically protect everything.',
 5, 18, 1037, 1,
 'Scope of synchronization matters.',
 'https://cppreference.com/w/cpp/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1037), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=1037), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Correctness Workshop Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::atomic<int> a{10};\nstd::cout << a.fetch_sub(4) << ":" << a.load();',
 'fetch_sub returns old value 10, then new value is 6.',
 4, 15, 1038, 1,
 'Old:new pattern.',
 'https://cppreference.com/w/cpp/atomic/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1038), '10:6', 1, 1),
((SELECT id FROM questions WHERE order_index=1038), '6:10', 0, 2),
((SELECT id FROM questions WHERE order_index=1038), '6:6', 0, 3),
((SELECT id FROM questions WHERE order_index=1038), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Correctness Workshop Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'atomic_ref requires what precondition on referenced object?',
 NULL,
 'Referenced object must meet atomic_ref requirements (alignment/lifetime and no conflicting non-atomic accesses).',
 5, 18, 1039, 1,
 'Correctness contract.',
 'https://cppreference.com/w/cpp/atomic/atomic_ref.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1039), 'Object must satisfy atomic_ref usage requirements', 1, 1),
((SELECT id FROM questions WHERE order_index=1039), 'Object must be const', 0, 2),
((SELECT id FROM questions WHERE order_index=1039), 'Object must be on stack only', 0, 3),
((SELECT id FROM questions WHERE order_index=1039), 'No lifetime constraints', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Correctness Workshop Quiz' ORDER BY id DESC LIMIT 1), 13, 'multi_select',
 'Select statements about atomic correctness.',
 NULL,
 'Atomics prevent races on guarded object; overall design still needs memory ordering and invariant reasoning.',
 5, 18, 1040, 1,
 'Primitive-level vs system-level correctness.',
 'https://cppreference.com/w/cpp/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1040), 'Atomics can remove races on that atomic object', 1, 1),
((SELECT id FROM questions WHERE order_index=1040), 'Compound invariants may still need mutex/coordination', 1, 2),
((SELECT id FROM questions WHERE order_index=1040), 'Atomics make every multi-variable protocol trivially safe', 0, 3),
((SELECT id FROM questions WHERE order_index=1040), 'Design must consider visibility/order semantics', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Correctness Workshop Quiz' ORDER BY id DESC LIMIT 1), 13, 'fill_blank',
 'Type wrapper for lock-free style scalar synchronization is std::_____.',
 NULL,
 'atomic',
 2, 10, 1041, 1,
 'Core atomic wrapper.',
 'https://cppreference.com/w/cpp/atomic/atomic.html');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Correctness Workshop Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Which statement about lock-free is correct?',
 NULL,
 'Lock-free availability is implementation/type dependent; not guaranteed for all atomics.',
 4, 15, 1042, 1,
 'Avoid blanket assumptions.',
 'https://cppreference.com/w/cpp/atomic/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1042), 'Depends on type/platform; not universal', 1, 1),
((SELECT id FROM questions WHERE order_index=1042), 'All atomics are lock-free always', 0, 2),
((SELECT id FROM questions WHERE order_index=1042), 'Lock-free means race-free for all program state', 0, 3),
((SELECT id FROM questions WHERE order_index=1042), 'Lock-free requires shared_mutex', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Correctness Workshop Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'Data race on non-atomic shared variable is undefined behavior in C++.',
 NULL,
 'True.',
 3, 12, 1043, 1,
 'Fundamental race rule.',
 'https://cppreference.com/w/cpp/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1043), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1043), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Correctness Workshop Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Best practice before switching mutex-protected code to atomics?',
 NULL,
 'Prove correctness model, benchmark, and validate memory-order semantics.',
 5, 18, 1044, 1,
 'Correctness before micro-optimization.',
 'https://cppreference.com/w/cpp/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1044), 'Formally reason + test + benchmark before changing', 1, 1),
((SELECT id FROM questions WHERE order_index=1044), 'Replace locks blindly for speed', 0, 2),
((SELECT id FROM questions WHERE order_index=1044), 'Ignore memory ordering entirely', 0, 3),
((SELECT id FROM questions WHERE order_index=1044), 'Use atomics for all code paths automatically', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Correctness Workshop Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::atomic<int> x{1};\nint old = x.exchange(9);\nstd::cout << old << ":" << x.load();',
 'exchange returns old value and stores new one => 1:9.',
 4, 15, 1045, 1,
 'Atomic swap semantics.',
 'https://cppreference.com/w/cpp/atomic/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1045), '1:9', 1, 1),
((SELECT id FROM questions WHERE order_index=1045), '9:1', 0, 2),
((SELECT id FROM questions WHERE order_index=1045), '9:9', 0, 3),
((SELECT id FROM questions WHERE order_index=1045), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- E) Resource Ownership in Systems Quiz (1046..1055)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Resource Ownership in Systems Quiz' ORDER BY id DESC LIMIT 1), 11, 'mcq',
 'What does RAII primarily guarantee?',
 NULL,
 'Resource lifetime tied to object lifetime (acquire in ctor, release in dtor).',
 2, 10, 1046, 1,
 'Scope-bound resources.',
 'https://www.studyplan.dev/pro-cpp/smart-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1046), 'Deterministic resource cleanup via object lifetime', 1, 1),
((SELECT id FROM questions WHERE order_index=1046), 'Automatic parallelism', 0, 2),
((SELECT id FROM questions WHERE order_index=1046), 'No exceptions possible', 0, 3),
((SELECT id FROM questions WHERE order_index=1046), 'No need for ownership thinking', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Resource Ownership in Systems Quiz' ORDER BY id DESC LIMIT 1), 11, 'true_false',
 'Copy semantics may be expensive or unsafe for resource-owning objects without custom design.',
 NULL,
 'True.',
 3, 12, 1047, 1,
 'Ownership and copy cost.',
 'https://www.studyplan.dev/pro-cpp/copy-semantics');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1047), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1047), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Resource Ownership in Systems Quiz' ORDER BY id DESC LIMIT 1), 11, 'code_output',
 'What is printed?',
 'auto p = std::make_unique<int>(11);\nstd::cout << *p;',
 'unique_ptr owns integer value 11.',
 1, 10, 1048, 1,
 'Simple owner dereference.',
 'https://www.learncpp.com/cpp-tutorial/stdunique_ptr/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1048), '11', 1, 1),
((SELECT id FROM questions WHERE order_index=1048), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=1048), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE order_index=1048), 'nullptr', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Resource Ownership in Systems Quiz' ORDER BY id DESC LIMIT 1), 11, 'mcq',
 'When should shared_ptr be preferred over unique_ptr?',
 NULL,
 'When multiple independent owners must share lifetime of same object.',
 2, 10, 1049, 1,
 'Shared ownership use-case.',
 'https://www.studyplan.dev/pro-cpp/shared-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1049), 'When multiple owners are truly required', 1, 1),
((SELECT id FROM questions WHERE order_index=1049), 'Always by default', 0, 2),
((SELECT id FROM questions WHERE order_index=1049), 'For stack objects only', 0, 3),
((SELECT id FROM questions WHERE order_index=1049), 'To avoid all synchronization', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Resource Ownership in Systems Quiz' ORDER BY id DESC LIMIT 1), 11, 'multi_select',
 'Select ownership-oriented API design guidelines.',
 NULL,
 'APIs should communicate transfer/share/borrow semantics clearly.',
 3, 15, 1050, 1,
 'Ownership in contracts.',
 'https://www.studyplan.dev/pro-cpp/smart-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1050), 'Use distinct types for owning vs non-owning parameters', 1, 1),
((SELECT id FROM questions WHERE order_index=1050), 'Document lifetime assumptions', 1, 2),
((SELECT id FROM questions WHERE order_index=1050), 'Hide ownership transfer details', 0, 3),
((SELECT id FROM questions WHERE order_index=1050), 'Prefer minimal ownership surface area', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Resource Ownership in Systems Quiz' ORDER BY id DESC LIMIT 1), 11, 'fill_blank',
 'Move semantics can help avoid expensive deep ____ of resources.',
 NULL,
 'copies.',
 2, 10, 1051, 1,
 'Transfer instead of duplicate.',
 'https://www.studyplan.dev/pro-cpp/move-semantics');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Resource Ownership in Systems Quiz' ORDER BY id DESC LIMIT 1), 11, 'mcq',
 'Which anti-pattern commonly leads to ownership confusion?',
 NULL,
 'Mixing raw owning pointers with unclear transfer rules across modules.',
 4, 15, 1052, 1,
 'Ambiguous ownership contracts.',
 'https://www.studyplan.dev/intro-to-programming/manual-memory-management');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1052), 'Implicit/raw ownership without explicit contract', 1, 1),
((SELECT id FROM questions WHERE order_index=1052), 'RAII wrappers with clear docs', 0, 2),
((SELECT id FROM questions WHERE order_index=1052), 'Unique ownership factory APIs', 0, 3),
((SELECT id FROM questions WHERE order_index=1052), 'Scoped lock wrappers', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Resource Ownership in Systems Quiz' ORDER BY id DESC LIMIT 1), 11, 'true_false',
 'clear ownership boundaries improve testability and maintainability.',
 NULL,
 'True.',
 2, 10, 1053, 1,
 'Architectural benefit.',
 'https://www.studyplan.dev/pro-cpp/smart-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1053), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1053), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Resource Ownership in Systems Quiz' ORDER BY id DESC LIMIT 1), 11, 'mcq',
 'Best-practice when exposing shared_ptr from library boundary?',
 NULL,
 'Do so only when caller should participate in shared lifetime ownership.',
 3, 12, 1054, 1,
 'Avoid over-sharing ownership.',
 'https://www.studyplan.dev/pro-cpp/shared-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1054), 'Expose shared_ptr only for genuine shared lifetime', 1, 1),
((SELECT id FROM questions WHERE order_index=1054), 'Always expose shared_ptr for every object', 0, 2),
((SELECT id FROM questions WHERE order_index=1054), 'Expose raw pointer for ownership transfer', 0, 3),
((SELECT id FROM questions WHERE order_index=1054), 'Hide all ownership from docs', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Resource Ownership in Systems Quiz' ORDER BY id DESC LIMIT 1), 11, 'code_output',
 'What is printed?',
 'auto p = std::make_shared<int>(2);\nauto w = std::weak_ptr<int>(p);\np.reset();\nstd::cout << (w.expired() ? 1 : 0);',
 'After last shared owner reset, weak_ptr reports expired => 1.',
 3, 12, 1055, 1,
 'Observer after owner release.',
 'https://www.studyplan.dev/pro-cpp/weak-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1055), '1', 1, 1),
((SELECT id FROM questions WHERE order_index=1055), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=1055), '2', 0, 3),
((SELECT id FROM questions WHERE order_index=1055), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- Primary tag mapping (956..1055)
-- -------------------------------------------------------------
INSERT OR IGNORE INTO question_tags (question_id, tag_id)
SELECT q.id,
       (SELECT id FROM tags WHERE name = CASE
            WHEN q.order_index BETWEEN 956 AND 975 THEN 'smart-pointers'
            WHEN q.order_index BETWEEN 976 AND 1005 THEN 'concurrency'
            WHEN q.order_index BETWEEN 1006 AND 1015 THEN 'concurrency'
            WHEN q.order_index BETWEEN 1016 AND 1025 THEN 'concurrency'
            WHEN q.order_index BETWEEN 1026 AND 1035 THEN 'concurrency'
            WHEN q.order_index BETWEEN 1036 AND 1045 THEN 'concurrency'
            WHEN q.order_index BETWEEN 1046 AND 1055 THEN 'memory'
            ELSE 'best-practices'
       END)
FROM questions q
WHERE q.order_index BETWEEN 956 AND 1055;

-- Secondary tags
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE order_index=960), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=970), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=980), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=990), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1000), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1010), (SELECT id FROM tags WHERE name='memory')),
((SELECT id FROM questions WHERE order_index=1020), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1030), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1040), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1050), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1051), (SELECT id FROM tags WHERE name='smart-pointers'));

-- Derive quiz_tags for affected quizzes
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.order_index BETWEEN 956 AND 1055;