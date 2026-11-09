#!/usr/bin/env python3
"""Create a daily C practice file template in the repository."""

from __future__ import annotations

import argparse
from datetime import datetime
from pathlib import Path


def build_template(date_text: str) -> str:
    return f'''#include <stdio.h>

int main(void) {{
    printf("Daily C practice file for {date_text}\\n");
    return 0;
}}
'''


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Create a new daily C file template in a daily/ directory."
    )
    parser.add_argument(
        "--date",
        default=datetime.now().strftime("%Y-%m-%d"),
        help="Date to use in the generated filename (YYYY-MM-DD).",
    )
    parser.add_argument(
        "--output-dir",
        default="daily",
        help="Directory where the generated C file should be created.",
    )
    args = parser.parse_args()

    output_dir = Path(args.output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    file_name = f"day_{args.date}.c"
    file_path = output_dir / file_name

    if file_path.exists():
        raise SystemExit(f"File already exists: {file_path}")

    file_path.write_text(build_template(args.date), encoding="utf-8")
    print(f"Created {file_path}")


if __name__ == "__main__":
    main()
