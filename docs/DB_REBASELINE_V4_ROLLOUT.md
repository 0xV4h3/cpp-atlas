# Database Rebaseline v4 - Rollout Guide

## Overview

This document describes the process for deploying the Database Rebaseline v4 changes to ensure a clean, consistent database from a fresh install.

## What Changed in v4

### Schema Changes
- **fill_blank_answers table**: Canonical storage for accepted fill-blank question answers
- **admin_patch_journal table**: Records all patch apply/rollback operations with snapshots
- **admin_deletion_log table**: Audit log for soft-deleted questions and quizzes
- **Difficulty constraints**: CHECK constraints enforce difficulty ∈ [1, 4] for topics, quizzes, and questions

### Patch Chain Updates
- **Renamed patches**: Foundation and Core C++ pack files now consistently use `_part1.sql` suffix
- **New patches**:
  - `2026_03_16_01_difficulty_normalization.sql`: Clamps all difficulty values to [1, 4]
  - Consolidated fill_blank schema patches (removed duplicates)
- **Lexical ordering**: All patches now apply cleanly in alphabetical order

### Content Fixes
- **Fill-blank answers**: All 27 fill_blank questions now have explicit accepted answer tokens
- **Difficulty normalization**: All content difficulty values are within [1, 4] range
- **Answer quality**: Short token answers (e.g., `nullptr`, `virtual`) replace sentence-style keys

## Rollout Procedure

### Prerequisites
- Access to the local database file (typically `~/.local/share/CppAtlas/quiz.db`)
- CppAtlas application binary
- **BACKUP YOUR DATA FIRST** (see step 1 below)

### Step 1: Backup Existing Database

```bash
# Locate your database
DB_PATH="$HOME/.local/share/CppAtlas/quiz.db"

# Create backup with timestamp
BACKUP_PATH="$HOME/.local/share/CppAtlas/quiz_backup_$(date +%Y%m%d_%H%M%S).db"
cp "$DB_PATH" "$BACKUP_PATH"

echo "Backup created at: $BACKUP_PATH"
```

### Step 2: Clean Bootstrap Test (Recommended)

To verify that v4 works correctly from scratch:

```bash
# Move current database aside
mv "$DB_PATH" "${DB_PATH}.old"

# Launch application (creates fresh DB with v4 schema)
./CppAtlas

# Application will:
# 1. Create schema v4 with all new tables and constraints
# 2. Load seed_data.sql (basic quiz content)
# 3. Prompt for content patch application
```

### Step 3: Apply Content Patches

From within the application:

1. Log in with an admin account
2. Open Admin Panel (Ctrl+Alt+Shift+M or --admin flag)
3. Go to **Content** tab
4. Click **Apply Content Updates**
5. Review patch list in activity log
6. Confirm application

Patches will apply in this order:
```
2026_03_16_00_fill_blank_answer_keys_schema.sql
2026_03_16_01_difficulty_normalization.sql
2026_03_16_02_fill_blank_bulk_migration.sql
2026_03_16_03_fill_blank_full_repair.sql
2026_03_16_04_fill_blank_quality_checks.sql
2026_03_16_admin_patch_journal.sql
2026_03_16_all_quizzes_pack_05_difficulty_fix.sql
2026_03_16_core_cpp_pack_02_part1.sql
...
```

Or via CLI:
```bash
./quiz_admin --db "$DB_PATH" apply-content --content-dir resources/db/patches
```

### Step 4: Run Validation

Verify content integrity:

**From Admin Panel:**
1. Go to **Validation** tab
2. Click **Validate Content**
3. Review findings in activity log
4. **Expected result**: Zero errors for fill_blank missing-answer issues

**From CLI:**
```bash
./quiz_admin --db "$DB_PATH" validate --content-dir resources/db/patches
echo "Exit code: $?"  # Should be 0 or 2 (warnings OK, errors not OK)
```

### Step 5: Verify Database Stats

Check that content loaded correctly:

```bash
./quiz_admin --db "$DB_PATH" stats
```

Expected output similar to:
```
Schema version: 1
Quizzes: 12
Questions: 150+
Users: 1 (admin)
Sessions: 0
Applied patches: 24
```

### Step 6: Test Fill-Blank Questions

From the application:
1. Start a quiz with fill-blank questions (e.g., "Foundations Pack 01")
2. Answer fill-blank questions with expected tokens
3. Verify scoring is correct
4. Check explanations are pedagogical (not answer keys)

### Step 7: Restore Old Database (If Needed)

