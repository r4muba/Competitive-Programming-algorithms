#!/usr/bin/env python3
"""Genera un algoritmario ICPC en PDF a partir de carpetas con codigo.

Uso rapido:
    python3 generate_algoritmario.py

El PDF se guarda, por defecto, en output/pdf/algoritmario_icpc.pdf.
"""

from __future__ import annotations

import argparse
import html
import re
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
from typing import Iterable

from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER, TA_RIGHT
from reportlab.lib.pagesizes import letter
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.lib.units import inch
from reportlab.platypus import (
    BaseDocTemplate,
    Frame,
    LongTable,
    PageBreak,
    PageTemplate,
    Paragraph,
    Spacer,
    Table,
    TableStyle,
    XPreformatted,
)
from reportlab.platypus.tableofcontents import TableOfContents


# Paleta sobria y de alto contraste, pensada para impresion y pantalla.
NAVY = colors.HexColor("#0B1F33")
BLUE = colors.HexColor("#0D5C8C")
TEAL = colors.HexColor("#00A6A6")
ORANGE = colors.HexColor("#FFB000")
INK = colors.HexColor("#17212B")
MUTED = colors.HexColor("#5D6B78")
RULE = colors.HexColor("#DCE4EC")
CODE_BG = colors.HexColor("#F5F7FA")
GUTTER_BG = colors.HexColor("#E9EEF4")

DEFAULT_EXTENSIONS = (".cpp", ".h", ".hpp", ".cc", ".cxx", ".c")
CODE_WRAP_COLUMNS = 108
IGNORED_DIRECTORIES = {
    ".git",
    ".github",
    ".idea",
    ".vscode",
    "__pycache__",
    "build",
    "dist",
    "node_modules",
    "output",
    "tmp",
    "venv",
    ".venv",
}


@dataclass(frozen=True)
class Algorithm:
    category: str
    path: Path
    title: str
    language: str
    lines: tuple[str, ...]


class AlgorithmsDocTemplate(BaseDocTemplate):
    """Documento con indice automatico, enlaces internos y marcadores."""

    def __init__(self, filename: str, *, title: str, **kwargs) -> None:
        super().__init__(filename, **kwargs)
        self.document_title = title

    def afterFlowable(self, flowable) -> None:  # noqa: N802 (API de ReportLab)
        level = getattr(flowable, "toc_level", None)
        key = getattr(flowable, "bookmark_key", None)
        text = getattr(flowable, "toc_text", None)
        if level is None or key is None or text is None:
            return

        self.canv.bookmarkPage(key)
        # multiBuild usa un canvas nuevo en cada pasada, por lo que cada canvas
        # necesita recibir su propio arbol de marcadores.
        self.canv.addOutlineEntry(text, key, level=level, closed=False)
        self.notify("TOCEntry", (level, text, self.page, key))


def slug(value: str) -> str:
    cleaned = re.sub(r"[^a-zA-Z0-9]+", "-", value).strip("-").lower()
    return cleaned or "seccion"


def display_category(relative_parent: Path) -> str:
    parts = []
    for part in relative_parent.parts:
        parts.append(part[:1].upper() + part[1:] if part else part)
    return " / ".join(parts)


def language_for(path: Path) -> str:
    return {
        ".c": "C",
        ".cc": "C++",
        ".cpp": "C++",
        ".cxx": "C++",
        ".h": "C/C++ Header",
        ".hpp": "C++ Header",
    }.get(path.suffix.lower(), path.suffix.lstrip(".").upper())


def discover_algorithms(root: Path, extensions: Iterable[str]) -> list[Algorithm]:
    normalized = {
        extension.lower() if extension.startswith(".") else f".{extension.lower()}"
        for extension in extensions
    }
    algorithms: list[Algorithm] = []

    for path in root.rglob("*"):
        if not path.is_file() or path.suffix.lower() not in normalized:
            continue
        relative = path.relative_to(root)
        if len(relative.parts) < 2:
            # El generador representa carpetas como categorias; ignora codigo suelto.
            continue
        if any(part in IGNORED_DIRECTORIES or part.startswith(".") for part in relative.parts[:-1]):
            continue
        source = path.read_text(encoding="utf-8", errors="replace")
        algorithms.append(
            Algorithm(
                category=display_category(relative.parent),
                path=path,
                title=path.stem,  # Quita .cpp/.h sin alterar el nombre del algoritmo.
                language=language_for(path),
                lines=tuple(source.splitlines()) or ("",),
            )
        )

    return sorted(algorithms, key=lambda item: (item.category.casefold(), item.title.casefold()))


