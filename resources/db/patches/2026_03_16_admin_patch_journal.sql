-- Patch: create the admin patch operation journal table.
-- Records every APPLY / ROLLBACK action with snapshot path and status.

CREATE TABLE IF NOT EXISTS admin_patch_journal (
    id            INTEGER  PRIMARY KEY AUTOINCREMENT,
    patch_id      TEXT     NOT NULL,
    action        TEXT     NOT NULL,   -- 'APPLY' or 'ROLLBACK'
    snapshot_path TEXT,
    status        TEXT     NOT NULL,   -- 'OK' or 'FAIL'
    details       TEXT,
    created_at    DATETIME DEFAULT CURRENT_TIMESTAMP
);