If issues are encountered:

```bash
# Stop application
# Restore backup
mv "${DB_PATH}.old" "$DB_PATH"

# Or restore from timestamped backup
cp "$BACKUP_PATH" "$DB_PATH"
```

## Migrating Existing Database (Alternative)

If you want to preserve user data from an existing database:

### Migration Script (Experimental)

**WARNING**: This is more complex than a clean bootstrap. Only proceed if you need to preserve user session history.

```bash
# 1. Backup current database
cp "$DB_PATH" "${DB_PATH}.backup"

# 2. Apply schema patches manually
sqlite3 "$DB_PATH" < resources/db/patches/2026_03_16_00_fill_blank_answer_keys_schema.sql
sqlite3 "$DB_PATH" < resources/db/patches/2026_03_16_admin_patch_journal.sql

# 3. Add CHECK constraints (requires table recreation in SQLite)
# This is complex - recommend using clean bootstrap instead

# 4. Apply all content patches
./quiz_admin --db "$DB_PATH" apply-content --content-dir resources/db/patches

# 5. Validate
./quiz_admin --db "$DB_PATH" validate --content-dir resources/db/patches
```

**Note**: SQLite doesn't support adding CHECK constraints to existing tables. You must:
1. Create new tables with constraints
2. Copy data
3. Drop old tables
4. Rename new tables

For most users, **clean bootstrap is simpler and safer**.

## Verification Checklist

After rollout, verify:

- [ ] Application launches without errors
- [ ] Admin panel opens successfully
- [ ] Content validation shows zero fill_blank missing-answer errors
- [ ] Difficulty values are all within [1, 4] (no "Unknown" labels in UI)
- [ ] Fill-blank questions are answerable with intuitive short tokens
- [ ] Patch history is visible in Admin Panel → Maintenance → Show Journal History
- [ ] Database stats command shows expected entity counts
- [ ] User session data preserved (if migrating existing DB)

## Troubleshooting

### Issue: "No such table: fill_blank_answers"

**Solution**: Apply patch `2026_03_16_00_fill_blank_answer_keys_schema.sql` first:
```bash
./quiz_admin --db "$DB_PATH" apply-content --content-dir resources/db/patches
```

### Issue: "Difficulty constraint violation"

**Symptom**: Cannot insert/update questions with difficulty outside [1, 4]

**Solution**: This is expected behavior in v4. Use difficulty values 1 (Beginner), 2 (Intermediate), 3 (Advanced), or 4 (Expert).

### Issue: Fill-blank question shows "incorrect" for valid answer

**Cause**: Question missing accepted answer in fill_blank_answers table

**Solution**:
1. Run validation: `./quiz_admin validate`
2. Identify missing question IDs
3. Add accepted answer via Admin Panel → Maintenance → Edit Question

### Issue: Patch application fails mid-stream

**Symptom**: Some patches applied, some failed

**Solution**:
1. Check `admin_patch_journal` table for failure details:
   ```sql
   sqlite3 "$DB_PATH" "SELECT * FROM admin_patch_journal WHERE status='FAIL';"
   ```
2. Review error in `details` column
3. Fix underlying issue (e.g., missing table, constraint violation)
4. Re-apply patches (idempotent patches use INSERT OR IGNORE)

## Rollback Plan

If v4 causes critical issues:

1. Stop application
2. Restore pre-v4 backup:
   ```bash
   cp "$BACKUP_PATH" "$DB_PATH"
   ```
3. Use previous application version (before v4 merge)
4. Report issue at https://github.com/0xV4h3/cpp-atlas/issues

## Testing Recommendations

Before production rollout:

1. **Unit tests**: Run test suite to verify core functionality
   ```bash
   cd build
   ./tests/CppAtlasTests
   ./tests/quiz/AdminContentServiceTests
   ./tests/quiz/FillBlankEvaluationTests
   ./tests/quiz/FillBlankRecoveryTests
   ./tests/quiz/ContentPatchServiceTests
   ```

2. **Integration test**: Fresh DB → load schema → apply patches → validate → test quiz

3. **Regression test**: Verify existing features still work (login, quiz taking, results)

## Support

For issues or questions:
- GitHub Issues: https://github.com/0xV4h3/cpp-atlas/issues
- Documentation: `docs/admin_workflow.md`

## Change History

- **2026-03-16**: v4 rebaseline initial release
  - Schema v4 baseline with new tables
  - Patch chain canonicalization
  - Fill-blank answer model canonical
  - Difficulty constraints enforced