CPP_KEYWORDS = {
    "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor",
    "bool", "break", "case", "catch", "char", "char16_t", "char32_t", "class",
    "compl", "concept", "const", "consteval", "constexpr", "constinit", "const_cast",
    "continue", "co_await", "co_return", "co_yield", "decltype", "default", "delete",
    "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern",
    "false", "float", "for", "friend", "goto", "if", "inline", "int", "long",
    "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator",
    "or", "or_eq", "private", "protected", "public", "register", "reinterpret_cast",
    "requires", "return", "short", "signed", "sizeof", "static", "static_assert",
    "static_cast", "struct", "switch", "template", "this", "thread_local", "throw",
    "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using",
    "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq",
}
CPP_COMMON_TYPES = {
    "array", "deque", "function", "ll", "map", "multimap", "multiset", "pair",
    "priority_queue", "queue", "set", "stack", "string", "unordered_map",
    "unordered_set", "vector",
}


def _font(text: str, color: str, bold: bool = False) -> str:
    tag = "b" if bold else "font"
    escaped = html.escape(text, quote=False)
    if bold:
        return f'<font color="{color}"><b>{escaped}</b></font>'
    return f'<font color="{color}">{escaped}</font>'


def highlight_cpp_line(line: str, in_block_comment: bool) -> tuple[str, bool]:
    """Resaltador pequeno, sin dependencias, suficiente para C/C++ competitivo."""

    if not line:
        return " ", in_block_comment
    if not in_block_comment and line.lstrip().startswith("#"):
        return _font(line, "#8B3FA0", bold=True), False

    result: list[str] = []
    index = 0
    length = len(line)
    while index < length:
        if in_block_comment:
            end = line.find("*/", index)
            if end < 0:
                result.append(_font(line[index:], "#6A7F45"))
                return "".join(result), True
            result.append(_font(line[index : end + 2], "#6A7F45"))
            index = end + 2
            in_block_comment = False
            continue

        if line.startswith("//", index):
            result.append(_font(line[index:], "#6A7F45"))
            break
        if line.startswith("/*", index):
            end = line.find("*/", index + 2)
            if end < 0:
                result.append(_font(line[index:], "#6A7F45"))
                return "".join(result), True
            result.append(_font(line[index : end + 2], "#6A7F45"))
            index = end + 2
            continue

        char = line[index]
        if char in {'"', "'"}:
            quote = char
            end = index + 1
            while end < length:
                if line[end] == "\\":
                    end += 2
                    continue
                if line[end] == quote:
                    end += 1
                    break
                end += 1
            result.append(_font(line[index:end], "#B14A2A"))
            index = end
            continue

        identifier = re.match(r"[A-Za-z_][A-Za-z0-9_]*", line[index:])
        if identifier:
            token = identifier.group(0)
            if token in CPP_KEYWORDS:
                result.append(_font(token, "#135FA7", bold=True))
            elif token in CPP_COMMON_TYPES:
                result.append(_font(token, "#007C78", bold=True))
            else:
                result.append(html.escape(token, quote=False))
            index += len(token)
            continue

        number = re.match(r"(?:0[xX][0-9A-Fa-f]+|\d+(?:\.\d+)?(?:[eE][+-]?\d+)?)[uUlLfF]*", line[index:])
        if number:
            token = number.group(0)
            result.append(_font(token, "#9A4D00"))
            index += len(token)
            continue

        result.append(html.escape(char, quote=False))
        index += 1

    return "".join(result) or " ", in_block_comment


