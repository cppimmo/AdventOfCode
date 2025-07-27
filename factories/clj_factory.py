import os
from jinja2 import Environment, FileSystemLoader

TEMPLATE_DIR = os.path.join(os.path.dirname(__file__), "../templates/clj")
SRC_TEMPLATE_DIR = os.path.join(TEMPLATE_DIR, "src")


def generate_clj_template(year: int, base_dir: str):
    src_dir = os.path.join(base_dir, "src")
    # print(base_dir, src_dir)
    env = Environment(loader=FileSystemLoader(TEMPLATE_DIR))
    src_env = Environment(loader=FileSystemLoader(SRC_TEMPLATE_DIR))

    # Create the src/ directory
    os.makedirs(src_dir, exist_ok=True)

    aoc_template = src_env.get_template("aoc.clj.j2")
    with open(os.path.join(src_dir, "aoc.clj"), "w") as f:
        f.write(aoc_template.render())
    
    # Generate src/day_01.clj to src/day_25.clj
    for day in range(1, 26):
        filename = f"day_{day:02d}.clj"
        template = src_env.get_template("day_xx.clj.j2")
        output_path = os.path.join(src_dir, filename)
        with open(output_path, "w") as f:
            f.write(template.render(year=year, day=day))
        
    for template_name in ["README.md.j2", "deps.edn.j2", ".gitignore.j2"]:
        template = env.get_template(template_name)
        output_path = os.path.join(base_dir, template_name.replace(".j2", ""))
        with open(output_path, "w") as f:
            f.write(template.render(year=year, day=day))
