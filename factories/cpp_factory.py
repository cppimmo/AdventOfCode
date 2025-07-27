import os
from jinja2 import Environment, FileSystemLoader

TEMPLATE_DIR = os.path.join(os.path.dirname(__file__), "../templates/cpp")
SRC_TEMPLATE_DIR = os.path.join(TEMPLATE_DIR, "Source")


def generate_cpp_template(year: int, base_dir: str):
    env = Environment(loader=FileSystemLoader(TEMPLATE_DIR))
    src_env = Environment(loader=FileSystemLoader(SRC_TEMPLATE_DIR))

    # Generate Day01/ to Day25/ (create parent YEAR/ automatically)
    for day in range(1, 26):
        day_str = f"Day{day:02d}"
        day_dir = os.path.join(base_dir, day_str)
        os.makedirs(day_dir, exist_ok=True)

        for part in ["Part1.cpp", "Part2.cpp"]:
            template = env.get_template("main.cpp.j2")
            output_path = os.path.join(day_dir, part)
            with open(output_path, "w") as f:
                f.write(template.render(year=year, day=day))

        # Create an empty sample_input.txt file
        open(os.path.join(day_dir, "sample_input.txt"), "w").close()

    # Generate Source/ directory
    source_dir = os.path.join(base_dir, "Source")
    os.makedirs(source_dir, exist_ok=True)

    for template_name in ["AdventOfCodeStd.hpp.j2", "AdventOfCodeStd.cpp.j2"]:
        template = src_env.get_template(template_name)
        out_filename = template_name.replace(".j2", "")

        with open(os.path.join(source_dir, out_filename), "w") as f:
            f.write(template.render(year=year))

    # Install the CMakeLists.txt in YEAR/
    for template_name in ["CMakeLists.txt.j2"]:
        template = env.get_template(template_name)
        output_path = os.path.join(base_dir, template_name.replace(".j2", ""))
        with open(output_path, "w") as f:
            f.write(template.render(year=year))