def wrap_code_line(line: str, width: int = CODE_WRAP_COLUMNS) -> list[str]:
    """Divide visualmente una linea larga sin modificar el archivo fuente."""

    if len(line) <= width:
        return [line]

    indentation = line[: len(line) - len(line.lstrip())]
    remaining = line[len(indentation) :]
    continuation = indentation + "    "
    wrapped: list[str] = []
    prefix = indentation

    while remaining:
        available = max(24, width - len(prefix))
        if len(remaining) <= available:
            wrapped.append(prefix + remaining)
            break

        window = remaining[:available]
        candidates = [window.rfind(char) for char in " \t,;)]}+-*/&|<>=?"]
        cut = max(candidates) + 1
        # Evita crear fragmentos diminutos cuando no hay un corte natural cercano.
        if cut < max(24, int(available * 0.58)):
            cut = available

        fragment = remaining[:cut].rstrip()
        wrapped.append(prefix + fragment)
        remaining = remaining[cut:].lstrip()
        prefix = continuation

    return wrapped or [""]


def make_styles():
    styles = getSampleStyleSheet()
    return {
        "cover_kicker": ParagraphStyle(
            "CoverKicker", parent=styles["Normal"], fontName="Helvetica-Bold",
            fontSize=10, leading=13, textColor=TEAL, tracking=2.4, alignment=TA_CENTER,
        ),
        "cover_title": ParagraphStyle(
            "CoverTitle", parent=styles["Title"], fontName="Helvetica-Bold",
            fontSize=34, leading=38, textColor=colors.white, alignment=TA_CENTER,
            spaceAfter=16,
        ),
        "cover_subtitle": ParagraphStyle(
            "CoverSubtitle", parent=styles["Normal"], fontName="Helvetica",
            fontSize=13, leading=19, textColor=colors.HexColor("#C9D6E2"),
            alignment=TA_CENTER,
        ),
        "small_caps": ParagraphStyle(
            "SmallCaps", parent=styles["Normal"], fontName="Helvetica-Bold",
            fontSize=8, leading=10, textColor=MUTED, tracking=1.4,
        ),
        "toc_title": ParagraphStyle(
            "TocTitle", parent=styles["Heading1"], fontName="Helvetica-Bold",
            fontSize=25, leading=30, textColor=NAVY, spaceAfter=8,
        ),
        "toc_intro": ParagraphStyle(
            "TocIntro", parent=styles["Normal"], fontName="Helvetica",
            fontSize=9.5, leading=14, textColor=MUTED, spaceAfter=18,
        ),
        "category_label": ParagraphStyle(
            "CategoryLabel", parent=styles["Normal"], fontName="Helvetica-Bold",
            fontSize=9, leading=12, textColor=TEAL, tracking=2.0, alignment=TA_CENTER,
        ),
        "category_title": ParagraphStyle(
            "CategoryTitle", parent=styles["Heading1"], fontName="Helvetica-Bold",
            fontSize=28, leading=34, textColor=NAVY, alignment=TA_CENTER,
            spaceBefore=12, spaceAfter=12,
        ),
        "category_count": ParagraphStyle(
            "CategoryCount", parent=styles["Normal"], fontName="Helvetica",
            fontSize=10, leading=14, textColor=MUTED, alignment=TA_CENTER,
        ),
        "algorithm_title": ParagraphStyle(
            "AlgorithmTitle", parent=styles["Heading1"], fontName="Helvetica-Bold",
            fontSize=21, leading=25, textColor=NAVY, spaceAfter=5, keepWithNext=True,
        ),
        "algorithm_meta": ParagraphStyle(
            "AlgorithmMeta", parent=styles["Normal"], fontName="Helvetica-Bold",
            fontSize=8, leading=11, textColor=BLUE, spaceAfter=12, keepWithNext=True,
        ),
        "code": ParagraphStyle(
            "Code", parent=styles["Code"], fontName="Courier", fontSize=7.0,
            leading=8.05, textColor=INK, leftIndent=0, rightIndent=0, spaceAfter=0,
            allowWidows=1, allowOrphans=1,
        ),
        "line_number": ParagraphStyle(
            "LineNumber", parent=styles["Normal"], fontName="Courier-Bold",
            fontSize=6.1, leading=8.05, textColor=colors.HexColor("#778592"),
            alignment=TA_RIGHT,
        ),
        "code_header": ParagraphStyle(
            "CodeHeader", parent=styles["Normal"], fontName="Helvetica-Bold",
            fontSize=6.5, leading=8, textColor=colors.white, tracking=0.8,
        ),
    }


