from .cpp_factory import generate_cpp_template
from .clj_factory import generate_clj_template

FACTORY_MAP = {
    "cpp": generate_cpp_template,
    "clj": generate_clj_template
}


def get_factory_for_lang(lang: str):
    if lang not in FACTORY_MAP:
        raise ValueError(f"No factory function found for language: {lang}")
    return FACTORY_MAP[lang]
