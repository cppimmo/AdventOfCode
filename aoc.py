#!/usr/bin/env python3
# aoc.py
# 
# Use this script to generate a template for an Advent of Code year.
# Examples:
# ./aoc.py --year 2025 --lang cpp
# ./aoc.py # Generates a C++ template for the current year

import argparse
from argparse import ArgumentParser
from datetime import date
import os
from os import getcwd
from os import EX_OK, EX_USAGE, EX_SOFTWARE, EX_NOPERM, EX_DATAERR
from os.path import basename
import re
from sys import exit
from factories import get_factory_for_lang

# Constants:
CWD = getcwd()
PRGNAM = basename(__file__)
DEF_LANG = "cpp"
LANG_RE = {
    "cpp": r"cpp|c\+\+|cplusplus",
    "clj": r"clj|clojure"
}

def init_argparse() -> ArgumentParser:
    """Intialize argparse ArgumentParse instance."""
    parser = ArgumentParser(usage=f"{PRGNAM} [OPTION(s)]",
                            description="Templating tool for Advent of Code.")

    parser.add_argument("-y", "--year", default=date.today().year, type=int, help="Year for Advent of Code")
    parser.add_argument("-l", "--lang", default=DEF_LANG, type=str, help="Programming language (e.g., cpp, python)")
    parser.add_argument("--force", action="store_true", help="Force overwrite of existing year directory contents")
    parser.add_argument("-v", "--version", action="version",
                        version=f"{parser.prog} version 1.0.0")
    return parser


def main() -> None:
    parser = init_argparse()
    args = parser.parse_args()

    # print(type(args.lang), args.lang)
    lang_input = args.lang.lower()
    matched_lang = None

    for lang_key, lang_pattern in LANG_RE.items():
        if re.fullmatch(lang_pattern, lang_input):
            matched_lang = lang_key
            break

    if not matched_lang:
        print(f"Unsupported programming language: {lang_input}")
        print("The following languages are supported:", list(LANG_RE.keys()))
        exit(1)

    try:
        # The root path of the given AoC year
        base_dir = os.path.join(os.getcwd(), str(args.year))
        # The --force argument can be used to perform the factory operation anyway.
        if os.path.isdir(base_dir) and (not args.force):
            raise FileExistsError(f"A directory for the year you chose already exists!")
        
        factory = get_factory_for_lang(matched_lang)
        factory(args.year, base_dir)
    except Exception as e:
        print(f"Error generating template: {e}")
        exit(1)
    
    print(f"Advent of Code {matched_lang} template for {args.year} generated successfully.")
    exit(EX_OK)

if __name__ == "__main__":
    main()