def marked_paragraph(text: str, style, *, level: int, key: str) -> Paragraph:
    paragraph = Paragraph(text, style)
    paragraph.toc_level = level
    paragraph.bookmark_key = key
    paragraph.toc_text = re.sub(r"<[^>]+>", "", text)
    return paragraph


def month_name(month: int) -> str:
    months = (
        "enero", "febrero", "marzo", "abril", "mayo", "junio",
        "julio", "agosto", "septiembre", "octubre", "noviembre", "diciembre",
    )
    return months[month - 1]


def format_date(moment: datetime) -> str:
    return f"{moment.day} de {month_name(moment.month)} de {moment.year}"


def draw_page_background(canvas, doc) -> None:
    page = canvas.getPageNumber()
    width, height = letter
    canvas.saveState()
    canvas.setTitle(doc.document_title)
    canvas.setAuthor("ICPC Team Reference")
    canvas.setSubject("Algoritmos de programacion competitiva")

    if page == 1:
        canvas.setFillColor(NAVY)
        canvas.rect(0, 0, width, height, stroke=0, fill=1)
        canvas.setFillColor(BLUE)
        canvas.rect(0, 0, width, 0.17 * inch, stroke=0, fill=1)
        canvas.setFillColor(TEAL)
        canvas.rect(0.17 * width, 0, 0.34 * width, 0.17 * inch, stroke=0, fill=1)
        canvas.setFillColor(ORANGE)
        canvas.rect(0.51 * width, 0, 0.49 * width, 0.17 * inch, stroke=0, fill=1)
    canvas.restoreState()


def draw_page_chrome(canvas, doc) -> None:
    """Dibuja al final para que tablas largas no oculten encabezado ni pie."""

    page = canvas.getPageNumber()
    if page == 1:
        return

    width, height = letter
    canvas.saveState()
    left = doc.leftMargin
    right = width - doc.rightMargin
    canvas.setStrokeColor(RULE)
    canvas.setLineWidth(0.55)
    canvas.line(left, height - 0.48 * inch, right, height - 0.48 * inch)
    canvas.setFillColor(BLUE)
    canvas.setFont("Helvetica-Bold", 7)
    canvas.drawString(left, height - 0.36 * inch, "ALGORITMARIO ICPC")
    canvas.setFillColor(MUTED)
    canvas.setFont("Helvetica", 6.8)
    canvas.drawRightString(right, height - 0.36 * inch, "REFERENCIA DE COMPETENCIA")

    canvas.setStrokeColor(RULE)
    canvas.line(left, 0.43 * inch, right, 0.43 * inch)
    canvas.setFillColor(MUTED)
    canvas.setFont("Helvetica", 7)
    canvas.drawString(left, 0.28 * inch, doc.document_title)
    canvas.setFont("Helvetica-Bold", 7)
    canvas.setFillColor(BLUE)
    canvas.drawRightString(right, 0.28 * inch, f"{page:02d}")
    canvas.restoreState()


def add_cover(story: list, styles: dict, title: str, subtitle: str, algorithms: list[Algorithm]) -> None:
    categories = len({algorithm.category for algorithm in algorithms})
    lines = sum(len(algorithm.lines) for algorithm in algorithms)
    generated = format_date(datetime.now())

    story.extend(
        [
            Spacer(1, 1.48 * inch),
            Paragraph("PROGRAMACIÓN COMPETITIVA · ICPC", styles["cover_kicker"]),
            Spacer(1, 0.35 * inch),
            Paragraph(html.escape(title), styles["cover_title"]),
            Paragraph(html.escape(subtitle), styles["cover_subtitle"]),
            Spacer(1, 0.62 * inch),
        ]
    )
    stats = [
        [str(len(algorithms)), str(categories), f"{lines:,}"],
        ["ALGORITMOS", "CATEGORÍAS", "LÍNEAS DE CÓDIGO"],
    ]
    table = Table(stats, colWidths=[1.45 * inch] * 3, rowHeights=[0.42 * inch, 0.26 * inch])
    table.setStyle(
        TableStyle(
            [
                ("TEXTCOLOR", (0, 0), (-1, 0), colors.white),
                ("TEXTCOLOR", (0, 1), (-1, 1), colors.HexColor("#91A8BA")),
                ("FONTNAME", (0, 0), (-1, 0), "Helvetica-Bold"),
                ("FONTSIZE", (0, 0), (-1, 0), 19),
                ("FONTNAME", (0, 1), (-1, 1), "Helvetica-Bold"),
                ("FONTSIZE", (0, 1), (-1, 1), 6.5),
                ("ALIGN", (0, 0), (-1, -1), "CENTER"),
                ("VALIGN", (0, 0), (-1, -1), "MIDDLE"),
                ("LINEBEFORE", (1, 0), (1, -1), 0.5, colors.HexColor("#416176")),
                ("LINEBEFORE", (2, 0), (2, -1), 0.5, colors.HexColor("#416176")),
            ]
        )
    )
    story.extend(
        [
            table,
            Spacer(1, 1.05 * inch),
            Paragraph(f"EDICIÓN GENERADA EL {generated.upper()}", styles["cover_kicker"]),
            PageBreak(),
        ]
    )


