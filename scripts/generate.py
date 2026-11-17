#!/usr/bin/env python3
"""Generate one new small C project per day using GitHub Models (no API key needed).

Authenticates with the workflow's built-in GITHUB_TOKEN.
Creates projects/YYYY-MM-DD-<slug>/main.c and README.md.
The C code is compiled AND run; on failure the error is sent back to the
model for a fix (up to MAX_TRIES attempts).
"""
import datetime
import json
import os
import pathlib
import re
import subprocess
import sys
import time
import urllib.error
import urllib.request

ENDPOINT = "https://models.github.ai/inference/chat/completions"
MODEL = os.environ.get("MODEL", "openai/gpt-4.1")
# Older GitHub Models endpoint (also accepts the GitHub token), used as a fallback.
FALLBACK_ENDPOINT = "https://models.inference.ai.azure.com/chat/completions"
FALLBACK_MODEL = "gpt-4.1"
MAX_TRIES = 3
PROJECTS = pathlib.Path("projects")

CATEGORIES = [
    "robotics (control, kinematics, sensors, motion planning)",
    "embedded systems and microcontroller-style C",
    "computer architecture and CPU/memory simulation",
    "operating systems concepts (scheduling, memory, IPC)",
    "networking and protocols",
    "data structures and algorithms",
    "signal processing and sensor filtering",
    "computer vision basics without external libraries",
    "cryptography basics (educational only)",
    "AI/ML fundamentals implemented in plain C",
]

PROMPT = """Write a small, compilable C program for a new daily project.

Category for today: {category}
Date: {date}

Already-used project names (do NOT repeat or closely resemble these):
{existing}

Requirements:
- Single file, standard C11 only, no external libraries (libc and libm are fine).
- 80-200 lines, well commented, compiles with: gcc -Wall -Wextra -std=c11 main.c -lm
- Must run on its own, print clear output, and finish quickly (no infinite loops, no user input).
- Educational: a beginner-to-intermediate student should learn something.

Reply in EXACTLY this format and nothing else:

SLUG: short-kebab-case-name
===C===
<full contents of main.c>
===README===
<full README.md: title, what it does, how it works, how to compile and run, sample output, ideas to extend>
===END===
"""


def _post(url, model, messages):
    """Return (text, error). Never raises on bad responses."""
    token = os.environ["GITHUB_TOKEN"]
    body = json.dumps({"model": model, "messages": messages, "max_tokens": 4000}).encode()
    req = urllib.request.Request(
        url, data=body, method="POST",
        headers={
            "Authorization": f"Bearer {token}",
            "Accept": "application/json",
            "X-GitHub-Api-Version": "2022-11-28",
            "Content-Type": "application/json",
        },
    )
    try:
        with urllib.request.urlopen(req, timeout=120) as r:
            raw, status = r.read().decode(errors="replace"), r.status
    except urllib.error.HTTPError as e:
        return None, f"HTTP {e.code}: {e.read().decode(errors='replace')[:300]}"
    except (urllib.error.URLError, TimeoutError) as e:
        return None, f"network error: {e}"
    try:
        return json.loads(raw)["choices"][0]["message"]["content"], ""
    except (ValueError, KeyError, IndexError, TypeError):
        return None, f"HTTP {status}, unexpected body: {raw[:300]!r}"


def chat(messages):
    """Try the main endpoint, then the fallback, with a short retry on each."""
    errors = []
    for url, model in ((ENDPOINT, MODEL), (FALLBACK_ENDPOINT, FALLBACK_MODEL)):
        for attempt in range(2):
            text, err = _post(url, model, messages)
            if text:
                return text
            msg = f"{url} [{model}] -> {err}"
            errors.append(msg)
            print(msg, file=sys.stderr)
            time.sleep(5 * (attempt + 1))
    sys.exit("All model calls failed:\n" + "\n".join(errors))


def parse(text):
    slug = re.search(r"SLUG:\s*([a-z0-9-]+)", text)
    code = re.search(r"===C===\s*(.*?)\s*===README===", text, re.S)
    readme = re.search(r"===README===\s*(.*?)\s*===END===", text, re.S)
    if not (slug and code and readme):
        raise ValueError("Model reply did not match the expected format")
    c = re.sub(r"^```[a-z]*\n|\n```$", "", code.group(1).strip())
    return slug.group(1), c + "\n", readme.group(1).strip() + "\n"


def compile_check(c_code):
    tmp = pathlib.Path("/tmp/check")
    tmp.mkdir(exist_ok=True)
    (tmp / "main.c").write_text(c_code)
    r = subprocess.run(
        ["gcc", "-Wall", "-Wextra", "-std=c11", "main.c", "-o", "check", "-lm"],
        cwd=tmp, capture_output=True, text=True,
    )
    if r.returncode != 0:
        return False, r.stderr
    run = subprocess.run(["./check"], cwd=tmp, capture_output=True, text=True, timeout=10)
    if run.returncode != 0:
        return False, f"Program exited with code {run.returncode}\n{run.stderr}"
    return True, ""


def main():
    today = datetime.datetime.now(
        datetime.timezone(datetime.timedelta(hours=5, minutes=30))).date()
    PROJECTS.mkdir(exist_ok=True)
    existing = sorted(p.name for p in PROJECTS.iterdir() if p.is_dir())
    category = CATEGORIES[today.toordinal() % len(CATEGORIES)]

    messages = [{
        "role": "user",
        "content": PROMPT.format(
            category=category, date=today.isoformat(),
            existing="\n".join(existing[-60:]) or "(none yet)"),
    }]

    for attempt in range(1, MAX_TRIES + 1):
        reply = chat(messages)
        try:
            slug, c_code, readme = parse(reply)
            ok, err = compile_check(c_code)
        except Exception as e:
            ok, err = False, str(e)
        if ok:
            out = PROJECTS / f"{today.isoformat()}-{slug}"
            out.mkdir(exist_ok=True)
            (out / "main.c").write_text(c_code)
            (out / "README.md").write_text(readme)
            print(f"Created {out}")
            return
        print(f"Attempt {attempt} failed:\n{err}", file=sys.stderr)
        messages += [
            {"role": "assistant", "content": reply},
            {"role": "user", "content":
                f"That failed:\n{err}\nFix it and reply again in the exact same format."},
        ]
    sys.exit("Could not produce a compiling project today.")


if __name__ == "__main__":
    main()
