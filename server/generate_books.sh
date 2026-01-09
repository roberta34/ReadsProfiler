#!/bin/bash

DB="library.db"
BOOKS_DIR="books"

mkdir -p "$BOOKS_DIR"

sqlite3 "$DB" "SELECT title, file_path FROM books;" | while IFS='|' read -r title path; do
    filename=$(basename "$path")
    filepath="$BOOKS_DIR/$filename"

    cat <<EOF > "$filepath"
$title

--------------------------------------------------

This is a demo fragment from the book "$title".

This content is provided for educational and demonstration purposes only.
It simulates a real book text inside the ReadsProfiler application.

EOF

    echo "Generated $filepath"
done