def add_toc(story: list, styles: dict) -> None:
    story.extend(
        [
            Spacer(1, 0.28 * inch),
            Paragraph("Índice", styles["toc_title"]),
            Paragraph(
                "Las categorías y los algoritmos están enlazados. Haz clic en cualquier entrada "
                "para ir directamente a su página.",
                styles["toc_intro"],
            ),
        ]
    )
    toc = TableOfContents()
    toc.levelStyles = [
        ParagraphStyle(
            "TOCCategory", fontName="Helvetica-Bold", fontSize=9.8, leading=14.5,
            leftIndent=0, firstLineIndent=0, textColor=NAVY, spaceBefore=4.5,
        ),
        ParagraphStyle(
            "TOCAlgorithm", fontName="Helvetica", fontSize=8.4, leading=11.8,
            leftIndent=16, firstLineIndent=0, textColor=INK,
        ),
    ]
    toc.dotsMinLevel = 0
    story.extend([toc, PageBreak()])


def code_table(algorithm: Algorithm, styles: dict) -> LongTable:
    header = [
        Paragraph("#", styles["code_header"]),
        Paragraph(f"CÓDIGO · {html.escape(algorithm.title)}", styles["code_header"]),
    ]
    rows = [header]
    in_block_comment = False
    for number, source_line in enumerate(algorithm.lines, start=1):
        visual_lines = []
        for visual_line in wrap_code_line(source_line.expandtabs(4)):
            markup, in_block_comment = highlight_cpp_line(visual_line, in_block_comment)
            visual_lines.append(markup)
        rows.append(
            [
                Paragraph(str(number), styles["line_number"]),
                XPreformatted("\n".join(visual_lines), styles["code"]),
            ]
        )

    table = LongTable(
        rows,
        colWidths=[0.36 * inch, 6.88 * inch],
        repeatRows=1,
        splitByRow=True,
        hAlign="LEFT",
    )
    table.setStyle(
        TableStyle(
            [
                ("BACKGROUND", (0, 0), (-1, 0), NAVY),
                ("BACKGROUND", (0, 1), (0, -1), GUTTER_BG),
                ("BACKGROUND", (1, 1), (1, -1), CODE_BG),
                ("BOX", (0, 0), (-1, -1), 0.6, colors.HexColor("#C9D3DD")),
                ("LINEAFTER", (0, 1), (0, -1), 0.45, colors.HexColor("#CDD6DF")),
                ("VALIGN", (0, 0), (-1, -1), "TOP"),
                ("ALIGN", (0, 0), (0, 0), "RIGHT"),
                ("LEFTPADDING", (0, 0), (0, -1), 4),
                ("RIGHTPADDING", (0, 0), (0, -1), 5),
                ("LEFTPADDING", (1, 0), (1, -1), 7),
                ("RIGHTPADDING", (1, 0), (1, -1), 5),
                ("TOPPADDING", (0, 0), (-1, 0), 5),
                ("BOTTOMPADDING", (0, 0), (-1, 0), 5),
                ("TOPPADDING", (0, 1), (-1, -1), 0.7),
                ("BOTTOMPADDING", (0, 1), (-1, -1), 0.7),
            ]
        )
    )
    return table


