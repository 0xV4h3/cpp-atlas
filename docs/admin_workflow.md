# CppAtlas Administrator Workflow

## Entry points

| Method | Description |
|---|---|
| `--admin` CLI flag | Launch the app with `./CppAtlas --admin`. Opens admin flow after login. |
| `Ctrl+Alt+Shift+M` | Hidden hotkey while main window is active. |

## Access model

| Build type | Second password gate |
|---|---|
| **Debug** (`QT_DEBUG`) | Disabled — admin account login is sufficient. |
| **Release** | Enabled — second SHA-256 password check is required. |

### Setting release admin hash

```bash
echo -n "MySecretPassword" | sha256sum
export CPPATLAS_ADMIN_HASH="<64-lowercase-hex>"
./CppAtlas
```

`CPPATLAS_ADMIN_HASH` must be exactly 64 lowercase hex chars (SHA-256 digest).

---

## Admin Panel Tabs

- **Content**: apply incremental SQL patches.
- **Validation**: run content-integrity checks.
- **Export**: export deterministic SQL backup.
- **Stats**: DB/entity statistics.
- **Maintenance**: full CRUD for quizzes/questions + patch rollback tools.

---

## Content patch workflow

### Naming convention

```text
YYYY_MM_DD_pack_name_partN.sql
```

Patches are discovered and applied in **lexicographic order**.

### Apply (UI)

1. Admin Panel → **Content**
2. Click **Apply Content Updates**
3. Select patch directory
4. Review log output (applied/skipped/failed)

### Apply (CLI)

```bash
quiz_admin --db /path/to/cppatlas.db apply-content --content-dir ./patches
```

---

## Rollback workflow

Patch operations are transaction-safe and tracked:

- `content_patches` records applied patch IDs.
- `admin_patch_journal` records apply/rollback actions.
- Snapshot restore is available for rollback flows.

If apply fails, operation is aborted and logged.

---

## Validation rules

Validation checks include:

1. Quiz/question difficulty in range `[1..4]`
2. Every active `fill_blank` question has ≥1 active row in `fill_blank_answers`
3. `fill_blank` answer token quality (length/sentence-like warnings)
4. MCQ has options
5. MCQ has at least one correct option
6. Orphan options detection

### Run validation (CLI)

```bash
quiz_admin --db /path/to/cppatlas.db validate --content-dir ./patches
```

---

## Export

Export SQL backup:

```bash
quiz_admin --db /path/to/cppatlas.db export --out backup_$(date +%Y%m%d).sql
```

Exported tables include:
`topics`, `tags`, `quizzes`, `questions`, `options`, `fill_blank_answers`, `question_tags`, `quiz_tags` (and other content tables if configured).

---

## Maintenance (Full CRUD)

### Quizzes
- Create / Edit / Soft-delete / Restore
- Filter by title
- Active state visible in table

### Questions
- Create / Edit / Soft-delete / Restore
- Filter by text, quiz, type, state
- Explicit quiz assignment on create/edit
- `Order` = sorting position inside selected quiz

### fill_blank authoring
For `type=fill_blank`, accepted answers are managed via `fill_blank_answers` (canonical source).  
`explanation` is pedagogical text only and is **not** used for scoring.

---