def build_story(algorithms: list[Algorithm], title: str, subtitle: str) -> list:
    styles = make_styles()
    story: list = []
    add_cover(story, styles, title, subtitle, algorithms)
    add_toc(story, styles)

    categories: dict[str, list[Algorithm]] = {}
    for algorithm in algorithms:
        categories.setdefault(algorithm.category, []).append(algorithm)

    category_items = list(categories.items())
    algorithm_counter = 0
    for category_index, (category, category_algorithms) in enumerate(category_items, start=1):
        category_key = f"category-{slug(category)}-{category_index}"
        story.extend(
            [
                Spacer(1, 1.72 * inch),
                Paragraph(f"SECCIÓN {category_index:02d}", styles["category_label"]),
                marked_paragraph(
                    html.escape(category), styles["category_title"], level=0, key=category_key,
                ),
                Table(
                    [[""]], colWidths=[0.78 * inch], rowHeights=[0.055 * inch],
                    style=TableStyle([("BACKGROUND", (0, 0), (-1, -1), ORANGE)]),
                ),
                Spacer(1, 0.18 * inch),
                Paragraph(
                    f"{len(category_algorithms)} "
                    f"{'algoritmo' if len(category_algorithms) == 1 else 'algoritmos'}",
                    styles["category_count"],
                ),
                PageBreak(),
            ]
        )

        for algorithm in category_algorithms:
            algorithm_counter += 1
            key = f"algorithm-{slug(category)}-{slug(algorithm.title)}-{algorithm_counter}"
            story.extend(
                [
                    marked_paragraph(
                        html.escape(algorithm.title),
                        styles["algorithm_title"],
                        level=1,
                        key=key,
                    ),
                    Paragraph(
                        f"{html.escape(category.upper())} &nbsp;·&nbsp; "
                        f"{html.escape(algorithm.language)} &nbsp;·&nbsp; "
                        f"{len(algorithm.lines)} LÍNEAS",
                        styles["algorithm_meta"],
                    ),
                    code_table(algorithm, styles),
                ]
            )
            is_last = (
                category_index == len(category_items)
                and algorithm is category_algorithms[-1]
            )
            if not is_last:
                story.append(PageBreak())

    return story


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Genera un PDF tamano carta con indice y resaltado de codigo.",
    )
    parser.add_argument(
        "--root", type=Path, default=Path(__file__).resolve().parent,
        help="Carpeta raiz del algoritmario (por defecto: carpeta del script).",
    )
    parser.add_argument(
        "--output", type=Path, default=None,
        help="Ruta del PDF (por defecto: ROOT/output/pdf/algoritmario_icpc.pdf).",
    )
    parser.add_argument("--title", default="Algoritmario ICPC")
    parser.add_argument(
        "--subtitle", default="Referencia rápida de algoritmos para competencia",
    )
    parser.add_argument(
        "--extensions", nargs="+", default=list(DEFAULT_EXTENSIONS),
        help="Extensiones incluidas, separadas por espacios.",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    root = args.root.resolve()
    output = (args.output or root / "output" / "pdf" / "algoritmario_icpc.pdf").resolve()
    algorithms = discover_algorithms(root, args.extensions)
    if not algorithms:
        raise SystemExit(
            "No se encontraron archivos de codigo dentro de carpetas. "
            "Revisa --root y --extensions."
        )

    output.parent.mkdir(parents=True, exist_ok=True)
    document = AlgorithmsDocTemplate(
        str(output),
        title=args.title,
        pagesize=letter,
        leftMargin=0.63 * inch,
        rightMargin=0.63 * inch,
        topMargin=0.64 * inch,
        bottomMargin=0.59 * inch,
        pageCompression=1,
    )
    frame = Frame(
        document.leftMargin,
        document.bottomMargin,
        document.width,
        document.height,
        id="content",
        leftPadding=0,
        rightPadding=0,
        topPadding=0,
        bottomPadding=0,
    )
    document.addPageTemplates(
        [
            PageTemplate(
                id="main",
                frames=[frame],
                onPage=draw_page_background,
                onPageEnd=draw_page_chrome,
            )
        ]
    )
    document.multiBuild(build_story(algorithms, args.title, args.subtitle))

    print(f"PDF generado: {output}")
    print(f"Incluidos: {len(algorithms)} algoritmos en {len({a.category for a in algorithms})} categorias")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
